using System;
using System.Net;
using System.Text;
using System.Net.Sockets;

namespace HashBully
{
	public class NetClient
	{

		private TcpClient sock;

		public NetClient ()
		{
			sock = new TcpClient ();
		}

		public bool Connect(string domain, ushort port){
			sock.Connect (new IPEndPoint (Dns.GetHostAddresses (domain) [0], port));
			return sock.Connected;
		}

		public void SendData(string input){
			using (NetworkStream netStream = sock.GetStream ()) {
				byte[] bytes = Encoding.ASCII.GetBytes (input);
				netStream.Write (bytes,0,bytes.Length);
				netStream.Close ();
			}
		}

	}
}