//
// Created by paul on 27/01/25.
//

#include <sofa/igtlink/messages/iGTLinkMessageFactory.h>
namespace sofa::openigtlink
{
    iGTLinkMessageFactory::SPtr iGTLinkMessageFactory::getFactory()
    {
        static iGTLinkMessageFactory::SPtr factory = nullptr;
        if (factory == nullptr) factory = std::make_shared<iGTLinkMessageFactory>();
        return factory;
    }

}