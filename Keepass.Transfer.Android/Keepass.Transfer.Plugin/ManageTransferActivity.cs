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
        public const string DataEntriesExtra = nameof(DataEntriesExtra);
        public const string GuardedEntriesExtra = nameof(GuardedEntriesExtra);

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
        
        private IList<DataEntry> _transferEntries = new List<DataEntry>();

        private ListView _listView;

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            if(Intent?.HasExtra(DataEntriesExtra) ?? false) {
                var protectedFields = Intent.GetStringArrayListExtra(GuardedEntriesExtra);
                _transferEntries = JsonConvert
                    .DeserializeObject<Dictionary<string, string>>(Intent.GetStringExtra(DataEntriesExtra))
                    .Select(pair => new DataEntry {
                        Key = pair.Key,
                        Value = pair.Value,
                        Guarded = protectedFields.Contains(pair.Key)
                    })
                    .ToList();
            } else if (savedInstanceState?.ContainsKey(DataEntriesExtra) ?? false)
                _transferEntries = JsonConvert.DeserializeObject<List<DataEntry>>(savedInstanceState.GetString(DataEntriesExtra));

            SetContentView(Resource.Layout.ManageTransferActivity);

            _listView = FindViewById<ListView>(Resource.Id.entriesListView);
            _listView.ChoiceMode = ChoiceMode.Multiple;
            _listView.Adapter = new ArrayAdapter<DataEntry>(this, Android.Resource.Layout.SimpleListItemMultipleChoice, _transferEntries.ToList());

            FindViewById<Button>(Resource.Id.transferButton).Click += TransferButtonClicked;

            if (_transferEntries.Count == 0)
                new InvalidStartDialog().Show(FragmentManager, InvalidStartDialog.Tag);
        }

        protected override void OnSaveInstanceState(Bundle outState)
        {
            base.OnSaveInstanceState(outState);
            outState.PutString(DataEntriesExtra, JsonConvert.SerializeObject(_transferEntries));
        }

        private void TransferButtonClicked(object sender, EventArgs e)
        {
            StartDataTransfer(_transferEntries.Where((entry, index) => _listView.CheckedItemPositions.Get(index)));
        }
    }
}