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
#include "inet/common/newqueue/PacketProducer.h"
#include "inet/common/Simsignals.h"
#include "inet/common/StringFormat.h"

namespace inet {
namespace queue {

Define_Module(PacketProducer);

void PacketProducer::initialize(int stage)
{
    PacketSourceBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        displayStringTextFormat = par("displayStringTextFormat");
        outputGate = gate("out");
        consumer = check_and_cast<IPacketConsumer *>(getConnectedModule(outputGate));
        productionIntervalParameter = &par("productionInterval");
        productionTimer = new cMessage("ProductionTimer");
    }
    else if (stage == INITSTAGE_LAST) {
        checkPushPacketSupport(outputGate);
//        scheduleProductionTimer();
        updateDisplayString();
    }
}

void PacketProducer::handleMessage(cMessage *message)
{
    if (message == productionTimer) {
        if (consumer->canPushSomePacket(outputGate->getPathEndGate())) {
            producePacket();
            scheduleProductionTimer();
        }
    }
    else
        throw cRuntimeError("Unknown message");
}

void PacketProducer::scheduleProductionTimer()
{
    scheduleAt(simTime() + productionIntervalParameter->doubleValue(), productionTimer);
}

void PacketProducer::producePacket()
{
    auto packet = createPacket();
    EV_INFO << "Producing packet " << packet->getName() << "." << endl;
    pushOrSendPacket(packet, outputGate, consumer);
    updateDisplayString();
}

void PacketProducer::handleCanPushPacket(cGate *gate)
{
    if (gate->getPathStartGate() == outputGate && !productionTimer->isScheduled()) {
        producePacket();
        scheduleProductionTimer();
    }
}

} // namespace queue
} // namespace inet

