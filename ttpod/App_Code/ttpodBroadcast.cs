using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using Microsoft.AspNet.SignalR;
using Microsoft.AspNet.SignalR.Hubs;

public class ttpodBroadcast : Hub
{
	[HubMethodName("BroadcastToPlay")]
	public void NotifyAll(string type, string title, string url)
	{
		Clients.All.playByNotified(type, title, url);
	}
}
