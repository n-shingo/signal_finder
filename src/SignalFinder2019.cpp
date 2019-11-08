//-------------------------------------------------
// SignalFinder2019.cpp
// S. NAKAMURA
// since: 2019-11-05
//-------------------------------------------------

#include "SignalFinder2019.h"
#include "ToolFunc.h"

using namespace sn;
using namespace sn::Tool;


// �M�����o�����̊J�n
void SignalFinder2019::Start()
{
    MoveMode(Mode::FindingRed);
}

// �M�����o�̏I��(�����I�ɃA�C�h�����O�ɂ���)
void SignalFinder2019::Stop()
{
    MoveMode(Mode::Idling);
}


// �摜��ǉ����A��́��������s��
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
//  �����������J���\�b�h
//


// �A�C�h�����O���̉������Ȃ��������s��
Result SignalFinder2019::ProcessNothing(Mat& img, Mat& dst, bool drawResult)
{
    assert(_mode == Mode::Idling);

    if (drawResult) {
        dst = img.clone();
        Labeling(dst, "Idling");
    }

    return Result::NoProcess;
}

// �ԐM����T���������s��
Result SignalFinder2019::TryFindRedSignal(Mat& img, Mat& dst, bool drawResult)
{
    assert(_mode == Mode::FindingRed);

    const int gaussKSize = 5;          // �K�E�X�t�B���^�̃J�[�l���T�C�Y
    const double gaussSigma = 3.0;     // �K�E�X�t�B���^�̔��a
    const double red_threshold = 0.6;  // �ԐM���̑��֌W��臒l

                                       // �`��p�̌��摜�̃N���[��
    if (drawResult) dst = img.clone();

    // �K�E�V�A������
    Mat gaussImg;
    Gaussian(img, gaussImg, gaussKSize, gaussSigma, true);

    // HSV��臒l����
    Mat hsvRes;
    GetRedSignalArea(gaussImg, hsvRes);
    this->_redAreaBuf.AddImage(hsvRes);
    if (!_redAreaBuf.IsFilled())
    {
        // �\���ȉ摜���o�b�t�@�ɗ��܂��Ă��Ȃ��ꍇ�͏I��
        if (drawResult) Labeling(dst, "Looking for red", Point(0, 0), 0.375, Scalar(0, 200, 200));
        return Result::LookingForRed;
    }

    // ���ω摜���擾
    Mat hsvAve;
    _redAreaBuf.AverageImage(hsvAve);
    cv::threshold(hsvAve, hsvAve, 100, 255, CV_THRESH_BINARY);

    // �N���[�W���O����(�c��->���k)
    cv::Mat hsvAveMo;
    cv::morphologyEx(hsvAve, hsvAveMo, cv::MORPH_CLOSE, Mat());

    // ���̈撊�o
    std::vector< Mat > rgns;
    std::vector< Rect > rects;
    SplitImageRegion(hsvAveMo, rgns, rects);
    if (rgns.size() == 0) {
        // ���ɂȂ�̈悪�Ȃ��ꍇ�͏I��
        if (drawResult) Labeling(dst, "Looking for red", Point(0, 0), 0.375, Scalar(0, 200, 200));
        return Result::LookingForRed;
    }

    // �d�S�擾
    std::vector< Point > cogs;
    GetCOG(rgns, rects, cogs);
    if (drawResult) {
        // �ԐM���d�S�̕`��
        for (int i = 0; i < cogs.size(); i++)
            cv::drawMarker(dst, cogs[i], Scalar(255, 0, 255), cv::MARKER_TILTED_CROSS, 7);
    }

    // ���摜�擾
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
        // ���ɂȂ�摜���Ȃ��ꍇ�͏I��
        if (drawResult) Labeling(dst, "Looking for red", Point(0, 0), 0.375, Scalar(0, 200, 200));
        return Result::LookingForRed;
    }
    if (drawResult)
    {
        // ���摜�̋�`�`��
        for (int i = 0; i < cogs.size(); i++) {
            cv::Rect rct(cogs[i].x + sl->x, cogs[i].y + sl->y, sl->width, sl->height);
            cv::rectangle(dst, rct, Scalar(255, 255, 255));
        }
    }


    // ���֌W��������臒l�ȏ�ł���΁A�ԐM�������Ƃ��A��������ꍇ�͍ł��������̂Ƃ���
    double maxCC = -1.0;
    Mat redSignal;
    Rect redSignalRect;
    size_t redIndex;
    for (int i = 0; i < candImg.size(); i++)
    {
        // �܂��͌��P�ɑ΂���e���v���[�g���ł̍ō��l���o��
        std::vector<double> r;
        for (int j = 0; j < RED_SIGNAL_FILE_CNT; j++)
            r.push_back(CompareImages(RED_SIGNAL_BASE_IMG[j], candImg[i]));
        size_t r_max_index = std::distance(r.begin(), std::max_element(r.begin(), r.end()));
        double r_max = r[r_max_index];

        // ���܂Ō������ǂ���΁A�X�V
        if (r_max >= red_threshold && r_max > maxCC) {
            maxCC = r_max;
            redSignal = candImg[i];
            redSignalRect = candImgRect[i];
            redIndex = r_max_index;
        }

        // �ԐM�����֌W���̕`��
        if (drawResult) {
            Point pt = Point(candImgRect[i].x + candImgRect[i].width, candImgRect[i].y);
            if (r_max >= red_threshold)
                Labeling(dst, std::to_string(r_max), pt, 0.3, Scalar(0, 0, 200));
            else
                Labeling(dst, std::to_string(r_max), pt, 0.3);
        }
    }
    if (maxCC < red_threshold) {
        // 臒l�𒴂�����̂��Ȃ���ΏI��
        if (drawResult) Labeling(dst, "Looking for red", Point(0, 0), 0.375, Scalar(0, 200, 200));
        return Result::LookingForRed;
    }

    //
    // �����܂ŗ�����ԐM�����ۂ����̂𔭌�!
    //
    _redSigImgBuf.AddImage(redSignal);
    _redCCBuf.AddDouble(maxCC);
    if (drawResult)
        cv::rectangle(dst, redSignalRect, Scalar(0, 0, 255));

    // �ԐM���̃o�b�t�@����t�łȂ���΁A�ԐM���摜���X�Ɏ��W���邽�߂ɏI��
    if (!_redSigImgBuf.IsFilled()) {
        if (drawResult) Labeling(dst, "Looking for red", Point(0, 0), 0.375, Scalar(0, 200, 200));
        return Result::LookingForRed;
    }

    // �ŗǂ̐ԐM���摜������
    int maxIndex = _redCCBuf.GetMaxIndex();
    this->_redSignal = _redSigImgBuf[maxIndex].clone();

    // �ԐM���T�����I�����A�M���҂��ɑJ�ڂ��邷��
    MoveMode(Mode::WaitingBlue);
    if (drawResult) {
        Labeling(dst, "Found red signal", Point(0, 0), 0.375, Scalar(0, 200, 0));
        // �E��ɐԐM���e���v���[�g�摜��\��
        Mat tmpArea = dst(Rect(dst.cols - _redSignal.cols, 0, _redSignal.cols, _redSignal.rows));
        _redSignal.copyTo(tmpArea);
    }



    return Result::FoundRed;
}

// �M����҂������s��
Result SignalFinder2019::WaitForBlueSignal(Mat& img, Mat& dst, bool drawResult)
{
    assert(_mode == Mode::WaitingBlue);
    assert(_redAreaBuf.IsFilled());


    const int gaussKSize = 5;          // �K�E�X�t�B���^�̃J�[�l���T�C�Y
    const double gaussSigma = 3.0;     // �K�E�X�t�B���^�̔��a
    const double red_threshold = 0.925;  // �ԐM���̑��֌W��臒l
    const double blu_threshold = 0.8;  // �M���̑��֌W��臒l

                                         // �`�揀��
    if (drawResult) {
        // �`��p�̌��摜�̃N���[��
        dst = img.clone();

        // �E��ɐԐM���e���v���[�g�摜��\��
        Mat tmpArea = dst(Rect(dst.cols - _redSignal.cols, 0, _redSignal.cols, _redSignal.rows));
        _redSignal.copyTo(tmpArea);
    }

    // �K�E�V�A������
    Mat gaussImg;
    Gaussian(img, gaussImg, gaussKSize, gaussSigma, true);

    // HSV��臒l����
    Mat hsvRes;
    GetRedSignalArea(gaussImg, hsvRes);
    _redAreaBuf.AddImage(hsvRes);

    // ���ω摜���擾
    Mat hsvAve;
    _redAreaBuf.AverageImage(hsvAve);
    cv::threshold(hsvAve, hsvAve, 100, 255, CV_THRESH_BINARY);

    // �N���[�W���O����(�c��->���k)
    cv::Mat hsvAveMo;
    cv::morphologyEx(hsvAve, hsvAveMo, cv::MORPH_CLOSE, Mat());

    // ���̈撊�o
    std::vector< Mat > rgns;
    std::vector< Rect > rects;
    SplitImageRegion(hsvAveMo, rgns, rects);

    // ���ɂȂ�̈悪�Ȃ��ꍇ�͏I��
    if (rgns.size() == 0) {
        _redSignalLostCounter++;

        // MAX_LOST_FRAMES�ɒB���Ă�����A�ĂѐԐM����T�����[�h�ֈڍs
        if (_redSignalLostCounter == MAX_LOST_FRAMES) {
            if (drawResult) Labeling(dst, "Lost red signal!", Point(0, 0), 0.375, Scalar(0, 200, 0));
            MoveMode(Mode::FindingRed);
            return Result::LostRed;
        }
        // MAX_LOST_FRAMES�ɒB���Ă��Ȃ���΁A�P�ɏI��
        else {
            if (drawResult) Labeling(dst, "Waiting for blue", Point(0, 0), 0.375, Scalar(0, 0, 200));
            return Result::WaitingForBlue;
        }
    }

    // �d�S�擾
    std::vector< Point > cogs;
    GetCOG(rgns, rects, cogs);
    if (drawResult) {
        // �ԐM���d�S�̕`��
        for (int i = 0; i < cogs.size(); i++)
            cv::drawMarker(dst, cogs[i], Scalar(255, 0, 255), cv::MARKER_TILTED_CROSS, 7);
    }

    // ���摜�擾
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

    // ���ɂȂ�摜���Ȃ��ꍇ�͏I��
    if (candImg.size() == 0) {
        _redSignalLostCounter++;

        // MAX_LOST_FRAMES�ɒB���Ă�����A�ĂѐԐM����T�����[�h�ֈڍs
        if (_redSignalLostCounter == MAX_LOST_FRAMES) {
            if (drawResult) Labeling(dst, "Lost red signal!", Point(0, 0), 0.375, Scalar(0, 200, 0));
            MoveMode(Mode::FindingRed);
            return Result::LostRed;
        }
        // MAX_LOST_FRAMES�ɒB���Ă��Ȃ���΁A�P�ɏI��
        else {
            if (drawResult) Labeling(dst, "Waiting for blue", Point(0, 0), 0.375, Scalar(0, 0, 200));
            return Result::WaitingForBlue;
        }
    }
    if (drawResult)
    {
        // ���摜�`��
        for (int i = 0; i < cogs.size(); i++) {
            cv::Rect rct(cogs[i].x + sl->x, cogs[i].y + sl->y, sl->width, sl->height);
            cv::rectangle(dst, rct, Scalar(255, 255, 255));
        }
    }

    // �摜���ԐM�������f����
    double red_maxCC = -1.0;
    Mat max_redImg;
    Rect max_redRect;
    for (int i = 0; i < candImg.size(); i++)
    {
        // �ԐM���e���v���[�g�Ƃ̑��֌W�����v�Z
        double r = CompareImages(_redSignal, candImg[i]);
        if (r > red_maxCC) {
            red_maxCC = r;
            max_redImg = candImg[i];
            max_redRect = candImgRect[i];
        }
    }

    // �ԐM���̑��֌W���`��
    if (drawResult && candImg.size() > 0) {
        Point pt = Point(max_redRect.x + max_redRect.width, max_redRect.y);
        if (red_maxCC >= red_threshold)
            Labeling(dst, std::to_string(red_maxCC), pt, 0.3, Scalar(0, 0, 200));
        else
            Labeling(dst, std::to_string(red_maxCC), pt, 0.3);

    }

    // �ԐM���ł������ꍇ�̏���
    if (red_maxCC > red_threshold) {
        // �ԐM���e���v���[�g�摜���X�V���ALOST�J�E���^��0�Ƀ��Z�b�g���ďI��
        _redSignal = max_redImg.clone();
        _redSignalLostCounter = 0;
        if (drawResult) {
            rectangle(dst, max_redRect, Scalar(0, 0, 255));
            Labeling(dst, "Waiting for blue", Point(0, 0), 0.375, Scalar(0, 0, 200));
        }
        return Result::WaitingForBlue;
    }

    // �ԐM���łȂ������ꍇ�A�M�������f����
    double blu_maxCC = -1.0;
    Mat bluImg;
    Rect bluRect;
    for (int i = 0; i < candImg.size(); i++)
    {
        // �܂��͌��P�ɑ΂���e���v���[�g���ł̑��֌W���ō��l���o��
        std::vector<double> r;
        for (int j = 0; j < BLUE_SIGNAL_FILE_CNT; j++)
            r.push_back(CompareImages(BLUE_SIGNAL_BASE_IMG[j], candImg[i]));
        size_t r_max_index = std::distance(r.begin(), std::max_element(r.begin(), r.end()));
        double r_max = r[r_max_index];

        // ���܂ł̌������ǂ���΁A�X�V
        if (r_max > blu_maxCC) {
            blu_maxCC = r_max;
            bluImg = candImg[i];
            bluRect = candImgRect[i];
        }
    }

    // ���ʂ̐M���̑��֌W���`��
    if (drawResult && candImg.size() > 0 ) {
        Point pt = Point(bluRect.x + bluRect.width, bluRect.y + 20);
        if (blu_maxCC >= blu_threshold)
            Labeling(dst, std::to_string(blu_maxCC), pt, 0.3, Scalar(200, 200, 0));
        else
            Labeling(dst, std::to_string(blu_maxCC), pt, 0.3);
    }

    // �M�����ۂ������ꍇ�A�ԐM������M���ւ̕ω����`�F�b�N
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

    // �M���łȂ������ꍇ
    if (blu_maxCC < blu_threshold || r2b == 0 )
    {
        _redSignalLostCounter++;

        // MAX_LOST_FRAMES�ɒB���Ă�����A�ĂѐԐM����T�����[�h�ֈڍs
        if (_redSignalLostCounter == MAX_LOST_FRAMES) {
            if (drawResult) Labeling(dst, "Lost red signal!", Point(0, 0), 0.375, Scalar(0, 200, 0));
            MoveMode(Mode::FindingRed);
            return Result::LostRed;
        }
        // MAX_LOST_FRAMES�ɒB���Ă��Ȃ���΁A�P�ɏI��
        else {
            if (drawResult) Labeling(dst, "Waiting for blue", Point(0, 0), 0.375, Scalar(0, 0, 200));
            return Result::WaitingForBlue;
        }

    }

    //
    // �����܂ŗ�����M���Ŕ���!
    //
    MoveMode(Mode::FoundBlue);
    if (drawResult) {
        Labeling(dst, "Found blue signal", Point(0, 0), 0.375, Scalar(255, 0, 0));
    }
    return Result::FoundBlueSignal;
}

Result SignalFinder2019::FoundBlueSignal(Mat& img, Mat& dst, bool drawResult) {

    // �M�����������K��񐔂܂ŏo�͂���
    if (_bluSignalCounter < BLUE_SIGNAL_FRAMES) {

        if (drawResult) {
            Labeling(dst, "Found blue signal", Point(0, 0), 0.375, Scalar(255, 0, 0));
        }
        _bluSignalCounter++;
        return Result::FoundBlueSignal;
    }

    // �A�C�h�����O���[�h�ɑJ�ڂ���
    MoveMode(Mode::Idling);
    if (drawResult) {
        Labeling(dst, "Idling", Point(0, 0), 0.375);
    }
    return Result::FoundBlueSignal;
}

// ���[�h�̑J�ڂ��s��
void SignalFinder2019::MoveMode(Mode mode)
{
    if (mode == Mode::Idling) {
        _mode = Mode::Idling;
    }
    // �ԐM���������[�h�֑J��
    else if (mode == Mode::FindingRed) {
        _mode = Mode::FindingRed;
        _redAreaBuf.Clear();   // HSV�ɂ��Q�l�摜�̃o�b�t�@��������
        _redSigImgBuf.Clear(); // �ԐM���摜�o�b�t�@��������
        _redCCBuf.Clear();     // �ԐM�����֌W���o�b�t�@��������
    }
    else if (mode == Mode::WaitingBlue) {
        _mode = Mode::WaitingBlue;
        _redSignalLostCounter = 0; //�ԐM�������X�g�����񐔂𐔂���J�E���^�[
    }
    else if (mode == Mode::FoundBlue) {
        _mode = Mode::FoundBlue;
        _bluSignalCounter = 1;  // �J�ڂ���Ƃ��͊���1�񔭌����Ă���̂�1����n�܂�
    }
}




// HSV�ɂ��臒l�����ɂ���ĐԐM���̈�摜���擾����
// src�̓J���[�摜, dst��8bit�̂Q�l�摜
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

// �ԐM������M���ɕύX���ꂽ��
int SignalFinder2019::RedToBlueRatio(cv::Mat& red, cv::Mat& blu, cv::Mat& dst)
{
    assert(red.type() == CV_8UC3);
    assert(blu.type() == CV_8UC3);
    assert(red.cols == blu.cols);
    assert(red.rows == blu.rows);

    // ���낢���臒l
    const int th_dif1=140, th_dif2=115;  // ���摜�ł��̊Ԃ̒l�͌��Ȃ�
    const int th_up = 136, th_up_cnt = 10;
    const int th_dn = 115, th_dn_cnt = 20;
    
    // �O���C�摜��
    int h = red.rows, w = red.cols;
    cv::Mat red_gray, blu_gray;
    cvtColor(red, red_gray, CV_BGR2GRAY);
    cvtColor(blu, blu_gray, CV_BGR2GRAY);

    // �����v�Z
    dst = Mat::zeros(red_gray.size(), CV_8U);
    for( int y=0; y<h; y++ ){
        uchar *r_pt = red_gray.data + red_gray.step[0]*y;
        uchar *b_pt = blu_gray.data + blu_gray.step[0]*y;
        uchar *d_pt = dst.data + dst.step[0]*y;

        for( int x=0; x<dst.step[0]; x++ ){
            d_pt[x] = (r_pt[x] - b_pt[x] + 255)/2;  // �����v�Z
        }
    }

    // �Ԃ���֕ω������ dif �摜�͏㔼���͖��邢�~���A�������͈Â��~���ł���͂�
    // 臒l���̏㉺�̕��ςƐ����Z�o����
    int half_h = h/2;

    int cnt_up = 0, ave_up = 0;
    int cnt_dn = 0, ave_dn = 0;
    for( int y=0; y<h; y++ ){
        uchar *pt = dst.data + dst.step[0]*y;

        for( int x=0; x<dst.step[0]; x++ ){
            if( pt[x] < th_dif2 || pt[x] > th_dif1 ){
                // �㔼��
                if( y < half_h ){
                    ave_up += pt[x];
                    cnt_up++;
                }
                // ������
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

    // ���ʃ`�F�b�N
    //imshow( "difference", dst );
    //std::cout << "UP: " << ave_up << ", " << cnt_up << std::endl;
    //std::cout << "DN: " << ave_dn << ", " << cnt_dn << std::endl;

    // �ԐM������M���ւ̕ω�������
    if( ave_up >= th_up && cnt_up >= th_up_cnt 
            && ave_dn <= th_dn && cnt_dn >= th_dn_cnt){
        return 1;
    }
    else
        return 0;


    return 1.0;
}


