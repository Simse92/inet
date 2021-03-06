//
// Copyright (C) 2008 Irene Ruengeler
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_SCTPALGORITHM_H
#define __INET_SCTPALGORITHM_H

#include "inet/common/INETDefs.h"
#include "inet/transportlayer/sctp/SctpAssociation.h"
#include "inet/transportlayer/sctp/SctpQueue.h"

namespace inet {
namespace sctp {

/**
 * Abstract base class for SCTP algorithms which encapsulate all behaviour
 * during data transfer state: flavour of congestion control, fast
 * retransmit/recovery, selective acknowledgement etc. Subclasses
 * may implement various sets and flavours of the above algorithms.
 */
class INET_API SctpAlgorithm : public cObject
{
  protected:
    SctpAssociation *assoc;    // we belong to this association
    SctpQueue *transmissionQ;
    SctpQueue *retransmissionQ;

  public:
    /**
     * Ctor.
     */
    SctpAlgorithm() { assoc = nullptr; transmissionQ = nullptr; retransmissionQ = nullptr; }

    /**
     * Virtual dtor.
     */
    virtual ~SctpAlgorithm() {}

    inline void setAssociation(SctpAssociation *_assoc)
    {
        assoc = _assoc;
        transmissionQ = assoc->getTransmissionQueue();
        retransmissionQ = assoc->getRetransmissionQueue();
    }

    virtual void initialize() {}

    virtual SctpStateVariables *createStateVariables() = 0;

    virtual void established(bool active) = 0;

    virtual void connectionClosed() = 0;

    virtual void processTimer(cMessage *timer, SctpEventCode& event) = 0;

    virtual void sendCommandInvoked(SctpPathVariables *path) = 0;

    virtual void receivedDataAck(uint32 firstSeqAcked) = 0;

    virtual void receivedDuplicateAck() = 0;

    virtual void receivedAckForDataNotYetSent(uint32 seq) = 0;

    virtual void sackSent() = 0;

    virtual void dataSent(uint32 fromseq) = 0;
};

} // namespace sctp
} // namespace inet

#endif // ifndef __INET_SCTPALGORITHM_H

