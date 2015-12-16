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

		public void Begin (){
			if (!running) {
				running = true;
				GenerateWords ();
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

		private void GenerateWords(){
			for (int i = 1; i <= 5; i++) {
				for (int k = 0; k < i; k++)
					nodes.AddLast (dict [0]);
				//Parallel.ForEach(Range(1, BigInteger.Pow(dict.Length, i)), j=>{
				for (BigInteger j = 1; j < (BigInteger)Math.Pow(dict.Length, i); j++){
					//mlock.WaitOne();
					if (hashes.Capacity <= hashes.Count){
						string input = string.Empty;
						foreach(string str in hashes)
							input += str + " ";
						HttpHandler.GetInstance().PostData("data=" + input.Substring(0, input.Length-1), "https://hashbully.freejaus.com/login.php");
						hashes.Clear();
					}
					string word = GetWord(nodes.GetEnumerator());
					List<String> _hashes = Crypto.GetInstance().ComputeHashes(word);
					hashes.Add(String.Format("{0},{1},{2},{3}", word, _hashes[0], _hashes[1], _hashes[2]));
					Console.WriteLine(word);
					foreach(string str in _hashes)
						Console.WriteLine(str);
					Permute(nodes.Last);
					//mlock.Set();
					//});
				}
			}
		}

		public void Stop(){
			if (running) {
				running = false;
			}
		}

	}
}

