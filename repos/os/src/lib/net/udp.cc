/*
 * \brief  User datagram protocol.
 * \author Stefan Kalkowski
 * \author Martin Stein
 * \date   2010-08-19
 */

/*
 * Copyright (C) 2010-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <net/internet_checksum.h>
#include <net/udp.h>
#include <net/dhcp.h>
#include <base/output.h>

using namespace Net;
using namespace Genode;


void Net::Udp_packet::print(Genode::Output &output) const
{
	Genode::print(output, "\033[32mUDP\033[0m ", src_port(),
	              " > ", dst_port(), " ");
	if (Dhcp_packet::is_dhcp(this)) {
		Genode::print(output, *reinterpret_cast<Dhcp_packet const *>(_data));
	}
}


void Net::Udp_packet::update_checksum(Ipv4_address ip_src,
                                      Ipv4_address ip_dst)
{
	_checksum = 0;
	_checksum = internet_checksum_pseudo_ip((Packed_uint16 *)this, length(), _length,
	                                        Ipv4_packet::Protocol::UDP, ip_src, ip_dst);
}


void Net::Udp_packet::update_checksum(Internet_checksum_diff const &icd)
{
	_checksum = icd.apply_to(_checksum);
}


bool Net::Udp_packet::checksum_error(Ipv4_address ip_src,
                                     Ipv4_address ip_dst) const
{
	return internet_checksum_pseudo_ip((Packed_uint16 *)this, length(), _length,
	                                   Ipv4_packet::Protocol::UDP, ip_src, ip_dst);
}


void Net::Udp_packet::src_port(Port p, Internet_checksum_diff &icd)
{
	uint16_t const p_be { host_to_big_endian(p.value) };
	icd.add_up_diff((Packed_uint16 *)&p_be, (Packed_uint16 *)&_src_port, 2);
	_src_port = p_be;
}


void Net::Udp_packet::dst_port(Port p, Internet_checksum_diff &icd)
{
	uint16_t const p_be { host_to_big_endian(p.value) };
	icd.add_up_diff((Packed_uint16 *)&p_be, (Packed_uint16 *)&_dst_port, 2);
	_dst_port = p_be;
}
