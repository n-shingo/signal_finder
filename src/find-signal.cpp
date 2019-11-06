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
#include "SignalFinder2019.h"
#include "signalResultSSM.h"
#include "tc2018_typedef.hpp"
#include <ypspur.h>

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
    bool show_result = true;
    bool read_robot = true;
    Rect procArea(80, 80, 480, 160);
    bool flag_analysis = false;

    // キャプチャ用フレーム画像と結果画像
    Mat frm, res_img;

    // ロボット情報
    int new_stop_type=-256, old_stop_type=-256;

    // 画像から信号検出するクラス
    //SignalFinder finder;
    SignalFinder2019 finder;

    // <--- DECLARATION

    //==========================================================
    // ---> INITALIZE
    //==========================================================
    //--------------------------------------
    // オプション解析
    int c;
    while( (c = getopt(argc, argv, "n:vrah")) != -1 )
    {
        switch ( c )
        {
        case 'n':
            fprintf( stderr, "input ssm id = %d\n", atoi(optarg) );
            ssm_id = atoi(optarg);
            break;
        case 'v':
            show_result = false;
            break;
        case 'r':
            read_robot = false;
            break;
        case 'a':
            flag_analysis = true;
            break;
        case 'h':
        default:
            showHelp();
            exit(0);
        }
    }
    
    // YPSpurの初期化
    if( !flag_analysis ){
        if( Spur_init(  ) < 0 ){
            fprintf( stderr, "[\033[1m\033[31mERROR\033[30m\033[0m]: cannot open spur.\n" );
            exit( EXIT_FAILURE );
        }
    }

    // ssm関連の初期化
    if(!initSSM()){
        cerr << "SSM Error : initSSM()" << endl;
        return EXIT_FAILURE;
    }

    // 画像取得のためのssmのオープン
    SSMApi<ImageC3> cam_image(SNAME_IMGC3, ssm_id);
    if( !cam_image.open( ) ){
        cerr << "SSM Error : open()" << endl;
        return EXIT_FAILURE;
    }

    // ロボット情報取得のためのssmのオープン
    SSMApi<wp_gl> wpglSsm(WP_SNAME, ssm_id);
    if( read_robot ){
        if( !wpglSsm.open() ){
            cerr << "Failed to open ssm for wp_gl" << endl;
            return EXIT_FAILURE;
        }
    }

    // ssmにSignalResultを書き込む準備
    SSMApi<SignalResult> signalResultSsm(SNAME_SIGNALRESULT, ssm_id);
    // create( センサデータ保持時間[sec], おおよそのデータ更新周期[sec] )
    if( !signalResultSsm.create( 1.0, 1.0/30.0  ) ){
        cerr << "SSM Error : create()" << endl;
        return EXIT_FAILURE; 
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

        // ssmから最新のロボット情報取得
        if( read_robot ){
            if(wpglSsm.readNew()){
                new_stop_type = wpglSsm.data.stop_type;
//              printf("new_stop_type=%d\n",new_stop_type); // 確認用

                // stop_typeが2になったら,信号探索を行う
                if( new_stop_type == 2)
                    finder.Start();

                // 今回の値をとっておく
                old_stop_type = new_stop_type;
            }
        }

        // ssmから最新画像取得
        if(cam_image.readNew()){

            // cv::Matに変換
            ImageC3_to_Mat(cam_image.data,&frm);

            // 処理エリアを取り出す
            Mat roi = frm(procArea);
            Mat pro_frm = roi.clone();

            // 処理
            sn::Result result  = finder.AnalyzeImage( pro_frm, res_img, show_result );

            // 結果
            SignalResult res;
            switch( result ){
                // アイドリング中
                case sn::Result::NoProcess:
                    res.res = -1;
                    break;

                // 処理中
                case sn::Result::LookingForRed:
                case sn::Result::FoundRed:
                case sn::Result::WaitingForBlue:
                case sn::Result::LostRed:
                    res.res = 0;
                    break;

                // 青信号発見
                case sn::Result::FoundBlueSignal:
                    res.res = 1;
                    if( !flag_analysis ){
                        Spur_unfreeze( );
                    } else{
                        printf( "Engage!\n" );
                    }
                    break;

                // エラーとその他は-1(ありえないはず)
                case sn::Result::Error:
                default:
                    cout << "Error or invalid result from SignalFinder" << endl;
                    res.res = -1;
            }

            // SSMに結果書き込み
            signalResultSsm.data = res;
            signalResultSsm.write(cam_image.time);

            // 結果画像の表示
            if( show_result ){
                res_img.copyTo(roi);
                rectangle(frm, procArea, Scalar(0,0,0));
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
    fprintf( stdout, "\t\033[1mfind-signal\033[0m [-n ssmID] [-v] [-r] [-a]\n" );
    fprintf( stdout, "\t\033[1mfind-signal\033[0m [-h]\n" );
    fprintf( stdout, "\n" );

    // 説明
    fprintf( stdout, "\n" );
    fprintf( stdout, "\033[1m説明\033[0m\n" );
    fprintf( stdout, "\t\033[1m-n\033[0m\tSSMのIDを指定する\n" );
    fprintf( stdout, "\t\033[1m-v\033[0m\t画像処理結果を表示しない\n" );
    fprintf( stdout, "\t\033[1m-r\033[0m\tSSMからロボット情報を取得しない\n" );
    fprintf( stdout, "\t\033[1m-a\033[0m\t解析モード。YPSpurを動作させない。\n" );
    fprintf( stdout, "\t\033[1m-h\033[0m\tこのヘルプを表示する\n" );
    fprintf( stdout, "\n" );

    // 使用方法
    fprintf( stdout, "\n\033[1m使用方法\033[0m\n");
    fprintf( stdout, "\t\033[1m%s\033[0m\t信号検知モードにする\n", "S キー");
    fprintf( stdout, "\t\033[1m%s\033[0m\tアイドリングモードにする\n", "I キー");
    fprintf( stdout, "\t\033[1m%s\033[0m\t終了にする\n", "ESCキー");
    fprintf( stdout, "\n");

}
