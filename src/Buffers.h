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
    // 画像を溜めるためのバッファクラス
    //
    ///////////////////////////////
    class ImageBuffer {


    ////////////////
    //  公開メンバ  //
    ////////////////
    public:

        // コンストラクタ & デストラクタ
        ImageBuffer( int bufsize = 5);
        ~ImageBuffer();

        // これまでの画像をクリアし、busizeを変更する
        // bufsize=0でこれまでと同じbufsizeとする
        void Clear(int bufsize = 0);

        // 画像を一つ追加する
        void AddImage(Mat& src);

        // バッファの平均画像を取得する
        void AverageImage(Mat& ave);

        // 貯められたバッファの数
        int Count();

        // バッファが満タンか
        bool IsFilled();

        // 添字演算子でアクセス
        const Mat& operator[](std::size_t n) const& { return _buf[n]; }

    ////////////////
    // 非公開メンバ //
    ////////////////
    private:

        int _count;    // 溜まっている画像の数
        int _bufsize;  // バッファのサイズ
        Mat* _buf;     // 画像を貯めておく場所
        int _pos;      // 次に貯め込む画像の場所を示す変数

    };

    ///////////////////////////////
    //
    // class DoubleBuffer
    // doubleを溜めるためのバッファクラス
    //
    ///////////////////////////////
    class DoubleBuffer {

        ////////////////
        //  公開メンバ  //
        ////////////////
    public:

        // コンストラクタ & デストラクタ
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

        // これまでの内容をクリアし、busizeを変更する
        // bufsize=0でこれまでと同じbufsizeとする
        void Clear(int bufsize = 0) {
            if (bufsize != 0) {
                delete[] _buf;
                _buf = new double[bufsize];
                _bufsize = bufsize;
            }
            _count = 0;
            _pos = 0;
        }

        // データを一つ追加する
        void AddDouble(double data) {
            _buf[_pos] = data;
            if (_count != _bufsize) _count++;

            _pos++;
            _pos %= _bufsize;
        }

        // 貯められたバッファの数
        int Count() {
            return _count;
        }

        // バッファが満タンか
        bool IsFilled() {
            return _count == _bufsize;
        }

        // 最大値のインデックス番号を取得
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
        // 非公開メンバ //
        ////////////////
    private:

        int _count;    // 溜まっているデータの数
        int _bufsize;  // バッファのサイズ
        double* _buf;     // バッファへのポインタ
        int _pos;      // 次に格納する場所を示す変数

    };
}


#endif // __BUFFERS_H__
