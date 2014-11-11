/*
 * EndPoint.h
 * 
 * Copyright 2014 Unknown <kub0x@vps112366.ovh.net>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

class EndPoint {
	
	private:
	
		sockaddr_in *m_sockaddr;
	
	public:
	
		std::string GetIPAddress() { return  std::string(inet_ntoa(m_sockaddr->sin_addr)); }
		unsigned short GetPort() { return m_sockaddr->sin_port; }
	
};
