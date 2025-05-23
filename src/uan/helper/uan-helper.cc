/*
 * uan-helper.cc
 *
 *  Copyright (c) 2008 University of Washington
 *
 *  SPDX-License-Identifier: GPL-2.0-only
 *
 *  Created on: 9-Oct-2008
 *  Author: Leonard Tracy <lentracy@u.washington.edu>
 *
 *
 */

#include "uan-helper.h"

#include "ns3/config.h"
#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/simulator.h"
#include "ns3/uan-channel.h"
#include "ns3/uan-mac.h"
#include "ns3/uan-net-device.h"
#include "ns3/uan-noise-model-default.h"
#include "ns3/uan-phy.h"
#include "ns3/uan-prop-model-ideal.h"
#include "ns3/uan-prop-model.h"
#include "ns3/uan-transducer.h"
#include "ns3/uan-tx-mode.h"

#include <sstream>
#include <string>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("UanHelper");

/**
 * Ascii trace callback on Phy transmit events.
 *
 * @param os The output stream.
 * @param context The node and device ids.
 * @param packet The transmitted packet.
 * @param txPowerDb The transmission power.
 * @param mode The transmission mode.
 */
static void
AsciiPhyTxEvent(std::ostream* os,
                std::string context,
                Ptr<const Packet> packet,
                double txPowerDb [[maybe_unused]],
                UanTxMode mode [[maybe_unused]])
{
    *os << "+ " << Simulator::Now().GetSeconds() << " " << context << " " << *packet << std::endl;
}

/**
 * Ascii trace callback on successful packet reception.
 *
 * @param os The output stream.
 * @param context The node and device ids.
 * @param packet The received packet.
 * @param snr The received signal to noise ratio.
 * @param mode The channel transmission mode.
 */
static void
AsciiPhyRxOkEvent(std::ostream* os,
                  std::string context,
                  Ptr<const Packet> packet,
                  double snr [[maybe_unused]],
                  UanTxMode mode [[maybe_unused]])
{
    *os << "r " << Simulator::Now().GetSeconds() << " " << context << " " << *packet << std::endl;
}

UanHelper::UanHelper()
{
    m_mac.SetTypeId("ns3::UanMacAloha");
    m_phy.SetTypeId("ns3::UanPhyGen");
    m_transducer.SetTypeId("ns3::UanTransducerHd");
}

UanHelper::~UanHelper()
{
}

void
UanHelper::EnableAscii(std::ostream& os, uint32_t nodeid, uint32_t deviceid)
{
    Packet::EnablePrinting();
    std::ostringstream oss;

    oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::UanNetDevice/Phy/RxOk";
    Config::Connect(oss.str(), MakeBoundCallback(&AsciiPhyRxOkEvent, &os));

    oss.str("");

    oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::UanNetDevice/Phy/Tx";
    Config::Connect(oss.str(), MakeBoundCallback(&AsciiPhyTxEvent, &os));
}

void
UanHelper::EnableAscii(std::ostream& os, NetDeviceContainer d)
{
    for (auto i = d.Begin(); i != d.End(); ++i)
    {
        Ptr<NetDevice> dev = *i;
        EnableAscii(os, dev->GetNode()->GetId(), dev->GetIfIndex());
    }
}

void
UanHelper::EnableAscii(std::ostream& os, NodeContainer n)
{
    NetDeviceContainer devs;
    for (auto i = n.Begin(); i != n.End(); ++i)
    {
        Ptr<Node> node = *i;
        for (uint32_t j = 0; j < node->GetNDevices(); ++j)
        {
            devs.Add(node->GetDevice(j));
        }
    }
    EnableAscii(os, devs);
}

void
UanHelper::EnableAsciiAll(std::ostream& os)
{
    EnableAscii(os, NodeContainer::GetGlobal());
}

NetDeviceContainer
UanHelper::Install(NodeContainer c) const
{
    Ptr<UanChannel> channel = CreateObject<UanChannel>();
    Ptr<UanNoiseModelDefault> noise = CreateObject<UanNoiseModelDefault>();
    channel->SetPropagationModel(CreateObject<UanPropModelIdeal>());
    channel->SetNoiseModel(noise);

    return Install(c, channel);
}

NetDeviceContainer
UanHelper::Install(NodeContainer c, Ptr<UanChannel> channel) const
{
    NetDeviceContainer devices;
    for (auto i = c.Begin(); i != c.End(); i++)
    {
        Ptr<Node> node = *i;

        Ptr<UanNetDevice> device = Install(node, channel);

        devices.Add(device);
        NS_LOG_DEBUG("node=" << node << ", mob=" << node->GetObject<MobilityModel>());
    }
    return devices;
}

Ptr<UanNetDevice>
UanHelper::Install(Ptr<Node> node, Ptr<UanChannel> channel) const
{
    Ptr<UanNetDevice> device = CreateObject<UanNetDevice>();

    Ptr<UanMac> mac = m_mac.Create<UanMac>();
    Ptr<UanPhy> phy = m_phy.Create<UanPhy>();
    Ptr<UanTransducer> trans = m_transducer.Create<UanTransducer>();

    mac->SetAddress(Mac8Address::Allocate());
    device->SetMac(mac);
    device->SetPhy(phy);
    device->SetTransducer(trans);
    device->SetChannel(channel);

    node->AddDevice(device);

    return device;
}

int64_t
UanHelper::AssignStreams(NetDeviceContainer c, int64_t stream)
{
    int64_t currentStream = stream;
    Ptr<NetDevice> netDevice;
    for (auto i = c.Begin(); i != c.End(); ++i)
    {
        netDevice = (*i);
        Ptr<UanNetDevice> uan = DynamicCast<UanNetDevice>(netDevice);
        if (uan)
        {
            currentStream += uan->GetPhy()->AssignStreams(currentStream);
            currentStream += uan->GetMac()->AssignStreams(currentStream);
        }
    }
    return (currentStream - stream);
}

} // end namespace ns3
