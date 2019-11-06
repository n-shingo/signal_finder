//-------------------------------------------------
// Buffers.h
// S. NAKAMURA
// since: 2018-09-17
//-------------------------------------------------

#ifndef __BUFFERS_H__
#define __BUFFERS_H__

#include "opencv2/opencv.hpp"

namespace sn {

    using namespace cv;

    ///////////////////////////////
    //
    // class ImageBuffer
    // �摜�𗭂߂邽�߂̃o�b�t�@�N���X
    //
    ///////////////////////////////
    class ImageBuffer {


        ////////////////
        //  ���J�����o  //
        ////////////////
    public:

        // �R���X�g���N�^ & �f�X�g���N�^
        ImageBuffer(int bufsize = 5);
        ~ImageBuffer();

        // ����܂ł̉摜���N���A���Abusize��ύX����
        // bufsize=0�ł���܂łƓ���bufsize�Ƃ���
        void Clear(int bufsize = 0);

        // �摜����ǉ�����
        void AddImage(Mat& src);

        // �o�b�t�@�̕��ω摜���擾����
        void AverageImage(Mat& ave);

        // ���߂�ꂽ�o�b�t�@�̐�
        int Count();

        // �o�b�t�@�����^����
        bool IsFilled();

        // �Y�����Z�q�ŃA�N�Z�X
        const Mat& operator[](std::size_t n) const& { return _buf[n]; }

        ////////////////
        // ����J�����o //
        ////////////////
    private:

        int _count;    // ���܂��Ă���摜�̐�
        int _bufsize;  // �o�b�t�@�̃T�C�Y
        Mat* _buf;     // �摜�𒙂߂Ă����ꏊ
        int _pos;      // ���ɒ��ߍ��މ摜�̏ꏊ�������ϐ�

    };

    ///////////////////////////////
    //
    // class DoubleBuffer
    // double�𗭂߂邽�߂̃o�b�t�@�N���X
    //
    ///////////////////////////////
    class DoubleBuffer {

        ////////////////
        //  ���J�����o  //
        ////////////////
    public:

        // �R���X�g���N�^ & �f�X�g���N�^
        DoubleBuffer(int bufsize = 5) {
            assert(bufsize > 0);

            _buf = new double[bufsize];
            _count = 0;
            _bufsize = bufsize;
            _pos = 0;
        }
        ~DoubleBuffer() {
            delete[] _buf;
        }

        // �C�f�b�N�X�ŃA�N�Z�X
        double& operator[](std::size_t n) { return _buf[n]; }

        // ����܂ł̓��e���N���A���Abusize��ύX����
        // bufsize=0�ł���܂łƓ���bufsize�Ƃ���
        void Clear(int bufsize = 0) {
            if (bufsize != 0) {
                delete[] _buf;
                _buf = new double[bufsize];
                _bufsize = bufsize;
            }
            _count = 0;
            _pos = 0;
        }

        // �f�[�^����ǉ�����
        void AddDouble(double data) {
            _buf[_pos] = data;
            if (_count != _bufsize) _count++;

            _pos++;
            _pos %= _bufsize;
        }

        // ���߂�ꂽ�o�b�t�@�̐�
        int Count() {
            return _count;
        }

        // �o�b�t�@�����^����
        bool IsFilled() {
            return _count == _bufsize;
        }

        // �ő�l�̃C���f�b�N�X�ԍ����擾
        int GetMaxIndex()
        {
            if (_count == 0) return -1;

            int max = 0;
            for (int i = 1; i < _count; i++)
            {
                if (_buf[max] < _buf[i])
                    max = i;
            }
            return max;
        }


        ////////////////
        // ����J�����o //
        ////////////////
    private:

        int _count;    // ���܂��Ă���f�[�^�̐�
        int _bufsize;  // �o�b�t�@�̃T�C�Y
        double* _buf;     // �o�b�t�@�ւ̃|�C���^
        int _pos;      // ���Ɋi�[����ꏊ�������ϐ�

    };


    ///////////////////////////////
    //
    // class UintBuffer
    // uint�𗭂߂邽�߂̃o�b�t�@�N���X
    //
    ///////////////////////////////
    class UintBuffer {

        ////////////////
        //  ���J�����o  //
        ////////////////
    public:

        // �R���X�g���N�^ & �f�X�g���N�^
        UintBuffer(int bufsize = 5) {
            assert(bufsize > 0);

            _buf = new unsigned int[bufsize];
            _count = 0;
            _bufsize = bufsize;
            _pos = 0;
        }
        ~UintBuffer() {
            delete[] _buf;
        }

        // �C�f�b�N�X�ŃA�N�Z�X
        unsigned int& operator[](std::size_t n) { return _buf[n]; }

        // ����܂ł̓��e���N���A���Abusize��ύX����
        // bufsize=0�ł���܂łƓ���bufsize�Ƃ���
        void Clear(int bufsize = 0) {
            if (bufsize != 0) {
                delete[] _buf;
                _buf = new unsigned int[bufsize];
                _bufsize = bufsize;
            }
            _count = 0;
            _pos = 0;
        }

        // �f�[�^����ǉ�����
        void AddUint(unsigned int data) {
            _buf[_pos] = data;
            if (_count != _bufsize) _count++;

            _pos++;
            _pos %= _bufsize;
        }

        // ���߂�ꂽ�o�b�t�@�̐�
        int Count() {
            return _count;
        }

        // �o�b�t�@�����^����
        bool IsFilled() {
            return _count == _bufsize;
        }

        // �ő�l�̃C���f�b�N�X�ԍ����擾
        int GetMaxIndex()
        {
            if (_count == 0) return -1;

            int max = 0;
            for (int i = 1; i < _count; i++)
            {
                if (_buf[max] < _buf[i])
                    max = i;
            }
            return max;
        }


        ////////////////
        // ����J�����o //
        ////////////////
    private:

        int _count;    // ���܂��Ă���f�[�^�̐�
        int _bufsize;  // �o�b�t�@�̃T�C�Y
        unsigned int* _buf;     // �o�b�t�@�ւ̃|�C���^
        int _pos;      // ���Ɋi�[����ꏊ�������ϐ�

    };
}


#endif // __BUFFERS_H__
