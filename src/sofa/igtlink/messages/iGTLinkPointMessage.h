#pragma once

#include <sofa/igtlink/messages/iGTLinkMessage.h>

using namespace sofa::core::objectmodel;
namespace sofa::openigtlink
{
template<class DataType>
class iGTLinkPointMessage : public iGTLinkMessage
{
    typedef typename DataType::Coord Vec3;
public:
    SOFA_CLASS(SOFA_TEMPLATE(iGTLinkPointMessage,DataType),iGTLinkMessage);


    iGTLinkPointMessage();

    virtual igtl::MessageBase::Pointer getiGTLinkMessage() ;
    virtual void updateData(igtl::MessageBase::Pointer) ;

    Data<sofa::type::vector<Vec3>> d_points;
    Data<sofa::type::vector<double>> d_radius;
    Data<sofa::type::vector<std::string>> d_labels;
    Data<sofa::type::vector<std::string>> d_groups;
    Data<sofa::type::vector<std::string>> d_owners;
    Data<bool> d_enableRadius;
    Data<bool> d_enableLabels;
    Data<bool> d_enableGroups;
    Data<bool> d_enableOwners;
};
}
