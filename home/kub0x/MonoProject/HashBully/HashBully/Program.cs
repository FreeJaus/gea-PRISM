using System;

namespace HashBully
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			string username = Console.ReadLine ();
			string password = Console.ReadLine ();
			Console.Clear ();
			if (HttpHandler.GetInstance ().Login (username, password)) {
				Console.WriteLine ("Welcome to HashBully {0}!", username);
				Console.WriteLine ("Logged on {0}", DateTime.Now.ToString ());
				NATUPnP.GetInstance ().ScanDevices ();
			}
			Console.Read ();
		}
	}
}