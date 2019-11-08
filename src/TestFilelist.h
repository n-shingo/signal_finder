#ifndef __TESTFILELIST_H__
#define __TESTFILELIST_H__

#ifdef __linux
// 画像フォルダ
const char imgDir[][100] = {
    /*
    "/home/shingo/ssm/toBmp/bin/2018.0630.1377.bmp",     // 1個で取れる。ただし一瞬ドーナツ型になる
    "/home/shingo/ssm/toBmp/bin/2018.0630.1377.bmp",     // 1個で取れる。
    "/home/shingo/ssm/toBmp/bin/2018.0804.1129.bmp",     // 1個で取れる。サイズが小さい。
    "/home/shingo/ssm/toBmp/bin/2018.0804.1129.bmp",     // 1個で取れる。
    "/home/shingo/ssm/toBmp/bin/2018.0915.1116.bmp",     // １個で取れる。信号１で、トラック通過で一瞬消える。
    "/home/shingo/ssm/toBmp/bin/2018.0915.1116.bmp",     // １個で取れる。
    "/home/shingo/ssm/toBmp/bin/2018.0915.1348.bmp",     // 1個で取れる。何も問題なし
    "/home/shingo/ssm/toBmp/bin/2018.0915.1348.bmp",     // 1個で取れる。何も問題なし
    "/home/shingo/ssm/toBmp/bin/2018.0915.1427.bmp",     // 1個で取れる。問題なし。信号１で人が下でウロウロしている
    "/home/shingo/ssm/toBmp/bin/2018.0915.1427.bmp",     // 1個で取れる。問題なし。信号１で人が下でウロウロしている
    "/home/shingo/ssm/toBmp/bin/2018.0915.1502.bmp",     // ２個以上取れる時がある。赤い車に反応。
    "/home/shingo/ssm/toBmp/bin/2018.0915.1502.bmp",     // ２個取れる時がある。帽子に反応。
    "/home/shingo/ssm/toBmp/bin/2018.1110.1306.bmp",     // 逆光 1
    "/home/shingo/ssm/toBmp/bin/2018.1110.1306.bmp",     // 逆光 2
    "/home/shingo/ssm/toBmp/bin/2018.1110.1306.bmp",     // 逆光 3
    "/home/shingo/ssm/toBmp/bin/2018.1110.1321.bmp",     // 逆光 4
    "/home/shingo/ssm/toBmp/bin/2018.1110.1321.bmp",     // 普通の信号
    */

    /*
    "/home/shingo/ssm/toBmp/bin/2019.1021.1201.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1201.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1201.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1201.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1201.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1201.bmp",

    "/home/shingo/ssm/toBmp/bin/2019.1021.1423.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1423.bmp",

    "/home/shingo/ssm/toBmp/bin/2019.1021.1524.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1524.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1524.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1524.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1524.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1524.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1524.bmp",

    "/home/shingo/ssm/toBmp/bin/2019.1021.1547.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1021.1547.bmp",
    */

    "/home/shingo/ssm/toBmp/bin/2019.1102.1159.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1102.1205.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1102.1301.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1102.1301.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1102.1301.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1102.1301.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1102.1400.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1102.1400.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1102.1457.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1102.1457.bmp",

    "/home/shingo/ssm/toBmp/bin/2019.1108.1114.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1108.1114.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1108.1212.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1108.1212.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1108.1306.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1108.1306.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1108.1408.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1108.1408.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1108.1455.bmp",
    "/home/shingo/ssm/toBmp/bin/2019.1108.1455.bmp",

};

// 画像番号
const int imgNum[][2] = {
    /*
    {1434,1800},
    {2487,2783},
    {740,838},
    {1742,1968},
    {665,1040},
    {1921,2220},
    {82,500},
    {1276,1540},
    {43,435},
    {1281,1475},
    {51,524},
    {1377,1650},
    {0,440},   // 411で青
    {759, 1200}, // 1170で赤青両方点灯、1171で青だけ点灯
    {1526,1970}, // 1930で青赤両方消灯、1931で青だけ点灯
    {0, 250}, // 231で青
    {1758, 2245}, // 2164で青
    */

    /*
    { 0,500 },
    { 793, 1250 },
    { 1530, 2000 },
    { 2320, 2760 },
    { 3050, 3520 },
    { 3800, 4320 },

    { 180, 640 },
    { 920, 1390 },

    { 223, 730 },
    { 1000, 1480 },
    { 2675, 3010 },
    { 3300, 3800 },
    { 4030, 4520 },
    { 4780, 5280 },
    { 5538, 6055 },

    { 132, 600 },
    { 844, 1340 },
    */

    {719,1020},
    {604,906},
    {450,860},
    {2010,2350},
    {3460,3838},
    {4842,5350},
    {286,380},
    {1490,1890},
    {84,500},
    {1160,1276},

    {200,600},
    {1060,1360},
    {60,160},
    {1290, 1680},
    {105,445},
    {844, 1200},
    {75,400},
    {840, 1160},
    {140,585},
    {1734,2105},
};
#endif //__linux


#ifdef _WIN64
// 画像フォルダ
const char imgDir[][100] = {
    //"D:\\tc2018\\2018.0630.1377.bmp",     // 1個で取れる。ただし一瞬ドーナツ型になる
    //"D:\\tc2018\\2018.0630.1377.bmp",     // 1個で取れる。
    //"D:\\tc2018\\2018.0804.1129.bmp",     // 1個で取れる。サイズが小さい。
    //"D:\\tc2018\\2018.0804.1129.bmp",     // 1個で取れる。
    //"D:\\tc2018\\2018.0915.1116.bmp",     // １個で取れる。信号１で、トラック通過で一瞬消える。
    //"D:\\tc2018\\2018.0915.1116.bmp",     // １個で取れる。
    //"D:\\tc2018\\2018.0915.1348.bmp",     // 1個で取れる。何も問題なし
    //"D:\\tc2018\\2018.0915.1348.bmp",     // 1個で取れる。何も問題なし
    //"D:\\tc2018\\2018.0915.1427.bmp",     // 1個で取れる。問題なし。信号１で人が下でウロウロしている
    //"D:\\tc2018\\2018.0915.1427.bmp",     // 1個で取れる。問題なし。信号１で人が下でウロウロしている
    //"D:\\tc2018\\2018.0915.1502.bmp",     // ２個以上取れる時がある。赤い車に反応。  
    //"D:\\tc2018\\2018.0915.1502.bmp"      // ２個取れる時がある。帽子に反応。  

    "D:\\tc2019\\2019.1102.1159.bmp",
    "D:\\tc2019\\2019.1102.1205.bmp",
    "D:\\tc2019\\2019.1102.1301.bmp",
    "D:\\tc2019\\2019.1102.1301.bmp",
    "D:\\tc2019\\2019.1102.1301.bmp",
    "D:\\tc2019\\2019.1102.1301.bmp",
    "D:\\tc2019\\2019.1102.1400.bmp",
    "D:\\tc2019\\2019.1102.1400.bmp",
    "D:\\tc2019\\2019.1102.1457.bmp",
    "D:\\tc2019\\2019.1102.1457.bmp",
};

// 画像番号
const int imgNum[][2] = {
    //{ 1434,1800 },
    //{ 2487,2783 },
    //{ 740,838 },
    //{ 1742,1968 },
    //{ 665,1040 },
    //{ 1921,2220 },
    //{ 82,500 },
    //{ 1276,1540 },
    //{ 43,435 },
    //{ 1281,1475 },
    //{ 51,524 },
    //{ 1377,1650 },

    {719,1020},
    {604,906},
    {450,860},
    {2010,2350},
    {3460,3838},
    {4842,5350},
    {286,380},
    {1490,1890},
    {84,500},
    {1160,1276},


};
#endif //_WIN64

#endif //__TESTFILELIST_H__
