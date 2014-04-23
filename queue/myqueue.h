
#ifndef __MYQUEUE__
#define __MYQUEUE__

#define MAX_TOTAL_MSG   65536
#define MAX_SINGLE_MSG  2048

typedef struct{
	long mtype; /* message type */
	char mtext[MAX_SINGLE_MSG]; /* message text*/
}_MSG_BUF;

typedef struct {
    long key; /* sent message key */
    long type; /* message type */
    long len; /* message length */
    long ret_id; /* message result id */
    long ret_type; /* message result type */
    bool waiting; /* false is IPC_NOWAIT */
    char data[512]; /* message content */
}MSG_BUF;





#endif