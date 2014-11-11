/*
 * IPeer.h
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

class IPeer {
	
	public:
	
		enum IPEER_RESULT{
			IPEER_RESULT_SEND_OK = 0x0,
			IPEER_RESULT_SEND_ERROR = 0x1,
			IPEER_RESULT_RECV_OK = 0x2,
			IPEER_RESULT_RECV_ERROR = 0x3
		};
		
		virtual IPEER_RESULT OnSend() = 0;
		virtual IPEER_RESULT OnReceive() = 0;
	
};
