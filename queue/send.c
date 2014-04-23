
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <assert.h>
#include "myqueue.h"


bool SendToQueue()
{
	g_lReqQueueId = strtol(link.post_q_id, 0, 16);
	
	
}

int q_post(MSG_BUF *msgbuf){
    return _que_post(msgbuf->key, msgbuf->type, msgbuf, msgbuf->len,msgbuf->waiting); 
}

int _que_post(long key, long type, void* buff, int len, bool waiting) {
    char* buffer = (char*) buff;
    _MSG_BUF msg;
    struct msqid_ds buf;
    int msgqid;

    if (!key) return -1;

    if ((msgqid = msgget((key_t) key, IPC_NOWAIT)) < 0) {
        msgqid = msgget((key_t) key, IPC_CREAT);
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


bool SendToQueueByCode(long lReqQueue, OUT_DATA* pReq, int code, bool bWaitingFor) {

    assert(pReq != 0);
    TRACE("%s", __PRETTY_FUNCTION__);

    pReq->ret_q_type = 1;
    pReq->code = code;
    pReq->ret_q_id = g_lRespQueueId;

    if (q_post(lReqQueue, pReq, sizeof (OUT_DATA), bWaitingFor ? 0 : 1) < 0) {
        ShowMsg(NORMAL_MSG, "send queue %#x failed: %s", lReqQueue, strerror(errno));
        return false;
    }
    return true;
}

int q_t_post(long key, long type, void* buff, int len, int flag) {
    char* buffer = (char*) buff;
    CPSMsg_t msg;
    struct msqid_ds buf;
    int msgqid;

    assert(_MAX_SINGLE_MSG_ >= len);

    if (!key) return -1;

    if ((msgqid = msgget((key_t)key, IPC_NOWAIT)(key)) < 0) {
        msgqid = msgget((key_t) key, _MSG_CREAT_);
        msgctl(msgqid, IPC_STAT, &buf);
        buf.msg_perm.mode = 0666;
        buf.msg_perm.uid = getuid();
        buf.msg_perm.gid = getgid();
        buf.msg_qbytes = _MAX_TOTAL_MSG_;
        msgctl(msgqid, IPC_SET, &buf);
    }
    msg.mtype = type;
    memmove(msg.mtext, buffer, len);
    return msgsnd(msgqid, &msg, len, (flag != 0 ? IPC_NOWAIT : 0));
}



