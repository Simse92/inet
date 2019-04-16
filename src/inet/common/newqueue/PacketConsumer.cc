//
// Copyright (C) OpenSim Ltd.
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
// along with this program; if not, see http://www.gnu.org/licenses/.
//

#include "inet/common/ModuleAccess.h"
#include "inet/common/newqueue/PacketConsumer.h"
#include "inet/common/Simsignals.h"
#include "inet/common/StringFormat.h"

namespace inet {
namespace queue {

Define_Module(PacketConsumer);

void PacketConsumer::initialize(int stage)
{
    if (stage == INITSTAGE_LOCAL) {
        displayStringTextFormat = par("displayStringTextFormat");
        inputGate = gate("in");
        producer = dynamic_cast<IPacketProducer *>(getConnectedModule(inputGate));
        consumptionIntervalParameter = &par("consumptionInterval");
        consumptionTimer = new cMessage("ConsumptionTimer");
        WATCH(numPacket);
        WATCH(totalLength);
    }
    else if (stage == INITSTAGE_LAST) {
        checkPushPacketSupport(inputGate);
        scheduleConsumptionTimer();
        updateDisplayString();
    }
}

void PacketConsumer::handleMessage(cMessage *message)
{
    if (message == consumptionTimer) {
        if (producer != nullptr)
            producer->handleCanPushPacket(inputGate);
    }
    else
        PacketConsumerBase::handleMessage(message);
}

void PacketConsumer::scheduleConsumptionTimer()
{
    simtime_t interval = consumptionIntervalParameter->doubleValue();
    if (interval != 0 || consumptionTimer->getArrivalModule() == nullptr)
        scheduleAt(simTime() + interval, consumptionTimer);
}

void PacketConsumer::pushPacket(Packet *packet, cGate *gate)
{
    if (consumptionTimer->isScheduled() && consumptionTimer->getArrivalTime() > simTime())
        throw cRuntimeError("Another packet is already being consumed");
    else {
        emit(packetPushedSignal, packet);
        consumePacket(packet);
        scheduleConsumptionTimer();
    }
}

void PacketConsumer::consumePacket(Packet *packet)
{
    EV_INFO << "Consuming packet " << packet->getName() << "." << endl;
    numPacket++;
    totalLength += packet->getDataLength();
    PacketDropDetails details;
    details.setReason(OTHER_PACKET_DROP);
    emit(packetDroppedSignal, packet, &details);
    updateDisplayString();
    delete packet;
}

void PacketConsumer::updateDisplayString()
{
    auto text = StringFormat::formatString(displayStringTextFormat, [&] (char directive) {
        static std::string result;
        switch (directive) {
            case 'p':
                result = std::to_string(numPacket);
                break;
            case 'l':
                result = totalLength.str();
                break;
            default:
                throw cRuntimeError("Unknown directive: %c", directive);
        }
        return result.c_str();
    });
    getDisplayString().setTagArg("t", 0, text);
}

} // namespace queue
} // namespace inet

