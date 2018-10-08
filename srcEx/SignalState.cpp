//
// SignalState.cpp
// S.Nakamura
// 2018-10-08
//


#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

bool signalState(void){

    using namespace std;

    const char key_file[] = "/tmp";
    const int key_id = 42;
    static key_t shm_key;
    static int first = 1;
    static int old_data = -2;

    // 初回呼び出しは shm_key を作成しておく
    if( first )
    {
        shm_key = ftok(key_file, key_id);
        if( shm_key == -1){
            cerr << "Create key Error for shared memory : ftok" << endl;
            return false;
        }
        first = 0;
    }

    // 共有メモリのid取得
    const int shm_id = shmget(shm_key, 0, 0);
    if( shm_id == -1){
        cerr << "Get shared memory ID error : shmget" << endl;
        return false;
    }

    // 共有メモリのアドレス取得
    void *shm_adr = shmat(shm_id, 0, 0);
    if( shm_adr == (void*)-1 ){
        cerr << "Get shared memory address error : shmat" << endl;
        return false;
    }

    // 共有メモリから読み込み
    int data;
    memcpy(&data, shm_adr, sizeof(data));

    // 共有メモリのデタッチ
    shmdt(shm_adr);


    // ロボット発進条件
    if( (old_data == 0 && data == 1) || (old_data==1 && data==1) )
    {
        old_data = data;
        return true;
    }

    else{
        old_data = data;
        return false;
    }


}

