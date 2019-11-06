//-------------------------------------------------
// TestMain.cpp
// S. NAKAMURA
// since: 2018-09-17
//-------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "Buffers.h"
#include "SignalFinder.h"

using namespace cv;


// 画像フォルダ
const char imgDir[][100] = {
    /*
    "/home/shingo/ssm/toBmp/bin/2018.0630.1377.bmp",     // 1個で取れる。ただし一瞬ドーナツ型になる
    "/home/shingo/ssm/toBmp/bin/2018.0630.1377.bmp",     // 1個で取れる。
    "/home/shingo/ssm/toBmp/bin/2018.0804.1129.bmp",     // 1個で取れる。サイズが小さい。
    "/home/shingo/ssm/toBmp/bin/2018.0804.1129.bmp",     // 1個で取れる。
    "/home/shingo/ssm/toBmp/bin/2018.0915.1116.bmp",     // １個で取れる。信号１で、トラック通過で一瞬消える。
    "/home/shingo/ssm/toBmp/bin/2018.0915.1116.bmp",     // １個で取れる。
    "/home/shingo/ssm/toBmp/bin/2018.0915.1348.bmp",     // 1個で取れる。何も問題なし
    "/home/shingo/ssm/toBmp/bin/2018.0915.1348.bmp",     // 1個で取れる。何も問題なし
    "/home/shingo/ssm/toBmp/bin/2018.0915.1427.bmp",     // 1個で取れる。問題なし。信号１で人が下でウロウロしている
    "/home/shingo/ssm/toBmp/bin/2018.0915.1427.bmp",     // 1個で取れる。問題なし。信号１で人が下でウロウロしている
    "/home/shingo/ssm/toBmp/bin/2018.0915.1502.bmp",      // ２個以上取れる時がある。赤い車に反応。  
    "/home/shingo/ssm/toBmp/bin/2018.0915.1502.bmp",     // ２個取れる時がある。帽子に反応。  
    "/home/shingo/ssm/toBmp/bin/2018.1110.1306.bmp",     // 逆光 1
    "/home/shingo/ssm/toBmp/bin/2018.1110.1306.bmp",     // 逆光 2
    "/home/shingo/ssm/toBmp/bin/2018.1110.1306.bmp",     // 逆光 3
    "/home/shingo/ssm/toBmp/bin/2018.1110.1321.bmp",     // 逆光 4
    "/home/shingo/ssm/toBmp/bin/2018.1110.1321.bmp",     // 普通の信号
    */

    "/home/shingo/ssm/toBmp/bin/2019.1021.1201.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1201.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1201.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1201.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1201.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1201.bmp",

    "/home/shingo/ssm/toBmp/bin/2019.1021.1423.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1423.bmp",

    "/home/shingo/ssm/toBmp/bin/2019.1021.1524.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1524.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1524.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1524.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1524.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1524.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1524.bmp",
    
    "/home/shingo/ssm/toBmp/bin/2019.1021.1547.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1547.bmp",
};

// 画像番号
const int imgNum[][2] = {
    /*
    {1434,1800},
    {2487,2783},
    {740,838},
    {1742,1968},
    {665,1040},
    {1921,2220},
    {82,500},
    {1276,1540},
    {43,435},
    {1281,1475},
    {51,524},
    {1377,1650},
    {0,440},   // 411で青
    {759, 1200}, // 1170で赤青両方点灯、1171で青だけ点灯
    {1526,1970}, // 1930で青赤両方消灯、1931で青だけ点灯
    {0, 250}, // 231で青
    {1758, 2245}, // 2164で青
    */

    {0,500},
    {793, 1250},
    {1530, 2000},
    {2320, 2760},
    {3050, 3520},
    {3800, 4320},

    {180, 640},
    {920, 1390},
    
    {223, 730},
    {1000, 1480},
    {2675, 3010},
    {3300, 3800},
    {4030, 4520},
    {4780, 5280},
    {5538, 6055},

    {132, 600},
    {844, 1340},
};

// 処理エリア
const cv::Rect procArea(80, 80, 480, 160);

// テンプレート位置
const cv::Rect tmplate1(-8, -8, 18, 32);



//
// ユーティリティ関数
//

// 処理対象の総画像数を取得
int getTotalNum(void);

// 番号から画像のファイルパスを取得する
void getFilePath(int num, char* path);

// ガンマ補正
void gammaCrrection( Mat& src, Mat& dst, double gamma );

// 単純に画像を暗くする
void getDark( Mat& src, Mat&dst, double ratio );

// main関数
int main(void)
{
    int num = 0;
    int totalNum = getTotalNum();
    char fn[256];
    Mat img, dst, imgHSV[3];
    sn::SignalFinder sf;
    int mode = 0;  // 0:手動, 1:自動再生


    while (1) {
        getFilePath(num, fn);
        std::cout << fn << std::endl;
        img = imread(fn);
        dst = img(procArea);
        img = dst.clone();
        imshow("Original", img);

        Mat gamma_img = dst.clone();
        gammaCrrection(img,gamma_img, 0.7);
        Mat dark_img = gamma_img.clone();
        //getDark(gamma_img, dark_img, 0.7 );
        sf.AnalyzeImage(dark_img, dst, true);
        imshow("Original2", dst);

        int key;
        if (mode == 0)
            key = waitKey(0);
        else
            key = waitKey(40);

        if (key == 27) break; //esc
        else if (key == 'j') {
            mode = 0;
            num++;
        }
        else if (key == 'k') {
            mode = 0;
            num--;
        }
        else if (key == 'i'){
            sf.Stop();
        }
        else if (key == 's') {
            sf.Start();
        }
        else if (key == 'p') {
            mode = 1;
        }
        else {
            num++;
        }


        if (num < 0)  num = totalNum - 1;
        else if (num >= totalNum) num = 0;


    }

    cv::destroyAllWindows();


    return 0;
}





// 画像処理のトータル数を取得する
int getTotalNum(void)
{
    static int dirCnt = sizeof(imgNum) / (2 * sizeof(imgNum[0][0]));
    static int total = 0;

    if (total != 0) return total;

    for (int i = 0; i < dirCnt; i++)
        total += imgNum[i][1] - imgNum[i][0];


    return total;
}

// 番号からファイルパスを取得
void getFilePath(int num, char* path)
{
    static int dirCnt = sizeof(imgNum) / (2 * sizeof(imgNum[0][0]));
    int total = getTotalNum();
    if (num < 0) num += total;
    else if (num >= total) num -= total;
    assert(0 <= num && num < total);

    for (int i = 0; i < dirCnt; i++) {
        int cnt = imgNum[i][1] - imgNum[i][0];
        if (num - cnt < 0) {
            int filenum = imgNum[i][0] + num;
            sprintf(path, "%s/image%05d.bmp", imgDir[i], filenum);
            return;
        }
        else
            num -= cnt;
    }

}


void gammaCrrection( Mat& src, Mat& dst, double gamma )
{
    uchar lut[256];
    for( int i=0; i<256; i++ ){
        lut[i] = pow(i / 255.0, 1.0/gamma) * 255.0;
    }
    cv::LUT(src, Mat(Size(256,1), CV_8U, lut), dst);
}

void getDark( Mat& src, Mat&dst, double ratio )
{
    uchar lut[256];
    for( int i=0; i<256; i++ ){
        lut[i] = (int)(i*ratio);
    }
    cv::LUT(src, Mat(Size(256,1), CV_8U, lut), dst);
}
