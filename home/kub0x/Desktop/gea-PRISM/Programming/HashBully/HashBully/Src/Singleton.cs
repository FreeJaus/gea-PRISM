using System;

namespace HashBully
{
	public abstract class Singleton<T>
	{
		private static T myInstance = (T)Activator.CreateInstance(typeof(T), true);

		protected Singleton() {}

		public static T GetInstance(){ return myInstance; }
	}
}