using System.Collections.Generic;
using Android.App;
using Android.Content;
using Keepass2android.Pluginsdk;

namespace Keepass.Transfer.Plugin.Receivers
{
    [BroadcastReceiver(Enabled = true, Exported = true)]
    [IntentFilter(new[]{
        Strings.ActionTriggerRequestAccess,
        Strings.ActionReceiveAccess,
        Strings.ActionRevokeAccess
    })]
    public class TransferAccessReceiver : PluginAccessBroadcastReceiver
    {
        public override IList<string> Scopes => new List<string>
        {
            Strings.ScopeCurrentEntry,
            Strings.ScopeQueryCredentials
        };
    }
}