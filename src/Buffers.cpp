//-------------------------------------------------
// Buffers.cpp
// S. NAKAMURA
// since: 2018-09-17
//-------------------------------------------------

#include "Buffers.h"

using namespace sn;

// �R���X�g���N�^
ImageBuffer::ImageBuffer(int bufsize)
{
    assert(bufsize > 0);

    this->_bufsize = bufsize;
    this->_buf = new Mat[bufsize];
}

// �f�X�g���N�^
ImageBuffer::~ImageBuffer()
{
    delete[] this->_buf;
}

// ����܂ł̉摜���N���A���Abusize��ύX����
// bufsize=0�ł���܂łƓ���bufsize�Ƃ���
void ImageBuffer::Clear(int bufsize) {

    if (bufsize != 0) {
        this->_bufsize = bufsize;
        delete[] this->_buf;
        this->_buf = new Mat[bufsize];
    }
    this->_count = 0;
    this->_pos = 0;
}


//
// �摜����ǉ����āA����܂ł̕��ω摜���擾����
// �߂�l�͉����̉摜���g�p�������ς�
void ImageBuffer::AddImage(Mat& src) {

    // �摜�ǉ�
    _buf[_pos] = src.clone();
    if (_count != _bufsize) _count++;

    // ���ɒǉ�����ꏊ
    _pos++;
    _pos = _pos % _bufsize;

}

// ���ω摜���擾
void ImageBuffer::AverageImage(Mat& ave)
{
    // �f�[�^�Ȃ�
    if (_count == 0) return;

    // �f�[�^�P�Ȃ�R�s�[���ďI��
    if (_count == 1) {
        ave = _buf[0].clone();
        return;
    }

    //
    // �Q�ȏ�Ȃ畽�ς��쐬
    //
    ave = Mat(_buf[0].size(), _buf[0].type());

    int h = ave.rows, w = ave.cols;
    for (int y = 0; y < h; y++)
    {
        uchar* pDst = ave.data + y * ave.step[0];
        for (int x = 0; x < ave.step[0]; x++)
        {
            int sum = 0;
            for (int i = 0; i < _count; i++) {
                uchar *p = _buf[i].data + _buf[i].step[0] * y + x;
                sum += *p;
            }

            pDst[x] = (uchar)(sum / _count);
        }
    }
}

// ���܂����o�b�t�@�̐����擾
int ImageBuffer::Count(void)
{
    return _count;
}

// �o�b�t�@����t���擾
bool ImageBuffer::IsFilled(void)
{
    return (_count == _bufsize);
}

