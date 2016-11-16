using System;
using System.Collections.Generic;
using System.Net.WebSockets;
using Android.OS;
using System.Text;
using System.Threading;
using Newtonsoft.Json;
using Android.Content;
using Android.App;


namespace Keepass.Transfer.DataEngine
{
    internal class TransferEngine
    {
        public enum ResultCode
        {
            Success,
            EncryptError,
            NetworkError,
            TransferError
        }

        private class TransferMessage
        {
            public readonly string MessageType = "MC";//MobileClient
            public string Secret;
            public List<DataEntry> TransferData;
        }

        private class ProgressDialogFragment : DialogFragment
        {
            public new const string Tag = "ProgressDialogFragmentTag";

            public override Dialog OnCreateDialog(Bundle savedInstanceState)
            {
                var dialog = new ProgressDialog(this.Activity);
                dialog.SetTitle(Resource.String.progress_title);
                dialog.SetMessage(this.GetString(Resource.String.progress_text));
                dialog.SetCancelable(false);
                dialog.SetCanceledOnTouchOutside(false);
                return dialog;
            }
        }

        private class TransferErrorDialogFragment : MessageDialogFragment
        {
            public TransferErrorDialogFragment() { }

            public TransferErrorDialogFragment(int title, int message) : base(title, message) { }

            public override EventHandler<DialogClickEventArgs> DialogReadyHandler => (sender, args) => this.Activity.Finish();
        }

        private class TransferSuccessDialogFragment : MessageDialogFragment
        {
            public TransferSuccessDialogFragment() : base(Resource.String.completed_title, Resource.String.completed_text) { }

            public override EventHandler<DialogClickEventArgs> DialogReadyHandler => (sender, args) => this.Activity.Finish();
        }

        private static TransferEngine _instance;
        public static TransferEngine Instance
        {
            get {
                if (_instance == null)
                    _instance = new TransferEngine();
                return _instance;
            }
        }

        public Uri BackendUri { get; set; }

        private TransferEngine() { }

        public async void BeginTransfer(Activity activity)
        {

        }
    }
}