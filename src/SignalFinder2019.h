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

#define RED_SIGNAL_FILE_CNT  8  // �ԐM���̃e���v���[�g��
#define BLUE_SIGNAL_FILE_CNT 8  // �M���̃e���v���[�g��

namespace sn
{
    using namespace cv;

    class SignalFinder2019 {


    private:

        //
        // �Œ�l (�\���̂Ȃǂ̓R���X�g���N�^�ŏ�����)
        //
        Rect SIGNAL_LOCATE_FROM_RED; // �e���v���[�g���Έʒu


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
        const int RED_AREA_BUF_SIZE = 10; // �ԐM���������邽�߂̉摜�o�b�t�@�T�C�Y
        const int IMAGE_BUF_SIZE = 10; // ����Ă����ߋ��̉摜�̃o�b�t�@�T�C�Y
        const int RED_SIGNAL_BUF_SIZE = 3;  // �ԐM���ł��邩�m�F���邽�߂̃f�[�^�̃o�b�t�@��
        const int MAX_LOST_FRAMES = 10; // ���̃t���[�����ԐM�������X�g������A�ԐM���Č��o
        const int BLUE_SIGNAL_FRAMES = 5; // �M���������炱�̃t���[�����A�������ʂ��o�͂���


    public:
        // �R���X�g���N�^
        SignalFinder2019()
        {
            this->SIGNAL_LOCATE_FROM_RED = Rect(-8, -8, 18, 32);
            _redAreaBuf.Clear(RED_AREA_BUF_SIZE);
            _redSigImgBuf.Clear(RED_SIGNAL_BUF_SIZE);
            _redCCBuf.Clear(RED_SIGNAL_BUF_SIZE);

            // �ԐM���e���v���[�g�t�@�C��
            for(int i=0; i<RED_SIGNAL_FILE_CNT; i++)
                RED_SIGNAL_BASE_IMG[i] = imread(RED_SIGNAL_FILE[i]);

            // �M���e���v���[�g�t�@�C��
            for( int i=0; i<BLUE_SIGNAL_FILE_CNT; i++)
                BLUE_SIGNAL_BASE_IMG[i] = imread(BLUE_SIGNAL_FILE[i]);
        }

        // �f�X�g���N�^
        ~SignalFinder2019() {}

        // �M�����o�����̊J�n
        void Start();

        // �M�����o�����̏I��(�����I��Idling��Ԃɂ���)
        void Stop();

        // �摜��ǉ����A��́��������s��
        Result AnalyzeImage(Mat& img, Mat& dst, bool resultImg);



    private:

        //////////////////////
        //    ����J�����o    //
        //////////////////////

        Mode _mode = Mode::Idling; // ���݂̃��[�h
        ImageBuffer _redAreaBuf;   // HSV����ԗ̈�����߂��Q�l�摜�𗭂߂Ă߂Ă������߂̃o�b�t�@
        ImageBuffer _redSigImgBuf; // �ԐM���ƌ��o���ꂽ�摜�̃o�b�t�@
        DoubleBuffer _redCCBuf;    // �ԐM���Ƃ̑��֌W���𗭂߂Ă����o�b�t�@
        Mat _redSignal;            // �ԐM���������̐ԐM���摜
        int _redSignalLostCounter; // �M���҂��̎��A�ԐM�������X�g�����񐔂𐔂���J�E���^�[
        int _bluSignalCounter;     // �M����������o�߂����t���[����


                                   //////////////////////
                                   //   ����J���\�b�h   //
                                   //////////////////////

                                   // �A�C�h�����O���̉������Ȃ�����
        Result ProcessNothing(Mat& img, Mat& dst, bool drawResult);

        // �ԐM���̒T�����s��
        Result TryFindRedSignal(Mat& img, Mat& dst, bool drawResult);

        // �M����҂������s��
        Result WaitForBlueSignal(Mat& img, Mat& dst, bool drawResult);

        // �M������������̏������s��
        Result FoundBlueSignal(Mat& img, Mat& dst, bool drawResult);

        // ���[�h�J�ڂ�����
        void MoveMode(Mode mode);


        // HSV�ɂ��臒l�����ɂ���ĐԐM���̈�摜���擾����
        // src�̓J���[�摜, dst��8bit�̂Q�l�摜
        void GetRedSignalArea(cv::Mat& src, cv::Mat& dst);


    };
}


#endif //__SIGNALFINDER2019_H__
