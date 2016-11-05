using System.Collections.Generic;
using Android.App;
using Android.Content;
using Keepass2android.Pluginsdk;

namespace Keepass.Transfer.Plugin.Receivers
{
    [BroadcastReceiver(Enabled = true, Exported = true)]
    [IntentFilter(new[]{
        "keepass2android.ACTION_TRIGGER_REQUEST_ACCESS",
        "keepass2android.ACTION_RECEIVE_ACCESS",
        "keepass2android.ACTION_REVOKE_ACCESS"
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