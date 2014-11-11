/*
 * IListener.h
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


class IListener {
	
	public:
		
		enum ILISTENER_RESULT{
			ILISTENER_RESULT_LISTEN_OK = 0x0,
			ILISTENER_RESULT_LISTEN_ERROR = 0x1,
			ILISTENER_RESULT_CLIENT_ACCEPTED_OK = 0x2,
			ILISTENER_RESULT_CLIENT_ACCEPTED_ERROR = 0x3,
			ILISTENER_RESULT_CLIENT_SEND_OK = 0x4,
			ILISTENER_RESULT_CLIENT_SEND_ERROR = 0x5,
			ILISTENER_RESULT_CLIENT_RECV_OK = 0x6,
			ILISTENER_RESULT_CLIENT_RECV_ERROR = 0x7
		};
		
		virtual ILISTENER_RESULT Start() = 0;
		virtual ILISTENER_RESULT Stop() = 0;
		
	
};
