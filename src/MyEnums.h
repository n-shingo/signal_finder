#ifndef __ENUMS_H__
#define __ENUMS_H__

namespace sn
{

    // SignalFinderの状態
    enum Mode {
        Idling,     // アイドリング中
        FindingRed, // 赤信号探索中
        WaitingBlue, // 青信号待ち
        FoundBlue   // 青信号発見
    };

    // AnaylzeImageをした時の結果
    enum Result {
        NoProcess,         // 何もしない
        LookingForRed,     // 赤信号を探している
        FoundRed,          // 赤信号を発見した
        WaitingForBlue,    // 青信号待ち
        LostRed,           // 赤信号を見失った
        FoundBlueSignal,         // 青信号を発見した
        Error              // エラー
    };
}

#endif //__ENUMS_H__