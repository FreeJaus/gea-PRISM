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
			args.Device.CreatePortMap(new Mapping(Protocol.Tcp, 4444, 4444));
		}

		private void OnDeviceLost(object sender, DeviceEventArgs args){
			args.Device.DeletePortMap(new Mapping(Protocol.Tcp, 4444, 4444));
		}

		public IPAddress[] GetIPs(){
			IPAddress[] ips = null;
			if (upnpdev != null) {
				ips = new IPAddress[2] { upnpdev.LocalAddress, upnpdev.GetExternalIP () };
			}
			return ips;
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

