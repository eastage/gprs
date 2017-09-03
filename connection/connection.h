
typedef enum _CONNECTION_STATE
{
    CONN_STATE_NEW,
    CONN_STATE_CONNECTED,
    CONN_STATE_DISCONNECTED
} CONNECTION_STATE;

typedef enum _CONNECTION_EVENT
{
    CONN_EVENT_INIT,
    CONN_EVENT_CONNECTING,    /* transport layer being established */
    CONN_EVENT_CONNECTED,     /* transport layer established */
    CONN_EVENT_DISCONNECTED,  /* response timeout at transport layer */
    
    CONN_EVENT_DATAINCOMING,  /* data incoming notification  */
    CONN_EVENT_DATAOBTAINED,  /* data obtained asynchronously from device */
    CONN_EVENT_SENDACCEPTED,  /* data send request accepted */
    CONN_EVENT_SENDCOMPLETE,  /* all data sent out  */

    CONN_EVENT_SIGNALSTR      /* signal strength updated  */
} CONNECTION_EVENT;

typedef struct _CONNECTION_READ_DATA
{
    unsigned char * pData;
    unsigned int    dataLen;
} CONNECTION_READ_DATA;

typedef unsigned int (* LEN_MATCH_ALGO)(unsigned char * pData, unsigned int dataLen);

typedef void (* CONNECTION_INIT)(void);

typedef void (* CONNECTION_START)(void);

typedef void (* CONNECTION_STOP)(void);

typedef void (* CONNECTION_GETSTATE)(void);

typedef void (* CONNECTION_CALLBACK)(CONNECTION_EVENT event, void * pCallbackData, void * userData);

typedef unsigned int (* CONNECTION_READDATA)(unsigned int minLen, unsigned int fixedLen, LEN_MATCH_ALGO * match, unsigned int timeout); /* timeout: ms */

typedef unsigned int (* CONNECTION_WRITEDATA)(unsigned int outLen, unsigned int timeout); /* timeout: ms */

typedef void (* CONNECTION_REGCALLBACK)(CONNECTION_CALLBACK * userCallback);

typedef struct _CONNECTION
{
    /* attr */
    unsigned char * connName;
    
    unsigned char cid; /* connection id */

    void * userData;
    
    CONNECTION_CALLBACK usercallback;

    unsigned int rebootCnt; /* reconnect count */

    unsigned int ticks;     /* time ticks of connected state */

    CONNECTION_STATE connState;

    unsigned int bStarted;
    
    /* behavior */
    CONNECTION_INIT init;

    CONNECTION_START start;

    CONNECTION_STOP stop;

    CONNECTION_GETSTATE getState;

    CONNECTION_REGCALLBACK regUserCallback;
    
    CONNECTION_READDATA readData;   /* read data asynchronously */

    CONNECTION_WRITEDATA writeData; /* write data asynchronously */
    
} CONNECTION;

