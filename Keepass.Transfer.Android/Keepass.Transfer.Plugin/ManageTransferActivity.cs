using System;
using System.Collections.Generic;
using System.Linq;
using Android.App;
using Android.Content;
using Android.OS;
using Android.Widget;
using Keepass.Transfer.DataEngine;
using Newtonsoft.Json;

namespace Keepass.Transfer.Plugin
{
    [Activity(Label = "@string/application_name",
        Icon = "@drawable/launcher_ic",
        MainLauncher = true,
        Theme = "@android:style/android:Theme.Material.Light.Dialog")]
    public class ManageTransferActivity : DataControllerActivity
    {
        public const string DataEntriesExtra = "DATA_ENTRIES_EXTRA";
        public const string GuardedEntriesExtra = "GUARDED_ENTRIES_EXTRA";

        private class InvalidStartDialog : DialogFragment//TODO
        {
            public new const string Tag = "InvalidStartDialog";

            public InvalidStartDialog()
            {
                Cancelable = false;
            }

            public override Dialog OnCreateDialog(Bundle savedInstanceState)
            {
                var dialog = new AlertDialog.Builder(Activity)
                    .SetTitle(Resource.String.no_entry_title)
                    .SetMessage(Resource.String.no_entry_text)
                    .SetCancelable(false)
                    .SetPositiveButton(Resource.String.no_entry_start_button, RunKeePassHandler)
                    .SetNegativeButton(Android.Resource.String.Cancel, (o, args) => Activity.Finish())
                    .Create();
                dialog.SetCanceledOnTouchOutside(false);
                return dialog;
            }

            private void RunKeePassHandler(object o, EventArgs args)
            {
                var packageManager = Activity.PackageManager;
                try
                {
                    var intent = packageManager.GetLaunchIntentForPackage("keepass2android.keepass2android");
                    if(intent != null)
                    {
                        intent.AddCategory(Intent.CategoryLauncher);
                        Activity.StartActivity(intent);
                    }
                }
                catch(Exception) { }
                Activity.Finish();
            }
        }

        private IDictionary<string, string> _transferEntries;
        private IList<string> _protectedEntries; 

        private ListView _listView;

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            _transferEntries = JsonConvert.DeserializeObject<Dictionary<string, string>>(Intent?.GetStringExtra(DataEntriesExtra) ?? 
                                                                                              savedInstanceState?.GetString(DataEntriesExtra) ?? 
                                                                                              "{}");
            _protectedEntries = Intent?.GetStringArrayListExtra(GuardedEntriesExtra) ??
                                     savedInstanceState?.GetStringArrayList(GuardedEntriesExtra) ??
                                     new List<string>();

            SetContentView(Resource.Layout.ManageTransferActivity);

            _listView = FindViewById<ListView>(Resource.Id.entriesListView);
            _listView.ChoiceMode = ChoiceMode.Multiple;
            _listView.Adapter = new ArrayAdapter<string>(this, Android.Resource.Layout.SimpleListItemMultipleChoice, _transferEntries.Keys.ToList());

            FindViewById<Button>(Resource.Id.transferButton).Click += TransferButtonClicked;

            if (_transferEntries.Count == 0)
                new InvalidStartDialog().Show(FragmentManager, InvalidStartDialog.Tag);
        }

        protected override void OnSaveInstanceState(Bundle outState)
        {
            base.OnSaveInstanceState(outState);
            outState.PutString(DataEntriesExtra, JsonConvert.SerializeObject(_transferEntries));
            outState.PutStringArrayList(GuardedEntriesExtra, _protectedEntries);
        }

        private void TransferButtonClicked(object sender, EventArgs e)
        {
            var transferData = new List<DataEntry>();

            var positions = _listView.CheckedItemPositions;
            var keys = _transferEntries.Keys.ToList();
            for (int i = 0; i < positions.Size(); i++)
            {
                if (positions.ValueAt(i))
                {
                    var adapter = (ArrayAdapter<string>)_listView.Adapter;
                    var key = adapter.GetItem(positions.KeyAt(i));
                    transferData.Add(new DataEntry
                    {
                        Key = key,
                        Guarded = _protectedEntries.Contains(key),
                        Value = _transferEntries[key]
                    });
                }
            }

            if (transferData.Count > 0)
                StartDataTransfer(transferData);
        }
    }
}