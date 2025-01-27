#pragma once

#include <string>
#include <map>
#include <functional>
#include <iostream>
#include <memory>
#include <igtlMessageBase.h>


namespace sofa::openigtlink
{
class iGTLinkMessageFactory
{
public:

    typedef typename std::shared_ptr<iGTLinkMessageFactory> SPtr;

    template<class MessageType>
    int doRegisterMessage(std::string iGTLinkDeviceType)
    {
        auto found = m_internalFactoryMap.find(iGTLinkDeviceType);
        if(found != m_internalFactoryMap.end())
        {
            std::cout<<"iGTLinkMessageFactory : Warning " << iGTLinkDeviceType << " already registered" <<std::endl;
            found->second = &iGTLinkMessageFactory::createMessage<MessageType>;
        }
        else
        {
            m_internalFactoryMap[iGTLinkDeviceType] = &iGTLinkMessageFactory::createMessage<MessageType>;
        }
        return 0;
    }


    template<class MessageType>
    igtl::MessageBase::Pointer getMessagePtr(std::string iGTLinkDeviceType)
    {
        auto found = m_internalFactoryMap.find(iGTLinkDeviceType);
        if(found != m_internalFactoryMap.end())
        {
            return found->second();
        }
        return nullptr;
    }

    static SPtr getFactory();

    template<class MessageType>
    static igtl::MessageBase::Pointer createMessage()
    {
        return MessageType::New();
    }

    template<class MessageType>
    static int registerMessage(std::string iGTLinkDeviceType)
    {
        return getFactory()->doRegisterMessage<MessageType>(iGTLinkDeviceType);
    }


private:
    iGTLinkMessageFactory()=default;
    ~iGTLinkMessageFactory()=default;

    std::map<std::string,std::function<igtl::MessageBase::Pointer ()> > m_internalFactoryMap;

};
}

