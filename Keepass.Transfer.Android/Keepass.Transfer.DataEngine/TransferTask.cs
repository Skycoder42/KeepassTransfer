using System;
using System.Collections.Generic;
using System.Net.WebSockets;
using Android.OS;
using Java.Net;
using System.Text;
using System.Threading;
using Newtonsoft.Json;
using Android.Content;
using Android.App;
using Javax.Net.Ssl;
using Java.Security;

namespace Keepass.Transfer.DataEngine
{
    internal class TransferTask : AsyncTask<string, Java.Lang.Void, TransferTask.ResultCode>
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

        private readonly DataControllerActivity _activity;

        public TransferTask(DataControllerActivity activity)
        {
            this._activity = activity;
        }

        protected override void OnPreExecute()
        {
            new ProgressDialogFragment().Show(this._activity.FragmentManager, ProgressDialogFragment.Tag);
        }

        protected override ResultCode RunInBackground(params string[] @params)
        {
            var message = new TransferMessage {
                Secret = @params[1],
                TransferData = JsonConvert.DeserializeObject<List<DataEntry>>(@params[0])
            };
            var publicKey = @params[2];

            try
            {
                DataEncryptor.EncryptData(message.TransferData, publicKey);
            }
            catch (Exception)
            {
                return ResultCode.EncryptError;
            }

            try {
                //connect
                var socket = new ClientWebSocket();
                socket.ConnectAsync(new Uri("wss://192.168.179.20:11221"), CancellationToken.None).Wait();//DEBUG, normal: wss://kpt.skycoder42.de/backend/
                if (socket.State != WebSocketState.Open)
                    return ResultCode.NetworkError;

                //send
                var sendData = JsonConvert.SerializeObject(message);
                socket.SendAsync(new ArraySegment<byte>(Encoding.UTF8.GetBytes(sendData)), 
                    WebSocketMessageType.Text,
                    true, 
                    CancellationToken.None)
                    .Wait();

                //close
                socket.CloseAsync(WebSocketCloseStatus.NormalClosure, string.Empty, CancellationToken.None).Wait();

                if (socket.State == WebSocketState.Closed)
                    return ResultCode.Success;
                else
                    return ResultCode.NetworkError;
            }
            catch (Exception)
            {
                return ResultCode.NetworkError;
            }
        }

        protected override void OnPostExecute(ResultCode result)
        {
            this._activity.FragmentManager.FindFragmentByTag<ProgressDialogFragment>(ProgressDialogFragment.Tag)?.Dismiss();
            int title = -1;
            int text = -1;

            switch(result)
            {
                case ResultCode.Success:
                    new TransferSuccessDialogFragment().Show(this._activity.FragmentManager, MessageDialogFragment.Tag);
                    return;
                case ResultCode.EncryptError:
                    title = Resource.String.invalid_key_title;
                    text = Resource.String.invalid_key_text;
                    break;
                case ResultCode.NetworkError:
                    title = Resource.String.network_error_title;
                    text = Resource.String.network_error_text;
                    break;
                case ResultCode.TransferError:
                    title = Resource.String.network_error_title;
                    text = Resource.String.network_error_text;
                    break;
            }

            new TransferErrorDialogFragment(title, text).Show(this._activity.FragmentManager, MessageDialogFragment.Tag);
        }
    }
}