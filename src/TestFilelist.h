#ifndef __TESTFILELIST_H__
#define __TESTFILELIST_H__

#ifdef __linux
// �摜�t�H���_
const char imgDir[][100] = {
    /*
    "/home/shingo/ssm/toBmp/bin/2018.0630.1377.bmp",     // 1�Ŏ���B��������u�h�[�i�c�^�ɂȂ�
    "/home/shingo/ssm/toBmp/bin/2018.0630.1377.bmp",     // 1�Ŏ���B
    "/home/shingo/ssm/toBmp/bin/2018.0804.1129.bmp",     // 1�Ŏ���B�T�C�Y���������B
    "/home/shingo/ssm/toBmp/bin/2018.0804.1129.bmp",     // 1�Ŏ���B
    "/home/shingo/ssm/toBmp/bin/2018.0915.1116.bmp",     // �P�Ŏ���B�M���P�ŁA�g���b�N�ʉ߂ň�u������B
    "/home/shingo/ssm/toBmp/bin/2018.0915.1116.bmp",     // �P�Ŏ���B
    "/home/shingo/ssm/toBmp/bin/2018.0915.1348.bmp",     // 1�Ŏ���B�������Ȃ�
    "/home/shingo/ssm/toBmp/bin/2018.0915.1348.bmp",     // 1�Ŏ���B�������Ȃ�
    "/home/shingo/ssm/toBmp/bin/2018.0915.1427.bmp",     // 1�Ŏ���B���Ȃ��B�M���P�Ől�����ŃE���E�����Ă���
    "/home/shingo/ssm/toBmp/bin/2018.0915.1427.bmp",     // 1�Ŏ���B���Ȃ��B�M���P�Ől�����ŃE���E�����Ă���
    "/home/shingo/ssm/toBmp/bin/2018.0915.1502.bmp",     // �Q�ȏ���鎞������B�Ԃ��Ԃɔ����B
    "/home/shingo/ssm/toBmp/bin/2018.0915.1502.bmp",     // �Q���鎞������B�X�q�ɔ����B
    "/home/shingo/ssm/toBmp/bin/2018.1110.1306.bmp",     // �t�� 1
    "/home/shingo/ssm/toBmp/bin/2018.1110.1306.bmp",     // �t�� 2
    "/home/shingo/ssm/toBmp/bin/2018.1110.1306.bmp",     // �t�� 3
    "/home/shingo/ssm/toBmp/bin/2018.1110.1321.bmp",     // �t�� 4
    "/home/shingo/ssm/toBmp/bin/2018.1110.1321.bmp",     // ���ʂ̐M��
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

// �摜�ԍ�
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
    {0,440},   // 411�Ő�
    {759, 1200}, // 1170�ŐԐ����_���A1171�Ő����_��
    {1526,1970}, // 1930�Őԗ��������A1931�Ő����_��
    {0, 250}, // 231�Ő�
    {1758, 2245}, // 2164�Ő�
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
// �摜�t�H���_
const char imgDir[][100] = {
    //"D:\\tc2018\\2018.0630.1377.bmp",     // 1�Ŏ���B��������u�h�[�i�c�^�ɂȂ�
    //"D:\\tc2018\\2018.0630.1377.bmp",     // 1�Ŏ���B
    //"D:\\tc2018\\2018.0804.1129.bmp",     // 1�Ŏ���B�T�C�Y���������B
    //"D:\\tc2018\\2018.0804.1129.bmp",     // 1�Ŏ���B
    //"D:\\tc2018\\2018.0915.1116.bmp",     // �P�Ŏ���B�M���P�ŁA�g���b�N�ʉ߂ň�u������B
    //"D:\\tc2018\\2018.0915.1116.bmp",     // �P�Ŏ���B
    //"D:\\tc2018\\2018.0915.1348.bmp",     // 1�Ŏ���B�������Ȃ�
    //"D:\\tc2018\\2018.0915.1348.bmp",     // 1�Ŏ���B�������Ȃ�
    //"D:\\tc2018\\2018.0915.1427.bmp",     // 1�Ŏ���B���Ȃ��B�M���P�Ől�����ŃE���E�����Ă���
    //"D:\\tc2018\\2018.0915.1427.bmp",     // 1�Ŏ���B���Ȃ��B�M���P�Ől�����ŃE���E�����Ă���
    //"D:\\tc2018\\2018.0915.1502.bmp",     // �Q�ȏ���鎞������B�Ԃ��Ԃɔ����B  
    //"D:\\tc2018\\2018.0915.1502.bmp"      // �Q���鎞������B�X�q�ɔ����B  

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

// �摜�ԍ�
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
