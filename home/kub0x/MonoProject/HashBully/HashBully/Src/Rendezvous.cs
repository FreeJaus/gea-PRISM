using System;
using System.Net;
using System.Threading;
using System.Net.Sockets;
using System.Collections;
using System.Collections.Generic;

namespace HashBully
{
	public class Rendezvous : Singleton<Rendezvous>
	{
		private List<NetClient> DHT;
		private const string domain = "hashbully.freejaus.com";
		private const ushort port = 4444;
		private NetClient client;

		private Rendezvous () : base() { 
			DHT = new List<NetClient> ();
			client = new NetClient (); 
		}

		public void PublishInfo(string input) {
			if (client.Connect (domain, port)) { //TODO Auth w/ sessid cookie
				client.SendData (input);
				ReadPeerList ();
			}
		}

		private void ReadPeerList(){
			string list = client.ReadData ();
			if (list != null) {
				string[] ips = list.Split (" ".ToCharArray ());
				ConnectToPeer(IPAddress.Parse (ips [0]));
				ConnectToPeer(IPAddress.Parse (ips [1]));
			}
		}

		private void ConnectToPeer(NetClient peer, IPAddress ip){
			int tries = 0;
			bool connected = false;
			while(!(connected = peer.Connect(ip.ToString(), 4444)) && tries < 20){
				tries++;
				Thread.Sleep (50);
			}
			if (connected)
				DHT.Add (peer);
		}

	}
}

