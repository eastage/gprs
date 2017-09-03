
#define READBUF_MAX_SIZE (1500)
#define OUTGOSTR_MAX_SIZE (128)

typedef enum _WORK_STATE
{
    STATE_IDLE,
    STATE_NEW,
    STATE_WAITRESP,
    STATE_SUCCESS,
    STATE_TIMEOUT,
    STATE_ERROR
} WORK_STATE;

typedef enum _WORK_CMDTYPE
{
    CMDTYPE_READ     = 1, /* used for app to read from network */
    CMDTYPE_WRITE    = 2, /* used for app to write to network */
    CMDTYPE_CONTROL  = 4, /* used for config and control DCE */
    CMDTYPE_PERIODIC = 8  /* an attribute can go along with above type */
} WORK_CMDTYPE;

typedef enum _LEN_METHOD
{
    LEN_METHOD_MIN            = 0x01,
    LEN_METHOD_FIXED          = 0x02,
    LEN_METHOD_LENMATCHALGO   = 0x04,
    LEN_METHOD_CR             = 0x08,
    LEN_METHOD_ATRESP_SPECIFY = 0x10;
} LEN_METHOD;

typedef enum _WORK_EVENT
{
    WORKEVENT_READDONE,
    WORKEVENT_WRITEDONE,
    WORKEVENT_CTLDONE,
    WORKEVENT_TIMEOUT
} WORK_EVENT;

typedef void (* WORK_EVENT_CALLBACK)(WORK_EVENT event, void * pCallbackData, void * userData);

typedef void (* WORK_SET_CMDTYPE)(WORK * pWork, WORK_CMDTYPE cmdtype);

typedef void (* WORK_SET_STATE)(WORK * pWork, WORK_STATE state);

typedef void (* WORK_SET_OUTGOSTR)(WORK * pWork, unsigned char * pStr);

typedef void (* WORK_SET_EVTCALLBACK)(WORK * pWork, WORK_EVENT_CALLBACK callback);

typedef void (* WORK_SET_LENMETHOD)(WORK * pWork, LEN_METHOD method);

typedef void (* WORK_SET_REQUESTLEN)(WORK * pWork, unsigned int requestLen);

typedef void (* WORK_SET_TIMEOUT)(WORK * pWork, unsigned int timeout);

typedef void (* WORK_SET_LENMATCHALGO)(WORK * pWork, LEN_MATCH_ALGO algo);

typedef void (* WORK_GET_COMMANDSTR)(WORK * pWork);

typedef void (* WORD_DO)(WORK * pWork);

typedef struct _WORK
{
    WORK_CMDTYPE  cmdtype;
    WORK_STATE    state;

    unsigned int  connId;
    unsigned char connName[32];
    unsigned int  profileId;
    unsigned int  attached;
    
    unsigned int  totalTimes;
    unsigned int  failTimes;

    WORK_EVENT_CALLBACK event_cb;

    /* expected read length or len match method */ 
    LEN_METHOD     lenMethod;
    LEN_MATCH_ALGO algo;
    unsigned int   requestLen;
    unsigned int   timeout;
    
    /* read related attrs */        
    unsigned char dataBuf[READBUF_MAX_SIZE];
    unsigned int  dataLen;

    /* write related attrs */
    unsigned char outgoStr[OUTGOSTR_MAX_SIZE];
    unsigned char outgoStrValid;
    
    /* common behavior */
    WORK_SET_CMDTYPE      set_cmdtype;
    WORK_SET_STATE        set_state;
    WORK_SET_EVTCALLBACK  set_evt_callback;
    WORK_SET_LENMETHOD    set_len_method;
    WORK_SET_REQUESTLEN   set_request_len;
    WORK_SET_TIMEOUT      set_request_timeout;
    WORK_SET_LENMATCHALGO set_len_match_algo;

    /* read related behavior, internally used by this class */
    WORK_GET_COMMANDSTR   get_command_str;
    //r_get_outgo_str_cb(CONNECTION conn, void * p, unsigned int userData);

#if 0
    read_income_data(unsigned char * pData, unsigned int dataLen);

    /* write related behavior */
    w_set_outgo_str();
    w_get_outgo_str_cb(CONNECTION conn, void * p, unsigned int userData);

    w_get_outgo_data_cb(CONNECTION conn, void * p, unsigned int userData);
    
    w_income_data_parser(unsigned char * pData, unsigned int dataLen);
    w_income_data_cb(CONNECTION conn, void * p, unsigned int userData);

    /* periodic related behavior */
    set_period();
#endif

    WORD_DO do_work;
} WORK;


