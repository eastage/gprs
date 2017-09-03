
static CONNECTION * conn = 0;
static unsigned int readLen = 0;

static unsigned char procPending[2];
static unsigned char rcvDataBuf[2][1500];
static unsigned int  rcvDataLen[2];

static SESSION_STATE state;
static unsigned int  s_sessionStart;

static reset_timeout(void)
{

}

void intest_session_callback(CONNECTION_EVENT event, void * pCallbackData, void * userData)
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


void intest_session_init()
{    
    conn = create_udp_connection();

    if(conn != 0)
    {
        conn->regUserCallback(&intest_session_callback);
    }
}

void intest_session_start()
{
    s_sessionStart = 1;
}

void intest_session_stop()
{
    s_sessionStart = 0;
}

void intest_session_drive()
{
    if(s_sessionStart == 0)
    {
        return;
    }

    if(session_timeout() == 1)
    {
        reset_timeout();
        state = SESSION_STATE_OFFLINE_IDLE;
    }
    
    if((conn != 0) && (conn->getState() == CONN_STATE_CONNECTED))
    {
        if(state == SESSION_STATE_OFFLINE_IDLE)
        {
            conn->writeData(generateLoginPacket(), 2000);
            state = SESSION_STATE_OFFLINE_WAIT;
        }
        else if(state == SESSION_STATE_OFFLINE_WAIT)
        {
            if(parseLoginReply() == 0)
            {
                state == SESSION_STATE_ONLINE_IDLE;
            }
        }
        else if(state == SESSION_STATE_ONLINE_IDLE)
        {
            if((procPending[0] == 1) || (procPending[1] == 1))
            {
                conn->writeData(processRequest(), 5000);
                state = SESSION_STATE_ONLINE_REPLY;
            }
            
            if(getReportDataLen() > 0)
            {
                conn->writeData(getReportData(), 5000);

                if(bNeedAck)
                {
                    state = SESSION_STATE_ONLINE_WAIT;
                }
            }
        }
        else if(state == SESSION_STATE_ONLINE_WAIT)
        {
            if((procPending[0] == 1) || (procPending[1] == 1))
            {
                if(repliedCorrectly())
                {
                    state = SESSION_STATE_ONLINE_IDLE;
                    return;
                }
            }
            
            if(timeout)
            {
                conn->writeData(getRetryData(), 2000);
            }
        }
    }
}

