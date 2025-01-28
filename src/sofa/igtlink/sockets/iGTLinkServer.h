#pragma once
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/objectmodel/Data.h>

#include <string>

#include <igtlOSUtil.h>
#include <igtlPointMessage.h>
#include <igtlServerSocket.h>
#include <sofa/igtlink/sockets/iGTLinkBase.h>
#include <sofa/igtlink/messages/iGTLinkMessage.h>

using namespace sofa::core::objectmodel;

namespace sofa::openigtlink
{
class iGTLinkServer : public iGTLinkBase
{
public:
    SOFA_CLASS(iGTLinkServer,iGTLinkBase);

    iGTLinkServer();
    ~iGTLinkServer();

    void init();

    virtual bool tryConnect();
    virtual bool isConnected();


private:
    igtl::ServerSocket::Pointer m_serverSocket;
    Data<int> d_port;

};
}

