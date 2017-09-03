
#include "work.h"

void work_set_cmdtype(WORK * pWork, WORK_CMDTYPE cmdtype)
{
    pWork->cmdtype = cmdtype;
}

void work_set_state(WORK * pWork, WORK_STATE state)
{
    pWork->state = state;
}

void work_attach_connection(WORK * pWork, unsigned int connId, unsigned char * connName)
{
    pWork->connId = connId;
    memset(pWork->connName, 0, sizeof(pWork->connName));
    strncpy(pWork->connName, connName, sizeof(pWork->connName) - 1);
    
    pWork->profileId = get_profileId_from_connId(connId);

    pWork->attached = 1;
}

unsigned int work_detach_connection(WORK * pWork)
{
    pWork->connId = 0;
    memset(pWork->connName, 0, sizeof(pWork->connName));
    
    pWork->profileId = 0;
    
    pWork->attached = 0;
    
    return get_connId_from_profileId(pWork->profileId);
}

void work_set_out_go_str(WORK * pWork, unsigned char * pStr)
{
    memset(pWork->outgoStr, 0, sizeof(pWork->outgoStr));
    pWork->outgoStrValid = 1;

    strncpy(pWork->outgoStr, pStr, sizeof(pWork->outgoStr) - 1);
}

void work_set_event_callback(WORK * pWork, WORK_EVENT_CALLBACK callback)
{
    if(callback != (WORK_EVENT_CALLBACK)NULL)
    {
        pWork->event_cb = callback;
    }
}

void work_set_len_method(WORK * pWork, LEN_METHOD method)
{
    pWork->lenMethod = method;
}

void work_set_request_len(WORK * pWork, unsigned int requestLen)
{
    pWork->requestLen = requestLen;
}

void work_set_len_match_algo(WORK * pWork, LEN_MATCH_ALGO algo)
{
    if(algo != (LEN_MATCH_ALGO)NULL)
    {
        pWork->algo = algo;
    }
}

void work_set_timeout(WORK * pWork, unsigned int timeout)
{
    pWork->timeout = timeout;
}

void work_do(WORK * pWork)
{
    unsigned int bPeriodic = 0;
    
    if( (pWork->cmdtype & CMDTYPE_PERIODIC) == CMDTYPE_READ )
    {
        bPeriodic = 1;
    }
    
    if( (pWork->cmdtype & CMDTYPE_READ ) == CMDTYPE_READ )
    {
        switch (pWork->state)
        {
            case STATE_NEW:
            {
                work_send_read_cmd(pWork);
                pWork->state = STATE_WAITRESP;
            }
            break;
            case STATE_WAITRESP:
            {
                if(work_is_read_complete(pWork) == RESULT_SUCCESS)
                {
                    pWork->state = STATE_SUCCESS;
                }
            }
            break;
            case STATE_SUCCESS:
            break;
            
            default:
            break;
        }
    }
    else if( (pWork->cmdtype & CMDTYPE_WRITE ) == CMDTYPE_WRITE )
    {
        
    }
    else
    {
        // do nothing
    }
}


