using System.Collections.Generic;
using System.Linq;
using Android.App;
using Android.OS;
using Android.Preferences;
using Keepass2android.Pluginsdk;

namespace Keepass.Transfer.Plugin
{
    [Activity(Label = "@string/application_name",
        Icon = "@drawable/launcher_ic",
        MainLauncher = true)]
    public class SettingsActivity : PreferenceActivity
    {
        public const int StartWithResult = 42;

        public class MainPreferenceFragment : PreferenceFragment
        {
            public override void OnCreate(Bundle savedInstanceState)
            {
                base.OnCreate(savedInstanceState);
                AddPreferencesFromResource(Resource.Xml.preferences);

                var listPref = (MultiSelectListPreference)PreferenceScreen.FindPreference("DefaultEntriesSettingsKey");
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

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            FragmentManager.BeginTransaction().Replace(Android.Resource.Id.Content, new MainPreferenceFragment()).Commit();
        }
    }
}