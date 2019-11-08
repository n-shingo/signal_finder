//-------------------------------------------------
// TestMain.cpp
// S. NAKAMURA
// since: 2018-09-17
//-------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "Buffers.h"
#include "SignalFinder.h"
#include "SignalFinder2019.h"
#include "TestFilelist.h"

using namespace cv;



// 処理エリア
const cv::Rect procArea(80, 80, 480, 160);

// テンプレート位置
//const cv::Rect tmplate1(-8, -8, 18, 32);



//
// ユーティリティ関数
//

// 処理対象の総画像数を取得
int getTotalNum(void);

// 番号から画像のファイルパスを取得する
void getFilePath(int num, char* path);


// main関数
int main(void)
{
	int num = 0;
	int totalNum = getTotalNum();
	char fn[256];
	Mat img, dst, imgHSV[3];
	sn::SignalFinder2019 sf;
	int mode = 0;  // 0:手動, 1:自動再生


	while (1) {
		getFilePath(num, fn);
        std::cout << fn << std::endl;
		img = imread(fn);
		dst = img(procArea);
		img = dst.clone();

		imshow("Original", img);

		sf.AnalyzeImage(img, dst, true);
		imshow("Original2", dst);

		int key;
		if (mode == 0)
			key = waitKey(0);
		else
			key = waitKey(40);

		if (key == 27) break; //esc
		else if (key == 'j') {
			mode = 0;
			num++;
		}
		else if (key == 'k') {
			mode = 0;
			num--;
		}
		else if (key == 'i'){
			sf.Stop();
		}
		else if (key == 's') {
			sf.Start();
		}
		else if (key == 'p') {
			mode = 1;
		}
		else {
			num++;
		}


		if (num < 0)  num = totalNum - 1;
		else if (num >= totalNum) num = 0;


	}

	cv::destroyAllWindows();


	return 0;
}





// 画像処理のトータル数を取得する
int getTotalNum(void)
{
	static int dirCnt = sizeof(imgNum) / (2 * sizeof(imgNum[0][0]));
	static int total = 0;

	if (total != 0) return total;

	for (int i = 0; i < dirCnt; i++)
		total += imgNum[i][1] - imgNum[i][0];


	return total;
}

// 番号からファイルパスを取得
void getFilePath(int num, char* path)
{
	static int dirCnt = sizeof(imgNum) / (2 * sizeof(imgNum[0][0]));
	int total = getTotalNum();
	if (num < 0) num += total;
	else if (num >= total) num -= total;
	assert(0 <= num && num < total);

	for (int i = 0; i < dirCnt; i++) {
		int cnt = imgNum[i][1] - imgNum[i][0];
		if (num - cnt < 0) {
			int filenum = imgNum[i][0] + num;
			sprintf(path, "%s/image%05d.bmp", imgDir[i], filenum);
			return;
		}
		else
			num -= cnt;
	}

}


