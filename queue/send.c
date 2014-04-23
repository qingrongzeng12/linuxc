
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <assert.h>
#include "myqueue.h"


int q_post(MSG_BUF *msgbuf){
    return _que_post(msgbuf->key, msgbuf->type, msgbuf, sizeof(MSG_BUF),msgbuf->waiting); 
}

int _que_post(long key, long type, void* buff, int len, int waiting) {
    char* buffer = (char*) buff;
    _MSG_BUF msg;
    struct msqid_ds buf;
    int msgqid;

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
    msg.mtype = type;
    memmove(msg.mtext, buffer, len);
    return msgsnd(msgqid, &msg, len, (waiting != 0 ? 0 : IPC_NOWAIT));
}


void testq_post(){
    
    MSG_BUF testmsg;
    char temp[80] = {0};
    int len = 0;
    
    memset(&testmsg, 0, sizeof(testmsg));
    
    testmsg.key = 0x5000;
    testmsg.type = 0x9;
    testmsg.ret_id = 0x5800;
    testmsg.type = 0x9;
    testmsg.waiting = 0;
    
    
    gets(temp);

                     
    len = strlen(temp);
    
    memmove(testmsg.data, temp, len);
    testmsg.len = sizeof(MSG_BUF) - sizeof(testmsg.data) + len + 1;
    
    printf("testmsg.len is %d", testmsg.len);
    printf("string length is %d\n", len);
    
    if (q_post(&testmsg) <0)
        return;
    

    printf("\nend test_post\n");
    
}


#ifdef ___TEST___
int main(){
    printf("start testq_post\n");
    testq_post();
}

#endif
