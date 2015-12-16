using System;
using Mono.Nat;
using System.Net;
using Mono.Nat.Upnp;

namespace HashBully
{
	public class NATUPnP : Singleton<NATUPnP>
	{
		private bool running;
		private INatDevice upnpdev;

		private NATUPnP () : base() { running = false; }

		private void OnDeviceFound(object sender, DeviceEventArgs args){
			upnpdev = args.Device;
			upnpdev.CreatePortMap(new Mapping(Protocol.Tcp, 4444, 4444));
			Rendezvous.GetInstance ().PublishInfo (string.Format ("{0} {1}", 
				upnpdev.GetExternalIP ().ToString (), upnpdev.LocalAddress.ToString ()));
		}

		private void OnDeviceLost(object sender, DeviceEventArgs args){
			upnpdev	.DeletePortMap(new Mapping(Protocol.Tcp, 4444, 4444));
		}

		public void ScanDevices(){
			if (!running){
				running=true;
				NatUtility.DeviceFound += OnDeviceFound;
				NatUtility.DeviceLost += OnDeviceLost;
				NatUtility.StartDiscovery();
			}
		}

		public void StopScan() {
			if (running) {
				NatUtility.StopDiscovery ();
				running = false;
			}
		}

	}
}

