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
        Theme = "@style/Kpt.Theme.DialogActivity")]
    public class ManageTransferActivity : DataControllerActivity
    {
        public const string DataEntriesExtra = nameof(DataEntriesExtra);
        public const string GuardedEntriesExtra = nameof(GuardedEntriesExtra);
        public const string TitleEntryExtra = nameof(TitleEntryExtra);

        public const string BackendUriSettingsKey = nameof(BackendUriSettingsKey);
        public const string EncryptUnGuardedSettingsKey = nameof(EncryptUnGuardedSettingsKey);
        public const string DefaultEntriesSettingsKey = nameof(DefaultEntriesSettingsKey);
        public const string AllEntryKeysSettingsKey = nameof(AllEntryKeysSettingsKey);

        public const int StartWithResult = 42;

        private ICollection<string> _defaultEntries = new[] {KeepassDefs.UserNameField, KeepassDefs.PasswordField, KeepassDefs.UrlField};
        private IList<DataEntry> _transferEntries = new List<DataEntry>();
        private string _titleEntry;

        private ListView _listView;

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            //load settings
            ReloadSettings();

            //load data
            if (Intent?.HasExtra(DataEntriesExtra) ?? false) {
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

                //update the total keys list
                var prefs = PreferenceManager.GetDefaultSharedPreferences(this);
                var keyList = _transferEntries
                    .Select(entry => entry.Key)
                    .Concat(prefs.GetStringSet(AllEntryKeysSettingsKey, new List<string>()));
                ISet<string> entrySet = new HashSet<string>(keyList);
                prefs.Edit()
                    .PutStringSet(AllEntryKeysSettingsKey, entrySet)
                    .Apply();
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

            FindViewById<Button>(Resource.Id.transferButton).Click += StartTransfer;
            FindViewById<Button>(Resource.Id.cancelButton).Click += (sender, args) => Finish();
            FindViewById<Button>(Resource.Id.settingsButton).Click += OpenSettings;

            //do default selection
            foreach (var index in _transferEntries.Select((entry, i) => new { entry, i })
                .Where(info => _defaultEntries.Contains(info.entry.Key))
                .Select(info => info.i)) {
                _listView.SetItemChecked(index, true);
            }

            if (_transferEntries.Count == 0)
                Finish();
        }

        protected override void OnSaveInstanceState(Bundle outState)
        {
            base.OnSaveInstanceState(outState);
            outState.PutString(DataEntriesExtra, JsonConvert.SerializeObject(_transferEntries));
            outState.PutString(TitleEntryExtra, _titleEntry);
        }

        protected override void OnActivityResult(int requestCode, Result resultCode, Intent data)
        {
            if (requestCode == StartWithResult)
                ReloadSettings();
            else
                base.OnActivityResult(requestCode, resultCode, data);
        }

        private void ReloadSettings()
        {
            var preferences = PreferenceManager.GetDefaultSharedPreferences(this);
            var uriString = preferences.GetString(BackendUriSettingsKey, null);
            if (!string.IsNullOrEmpty(uriString))
                BackendUri = new Uri(uriString);
            EncryptUnGuarded = preferences.GetBoolean(EncryptUnGuardedSettingsKey, EncryptUnGuarded);
            _defaultEntries = preferences.GetStringSet(DefaultEntriesSettingsKey, _defaultEntries);
        }

        private void StartTransfer(object sender, EventArgs e)
        {
            var entries = _transferEntries
                .Where((entry, index) => _listView.CheckedItemPositions.Get(index))
                .Prepend(new DataEntry {Key = KeepassDefs.TitleField, Value = _titleEntry})
                .ToList();
            StartDataTransfer(entries);
        }

        private void OpenSettings(object sender, EventArgs args)
        {
            var intent = new Intent(this, typeof(SettingsActivity));
            intent.PutExtra(SettingsActivity.ShowRunKeepassAction, false);
            StartActivityForResult(intent, StartWithResult);
        }
    }
}