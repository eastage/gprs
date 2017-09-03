
unsigned char cmdBuf[128];

unsigned char * sisr_fixed_part(WORK * pWork)
{
    return "AT^SISR=";
}

unsigned char * sisr_variable_part(WORK * pWork)
{
    memset(cmdBuf, 0, sizeof(cmdBuf));

    sprintf("%d,%d", pWork->profileId, pWork->requestLen);
    return 
}

WORK_STATE sisr_inject_incoming_data(WORK * pWork, unsigned char * pData, unsigned int dataLen)
{
    if(pWork->state == STATE_NEW)
    {
        if(reponse_header_validity(pData, dataLen) == 1)
        {
            pWork->set_len_method(LEN_METHOD_FIXED);
            pWork->set_request_len(parsedLen = sisr_resp_line_parse(pData, dataLen));
            
            pWork->state = STATE_WAITRESP;
            return STATE_WAITRESP;
        }
        else
        {
            pWork->state = STATE_ERROR;
            return STATE_ERROR;
        }
    }

    if(pWork->state == STATE_WAITRESP)
    {
        if(dataLen != pWork->requestLen)
        {
            pWork->state = STATE_ERROR;
            return STATE_ERROR;
        }

        pWork->event_cb(WORKEVENT_READDONE, pData, dataLen);
        return STATE_SUCCESS;
    }
}

