using System.Collections.Generic;
using System.Linq;
using Android.App;
using Android.Content;
using Keepass2android.Pluginsdk;
using Newtonsoft.Json;
using Android.OS;

namespace Keepass.Transfer.Plugin.Receivers
{
    [BroadcastReceiver(Enabled = true, Exported = true)]
    [IntentFilter(new[]{
        Strings.ActionOpenEntry,
        Strings.ActionEntryActionSelected
    })]
    public class TransferActionReceiver : PluginActionBroadcastReceiver
    {
        private Intent _currentIntent;

        protected override void OpenEntry(OpenEntryAction openAction)
        {
            try
            {
                openAction.AddEntryAction(openAction.Context.GetString(Resource.String.action_entry),
                    Resource.Drawable.context_ic,
                    new Bundle());
            }
            catch (PluginAccessException e)
            {
                e.PrintStackTrace();
            }
        }


        protected override void ActionSelected(ActionSelectedAction actionSelected)
        {
            var intent = new Intent(actionSelected.Context, typeof(ManageTransferActivity));

            intent.PutExtra(ManageTransferActivity.DataEntriesExtra,
                JsonConvert.SerializeObject(new Dictionary<string, string>(actionSelected.EntryFields)));

            string[] protFields = actionSelected.GetProtectedFieldsList();
            if (protFields == null) {//in case he fixed it --> only if not successful
                var extra = _currentIntent.GetStringExtra(Strings.ExtraProtectedFieldsList);
                protFields = JsonConvert.DeserializeObject<string[]>(extra);
            }
            intent.PutStringArrayListExtra(ManageTransferActivity.GuardedEntriesExtra, protFields);

            intent.SetFlags(ActivityFlags.NewTask | ActivityFlags.ClearTask);
            actionSelected.Context.StartActivity(intent);
        }

        public override void OnReceive(Context context, Intent intent)
        {
            _currentIntent = intent;
            base.OnReceive(context, intent);
            _currentIntent = null;
        }
    }
}