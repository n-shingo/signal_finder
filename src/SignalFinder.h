//-------------------------------------------------
// SignalFinder.h
// S. NAKAMURA
// since: 2018-09-17
//-------------------------------------------------

#ifndef __SIGNALFINDER_H__
#define __SIGNALFINDER_H__

#include <opencv2/opencv.hpp>
#include "Buffers.h"

namespace sn
{
    using namespace cv;

    // SignalFinderの状態
    enum Mode {
        Idling,     // アイドリング中
        FindingRed, // 赤信号探索中
        WaitingBlue, // 青信号待ち
        FoundBlue   // 青信号発見
    };

    // AnaylzeImageをした時の結果
    enum Result {
        NoProcess,         // 何もしない
        LookingForRed,     // 赤信号を探している
        FoundRed,          // 赤信号を発見した
        WaitingForBlue,    // 青信号待ち
        LostRed,           // 赤信号を見失った
        FoundBlueSignal,         // 青信号を発見した
        Error              // エラー
    };

    class SignalFinder {

    private:

        //
        // 固定値 (構造体などはコンストラクタで初期化)
        //
        Rect SIGNAL_LOCATE_FROM_RED; // テンプレート相対位置
        const char RED_SIGNAL_FILE[256] = "../res/red_signal_average.bmp";  // 平均赤信号画像
        const char BLUE_SIGNAL_FILE[256] = "../res/blu_signal_average.bmp"; // 平均青信号画像
        Mat RED_SIGNAL_BASE_IMG;
        Mat BLUE_SIGNAL_BASE_IMG;
        const int RED_AREA_BUF_SIZE = 10; // 赤信号を見つけるための画像バッファサイズ
        const int IMAGE_BUF_SIZE = 10; // 取っておく過去の画像のバッファサイズ
        const int RED_SIGNAL_BUF_SIZE = 3;  // 赤信号であるか確認するためのデータのバッファ数
        const int MAX_LOST_FRAMES = 10; // このフレーム数赤信号をロストしたら、赤信号再検出
        const int BLUE_SIGNAL_FRAMES = 5; // 青信号発見からこのフレーム数、発見結果を出力する


    public:
        // コンストラクタ
        SignalFinder()
        {
            this->SIGNAL_LOCATE_FROM_RED = Rect(-8, -8, 18, 32);
            _redAreaBuf.Clear(RED_AREA_BUF_SIZE);
            _redSigImgBuf.Clear(RED_SIGNAL_BUF_SIZE);
            _redCCBuf.Clear(RED_SIGNAL_BUF_SIZE);
            RED_SIGNAL_BASE_IMG = imread(RED_SIGNAL_FILE);
            BLUE_SIGNAL_BASE_IMG = imread(BLUE_SIGNAL_FILE);
        }

        // デストラクタ
        ~SignalFinder() {}

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
        Mat _redSignal; // 赤信号発見時の赤信号画像
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


#endif //__SIGNALFINDER_H__
