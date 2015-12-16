using System;
using System.Text;
using System.Numerics;
using System.Collections;
using System.Collections.Generic;

namespace HashBully
{
	public class Generator : Singleton<Generator>
	{
		private bool running;
		private List<string> hashes = new List<string>(1000);
		private LinkedList<char> nodes = new LinkedList<char>();
		private static string dict = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";

		private Generator () : base() { running = false; }

		public void Begin (string word, ulong nwords){
			if (!running) {
				running = true;
				Console.WriteLine ("start={0} end={1}", word, nwords.ToString ());
				GenerateWords (word, nwords);
				Console.WriteLine ("Finished interval");
				Stop ();
			}
		}

		private string GetWord(LinkedList<char>.Enumerator it){
			string word = null;
			while (it.MoveNext())
				word += it.Current;
			return word;
		}

		private void Permute(LinkedListNode<char> node){
			if (node.Value != dict [dict.Length - 1]) {
				node.Value = dict [dict.IndexOf (node.Value) + 1];
			} else {
				node.Value = dict [0];
				Permute (node.Previous);
			}
		}

		private void UploadHashes(){
			if (hashes.Capacity <= hashes.Count){
				Console.WriteLine ("Sending hashes to server...");
				string input = string.Empty;
				foreach(string str in hashes)
					input += str + " ";
				HttpHandler.GetInstance().PostData("data=" + input.Substring(0, input.Length-1), "https://hashbully.freejaus.com/index/upload.php");
				hashes.Clear();
			}
		}

		private void GenerateWords(string _word, ulong nwords){
			try{
				for (int k = 0; k < _word.Length; k++)
					nodes.AddLast(_word[k]);
				//Parallel.ForEach(Range(1, BigInteger.Pow(dict.Length, i)), j=>{
				for (BigInteger j = 1; j < nwords; j++){
					//mlock.WaitOne();
					UploadHashes();
					string word = GetWord(nodes.GetEnumerator());
					List<String> _hashes = Crypto.GetInstance().ComputeHashes(word);
					hashes.Add(String.Format("{0},{1},{2},{3}", word, _hashes[0], _hashes[1], _hashes[2]));
					//Console.WriteLine(word);
					//foreach(string str in _hashes)
					//	Console.WriteLine(str);
					Permute(nodes.Last);
					//mlock.Set();
					//});
				}
			}catch{}
		}

		public void Stop(){
			if (running) {
				running = false;
			}
		}

	}
}

