using System;
using System.IO;
using System.Net;
using System.Text;
using System.Numerics;
using System.Threading;
using System.Collections;
using System.Threading.Tasks;
using System.Collections.Generic;
using System.Security.Cryptography;

namespace HashBully
{
	class MainClass
	{
		
		private static AutoResetEvent mlock = new AutoResetEvent (true);

		private static string dict = "abcdefghijklmnñopqrstuvwxyzABCDEFGHIJKLMNÑOPQRSTUVWXYZ0123456789 ";

		private static void PostData(string input){
			HttpWebRequest req = (HttpWebRequest)HttpWebRequest.Create ("http://www.hashbully.coxslot.com/upload.php");
			req.Accept = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
			req.UserAgent = "Mozilla/5.0 (X11; Fedora; Linux x86_64; rv:42.0) Gecko/20100101 Firefox/42.0";
			req.ContentType = "application/x-www-form-urlencoded; charset=UTF-8";
			req.ContentLength = input.Length;
			req.Method = "POST";
			req.BeginGetRequestStream (PostDataCallBack, new object[]{ input, req });
		}

		private static void PostDataCallBack(IAsyncResult async){
			string input = (string) ((object[])async.AsyncState) [0];
			HttpWebRequest req = (HttpWebRequest)((object[])async.AsyncState) [1];
			byte[] bytes = Encoding.ASCII.GetBytes (input);
			using (Stream reqStream = req.EndGetRequestStream (async)) {
				reqStream.Write (bytes, 0, bytes.Length);
				reqStream.Close ();
			}
			mlock.Set();
		}

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

		private static List<string> ComputeHashes(string input){
			List<string> hashes = new List<string> ();
			hashes.Add (ComputeMD5 (input));
			hashes.Add (ComputeSHA128 (input));
			hashes.Add (ComputeSHA256 (input));
			return hashes;
		}

		private static string GetWord(LinkedList<char>.Enumerator it){
			string word = null;
			while (it.MoveNext())
				word += it.Current;
			return word;
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
			dict = dict.Substring (0, dict.IndexOf ("Z") + 2);
			for (int i = 1; i <= n; i++) {
				LinkedList<char> nodes = new LinkedList<char> ();
				for (int k = 0; k < i; k++)
					nodes.AddLast (dict [0]);
				Parallel.ForEach(Range(1, BigInteger.Pow(dict.Length, i)), j=>{
					mlock.WaitOne();
					string word = GetWord(nodes.GetEnumerator());
					List<String> hashes = ComputeHashes(word);
					PostData(String.Format("data={0},{1},{2},{3}", word, hashes[0], hashes[1], hashes[2]));
					Permute(nodes.Last);
					Console.WriteLine(word);
					foreach(string str in hashes)
						Console.WriteLine(str);
					//mlock.Set();
				});
			}
		}

	}
}