//-------------------------------------------------
//edge-detector.cpp
//S. Nakamrau
//since: 2018-10-05
//-------------------------------------------------

#include <cstdio>
#include <iostream>
#include <signal.h>
#include "opencv2/opencv.hpp"
#include <ssm.hpp>
#include <kaw_lib/SSM-Image.hpp>
#include "SignalFinder.h"
#include "signalResultSSM.h"

using namespace std;
using namespace cv;
using namespace sn;

bool gShutOff;

// シグナルハンドラ
void ctrlC(int aStatus)
{
    signal(SIGINT, NULL);
    gShutOff = true;
}

// Ctrl-C による正常終了を設定
inline void setSigInt(){ signal(SIGINT, ctrlC); }


// ヘルプを表示関数
void showHelp(void);


int main(int argc, char ** argv)
{

    //==========================================================
    // ---> DECLARATION
    //==========================================================
    int ssm_id = 0;
    bool result_view = false;
    Rect procArea(80, 80, 480, 160);

    // キャプチャ用フレーム画像と結果画像
    Mat frm, res_img;

    // 画像から信号検出するクラス
    SignalFinder finder;

    // <--- DECLARATION

    //==========================================================
    // ---> INITALIZE
    //==========================================================
    //--------------------------------------
    // オプション解析
    int c;
    while( (c = getopt(argc, argv, "n:vh")) != -1 )
    {
        switch ( c )
        {
        case 'n':
            fprintf( stderr, "input ssm id = %d\n", atoi(optarg) );
            ssm_id = atoi(optarg);
            break;
        case 'v':
            result_view = true;
            break;
        case 'h':
        default:
            showHelp();
            exit(0);
        }
    }


    // ssm関連の初期化
    if(!initSSM()){
        cerr << "SSM Error : initSSM()" << endl;
        return 0;
    }

    // 画像取得のためのssmのオープン
    SSMApi<ImageC3> cam_image(SNAME_IMGC3, ssm_id);
    if( !cam_image.open( ) ){
        cerr << "SSM Error : open()" << endl;
        return 1;
    }

    // ssmにSignalResultを書き込む準備
    SSMApi<SignalResult> signalResultSsm(SNAME_SIGNALRESULT, ssm_id);
    // create( センサデータ保持時間[sec], おおよそのデータ更新周期[sec] )
    if( !signalResultSsm.create( 1.0, 1.0/30.0  ) ){
        cerr << "SSM Error : create()" << endl;
        return 1;
    }

    // Ctlr-Cの動作登録
    setSigInt();

    // <--- INITALIZE

    //==========================================================
    // ---> OPERATION
    //==========================================================
    //ループ処理の開始
    cerr << "Main Loop Started" << endl;

    while(!gShutOff){
        char key;

        // ssmから最新画像取得
        if(cam_image.readNew()){

            // cv::Matに変換
            ImageC3_to_Mat(cam_image.data,&frm);

            // 処理エリアを取り出す
            Mat roi = frm(procArea);
            Mat pro_frm = roi.clone();

            // 処理
            finder.AnalyzeImage( pro_frm, res_img, true );
            res_img.copyTo(roi);
            rectangle(frm, procArea, Scalar(0,0,0));

            // 結果
            SignalResult res;
            res.res = -1;

            // SSMにEdgePos書き込み
            signalResultSsm.data = res;
            signalResultSsm.write(cam_image.time);

            // -vオプションであれば画像表示
            if( result_view ){
                imshow( "find-signal result", frm); // 結果画像
            }

        }

        usleep(25000);

        key = waitKey(1);
        switch(key){
        case 27: // ESC
            gShutOff = true;
            break;
        case 's':
            finder.Start();
            break;
        case 'i':
            finder.Stop();
        default:
            break;
        }
    }
    // <--- OPERATION
    

    //==========================================================
    // ---> FINALIZE
    //==========================================================
    cam_image.close();
    signalResultSsm.close();

    endSSM();
    cerr << "End SSM" << endl;
    // <--- FINALIZE

    cout << "End Successfully" << endl;

    return 0;
}

void showHelp(void){

    // 書式
    fprintf( stdout, "\n\n" );
    fprintf( stdout, "\033[1m書式\033[0m\n" );
    fprintf( stdout, "\t\033[1mfind-signal\033[0m [-n ssmID] [-v]\n" );
    fprintf( stdout, "\t\033[1mfind-signal\033[0m [-h]\n" );
    fprintf( stdout, "\n" );

    // 説明
    fprintf( stdout, "\n" );
    fprintf( stdout, "\033[1m説明\033[0m\n" );
    fprintf( stdout, "\t\033[1m-n\033[0m\tSSMのIDを指定する\n" );
    fprintf( stdout, "\t\033[1m-v\033[0m\t画像処理結果を表示する\n" );
    fprintf( stdout, "\t\033[1m-h\033[0m\tこのヘルプを表示する\n" );
    fprintf( stdout, "\n" );

    // 使用方法
    fprintf( stdout, "\n\033[1m使用方法\033[0m\n");
    fprintf( stdout, "\t\033[1m%s\033[0m\t信号検知モードにする\n", "S キー");
    fprintf( stdout, "\t\033[1m%s\033[0m\tアイドリングモードにする\n", "I キー");
    fprintf( stdout, "\t\033[1m%s\033[0m\t終了にする\n", "ESCキー");
    fprintf( stdout, "\n");

}
