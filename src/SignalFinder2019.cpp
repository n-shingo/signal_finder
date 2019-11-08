//-------------------------------------------------
// SignalFinder2019.cpp
// S. NAKAMURA
// since: 2019-11-05
//-------------------------------------------------

#include "SignalFinder2019.h"
#include "ToolFunc.h"

using namespace sn;
using namespace sn::Tool;


// 信号検出処理の開始
void SignalFinder2019::Start()
{
    MoveMode(Mode::FindingRed);
}

// 信号検出の終了(強制的にアイドリングにする)
void SignalFinder2019::Stop()
{
    MoveMode(Mode::Idling);
}


// 画像を追加し、解析＆処理を行う
Result SignalFinder2019::AnalyzeImage(Mat& img, Mat& dst, bool drawResult)
{
    switch (_mode) {

    case Mode::Idling:
        return ProcessNothing(img, dst, drawResult);

    case Mode::FindingRed:
        return TryFindRedSignal(img, dst, drawResult);

    case Mode::WaitingBlue:
        return WaitForBlueSignal(img, dst, drawResult);

    case Mode::FoundBlue:
        return FoundBlueSignal(img, dst, drawResult);
    }

    return Result::Error;
}


//
//  ここから非公開メソッド
//


// アイドリング中の何もしない処理を行う
Result SignalFinder2019::ProcessNothing(Mat& img, Mat& dst, bool drawResult)
{
    assert(_mode == Mode::Idling);

    if (drawResult) {
        dst = img.clone();
        Labeling(dst, "Idling");
    }

    return Result::NoProcess;
}

// 赤信号を探す処理を行う
Result SignalFinder2019::TryFindRedSignal(Mat& img, Mat& dst, bool drawResult)
{
    assert(_mode == Mode::FindingRed);

    const int gaussKSize = 5;          // ガウスフィルタのカーネルサイズ
    const double gaussSigma = 3.0;     // ガウスフィルタの半径
    const double red_threshold = 0.6;  // 赤信号の相関係数閾値

                                       // 描画用の原画像のクローン
    if (drawResult) dst = img.clone();

    // ガウシアン処理
    Mat gaussImg;
    Gaussian(img, gaussImg, gaussKSize, gaussSigma, true);

    // HSVで閾値処理
    Mat hsvRes;
    GetRedSignalArea(gaussImg, hsvRes);
    this->_redAreaBuf.AddImage(hsvRes);
    if (!_redAreaBuf.IsFilled())
    {
        // 十分な画像がバッファに溜まっていない場合は終了
        if (drawResult) Labeling(dst, "Looking for red", Point(0, 0), 0.375, Scalar(0, 200, 200));
        return Result::LookingForRed;
    }

    // 平均画像を取得
    Mat hsvAve;
    _redAreaBuf.AverageImage(hsvAve);
    cv::threshold(hsvAve, hsvAve, 100, 255, CV_THRESH_BINARY);

    // クロージング処理(膨張->収縮)
    cv::Mat hsvAveMo;
    cv::morphologyEx(hsvAve, hsvAveMo, cv::MORPH_CLOSE, Mat());

    // 候補領域抽出
    std::vector< Mat > rgns;
    std::vector< Rect > rects;
    SplitImageRegion(hsvAveMo, rgns, rects);
    if (rgns.size() == 0) {
        // 候補になる領域がない場合は終了
        if (drawResult) Labeling(dst, "Looking for red", Point(0, 0), 0.375, Scalar(0, 200, 200));
        return Result::LookingForRed;
    }

    // 重心取得
    std::vector< Point > cogs;
    GetCOG(rgns, rects, cogs);
    if (drawResult) {
        // 赤信号重心の描画
        for (int i = 0; i < cogs.size(); i++)
            cv::drawMarker(dst, cogs[i], Scalar(255, 0, 255), cv::MARKER_TILTED_CROSS, 7);
    }

    // 候補画像取得
    std::vector < Mat > candImg;
    std::vector < Rect > candImgRect;
    Rect *sl = &SIGNAL_LOCATE_FROM_RED;
    for (int i = 0; i < cogs.size(); i++) {
        cv::Rect rct(cogs[i].x + sl->x, cogs[i].y + sl->y, sl->width, sl->height);
        if (rct.x >= 0 && rct.y >= 0 && rct.x + rct.width < img.cols && rct.y + rct.height < img.rows) {
            Mat cand = gaussImg(rct);
            candImg.push_back(cand.clone());
            candImgRect.push_back(rct);
        }
    }
    if (candImg.size() == 0) {
        // 候補になる画像がない場合は終了
        if (drawResult) Labeling(dst, "Looking for red", Point(0, 0), 0.375, Scalar(0, 200, 200));
        return Result::LookingForRed;
    }
    if (drawResult)
    {
        // 候補画像の矩形描画
        for (int i = 0; i < cogs.size(); i++) {
            cv::Rect rct(cogs[i].x + sl->x, cogs[i].y + sl->y, sl->width, sl->height);
            cv::rectangle(dst, rct, Scalar(255, 255, 255));
        }
    }


    // 相関係数を見て閾値以上であれば、赤信号発見とし、複数ある場合は最も高いものとする
    double maxCC = -1.0;
    Mat redSignal;
    Rect redSignalRect;
    size_t redIndex;
    for (int i = 0; i < candImg.size(); i++)
    {
        // まずは候補１つに対するテンプレート内での最高値を出す
        std::vector<double> r;
        for (int j = 0; j < RED_SIGNAL_FILE_CNT; j++)
            r.push_back(CompareImages(RED_SIGNAL_BASE_IMG[j], candImg[i]));
        size_t r_max_index = std::distance(r.begin(), std::max_element(r.begin(), r.end()));
        double r_max = r[r_max_index];

        // 今まで候補よりも良ければ、更新
        if (r_max >= red_threshold && r_max > maxCC) {
            maxCC = r_max;
            redSignal = candImg[i];
            redSignalRect = candImgRect[i];
            redIndex = r_max_index;
        }

        // 赤信号相関係数の描画
        if (drawResult) {
            Point pt = Point(candImgRect[i].x + candImgRect[i].width, candImgRect[i].y);
            if (r_max >= red_threshold)
                Labeling(dst, std::to_string(r_max), pt, 0.3, Scalar(0, 0, 200));
            else
                Labeling(dst, std::to_string(r_max), pt, 0.3);
        }
    }
    if (maxCC < red_threshold) {
        // 閾値を超えるものがなければ終了
        if (drawResult) Labeling(dst, "Looking for red", Point(0, 0), 0.375, Scalar(0, 200, 200));
        return Result::LookingForRed;
    }

    //
    // ここまで来たら赤信号っぽいものを発見!
    //
    _redSigImgBuf.AddImage(redSignal);
    _redCCBuf.AddDouble(maxCC);
    if (drawResult)
        cv::rectangle(dst, redSignalRect, Scalar(0, 0, 255));

    // 赤信号のバッファが一杯でなければ、赤信号画像を更に収集するために終了
    if (!_redSigImgBuf.IsFilled()) {
        if (drawResult) Labeling(dst, "Looking for red", Point(0, 0), 0.375, Scalar(0, 200, 200));
        return Result::LookingForRed;
    }

    // 最良の赤信号画像を決定
    int maxIndex = _redCCBuf.GetMaxIndex();
    this->_redSignal = _redSigImgBuf[maxIndex].clone();

    // 赤信号探索を終了し、青信号待ちに遷移するする
    MoveMode(Mode::WaitingBlue);
    if (drawResult) {
        Labeling(dst, "Found red signal", Point(0, 0), 0.375, Scalar(0, 200, 0));
        // 右上に赤信号テンプレート画像を表示
        Mat tmpArea = dst(Rect(dst.cols - _redSignal.cols, 0, _redSignal.cols, _redSignal.rows));
        _redSignal.copyTo(tmpArea);
    }



    return Result::FoundRed;
}

// 青信号を待つ処理を行う
Result SignalFinder2019::WaitForBlueSignal(Mat& img, Mat& dst, bool drawResult)
{
    assert(_mode == Mode::WaitingBlue);
    assert(_redAreaBuf.IsFilled());


    const int gaussKSize = 5;          // ガウスフィルタのカーネルサイズ
    const double gaussSigma = 3.0;     // ガウスフィルタの半径
    const double red_threshold = 0.925;  // 赤信号の相関係数閾値
    const double blu_threshold = 0.8;  // 青信号の相関係数閾値

                                         // 描画準備
    if (drawResult) {
        // 描画用の原画像のクローン
        dst = img.clone();

        // 右上に赤信号テンプレート画像を表示
        Mat tmpArea = dst(Rect(dst.cols - _redSignal.cols, 0, _redSignal.cols, _redSignal.rows));
        _redSignal.copyTo(tmpArea);
    }

    // ガウシアン処理
    Mat gaussImg;
    Gaussian(img, gaussImg, gaussKSize, gaussSigma, true);

    // HSVで閾値処理
    Mat hsvRes;
    GetRedSignalArea(gaussImg, hsvRes);
    _redAreaBuf.AddImage(hsvRes);

    // 平均画像を取得
    Mat hsvAve;
    _redAreaBuf.AverageImage(hsvAve);
    cv::threshold(hsvAve, hsvAve, 100, 255, CV_THRESH_BINARY);

    // クロージング処理(膨張->収縮)
    cv::Mat hsvAveMo;
    cv::morphologyEx(hsvAve, hsvAveMo, cv::MORPH_CLOSE, Mat());

    // 候補領域抽出
    std::vector< Mat > rgns;
    std::vector< Rect > rects;
    SplitImageRegion(hsvAveMo, rgns, rects);

    // 候補になる領域がない場合は終了
    if (rgns.size() == 0) {
        _redSignalLostCounter++;

        // MAX_LOST_FRAMESに達していたら、再び赤信号を探すモードへ移行
        if (_redSignalLostCounter == MAX_LOST_FRAMES) {
            if (drawResult) Labeling(dst, "Lost red signal!", Point(0, 0), 0.375, Scalar(0, 200, 0));
            MoveMode(Mode::FindingRed);
            return Result::LostRed;
        }
        // MAX_LOST_FRAMESに達していなければ、単に終了
        else {
            if (drawResult) Labeling(dst, "Waiting for blue", Point(0, 0), 0.375, Scalar(0, 0, 200));
            return Result::WaitingForBlue;
        }
    }

    // 重心取得
    std::vector< Point > cogs;
    GetCOG(rgns, rects, cogs);
    if (drawResult) {
        // 赤信号重心の描画
        for (int i = 0; i < cogs.size(); i++)
            cv::drawMarker(dst, cogs[i], Scalar(255, 0, 255), cv::MARKER_TILTED_CROSS, 7);
    }

    // 候補画像取得
    std::vector < Mat > candImg;
    std::vector < Rect > candImgRect;
    Rect *sl = &SIGNAL_LOCATE_FROM_RED;
    for (int i = 0; i < cogs.size(); i++) {
        cv::Rect rct(cogs[i].x + sl->x, cogs[i].y + sl->y, sl->width, sl->height);
        if (rct.x >= 0 && rct.y >= 0 && rct.x + rct.width < img.cols && rct.y + rct.height < img.rows) {
            Mat cand = gaussImg(rct);
            candImg.push_back(cand.clone());
            candImgRect.push_back(rct);
        }
    }

    // 候補になる画像がない場合は終了
    if (candImg.size() == 0) {
        _redSignalLostCounter++;

        // MAX_LOST_FRAMESに達していたら、再び赤信号を探すモードへ移行
        if (_redSignalLostCounter == MAX_LOST_FRAMES) {
            if (drawResult) Labeling(dst, "Lost red signal!", Point(0, 0), 0.375, Scalar(0, 200, 0));
            MoveMode(Mode::FindingRed);
            return Result::LostRed;
        }
        // MAX_LOST_FRAMESに達していなければ、単に終了
        else {
            if (drawResult) Labeling(dst, "Waiting for blue", Point(0, 0), 0.375, Scalar(0, 0, 200));
            return Result::WaitingForBlue;
        }
    }
    if (drawResult)
    {
        // 候補画像描画
        for (int i = 0; i < cogs.size(); i++) {
            cv::Rect rct(cogs[i].x + sl->x, cogs[i].y + sl->y, sl->width, sl->height);
            cv::rectangle(dst, rct, Scalar(255, 255, 255));
        }
    }

    // 画像が赤信号か判断する
    double red_maxCC = -1.0;
    Mat max_redImg;
    Rect max_redRect;
    for (int i = 0; i < candImg.size(); i++)
    {
        // 赤信号テンプレートとの相関係数を計算
        double r = CompareImages(_redSignal, candImg[i]);
        if (r > red_maxCC) {
            red_maxCC = r;
            max_redImg = candImg[i];
            max_redRect = candImgRect[i];
        }
    }

    // 赤信号の相関係数描画
    if (drawResult && candImg.size() > 0) {
        Point pt = Point(max_redRect.x + max_redRect.width, max_redRect.y);
        if (red_maxCC >= red_threshold)
            Labeling(dst, std::to_string(red_maxCC), pt, 0.3, Scalar(0, 0, 200));
        else
            Labeling(dst, std::to_string(red_maxCC), pt, 0.3);

    }

    // 赤信号であった場合の処理
    if (red_maxCC > red_threshold) {
        // 赤信号テンプレート画像を更新し、LOSTカウンタを0にリセットして終了
        _redSignal = max_redImg.clone();
        _redSignalLostCounter = 0;
        if (drawResult) {
            rectangle(dst, max_redRect, Scalar(0, 0, 255));
            Labeling(dst, "Waiting for blue", Point(0, 0), 0.375, Scalar(0, 0, 200));
        }
        return Result::WaitingForBlue;
    }

    // 赤信号でなかった場合、青信号か判断する
    double blu_maxCC = -1.0;
    Mat bluImg;
    Rect bluRect;
    for (int i = 0; i < candImg.size(); i++)
    {
        // まずは候補１つに対するテンプレート内での相関係数最高値を出す
        std::vector<double> r;
        for (int j = 0; j < BLUE_SIGNAL_FILE_CNT; j++)
            r.push_back(CompareImages(BLUE_SIGNAL_BASE_IMG[j], candImg[i]));
        size_t r_max_index = std::distance(r.begin(), std::max_element(r.begin(), r.end()));
        double r_max = r[r_max_index];

        // 今までの候補よりも良ければ、更新
        if (r_max > blu_maxCC) {
            blu_maxCC = r_max;
            bluImg = candImg[i];
            bluRect = candImgRect[i];
        }
    }

    // 普通の青信号の相関係数描画
    if (drawResult && candImg.size() > 0 ) {
        Point pt = Point(bluRect.x + bluRect.width, bluRect.y + 20);
        if (blu_maxCC >= blu_threshold)
            Labeling(dst, std::to_string(blu_maxCC), pt, 0.3, Scalar(200, 200, 0));
        else
            Labeling(dst, std::to_string(blu_maxCC), pt, 0.3);
    }

    // 青信号っぽかった場合、赤信号から青信号への変化かチェック
    cv::Mat diff;
    int r2b=0;
    if(blu_maxCC >= blu_threshold ){
        r2b = RedToBlueRatio(_redSignal, bluImg, diff);

        if(drawResult){
            Point pt = Point(bluRect.x + bluRect.width, bluRect.y + 40);
            if( r2b )
                Labeling(dst, std::to_string(r2b), pt, 0.3, Scalar(255,0,0));
            else
                Labeling(dst, std::to_string(r2b), pt, 0.3);
        }
    }

    // 青信号でなかった場合
    if (blu_maxCC < blu_threshold || r2b == 0 )
    {
        _redSignalLostCounter++;

        // MAX_LOST_FRAMESに達していたら、再び赤信号を探すモードへ移行
        if (_redSignalLostCounter == MAX_LOST_FRAMES) {
            if (drawResult) Labeling(dst, "Lost red signal!", Point(0, 0), 0.375, Scalar(0, 200, 0));
            MoveMode(Mode::FindingRed);
            return Result::LostRed;
        }
        // MAX_LOST_FRAMESに達していなければ、単に終了
        else {
            if (drawResult) Labeling(dst, "Waiting for blue", Point(0, 0), 0.375, Scalar(0, 0, 200));
            return Result::WaitingForBlue;
        }

    }

    //
    // ここまで来たら青信号で発見!
    //
    MoveMode(Mode::FoundBlue);
    if (drawResult) {
        Labeling(dst, "Found blue signal", Point(0, 0), 0.375, Scalar(255, 0, 0));
    }
    return Result::FoundBlueSignal;
}

Result SignalFinder2019::FoundBlueSignal(Mat& img, Mat& dst, bool drawResult) {

    // 青信号発見情報を規定回数まで出力する
    if (_bluSignalCounter < BLUE_SIGNAL_FRAMES) {

        if (drawResult) {
            Labeling(dst, "Found blue signal", Point(0, 0), 0.375, Scalar(255, 0, 0));
        }
        _bluSignalCounter++;
        return Result::FoundBlueSignal;
    }

    // アイドリングモードに遷移する
    MoveMode(Mode::Idling);
    if (drawResult) {
        Labeling(dst, "Idling", Point(0, 0), 0.375);
    }
    return Result::FoundBlueSignal;
}

// モードの遷移を行う
void SignalFinder2019::MoveMode(Mode mode)
{
    if (mode == Mode::Idling) {
        _mode = Mode::Idling;
    }
    // 赤信号検索モードへ遷移
    else if (mode == Mode::FindingRed) {
        _mode = Mode::FindingRed;
        _redAreaBuf.Clear();   // HSVによる２値画像のバッファを初期化
        _redSigImgBuf.Clear(); // 赤信号画像バッファを初期化
        _redCCBuf.Clear();     // 赤信号相関係数バッファを初期化
    }
    else if (mode == Mode::WaitingBlue) {
        _mode = Mode::WaitingBlue;
        _redSignalLostCounter = 0; //赤信号をロストした回数を数えるカウンター
    }
    else if (mode == Mode::FoundBlue) {
        _mode = Mode::FoundBlue;
        _bluSignalCounter = 1;  // 遷移するときは既に1回発見しているので1から始まる
    }
}




// HSVによる閾値処理によって赤信号領域画像を取得する
// srcはカラー画像, dstは8bitの２値画像
void SignalFinder2019::GetRedSignalArea(cv::Mat& src, cv::Mat& dst)
{
    assert(src.type() == CV_8UC3);
    int h = src.rows, w = src.cols;

    cv::Mat imgH, imgS, imgV;
    SplitToHSV(src, imgH, imgS, imgV);

    dst = Mat::zeros(src.size(), CV_8U);
    uchar *p, *pH, *pS, *pV;

    for (int y = 0; y < h; y++)
    {
        p = dst.data + dst.step[0] * y;
        pH = imgH.data + y * w;
        pS = imgS.data + y * w;
        pV = imgV.data + y * w;

        for (int x = 0; x < w; x++) {
            //if (pS[x] > 100 && (pH[x] > 170 || pH[x] < 15) && pV[x] > 50) {
            //if (pS[x] > 120 && (pH[x] > 170 || pH[x] < 15) && pV[x] > 100) {
            if (pS[x] > 140 && (pH[x] > 170 || pH[x] < 15) && pV[x] > 100) {
                p[x] = 255;
            }

        }
    }

}

// 赤信号から青信号に変更された率
int SignalFinder2019::RedToBlueRatio(cv::Mat& red, cv::Mat& blu, cv::Mat& dst)
{
    assert(red.type() == CV_8UC3);
    assert(blu.type() == CV_8UC3);
    assert(red.cols == blu.cols);
    assert(red.rows == blu.rows);

    // いろいろな閾値
    const int th_dif1=140, th_dif2=115;  // 差画像でこの間の値は見ない
    const int th_up = 136, th_up_cnt = 10;
    const int th_dn = 115, th_dn_cnt = 20;
    
    // グレイ画像化
    int h = red.rows, w = red.cols;
    cv::Mat red_gray, blu_gray;
    cvtColor(red, red_gray, CV_BGR2GRAY);
    cvtColor(blu, blu_gray, CV_BGR2GRAY);

    // 差を計算
    dst = Mat::zeros(red_gray.size(), CV_8U);
    for( int y=0; y<h; y++ ){
        uchar *r_pt = red_gray.data + red_gray.step[0]*y;
        uchar *b_pt = blu_gray.data + blu_gray.step[0]*y;
        uchar *d_pt = dst.data + dst.step[0]*y;

        for( int x=0; x<dst.step[0]; x++ ){
            d_pt[x] = (r_pt[x] - b_pt[x] + 255)/2;  // 差を計算
        }
    }

    // 赤から青へ変化すれば dif 画像は上半分は明るい円が、下半分は暗い円ができるはず
    // 閾値内の上下の平均と数を算出する
    int half_h = h/2;

    int cnt_up = 0, ave_up = 0;
    int cnt_dn = 0, ave_dn = 0;
    for( int y=0; y<h; y++ ){
        uchar *pt = dst.data + dst.step[0]*y;

        for( int x=0; x<dst.step[0]; x++ ){
            if( pt[x] < th_dif2 || pt[x] > th_dif1 ){
                // 上半分
                if( y < half_h ){
                    ave_up += pt[x];
                    cnt_up++;
                }
                // 下半分
                else{
                    ave_dn += pt[x];
                    cnt_dn++;
                }
            }
            else
                pt[x]=0;
        }
    }
    ave_up /= cnt_up;
    ave_dn /= cnt_dn;

    // 結果チェック
    //imshow( "difference", dst );
    //std::cout << "UP: " << ave_up << ", " << cnt_up << std::endl;
    //std::cout << "DN: " << ave_dn << ", " << cnt_dn << std::endl;

    // 赤信号から青信号への変化か判定
    if( ave_up >= th_up && cnt_up >= th_up_cnt 
            && ave_dn <= th_dn && cnt_dn >= th_dn_cnt){
        return 1;
    }
    else
        return 0;


    return 1.0;
}


