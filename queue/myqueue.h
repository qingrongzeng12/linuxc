
#ifndef __MYQUEUE__
#define __MYQUEUE__

#define MAX_TOTAL_MSG   65536
#define MAX_SINGLE_MSG  2048

#ifndef MSG_R
#define MSG_R 0400
#endif // MSG_R
#ifndef MSG_W
#define MSG_W 0200
#endif // MSG_W
#define _MSG_CREAT_ (IPC_CREAT|MSG_R|MSG_W|(MSG_R>>3)|(MSG_W>>3)|(MSG_R>>6)|(MSG_W>>6))

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
    int waiting; /* false is IPC_NOWAIT */
    char data[512]; /* message content */
}MSG_BUF;


int q_post(MSG_BUF *msgbuf);
int _que_post(long key, long type, void* buff, int len, int waiting);

int q_recv(MSG_BUF *msgbuf);
 int _que_recv(long key, long type, void* buff, int len, int waiting);
    
}
#define ___TEST___
#ifdef ___TEST___
void testq_post();
void testq_recv();
#endif


#endif