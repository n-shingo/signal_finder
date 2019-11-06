//-------------------------------------------------
// ToolFunc.h
// S. NAKAMURA
// since: 2018-09-17
//-------------------------------------------------

#ifndef __TOOLFUNC_H__
#define __TOOLFUNC_H__

#include "opencv2/opencv.hpp"

namespace sn {
    namespace Tool {

        using namespace cv;

        // �K�E�V�A���t�B���^��������
        void Gaussian(Mat& src, Mat& dst, int size, double sigma, bool inplace = false);

        // RGB�摜��HSV�����ɕ�������
        void SplitToHSV(Mat& src, Mat& dstH, Mat& dstS, Mat &dstV);

        //
        // �Ȃ����Ă���(8�ߖT)�̈斈�ɉ摜�𕪊�����.
        // dst�͕������ꂽ�摜�Q, rects�͌��摜�ɑ΂����`�̈��\��
        // �h�b�g�̂悤��1�_�̃f�[�^�͏o�̓f�[�^�Ɋ܂߂Ȃ�
        //
        void SplitImageRegion(Mat &gray, std::vector< Mat > &dst, std::vector< Rect > &rects);

        // �d�S���擾����
        void GetCOG(std::vector< Mat > &regions, std::vector< Rect > &rects, std::vector< Point > &cog);

        // �Q�̉摜���ׁA���֌W����Ԃ�
        double CompareImages(Mat& img1, Mat& img2);

        // ���x����`�悷��
        void Labeling(Mat& img, String str, Point pos = Point(0, 0), double fontscale = 0.375, Scalar bgColor = Scalar(0., 0., 0.));

    }
}


#endif //__TOOLFUNC_H__
