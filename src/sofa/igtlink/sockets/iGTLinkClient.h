#pragma once
#include <sofa/core/objectmodel/Data.h>

#include <string>

#include <igtlOSUtil.h>
#include <igtlPointMessage.h>
#include <sofa/igtlink/sockets/iGTLinkBase.h>

using namespace sofa::core::objectmodel;

namespace sofa::openigtlink
{
class iGTLinkClient : public iGTLinkBase
{
public:
    SOFA_CLASS(iGTLinkClient,iGTLinkBase);

    iGTLinkClient();
    ~iGTLinkClient() = default;

    void init();

    virtual bool tryConnect();
    virtual bool isConnected();

private:
    Data<std::string> d_hostname;
    Data<int> d_port;

};
}

