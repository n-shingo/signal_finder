//-------------------------------------------------
// SignalFinder.h
// S. NAKAMURA
// since: 2018-09-17
//-------------------------------------------------

#ifndef __SIGNALFINDER_H__
#define __SIGNALFINDER_H__

#include <opencv2/opencv.hpp>
#include "Buffers.h"
#include "MyEnums.h"

namespace sn
{
    class SignalFinder {

    private:

        //
        // �Œ�l (�\���̂Ȃǂ̓R���X�g���N�^�ŏ�����)
        //
        Rect SIGNAL_LOCATE_FROM_RED; // �e���v���[�g���Έʒu
#ifdef __linux__
        const char RED_SIGNAL_FILE[256] = "../res/red_signal_average.bmp";  // ���ϐԐM���摜
        const char BLUE_SIGNAL_FILE[256] = "../res/blu_signal_average.bmp"; // ���ϐM���摜
        const char BLUE_SIGNAL_BK_FILE[256] = "../res/blu_signal_backlight_average.bmp"; // �t�������ϐM���摜
#endif
#ifdef _WIN64
        const char RED_SIGNAL_FILE[256] = "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res/red_signal_average.bmp";  // ���ϐԐM���摜
        const char BLUE_SIGNAL_FILE[256] = "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res/blu_signal_average.bmp"; // ���ϐM���摜
        const char BLUE_SIGNAL_BK_FILE[256] = "C:/Users/shingo/source/repos/tc2018_signal_proto/x64/res/blu_signal_backlight_average.bmp"; // �t�������ϐM���摜
#endif
        Mat RED_SIGNAL_BASE_IMG;
        Mat BLUE_SIGNAL_BASE_IMG;
        Mat BLUE_SIGNAL_BK_BASE_IMG;
        const int RED_AREA_BUF_SIZE = 10; // �ԐM���������邽�߂̉摜�o�b�t�@�T�C�Y
        const int IMAGE_BUF_SIZE = 10; // ����Ă����ߋ��̉摜�̃o�b�t�@�T�C�Y
        const int RED_SIGNAL_BUF_SIZE = 3;  // �ԐM���ł��邩�m�F���邽�߂̃f�[�^�̃o�b�t�@��
        const int MAX_LOST_FRAMES = 10; // ���̃t���[�����ԐM�������X�g������A�ԐM���Č��o
        const int BLUE_SIGNAL_FRAMES = 5; // �M���������炱�̃t���[�����A�������ʂ��o�͂���


    public:
        // �R���X�g���N�^
        SignalFinder()
        {
            this->SIGNAL_LOCATE_FROM_RED = Rect(-8, -8, 18, 32);
            _redAreaBuf.Clear(RED_AREA_BUF_SIZE);
            _redSigImgBuf.Clear(RED_SIGNAL_BUF_SIZE);
            _redCCBuf.Clear(RED_SIGNAL_BUF_SIZE);
            RED_SIGNAL_BASE_IMG = imread(RED_SIGNAL_FILE);
            BLUE_SIGNAL_BASE_IMG = imread(BLUE_SIGNAL_FILE);
            BLUE_SIGNAL_BK_BASE_IMG = imread(BLUE_SIGNAL_BK_FILE);
        }

        // �f�X�g���N�^
        ~SignalFinder() {}

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
        Mat _redSignal; // �ԐM���������̐ԐM���摜
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


#endif //__SIGNALFINDER_H__
