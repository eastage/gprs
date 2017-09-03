
#include "connection.h"

void tcp_connection_callback(WORK_EVENT event, void * pCallbackData, void * userData)
{
    switch(event)
    {
        case CONN_EVENT_INIT:
        break;

        case CONN_EVENT_CONNECTING:
        break;

        case CONN_EVENT_CONNECTED:
        break;

        case CONN_EVENT_DATAINCOMING:
        {
            conn->readData(1, 0, NULL, 1000);
        }
        break;

        case CONN_EVENT_DATAOBTAINED:
        {
            CONNECTION_READ_DATA * pRead = (CONNECTION_READ_DATA *)pCallbackData;

            if(pRead != 0)
            {
                memcpy(&rcvDataBuf[0][0], pRead->pData, pRead->dataLen);
                rcvDataLen[0] = pRead->dataLen;
                procPending[0] = 1;
            }
        }
        break;

        case CONN_EVENT_SENDACCEPTED:
        {

        }
        break;

        case CONN_EVENT_SENDCOMPLETE:
        {
            if(state == SESSION_STATE_ONLINE_REPLY)
            {
                state = SESSION_STATE_ONLINE_IDLE;
            }
        }
        break;
        
        default:
        break;
    }
}

void tcp_connection_init(CONNECTION * pConn)
{
    pConn->connName = "tcp";
    pConn->cid = allocConnectionId();
    pConn->rebootCnt = 0;
    pConn->ticks = 0;
    pConn->connState = CONN_STATE_NEW;
}

void tcp_connection_start(CONNECTION * pConn)
{
    pConn->bStarted = 1;
}

void tcp_connection_stop(CONNECTION * pConn)
{
    pConn->bStarted = 0;
}

void tcp_connection_getState(CONNECTION * pConn)
{
    return pConn->connState;
}

void tcp_connection_regCallback(CONNECTION * pConn, CONNECTION_CALLBACK userCallback)
{
    pConn->usercallback = userCallback;
}

unsigned int tcp_connection_readData(CONNECTION * pConn, unsigned int minLen, unsigned int fixedLen, LEN_MATCH_ALGO * match, unsigned int timeout)
{
    create_at_sisr(pConn, LEN_METHOD_MIN | LEN_METHOD_CR, 1500, (LEN_MATCH_ALGO *)NULL, 3000);
}

void tcp_connection_writeData(CONNECTION * pConn, CONNECTION_CALLBACK * userCallback)
{

}

void tcp_connection_drive(CONNECTION * pConn)
{
    if(pConn->bStarted == 1)
    {
        switch (pConn->connState)
        {
            case CONN_STATE_NEW:
            case CONN_STATE_DISCONNECTED:
            {
                
            }
            break;
            case CONN_STATE_CONNECTED:
            {

            }
            break;
            default:
            break;
        }
    }
}



