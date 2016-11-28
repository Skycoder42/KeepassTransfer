using System.Linq;
using Android.App;
using Android.Content;
using Android.OS;
using Android.Preferences;
using Android.Views;
using Keepass2android.Pluginsdk;

namespace Keepass.Transfer.Plugin
{
    [Activity(Label = "@string/settings_name", 
        Icon = "@drawable/launcher_ic",
        MainLauncher = true,
        Theme = "@style/Kpt.Theme")]
    public class SettingsActivity : PreferenceActivity
    {
        public const string ShowRunKeepassAction = nameof(ShowRunKeepassAction);

        public class MainPreferenceFragment : PreferenceFragment
        {
            public override void OnCreate(Bundle savedInstanceState)
            {
                base.OnCreate(savedInstanceState);
                AddPreferencesFromResource(Resource.Xml.preferences);

                var listPref = (MultiSelectListPreference)PreferenceScreen.FindPreference(ManageTransferActivity.DefaultEntriesSettingsKey);
                var prefs = PreferenceManager.GetDefaultSharedPreferences(this.Activity);
                
                var items = prefs.GetStringSet(ManageTransferActivity.AllEntryKeysSettingsKey, new[]  {
                    KeepassDefs.UserNameField,
                    KeepassDefs.PasswordField,
                    KeepassDefs.UrlField,
                    KeepassDefs.NotesField
                }).ToArray();
                listPref.SetEntries(items);
                listPref.SetEntryValues(items);
                listPref.SetDefaultValue(new[] { KeepassDefs.UserNameField, KeepassDefs.PasswordField, KeepassDefs.UrlField });
            }
        }

        private bool _showRunAction;
        private int _itemId;

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            _showRunAction = Intent?.GetBooleanExtra(ShowRunKeepassAction, true) ?? true;
            FragmentManager.BeginTransaction().Replace(Android.Resource.Id.Content, new MainPreferenceFragment()).Commit();
        }

        public override bool OnCreateOptionsMenu(IMenu menu)
        {
            if (_showRunAction) {
                var item = menu.Add(Resource.String.open_keepass_action);
                item.SetShowAsAction(ShowAsAction.IfRoom | ShowAsAction.WithText);
                _itemId = item.ItemId;
            }
            return base.OnCreateOptionsMenu(menu);
        }

        public override bool OnOptionsItemSelected(IMenuItem item)
        {
            if (_showRunAction && item.ItemId == _itemId) {
                try {
                    var intent = PackageManager.GetLaunchIntentForPackage("keepass2android.keepass2android");
                    if (intent != null) {
                        intent.AddCategory(Intent.CategoryLauncher);
                        StartActivity(intent);
                    }
                } catch { }
                return true;
            } else
                return base.OnOptionsItemSelected(item);
        }
    }
}