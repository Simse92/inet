//
// Copyright (C) 2013 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_MESSAGEDISPATCHER_H
#define __INET_MESSAGEDISPATCHER_H

#include "inet/common/IInterfaceRegistrationListener.h"
#include "inet/common/IProtocolRegistrationListener.h"
#include "packet/Message.h"
#include "inet/common/packet/Packet.h"

namespace inet {

/**
 * This class implements the corresponding module. See module documentation for more details.
 */
class INET_API MessageDispatcher : public cSimpleModule, public IProtocolRegistrationListener, public IInterfaceRegistrationListener
{
    protected:
        std::map<int, int> socketIdToGateIndex;
        std::map<int, int> interfaceIdToGateIndex;
        std::map<std::pair<int, ServicePrimitive>, int> serviceToGateIndex;
        std::map<std::pair<int, ServicePrimitive>, int> protocolToGateIndex;

    protected:
        virtual void initialize() override;
        virtual void arrived(cMessage *message, cGate *inGate, simtime_t t) override;
        virtual cGate *handlePacket(Packet *packet, cGate *inGate);
        virtual cGate *handleMessage(Message *request, cGate *inGate);

        virtual int computeSocketReqSocketId(Packet *packet);
        virtual int computeSocketIndSocketId(Packet *packet);
        virtual int computeInterfaceId(Packet *packet);
        virtual std::pair<int, ServicePrimitive> computeDispatch(Packet *packet);

        virtual int computeSocketReqSocketId(Message *message);
        virtual int computeSocketIndSocketId(Message *message);
        virtual int computeInterfaceId(Message *message);
        virtual std::pair<int, ServicePrimitive> computeDispatch(Message *message);

    public:
        virtual void handleRegisterInterface(const InterfaceEntry &interface, cGate *out, cGate *in) override;
        virtual void handleRegisterService(const Protocol& protocol, cGate *out, ServicePrimitive servicePrimitive) override;
        virtual void handleRegisterProtocol(const Protocol& protocol, cGate *in, ServicePrimitive servicePrimitive) override;
};

} // namespace inet

#endif // ifndef __INET_MESSAGEDISPATCHER_H
