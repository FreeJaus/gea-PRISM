using System;
using System.IO;
using System.Net;
using System.Text;

namespace HashBully
{
	public class HttpHandler : Singleton<HttpHandler>
	{
		bool logged;
		private Cookie sessid;
		private CookieContainer reqcookies;
		private CookieCollection cookies;

		private HttpHandler () : base() { reqcookies = new CookieContainer (); logged = false; }

		public bool Login(string username, string password){
			if (!logged) {
				PostData (string.Format ("username={0}&password={1}", username, password), "https://hashbully.freejaus.com/index/login.php");
				foreach (Cookie cook in cookies)
					if (cook.Name == "sessid")
						sessid = cook;
				if (sessid != null)
					logged = true;
			}
			return logged;
		}

		public void LogOut(){
			if (logged) {
				GetData ("https://hashbully.freejaus.com/index/logout.php");
				sessid = null;
				logged = false;
			}
		}

		public void GetData(string url){
			HttpWebRequest req = (HttpWebRequest)HttpWebRequest.Create (url);
			req.CookieContainer = reqcookies;
			req.UserAgent = "Mozilla/5.0 (X11; Fedora; Linux x86_64; rv:42.0) Gecko/20100101 Firefox/42.0";
			req.Host = "hashbully.freejaus.com";
			req.KeepAlive = false;
			using (HttpWebResponse resp = (HttpWebResponse)req.GetResponse ()){
				using (Stream resStream = resp.GetResponseStream ());
			}
		}

		public void PostData(string input, string url){
			HttpWebRequest req = (HttpWebRequest)HttpWebRequest.Create (url);
			req.CookieContainer = reqcookies;
			req.Accept = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
			req.Host = "hashbully.freejaus.com";
			req.UserAgent = "Mozilla/5.0 (X11; Fedora; Linux x86_64; rv:42.0) Gecko/20100101 Firefox/42.0";
			req.ContentType = "application/x-www-form-urlencoded";
			req.Method = "POST";
			req.KeepAlive = true;
			byte[] bytes = Encoding.ASCII.GetBytes (input);
			req.ContentLength = bytes.Length;
			using (Stream reqStream = req.GetRequestStream ()) {
				reqStream.Write (bytes, 0, bytes.Length);
			}
			using (HttpWebResponse resp = (HttpWebResponse)req.GetResponse ()){
				cookies = resp.Cookies;
				using (Stream resStream = resp.GetResponseStream ());
			}
		}

	}
}