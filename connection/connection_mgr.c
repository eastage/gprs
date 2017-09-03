
#define MAX_CONNECTIONS (3)

static CONNECTION connections[MAX_CONNECTIONS];

CONNECTION create_tcp_connection(void)
{
    unsigned int id = 0;
    CONNECTION * tcpConnection = (CONNECTION *)0;

    tcpConnection = get_free_connection(&id);
    
    tcpConnection->connName = "tcp";
    tcpConnection->cid = id;
    
    tcpConnection->init = &tcp_connection_init;
    tcpConnection->start = &tcp_connection_start;
    tcpConnection->stop = &tcp_connection_stop;
    tcpConnection->getState = &tcp_connection_getState;
    tcpConnection->registerStateCallback = ;
    tcpConnection->readData = &tcp_connection_readData;
    tcpConnection->writeData = &tcp_connection_writeData;

    return tcpConnection;
}

CONNECTION create_udp_connection(void)
{
    unsigned int id = 0;
    CONNECTION * udpConnection = (CONNECTION *)0;

    udpConnection = get_free_connection(&id);
    
    udpConnection->connName = "udp";
    udpConnection->cid = id;
    
    udpConnection->init = ;
    udpConnection->start = ;
    udpConnection->stop = ;
    udpConnection->getState = ;
    udpConnection->registerStateCallback = ;
    udpConnection->readData = ;
    udpConnection->writeData = ;

    return udpConnection;
}


