using System;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using System.Security.Cryptography;

namespace HashBully
{
	public class Crypto : Singleton<Crypto>
	{
		private Crypto () : base() {}

		private string ComputeMD5(string input){
			string hash = null;
			using (MD5CryptoServiceProvider md5csp = new MD5CryptoServiceProvider ()) {
				md5csp.ComputeHash (Encoding.ASCII.GetBytes (input));
				hash = BitConverter.ToString (md5csp.Hash).Replace ("-", "");
			}
			return hash;
		}

		private string ComputeSHA128(string input){
			string hash = null;
			using (SHA1CryptoServiceProvider sha128csp = new SHA1CryptoServiceProvider ()) {
				sha128csp.ComputeHash (Encoding.ASCII.GetBytes (input));
				hash = BitConverter.ToString (sha128csp.Hash).Replace ("-", "");
			}
			return hash;
		}

		private string ComputeSHA256(string input){
			string hash = null;
			using (SHA256CryptoServiceProvider sha256csp = new SHA256CryptoServiceProvider ()) {
				sha256csp.ComputeHash (Encoding.ASCII.GetBytes (input));
				hash = BitConverter.ToString (sha256csp.Hash).Replace ("-", "");
			}
			return hash;
		}

		public List<string> ComputeHashes(string input){
			List<string> hashes = new List<string> ();
			hashes.Add (ComputeMD5 (input));
			hashes.Add (ComputeSHA128 (input));
			hashes.Add (ComputeSHA256 (input));
			return hashes;
		}

	}
}