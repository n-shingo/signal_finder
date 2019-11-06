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

        // ガウシアンフィルタをかける
        void Gaussian(Mat& src, Mat& dst, int size, double sigma, bool inplace = false);

        // RGB画像をHSV成分に分離する
        void SplitToHSV(Mat& src, Mat& dstH, Mat& dstS, Mat &dstV);

        //
        // つながっている(8近傍)領域毎に画像を分割する.
        // dstは分割された画像群, rectsは元画像に対する矩形領域を表す
        // ドットのような1点のデータは出力データに含めない
        //
        void SplitImageRegion(Mat &gray, std::vector< Mat > &dst, std::vector< Rect > &rects);

        // 重心を取得する
        void GetCOG(std::vector< Mat > &regions, std::vector< Rect > &rects, std::vector< Point > &cog);

        // ２つの画像を比べ、相関係数を返す
        double CompareImages(Mat& img1, Mat& img2);

        // ラベルを描画する
        void Labeling(Mat& img, String str, Point pos = Point(0, 0), double fontscale = 0.375, Scalar bgColor = Scalar(0., 0., 0.));

    }
}


#endif //__TOOLFUNC_H__
