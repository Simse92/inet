//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef __INET_UDPAPP_H
#define __INET_UDPAPP_H

#include "inet/applications/base/ApplicationBase.h"
#include "inet/common/newqueue/contract/IPacketConsumer.h"
#include "inet/common/newqueue/contract/IPacketProducer.h"
#include "inet/common/newqueue/contract/IPacketQueueingElement.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"

namespace inet {

class INET_API UdpApp : public ApplicationBase, public queue::IPacketConsumer, public queue::IPacketQueueingElement, public UdpSocket::ICallback
{
  protected:
    bool dontFragment = false;
    queue::IPacketProducer *trafficGenerator = nullptr;
    UdpSocket socket;
    int numSent = 0;
    int numReceived = 0;

  protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void finish() override;
    virtual void refreshDisplay() const override;

    virtual void setSocketOptions();

    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
    virtual void socketClosed(UdpSocket *socket) override;

    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

  public:
    virtual bool supportsPushPacket(cGate *gate) { return true; }
    virtual bool supportsPopPacket(cGate *gate) { return false; }
    virtual bool canPushSomePacket(cGate *gate = nullptr) override { return true; }
    virtual bool canPushPacket(Packet *packet, cGate *gate = nullptr) override { return true; }
    virtual void pushPacket(Packet *packet, cGate *gate = nullptr) override;
};

} // namespace inet

#endif // ifndef __INET_UDPAPP_H

