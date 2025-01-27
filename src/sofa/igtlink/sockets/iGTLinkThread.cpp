#include <sofa/igtlink/sockets/iGTLinkBase.h>
#include <sofa/igtlink/sockets/iGTLinkThread.h>
#include <igtlPointMessage.h>
#include <igtlPolyDataMessage.h>

namespace sofa::openigtlink
{


    void iGTLinkBaseThread::mainLoop()
    {
        m_running.store(true);
        while(m_running.load(std::memory_order_relaxed))
        {
            if(!m_link->isConnected())
                m_link->tryConnect();
            else
                doLoop();
        }
    }

    bool iGTLinkBaseThread::threadRunning()
    {
        return m_running.load(std::memory_order_relaxed);
    }

    void iGTLinkBaseThread::launchThread()
    {
        m_thread.reset(new std::thread(std::bind(&iGTLinkBaseThread::mainLoop,this)));
    }

    void iGTLinkBaseThread::stopThread()
    {
        if(m_thread)
        {
            m_running.store(false);
            if (m_thread->joinable())
                m_thread->join();
        }
        m_thread = NULL;
    }

    iGTLinkReceiverThread::iGTLinkReceiverThread(iGTLinkBase* _link) : iGTLinkBaseThread(_link), m_dataAvailable(false)
    {
        m_currentData.store(0);
    }

    bool iGTLinkReceiverThread::isDataAvailable()
    {
        return m_dataAvailable.load(std::memory_order_relaxed);
    }

    void iGTLinkReceiverThread::swapData()
    {
        unsigned swapped = (m_currentData.load(std::memory_order_relaxed) +1) % 2;
        m_dataAvailable.store(false);
        m_currentData.store(swapped);
//
//        //Lock to make sure the thread has stopped to work on it
//        m_mutex[(m_currentData.load(std::memory_order_relaxed) +1) % 2].lock();
//        //Unlock now we know the thread is not working on it anymore
//        m_mutex[(m_currentData.load(std::memory_order_relaxed) +1) % 2].unlock();
    }

    void iGTLinkReceiverThread::doLoop()
    {
//        std::lock_guard<std::mutex> lock(m_mutex[m_currentData.load(std::memory_order_relaxed)]);

        igtl::MessageHeader::Pointer headerMsg;
        headerMsg = igtl::MessageHeader::New();

        // Initialize receive buffer
        headerMsg->InitPack();

        // Receive generic header from the socket
        bool timeout(false);
        igtlUint64 r = m_link->m_socket->Receive(headerMsg->GetPackPointer(), headerMsg->GetPackSize(), timeout);
        if (r == 0)
        {
            m_link->m_socket->CloseSocket();
        }
        if (r != headerMsg->GetPackSize())
        {
            return;
        }
        headerMsg->Unpack();

        igtl::MessageBase::Pointer dataMessage = iGTLinkMessageFactory::getMessagePtr(headerMsg->GetDeviceType());
        if (dataMessage == nullptr)
        {
            msg_warning(m_link) << "Message  of type " << headerMsg->GetDeviceType() << " received, but not present in the iGTLinkMessageFactory";
            return;
        }


        dataMessage->SetMessageHeader(headerMsg);
        dataMessage->AllocatePack();
        m_link->m_socket->Receive(dataMessage->GetPackBodyPointer(), dataMessage->GetPackBodySize(), timeout);
        m_dataStructures[m_currentData.load(std::memory_order_relaxed)][dataMessage->GetDeviceName()] = dataMessage;
        m_dataAvailable.store(true);
    }

    std::map<std::string,igtl::MessageBase::Pointer>& iGTLinkReceiverThread::getAvailableData()
    {
        swapData();

        return m_dataStructures[(m_currentData.load(std::memory_order_relaxed) +1) % 2];
    }


}
