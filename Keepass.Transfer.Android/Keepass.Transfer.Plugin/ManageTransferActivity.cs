using System;
using System.Collections.Generic;
using System.Linq;
using Android.App;
using Android.Content;
using Android.OS;
using Android.Preferences;
using Android.Widget;
using Keepass.Transfer.DataEngine;
using Keepass2android.Pluginsdk;
using Newtonsoft.Json;

namespace Keepass.Transfer.Plugin
{
    [Activity(Label = "@string/application_name",
        Icon = "@drawable/launcher_ic",
        Theme = "@style/Kpt.Theme")]
    public class ManageTransferActivity : DataControllerActivity
    {
        public const string DataEntriesExtra = nameof(DataEntriesExtra);
        public const string GuardedEntriesExtra = nameof(GuardedEntriesExtra);
        public const string TitleEntryExtra = nameof(TitleEntryExtra);

        public const string BackendUriSettingsKey = nameof(BackendUriSettingsKey);
        public const string EncryptUnGuardedSettingsKey = nameof(EncryptUnGuardedSettingsKey);
        public const string DefaultEntriesSettingsKey = nameof(DefaultEntriesSettingsKey);

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

        private ICollection<string> _defaultEntries = new[] {KeepassDefs.UserNameField, KeepassDefs.PasswordField, KeepassDefs.UrlField};
        private IList<DataEntry> _transferEntries = new List<DataEntry>();
        private string _titleEntry;

        private ListView _listView;

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            //load settings
            var preferences = PreferenceManager.GetDefaultSharedPreferences(this);
            var uriString = preferences.GetString(BackendUriSettingsKey, null);
            if (!string.IsNullOrEmpty(uriString))
                BackendUri = new Uri(uriString);
            EncryptUnGuarded = preferences.GetBoolean(EncryptUnGuardedSettingsKey, EncryptUnGuarded);
            _defaultEntries = preferences.GetStringSet(DefaultEntriesSettingsKey, _defaultEntries);

            //load data
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

                _titleEntry = _transferEntries
                    .FirstOrDefault(entry => entry.Key == KeepassDefs.TitleField)
                    ?.Value;
                _transferEntries = _transferEntries.Where(entry => entry.Key != KeepassDefs.TitleField).ToList();
            } else if (savedInstanceState?.ContainsKey(DataEntriesExtra) ?? false) {
                _transferEntries = JsonConvert.DeserializeObject<List<DataEntry>>(savedInstanceState.GetString(DataEntriesExtra));
                _titleEntry = savedInstanceState.GetString(TitleEntryExtra);
            }

            //create ui
            SetContentView(Resource.Layout.ManageTransferActivity);
            if (!string.IsNullOrEmpty(_titleEntry))
                Title = GetString(Resource.String.transfer_selection_title) + _titleEntry;

            _listView = FindViewById<ListView>(Resource.Id.entriesListView);
            _listView.ChoiceMode = ChoiceMode.Multiple;
            _listView.Adapter = new ArrayAdapter<DataEntry>(this, 
                Android.Resource.Layout.SimpleListItemMultipleChoice,
                _transferEntries);

            FindViewById<Button>(Resource.Id.transferButton).Click += TransferButtonClicked;
            FindViewById<Button>(Resource.Id.cancelButton).Click += (sender, args) => Finish();
            FindViewById<Button>(Resource.Id.settingsButton).Click += (sender, args) => StartActivity(typeof(SettingsActivity));//TODO reload after!

            //do default selection
            foreach (var index in _transferEntries.Select((entry, i) => new { entry, i })
                .Where(info => _defaultEntries.Contains(info.entry.Key))
                .Select(info => info.i)) {
                _listView.SetItemChecked(index, true);
            }

            if (_transferEntries.Count == 0)
                new InvalidStartDialog().Show(FragmentManager, InvalidStartDialog.Tag);
        }

        protected override void OnSaveInstanceState(Bundle outState)
        {
            base.OnSaveInstanceState(outState);
            outState.PutString(DataEntriesExtra, JsonConvert.SerializeObject(_transferEntries));
            outState.PutString(TitleEntryExtra, _titleEntry);
        }

        private void TransferButtonClicked(object sender, EventArgs e)
        {
            var entries = _transferEntries
                .Where((entry, index) => _listView.CheckedItemPositions.Get(index))
                .Prepend(new DataEntry {Key = KeepassDefs.TitleField, Value = _titleEntry})
                .ToList();
            StartDataTransfer(entries);
        }
    }
}