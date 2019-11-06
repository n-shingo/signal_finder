//-------------------------------------------------
// SignalFinder2019.h
// S. NAKAMURA
// since: 2019-11-06
//-------------------------------------------------

#ifndef __SIGNALFINDER2019_H__
#define __SIGNALFINDER2019_H__

#include <opencv2/opencv.hpp>
#include "Buffers.h"
#include "MyEnums.h"

#define RED_SIGNAL_FILE_CNT  8  // 赤信号のテンプレート数
#define BLUE_SIGNAL_FILE_CNT 8  // 青信号のテンプレート数

namespace sn
{
    using namespace cv;

    class SignalFinder2019 {


    private:

        //
        // 固定値 (構造体などはコンストラクタで初期化)
        //
        Rect SIGNAL_LOCATE_FROM_RED; // テンプレート相対位置


#ifdef __linux
        const char RED_SIGNAL_FILE[RED_SIGNAL_FILE_CNT][256] = {
            "../res2019/12_01_red.bmp",
            "../res2019/12_02_red.bmp",
            "../res2019/13_01_red.bmp",
            "../res2019/13_02_red.bmp",
            "../res2019/14_01_red.bmp",
            "../res2019/14_02_red.bmp",
            "../res2019/15_01_red.bmp",
            "../res2019/15_02_red.bmp",
    };
        const char BLUE_SIGNAL_FILE[BLUE_SIGNAL_FILE_CNT][256] = {
            "../res2019/12_01_blu.bmp",
            "../res2019/12_02_blu.bmp",
            "../res2019/13_01_blu.bmp",
            "../res2019/13_02_blu.bmp",
            "../res2019/14_01_blu.bmp",
            "../res2019/14_02_blu.bmp",
            "../res2019/15_01_blu.bmp",
            "../res2019/15_02_blu.bmp",
        };
#endif
#ifdef _WIN64
        const char RED_SIGNAL_FILE[RED_SIGNAL_FILE_CNT][256] = {
            "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res2019/12_01_red.bmp",
            "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res2019/12_02_red.bmp",
            "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res2019/13_01_red.bmp",
            "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res2019/13_02_red.bmp",
            "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res2019/14_01_red.bmp",
            "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res2019/14_02_red.bmp",
            "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res2019/15_01_red.bmp",
            "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res2019/15_02_red.bmp",
        };
        const char BLUE_SIGNAL_FILE[BLUE_SIGNAL_FILE_CNT][256] = {
            "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res2019/12_01_blu.bmp",
            "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res2019/12_02_blu.bmp",
            "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res2019/13_01_blu.bmp",
            "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res2019/13_02_blu.bmp",
            "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res2019/14_01_blu.bmp",
            "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res2019/14_02_blu.bmp",
            "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res2019/15_01_blu.bmp",
            "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res2019/15_02_blu.bmp",
        };
#endif
        Mat RED_SIGNAL_BASE_IMG[RED_SIGNAL_FILE_CNT];
        Mat BLUE_SIGNAL_BASE_IMG[BLUE_SIGNAL_FILE_CNT];
        const int RED_AREA_BUF_SIZE = 10; // 赤信号を見つけるための画像バッファサイズ
        const int IMAGE_BUF_SIZE = 10; // 取っておく過去の画像のバッファサイズ
        const int RED_SIGNAL_BUF_SIZE = 3;  // 赤信号であるか確認するためのデータのバッファ数
        const int MAX_LOST_FRAMES = 10; // このフレーム数赤信号をロストしたら、赤信号再検出
        const int BLUE_SIGNAL_FRAMES = 5; // 青信号発見からこのフレーム数、発見結果を出力する


    public:
        // コンストラクタ
        SignalFinder2019()
        {
            this->SIGNAL_LOCATE_FROM_RED = Rect(-8, -8, 18, 32);
            _redAreaBuf.Clear(RED_AREA_BUF_SIZE);
            _redSigImgBuf.Clear(RED_SIGNAL_BUF_SIZE);
            _redCCBuf.Clear(RED_SIGNAL_BUF_SIZE);

            // 赤信号テンプレートファイル
            for(int i=0; i<RED_SIGNAL_FILE_CNT; i++)
                RED_SIGNAL_BASE_IMG[i] = imread(RED_SIGNAL_FILE[i]);

            // 青信号テンプレートファイル
            for( int i=0; i<BLUE_SIGNAL_FILE_CNT; i++)
                BLUE_SIGNAL_BASE_IMG[i] = imread(BLUE_SIGNAL_FILE[i]);
        }

        // デストラクタ
        ~SignalFinder2019() {}

        // 信号検出処理の開始
        void Start();

        // 信号検出処理の終了(強制的にIdling状態にする)
        void Stop();

        // 画像を追加し、解析＆処理を行う
        Result AnalyzeImage(Mat& img, Mat& dst, bool resultImg);



    private:

        //////////////////////
        //    非公開メンバ    //
        //////////////////////

        Mode _mode = Mode::Idling; // 現在のモード
        ImageBuffer _redAreaBuf;   // HSVから赤領域を求めた２値画像を溜めてめておくためのバッファ
        ImageBuffer _redSigImgBuf; // 赤信号と検出された画像のバッファ
        DoubleBuffer _redCCBuf;    // 赤信号との相関係数を溜めておくバッファ
        Mat _redSignal;            // 赤信号発見時の赤信号画像
        int _redSignalLostCounter; // 青信号待ちの時、赤信号をロストした回数を数えるカウンター
        int _bluSignalCounter;     // 青信号発見から経過したフレーム数


                                   //////////////////////
                                   //   非公開メソッド   //
                                   //////////////////////

                                   // アイドリング中の何もしない処理
        Result ProcessNothing(Mat& img, Mat& dst, bool drawResult);

        // 赤信号の探索を行う
        Result TryFindRedSignal(Mat& img, Mat& dst, bool drawResult);

        // 青信号を待つ処理を行う
        Result WaitForBlueSignal(Mat& img, Mat& dst, bool drawResult);

        // 青信号を見つけた後の処理を行う
        Result FoundBlueSignal(Mat& img, Mat& dst, bool drawResult);

        // モード遷移をする
        void MoveMode(Mode mode);


        // HSVによる閾値処理によって赤信号領域画像を取得する
        // srcはカラー画像, dstは8bitの２値画像
        void GetRedSignalArea(cv::Mat& src, cv::Mat& dst);


    };
}


#endif //__SIGNALFINDER2019_H__
