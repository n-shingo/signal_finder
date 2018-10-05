//
// signalResultSSM.h
// S. Nakamura
// since: 2018-10-05
//
// つくチャレ信号検出結果 for SSM
//

#ifndef __SIGNALRESULTSSM_H__
#define __SIGNALRESULTSSM_H__

#define SNAME_SIGNALRESULT "signalResult"

typedef struct{
    int res; // -1:Idling, 0:Processing, 1:Time to go
}SignalResult;


#endif // __SIGNALRESULTSSM_H__

