using System;
using System.Text;
using System.Numerics;
using System.Security.Cryptography;
using System.Threading;
using System.Threading.Tasks;
using System.Collections;
using System.Collections.Generic;


namespace HashBully
{
	class MainClass
	{

		private static AutoResetEvent mlock = new AutoResetEvent (true);

		private static string dict = "abcdefghijklmnñopqrstuvwxyzABCDEFGHIJKLMNÑOPQRSTUVWXYZ0123456789 ";

		private static string ComputeMD5(string input){
			string hash = null;
			using (MD5CryptoServiceProvider md5csp = new MD5CryptoServiceProvider ()) {
				md5csp.ComputeHash (Encoding.ASCII.GetBytes (input));
				hash = BitConverter.ToString (md5csp.Hash).Replace ("-", "");
			}
			return hash;
		}

		private static string ComputeSHA128(string input){
			string hash = null;
			using (SHA1CryptoServiceProvider sha128csp = new SHA1CryptoServiceProvider ()) {
				sha128csp.ComputeHash (Encoding.ASCII.GetBytes (input));
				hash = BitConverter.ToString (sha128csp.Hash).Replace ("-", "");
			}
			return hash;
		}

		private static string ComputeSHA256(string input){
			string hash = null;
			using (SHA256CryptoServiceProvider sha256csp = new SHA256CryptoServiceProvider ()) {
				sha256csp.ComputeHash (Encoding.ASCII.GetBytes (input));
				hash = BitConverter.ToString (sha256csp.Hash).Replace ("-", "");
			}
			return hash;
		}

		private static List<string> ComputeHashes(LinkedList<char>.Enumerator it){
			string input = null;
			List<string> hashes = new List<string> ();
			while (it.MoveNext())
				input += it.Current;
			Console.WriteLine (input);
			hashes.Add (ComputeMD5 (input));
			hashes.Add (ComputeSHA128 (input));
			hashes.Add (ComputeSHA256 (input));
			return hashes;
		}

		private static IEnumerable<BigInteger> Range(BigInteger start, BigInteger end){
			for (BigInteger i = start; i < end; i++)
				yield return i;
		}

		private static void Permute(LinkedListNode<char> node){
			if (node.Value != dict [dict.Length - 1]) {
				node.Value = dict [dict.IndexOf (node.Value) + 1];
			} else {
				node.Value = dict [0];
				Permute (node.Previous);
			}
		}

		public static void Main (string[] args)
		{
			int n = Int32.Parse (Console.ReadLine ());
			dict = dict.Substring (0, dict.IndexOf ("Z") + 1);
			for (int i = 1; i <= n; i++) {
				LinkedList<char> nodes = new LinkedList<char> ();
				for (int k = 0; k < i; k++)
					nodes.AddLast (dict [0]);
				Parallel.ForEach(Range(1, BigInteger.Pow(dict.Length, i)), j=>{
					mlock.WaitOne();
					Permute(nodes.Last);
					foreach(string str in ComputeHashes(nodes.GetEnumerator()))
						Console.WriteLine(str);
					mlock.Set();
				});
			}
		}
	}
}
