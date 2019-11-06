#ifndef __ENUMS_H__
#define __ENUMS_H__

namespace sn
{

    // SignalFinder�̏��
    enum Mode {
        Idling,     // �A�C�h�����O��
        FindingRed, // �ԐM���T����
        WaitingBlue, // �M���҂�
        FoundBlue   // �M������
    };

    // AnaylzeImage���������̌���
    enum Result {
        NoProcess,         // �������Ȃ�
        LookingForRed,     // �ԐM����T���Ă���
        FoundRed,          // �ԐM���𔭌�����
        WaitingForBlue,    // �M���҂�
        LostRed,           // �ԐM������������
        FoundBlueSignal,         // �M���𔭌�����
        Error              // �G���[
    };
}

#endif //__ENUMS_H__