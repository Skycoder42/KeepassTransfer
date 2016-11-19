using System;
using System.Collections.Generic;
using Android.App;
using Android.Views;
using Android.Widget;
using Android.OS;
using Keepass.Transfer.DataEngine;

namespace KeePass.Transfer.TestApp
{
    [Activity(
        Label = "KeePass.Transfer.TestApp",
        MainLauncher = true, 
        Icon = "@drawable/icon"/*, 
        ScreenOrientation = ScreenOrientation.Portrait*/)]
    public class MainActivity : DataControllerActivity
    {
        private EditText _username;
        private EditText _mail;
        private EditText _password;
        private TextView _dataView;
        private ProgressBar _busyBar;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.Main);

            _username = FindViewById<EditText>(Resource.Id.userNameEdit);
            _mail = FindViewById<EditText>(Resource.Id.emailEdit);
            _password = FindViewById<EditText>(Resource.Id.passwordEdit);
            _dataView = FindViewById<TextView>(Resource.Id.resultView);
            _busyBar = FindViewById<ProgressBar>(Resource.Id.busyBar);

            FindViewById<Button>(Resource.Id.goButton).Click += StartButton_Click;
        }

        private void StartButton_Click(object sender, EventArgs e)
        {
            _dataView.Visibility = ViewStates.Gone;
            _busyBar.Visibility = ViewStates.Visible;

            StartDataTransfer(new List<DataEntry>
            {
                new DataEntry {Key = "username", Value = _username.Text },
                new DataEntry {Key = "email", Value = _mail.Text },
                new DataEntry {Key = "password", Value = _password.Text, Guarded = true }
            });
        }
    }
}

