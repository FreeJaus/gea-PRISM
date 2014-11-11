/*
 * Rendezvous.h
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

#include <unordered_map>
#include "IListener.h"
#include "Peer.h"

class Rendezvous : public IListener {
	
	private:
		
		typedef int socket;
		socket server;
		
		std::unordered_map<std::string,Peer> dht;
		
		static Rendezvous *myInstance;
		
		Rendezvous() { }
		~Rendezvous() { }
	
	public:
	
		static Rendezvous* getInstance() { if (myInstance) myInstance = new Rendezvous(); return myInstance; }
		
		virtual ILISTENER_RESULT Start();
		virtual ILISTENER_RESULT Stop();
		virtual ILISTENER_RESULT BeginAccept();
		
};

