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
			}
		}

		public string ReadData(){
			string recv = null;
			try{
				using (NetworkStream netStream = sock.GetStream()){
					byte[] bytes = new byte[1024];
					int readbytes = netStream.Read(bytes,0,bytes.Length);
					Array.Resize(bytes, readbytes);
					recv = Encoding.ASCII.GetString(bytes);
				}
			}catch (Exception ex) { Console.WriteLine (ex.Message); }
			return recv;
		}

	}
}