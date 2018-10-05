//-------------------------------------------------
// Buffers.cpp
// S. NAKAMURA
// since: 2018-09-17
//-------------------------------------------------

#include "Buffers.h"

using namespace sn;

// コンストラクタ
ImageBuffer::ImageBuffer(int bufsize)
{
    assert(bufsize > 0);

    this->_bufsize = bufsize;
    this->_buf = new Mat[bufsize];
}

// デストラクタ
ImageBuffer::~ImageBuffer()
{
    delete [] this->_buf;
}

// これまでの画像をクリアし、busizeを変更する
// bufsize=0でこれまでと同じbufsizeとする
void ImageBuffer::Clear(int bufsize) {
    
    if (bufsize != 0) {
        this->_bufsize = bufsize;
        delete [] this->_buf;
        this->_buf = new Mat[bufsize];
    }
    this->_count = 0;
    this->_pos = 0;
}


//
// 画像を一つ追加して、これまでの平均画像を取得する
// 戻り値は何枚の画像を使用した平均か
void ImageBuffer::AddImage(Mat& src) {

    // 画像追加
    _buf[_pos] = src.clone();
    if (_count != _bufsize) _count++;

    // 次に追加する場所
    _pos++;
    _pos = _pos % _bufsize;

}

// 平均画像を取得
void ImageBuffer::AverageImage(Mat& ave)
{
    // データなし
    if (_count == 0) return;

    // データ１個ならコピーして終了
    if (_count == 1) {
        ave = _buf[0].clone();
        return;
    }

    //
    // ２個以上なら平均を作成
    //
    ave = Mat( _buf[0].size(), _buf[0].type());

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

// 溜まったバッファの数を取得
int ImageBuffer::Count(void)
{
    return _count;
}

// バッファか一杯か取得
bool ImageBuffer::IsFilled(void)
{
    return (_count == _bufsize);
}

