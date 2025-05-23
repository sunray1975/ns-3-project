/*
 * Copyright (c) 2014 Piotr Gawlowicz
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Piotr Gawlowicz <gawlowicz.p@gmail.com>
 *
 */

#include "lte-ffr-algorithm.h"

#include "ns3/boolean.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("LteFfrAlgorithm");

/// Type 0 RBG allocation (see table 7.1.6.1-1 of 3GPP TS 36.213)
static const int Type0AllocationRbg[4] = {
    10,  // RBG size 1
    26,  // RBG size 2
    63,  // RBG size 3
    110, // RBG size 4
};

NS_OBJECT_ENSURE_REGISTERED(LteFfrAlgorithm);

LteFfrAlgorithm::LteFfrAlgorithm()
    : m_needReconfiguration(true)
{
}

LteFfrAlgorithm::~LteFfrAlgorithm()
{
}

TypeId
LteFfrAlgorithm::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::LteFfrAlgorithm")
            .SetParent<Object>()
            .SetGroupName("Lte")
            .AddAttribute("FrCellTypeId",
                          "Downlink FR cell type ID for automatic configuration,"
                          "default value is 0 and it means that user needs to configure FR "
                          "algorithm manually,"
                          "if it is set to 1,2 or 3 FR algorithm will be configured automatically",
                          UintegerValue(0),
                          MakeUintegerAccessor(&LteFfrAlgorithm::SetFrCellTypeId,
                                               &LteFfrAlgorithm::GetFrCellTypeId),
                          MakeUintegerChecker<uint8_t>())
            .AddAttribute("EnabledInUplink",
                          "If FR algorithm will also work in Uplink, default value true",
                          BooleanValue(true),
                          MakeBooleanAccessor(&LteFfrAlgorithm::m_enabledInUplink),
                          MakeBooleanChecker());
    return tid;
}

void
LteFfrAlgorithm::DoDispose()
{
    NS_LOG_FUNCTION(this);
}

uint16_t
LteFfrAlgorithm::GetUlBandwidth() const
{
    NS_LOG_FUNCTION(this);
    return m_ulBandwidth;
}

void
LteFfrAlgorithm::SetUlBandwidth(uint16_t bw)
{
    NS_LOG_FUNCTION(this << bw);
    switch (bw)
    {
    case 6:
    case 15:
    case 25:
    case 50:
    case 75:
    case 100:
        m_ulBandwidth = bw;
        break;

    default:
        NS_FATAL_ERROR("invalid bandwidth value " << bw);
        break;
    }
}

uint16_t
LteFfrAlgorithm::GetDlBandwidth() const
{
    NS_LOG_FUNCTION(this);
    return m_dlBandwidth;
}

void
LteFfrAlgorithm::SetDlBandwidth(uint16_t bw)
{
    NS_LOG_FUNCTION(this << bw);
    switch (bw)
    {
    case 6:
    case 15:
    case 25:
    case 50:
    case 75:
    case 100:
        m_dlBandwidth = bw;
        break;

    default:
        NS_FATAL_ERROR("invalid bandwidth value " << bw);
        break;
    }
}

void
LteFfrAlgorithm::SetFrCellTypeId(uint8_t cellTypeId)
{
    NS_LOG_FUNCTION(this << uint16_t(cellTypeId));
    m_frCellTypeId = cellTypeId;
    m_needReconfiguration = true;
}

uint8_t
LteFfrAlgorithm::GetFrCellTypeId() const
{
    NS_LOG_FUNCTION(this);
    return m_frCellTypeId;
}

int
LteFfrAlgorithm::GetRbgSize(int dlbandwidth)
{
    for (int i = 0; i < 4; i++)
    {
        if (dlbandwidth < Type0AllocationRbg[i])
        {
            return i + 1;
        }
    }

    return -1;
}

void
LteFfrAlgorithm::DoSetCellId(uint16_t cellId)
{
    NS_LOG_FUNCTION(this);
    m_cellId = cellId;
}

void
LteFfrAlgorithm::DoSetBandwidth(uint16_t ulBandwidth, uint16_t dlBandwidth)
{
    NS_LOG_FUNCTION(this);
    SetDlBandwidth(dlBandwidth);
    SetUlBandwidth(ulBandwidth);
}

} // end of namespace ns3
