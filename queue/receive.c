
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <assert.h>
#include "myqueue.h"



int q_recv(MSG_BUF *msgbuf){
    return _que_send(msgbuf->key, msgbuf->type, msgbuf, sizeof(MSG_BUF),msgbuf->waiting); 
}
 
 int _que_recv(long key, long type, void* buff, int len, int waiting) {
    char* buffer = (char*) buff;
    _MSG_BUF msg;
    struct msqid_ds buf;
    int msgqid;
    int ret;

    assert(MAX_SINGLE_MSG >= len);

    if (!key) return -1;

    if ((msgqid = msgget((key_t) key, IPC_NOWAIT)) < 0) {
        msgqid = msgget((key_t) key, _MSG_CREAT_);
        msgctl(msgqid, IPC_STAT, &buf);
        buf.msg_perm.mode = 0666;
        buf.msg_perm.uid = getuid();
        buf.msg_perm.gid = getgid();
        buf.msg_qbytes = MAX_TOTAL_MSG;
        msgctl(msgqid, IPC_SET, &buf);
    }
    if ((ret = msgrcv(msgqid, &msg, len, type, (waiting != 0 ? 0 : IPC_NOWAIT))) > 0) {
        memmove(buffer, msg.mtext, len);
    }
    return ret;
}
 
 
 
void testq_recv(){
    
    MSG_BUF testmsg;
    int len = 0;
    
    memset(&testmsg, 0, sizeof(testmsg));
    
    testmsg.key = 0x5000;
    testmsg.type = 0x9;
    testmsg.ret_id = 0x5800;
    testmsg.type = 0x9;
    testmsg.waiting = 0;
    
    
    if (q_recv(&testmsg) <0)
        return;
    
    printf("\nreceive data is %s", testmsg.data);
    printf("\nend testq_recv\n");
    
}

#define ___TEST___
#ifdef ___TEST___
int main(){
    printf("start testq_post\n");
    testq_recv();
}


#endif