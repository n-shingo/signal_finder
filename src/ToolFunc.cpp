//-------------------------------------------------
// ToolFunc.h
// S. NAKAMURA
// since: 2018-09-17
//-------------------------------------------------

#include "ToolFunc.h"


using namespace cv;

// �K�E�V�A���t�B���^��������
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

// RGB�摜��HSV�����ɕ�������
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
// �Ȃ����Ă���(8�ߖT)�̈斈�ɉ摜�𕪊�����.
// dst�͕������ꂽ�摜�Q, rects�͌��摜�ɑ΂����`�̈��\��
// �h�b�g�̂悤��1�_�̃f�[�^�͏o�̓f�[�^�Ɋ܂߂Ȃ�
//
void sn::Tool::SplitImageRegion(Mat &gray, std::vector< Mat > &dst, std::vector< Rect > &rects)
{
    using namespace std;

    assert(gray.type() == CV_8UC1);

    int cols = gray.cols;
    int rows = gray.rows;
    dst.clear();
    rects.clear();

    Mat clone = gray.clone(); // �̈悪��������邲�Ƃɂǂ�ǂ񍕂��Ȃ��Ă����摜
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

                // countours���[���̎�(�h�b�g���ȁH�j�͕����Ɋ܂߂Ȃ�
                if (contours.size() == 0) continue;

                // contours�̐����H��2�ȏ�̎�������̂ŁA���̎��͋��E���̑������̂��̗p����
                int max = 0;
                for (int i = 1; i < contours.size(); i++)
                    if (contours[max].size() > contours[i].size())
                        max = i;

                // �̈�擾
                Rect rect1 = boundingRect(contours[max]);

                // �摜�쐬
                Mat img(rect1.size(), CV_8UC1);

                // �R�s�[����
                Mat src_roi(origin, rect1);
                src_roi.copyTo(img);

                // vector�Ɏ��߂�
                dst.push_back(img);
                rects.push_back(rect1);
            }
        }
    }
}

// �d�S���擾����
void sn::Tool::GetCOG(std::vector< Mat > &regions, std::vector< Rect > &rects, std::vector< Point > &cog)
{
    assert(regions.size() == rects.size());

    cog.clear();

    for (int i = 0; i < regions.size(); i++)
    {
        // �e�d�S�����߂�
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

// �Q�̉摜���ׁA���֌W����Ԃ�
double sn::Tool::CompareImages(Mat& img1, Mat& img2)
{
    assert(img1.rows == img2.rows);
    assert(img1.cols == img2.cols);
    assert(img1.type() == img2.type());

    int h = img1.rows;
    int w = img1.cols;

    // ���ς����߂�
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

    // ���U�����߂�
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

    // ���֌W��
    double r = sumab / sqrt(sumaa * sumbb);
    return r;
}

// ���x����`�悷��
void sn::Tool::Labeling(Mat& img, String str, Point pos, double fontscale, Scalar bgColor) {

    // �����̈�̃T�C�Y�擾
    const int fontface = cv::FONT_HERSHEY_TRIPLEX;
    const int thickness = 1;
    const int linetype = CV_AA;
    int baseline = 0;
    const cv::Size textsize = cv::getTextSize(str, fontface, fontscale, thickness, &baseline);

    // �w�i�F����
    if (img.type() == CV_8U) {
        bgColor = Scalar(bgColor[0]);
    }

    // ���x������
    const int mgn = 5;
    const int w = textsize.width + 2 * mgn;
    const int h = textsize.height + 2 * mgn + baseline;
    cv::Size size(w, h);
    cv::Mat label = cv::Mat(size, img.type(), bgColor);

    // ���x���ɕ����`��
    cv::Point textPos((size.width - textsize.width) / 2, (size.height + textsize.height - baseline) / 2);
    cv::putText(label, str, textPos, fontface, fontscale, cvScalar(255, 255, 255), thickness, linetype);

    // ���x�����摜�ɓ\���k��
    if (pos.x >= 0 && pos.y >= 0 && pos.x + w < img.cols && pos.y + h < img.rows) {
        cv::Mat roi = img(cv::Rect(pos.x, pos.y, w, h));
        label.copyTo(roi);
    }
}
