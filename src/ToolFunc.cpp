//-------------------------------------------------
// ToolFunc.h
// S. NAKAMURA
// since: 2018-09-17
//-------------------------------------------------

#include "ToolFunc.h"


using namespace cv;

// ガウシアンフィルタをかける
void sn::Tool::Gaussian(cv::Mat& src, cv::Mat& dst, int size, double sigma, bool inplace)
{
    if (inplace)
    {
        cv::Mat tmp(src.size(), src.type());
        cv::GaussianBlur(src, tmp, cv::Size(size, size), sigma, sigma);
        dst = tmp.clone();
    }
    else
    {
        dst = cv::Mat(src.size(), src.type());
        cv::GaussianBlur(src, dst, cv::Size(size, size), sigma, sigma);
    }

}

// RGB画像をHSV成分に分離する
void sn::Tool::SplitToHSV(cv::Mat& src, cv::Mat& dstH, cv::Mat& dstS, cv::Mat &dstV)
{
    assert(src.type() == CV_8UC3);

    cv::Mat hsvImg;
    cv::cvtColor(src, hsvImg, CV_BGR2HSV);
    dstH = cv::Mat(src.size(), CV_8U);
    dstS = cv::Mat(src.size(), CV_8U);
    dstV = cv::Mat(src.size(), CV_8U);
    cv::Mat hsv[] = { dstH, dstS, dstV };
    cv::split(hsvImg, hsv);
}


//
// つながっている(8近傍)領域毎に画像を分割する.
// dstは分割された画像群, rectsは元画像に対する矩形領域を表す
// ドットのような1点のデータは出力データに含めない
//
void sn::Tool::SplitImageRegion(Mat &gray, std::vector< Mat > &dst, std::vector< Rect > &rects)
{
    using namespace std;

    assert(gray.type() == CV_8UC1);

    int cols = gray.cols;
    int rows = gray.rows;
    dst.clear();
    rects.clear();

    Mat clone = gray.clone(); // 領域が処理されるごとにどんどん黒くなっていく画像
    uchar* ptr;
    for (int i = 0; i < rows; i++) {
        ptr = clone.data + i * clone.step;
        for (int j = 0; j < cols; j++) {
            if (ptr[j] == 255) {
                Mat origin = clone.clone();
                floodFill(clone, Point(j, i), cvScalar(0), NULL, Scalar(), Scalar(), 8);
                bitwise_xor(origin, clone, origin);

                vector< vector <Point> > contours;
                vector< Vec4i > hierarchy;
                Mat contImg = origin.clone();
                findContours(contImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

                // countoursがゼロの時(ドットかな？）は分割に含めない
                if (contours.size() == 0) continue;

                // contoursの数が稀に2以上の時があるので、その時は境界数の多いものを採用する
                int max = 0;
                for (int i = 1; i < contours.size(); i++)
                    if (contours[max].size() > contours[i].size())
                        max = i;

                // 領域取得
                Rect rect1 = boundingRect(contours[max]);

                // 画像作成
                Mat img(rect1.size(), CV_8UC1);

                // コピーする
                Mat src_roi(origin, rect1);
                src_roi.copyTo(img);

                // vectorに収める
                dst.push_back(img);
                rects.push_back(rect1);
            }
        }
    }
}

// 重心を取得する
void sn::Tool::GetCOG(std::vector< Mat > &regions, std::vector< Rect > &rects, std::vector< Point > &cog)
{
    assert(regions.size() == rects.size());

    cog.clear();

    for (int i = 0; i < regions.size(); i++)
    {
        // 各重心を求める
        cv::Mat img = regions[i];
        cv::Rect rct = rects[i];
        int h = img.rows, w = img.cols;
        int sumx = 0, sumy = 0, cnt = 0;

        for (int y = 0; y < h; y++)
        {
            uchar* ptr = img.data + img.step[0] * y;
            for (int x = 0; x < w; x++)
            {
                if (ptr[x] == 255) {
                    sumx += x;
                    sumy += y;
                    cnt++;
                }
            }
        }
        int cogx = (int)(sumx / (double)cnt + 0.5);
        int cogy = (int)(sumy / (double)cnt + 0.5);
        cog.push_back(cv::Point(rct.x + cogx, rct.y + cogy));
    }
}

// ２つの画像を比べ、相関係数を返す
double sn::Tool::CompareImages(Mat& img1, Mat& img2)
{
    assert(img1.rows == img2.rows);
    assert(img1.cols == img2.cols);
    assert(img1.type() == img2.type());

    int h = img1.rows;
    int w = img1.cols;

    // 平均を求める
    double ave1 = 0.0, ave2 = 0.0;
    for (int y = 0; y < h; y++)
    {
        uchar *p1 = img1.data + img1.step[0] * y;
        uchar *p2 = img2.data + img2.step[0] * y;

        for (int x = 0; x < img1.step[0]; x++)
        {
            ave1 += p1[x];
            ave2 += p2[x];
        }
    }
    ave1 /= (h*img1.step[0]);
    ave2 /= (h*img1.step[0]);

    // 分散を求める
    double sumaa = 0.0, sumbb = 0.0, sumab = 0.0;
    for (int y = 0; y < h; y++)
    {
        uchar *p1 = img1.data + img1.step[0] * y;
        uchar *p2 = img2.data + img2.step[0] * y;

        for (int x = 0; x < img1.step[0]; x++)
        {
            double da = p1[x] - ave1;
            double db = p2[x] - ave2;
            sumab += da * db;
            sumaa += da * da;
            sumbb += db * db;
        }
    }

    // 相関係数
    double r = sumab / sqrt(sumaa * sumbb);
    return r;
}

// ラベルを描画する
void sn::Tool::Labeling(Mat& img, String str, Point pos, double fontscale, Scalar bgColor) {

    // 文字領域のサイズ取得
    const int fontface = cv::FONT_HERSHEY_TRIPLEX;
    const int thickness = 1;
    const int linetype = CV_AA;
    int baseline = 0;
    const cv::Size textsize = cv::getTextSize(str, fontface, fontscale, thickness, &baseline);

    // 背景色準備
    if (img.type() == CV_8U) {
        bgColor = Scalar(bgColor[0]);
    }

    // ラベル準備
    const int mgn = 5;
    const int w = textsize.width + 2 * mgn;
    const int h = textsize.height + 2 * mgn + baseline;
    cv::Size size(w, h);
    cv::Mat label = cv::Mat(size, img.type(), bgColor);

    // ラベルに文字描画
    cv::Point textPos((size.width - textsize.width) / 2, (size.height + textsize.height - baseline) / 2);
    cv::putText(label, str, textPos, fontface, fontscale, cvScalar(255, 255, 255), thickness, linetype);

    // ラベルを画像に貼りつkる
    if (pos.x >= 0 && pos.y >= 0 && pos.x + w < img.cols && pos.y + h < img.rows) {
        cv::Mat roi = img(cv::Rect(pos.x, pos.y, w, h));
        label.copyTo(roi);
    }
}
