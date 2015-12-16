using System;

namespace HashBully
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			//Login here
			if (HttpHandler.GetInstance ().Login ("bully", "bully"))
				Console.WriteLine ("logged");
			Console.Read ();
			HttpHandler.GetInstance ().LogOut ();
			Console.Read ();
		}
	}
}