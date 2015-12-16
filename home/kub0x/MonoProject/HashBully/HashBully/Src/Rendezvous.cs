using System;
using System.Net;
using System.Net.Sockets;


namespace HashBully
{
	public class Rendezvous : Singleton<Rendezvous>
	{
		
		private const string domain = "hashbully.freejaus.com";
		private const ushort port = 4444;

		private NetClient client;

		private Rendezvous () : base() { client = new NetClient (); }

		public void PublishInfo() {
			if (client.Connect (domain, port)) {
				IPAddress[] ips = NATUPnP.GetInstance ().GetIPs ();
				string data = string.Format ("{0}:4444 {1}:4444 op={2}", ips[0].ToString(), ips[1].ToString(), "F");
			}
		}

	}
}

