using System.Collections.Generic;
using System.Linq;
using Android.App;
using Android.Content;
using Keepass2android.Pluginsdk;
using Newtonsoft.Json;

namespace Keepass.Transfer.Plugin.Receivers
{
	[BroadcastReceiver(Enabled = true, Exported = true)]
	[IntentFilter(new[]{
		"keepass2android.ACTION_OPEN_ENTRY",
		"keepass2android.ACTION_ENTRY_OUTPUT_MODIFIED",
		"keepass2android.ACTION_ENTRY_ACTION_SELECTED"
	})]
	public class TransferActionReceiver : PluginActionBroadcastReceiver
	{
		protected override void OpenEntry(OpenEntryAction oe)
		{
			try
			{
				oe.AddEntryAction(this.GetName(oe), Resource.Drawable.launcher_ic, null);
			}
			catch (PluginAccessException e)
			{
				e.PrintStackTrace();
			}
		}


		protected override void ActionSelected(ActionSelectedAction actionSelected)
		{
			var intent = new Intent(actionSelected.Context, typeof(ManageTransferActivity));
			intent.PutExtra(ManageTransferActivity.DataEntriesExtra, JsonConvert.SerializeObject(new Dictionary<string, string>(actionSelected.EntryFields)));
			intent.PutStringArrayListExtra(ManageTransferActivity.GuardedEntriesExtra, actionSelected.GetProtectedFieldsList()?.ToList() ?? new List<string>());
			intent.SetFlags(ActivityFlags.NewTask);
			actionSelected.Context.StartActivity(intent);
	    }

		protected override void EntryOutputModified(EntryOutputModifiedAction eom)
		{
			try
			{
				eom.AddEntryFieldAction("keepass.transfer.plugin",
					eom.ModifiedFieldId,
					this.GetName(eom),
					Resource.Drawable.launcher_ic,
					null);
			}
			catch (PluginAccessException e)
			{
				e.PrintStackTrace();
			}
		}

		private string GetName<T>(T action) where T : PluginEntryActionBase
		{
			return action.Context.GetString(Resource.String.action_entry);
		}
	}
}