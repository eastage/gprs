
#include "work.h"

static WORK workList[256];

static WORK * alloc_free_work(void)
{
    unsigned int ii = 0;
    
    for(; ii < sizeof(workList) / sizeof(WORK); ii++)
    {
        if( (workList[ii].workState == STATE_IDLE) || \
            (workList[ii].workState == STATE_TIMEOUT) || \
            (workList[ii].workState == STATE_SUCCESS))
        {
            return &workList[ii];
        }
    }

    return (WORK *)NULL;
}


void work_init(void)
{
    memset(workList, 0, sizeof(workList));
}

static RESULT work_add(WORK * pWork)
{   
    // TODO
    pWork->timeAdded = driverGetUptime();
    pWork->workState = ATSTATE_NEWWORK;
    
    return RESULT_OK;
}

/* AT_SISR */
void create_at_sisr(CONNECTION * pConn, LEN_METHOD method, unsigned int len, LEN_MATCH_ALGO match, unsigned int timeout)
{
    WORK * pWork = alloc_free_work();
    
    memset(pWork, 0, sizeof(WORK));

    /* create class */
    pWork->set_cmdtype = work_set_cmdtype;
    pWork->set_state   = work_set_state;
    pWork->set_profileId = work_attach_connection;
    pWork->get_profileId = work_detach_connection;
    pWork->set_evt_callback = work_set_event_callback;
    
    pWork->get_cmd_fixed_part = sisr_fixed_part;
    pWork->get_cmd_variable_part = sisr_variable_part;
    pWork->inject_incoming_data = &sisr_inject_incoming_data;
    
    pWork->set_len_method = work_set_len_method;
    pWork->set_data_len = work_set_request_len;
    pWork->set_data_timeout = work_set_timeout;
    pWork->set_len_match_algo = work_set_len_match_algo;

#if 0
    pWork->r_income_data_parser = &at_sisr_income_dataparser;
    pWork->r_income_data_cb = &connection_at_sisr_callback;
#endif

    /* instantiate */    
    (pWork->set_cmd_type)(pWork, CMDTYPE_READ);
    (pWork->set_cmd_state)(pWork, STATE_NEW);
    (pWork->set_out_go_str)(pWork, "AT^SISR=");
    (pWork->set_evt_callback)(pWork, (*pConn->getEventCallback()) );
    (pWork->set_len_method)(pWork, LEN_METHOD_MIN | LEN_METHOD_CR);
    (pWork->set_request_len)(pWork, len);
    (pWork->set_request_timeout)(pWork, timeout);
    (pWork->set_len_match_algo)(pWork, match);
    
    (pWork->attach_connection)(pWork, pConn);
    
    work_add(pWork);

    pConn->curWork = pWork;
}

#if 0
/* AT_SISW */
void create_at_sisw(WORK * pWork)
{
    memset(pWork, 0, sizeof(WORK));

    pWork->set_cmd_type(CMDTYPE_WRITE);
    
    pWork->r_set_outgo_str("AT^SISW=");
    pWork->r_income_data_parser = &at_sisr_income_dataparser;
    pWork->r_income_data_cb = &connection_at_sisr_callback;
}

void create_at_cops(WORK * )
{

}
#endif


