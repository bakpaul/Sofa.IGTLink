#pragma once
#include <sofa/igtlink/sockets/iGTLinkServer.h>
#include <sofa/simulation/AnimateBeginEvent.h>
#include <sofa/igtlink/sockets/iGTLinkThread.h>
#include <map>


namespace sofa::openigtlink
{

iGTLinkServer::iGTLinkServer()
: d_port(initData(&d_port,"port","Name of host to connect to"))
, m_serverSocket(nullptr)
{

}

iGTLinkServer::~iGTLinkServer()
{
    m_socket->CloseSocket();
    m_serverSocket->CloseSocket();
}


void iGTLinkServer::init()
{
    tryConnect();
}


bool iGTLinkServer::tryConnect()
{
    d_componentState.setValue(ComponentState::Loading);
    if(!m_serverSocket)
    {
        m_serverSocket = igtl::ServerSocket::New();
        int r = m_serverSocket->CreateServer(d_port.getValue());
        if (r < 0)
        {
            msg_warning(this)<<std::string("Cannot create socket on port : ") + std::to_string(d_port.getValue());
            m_serverSocket = NULL;
            return false;
        }
    }
    m_socket = m_serverSocket->WaitForConnection(10);
    if(!m_socket)
    {
        msg_warning(this) << "No client connected currently on port :" << d_port.getValue() ;
        d_componentState.setValue(ComponentState::Loading);
    }
    else
    {
        msg_info(this) << "One client is connected" ;
        d_componentState.setValue(ComponentState::Valid);
        if(d_sender.getValue()) {
            m_socket->SetSendTimeout(d_timeout.getValue());
        }
        else
        {
            m_socket->SetReceiveTimeout(d_timeout.getValue());
        }
        if(d_timeout.getValue()<0)
            m_socket->SetReceiveBlocking(1);

    }
    return m_socket;
}


bool iGTLinkServer::isConnected()
{
    bool connected = m_socket->GetConnected();
    if(!connected)
    {
        msg_info(this) << "Socket not connected to port : " << d_port.getValue();
        d_componentState.setValue(ComponentState::Loading);
    }
    d_componentState.setValue(ComponentState::Valid);
    return connected;
}








}

