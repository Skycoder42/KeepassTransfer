using System;
using System.Collections.Generic;
using System.Net.WebSockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Newtonsoft.Json;
using Android.Content;
using Android.App;


namespace Keepass.Transfer.DataEngine
{
    internal class TransferEngine
    {
        private class TransferMessage
        {
            public readonly string MessageType = "MC";//MobileClient
            public string Secret;
            public IList<DataEntry> TransferData;
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
        public Activity Activity { get; set; }

        private TransferEngine() { }

        public async Task BeginTransfer(IList<DataEntry> transferData, string secret, string publicKey)
        {
            UpdateProgressText();

            var message = new TransferMessage {
                Secret = secret,
                TransferData = transferData
            };

            //Try to encrypt the data
            try {
                await DataEncryptor.EncryptDataAsync(message.TransferData, publicKey);
                UpdateProgressText(Resource.String.progress_text_transfer);
            } catch (Exception) {
                ShowError(Resource.String.invalid_key_title, Resource.String.invalid_key_text);
                return;
            }

            try {
                //connect
                var socket = new ClientWebSocket();
                await socket.ConnectAsync(new Uri("ws://192.168.178.43:11221"), //DEBUG, normal: wss://kpt.skycoder42.de/backend/
                    new CancellationTokenSource(TimeSpan.FromSeconds(10)).Token);//wait at most 10 sec
                if (socket.State != WebSocketState.Open)
                    throw new WebSocketException();

                //send
                var sendData = JsonConvert.SerializeObject(message);
                await socket.SendAsync(new ArraySegment<byte>(Encoding.UTF8.GetBytes(sendData)),
                    WebSocketMessageType.Text,
                    true,
                    new CancellationTokenSource(TimeSpan.FromSeconds(10)).Token);

                //close
                await socket.CloseAsync(WebSocketCloseStatus.NormalClosure,
                    string.Empty,
                    new CancellationTokenSource(TimeSpan.FromSeconds(10)).Token);

                if (socket.State != WebSocketState.Closed)
                    throw new WebSocketException();
                UpdateProgressText(Resource.String.progress_text_wait);
            } catch (Exception) {
                ShowError(Resource.String.network_error_title, Resource.String.network_error_text);
                return;
            }

            ShowSuccess();
        }

        private void UpdateProgressText(int? textId = null)
        {
            if (Activity != null) {
                var fragment = Activity.FragmentManager.FindFragmentByTag(ProgressDialogFragment.Tag) as ProgressDialogFragment;
                if (fragment == null) {
                    fragment = new ProgressDialogFragment();
                    fragment.Show(Activity.FragmentManager, ProgressDialogFragment.Tag);
                }
                if (textId.HasValue)
                    fragment.TextId = textId.Value;
            }
        }

        private void ShowError(int title, int text)
        {
            if (Activity != null) {
                (Activity.FragmentManager.FindFragmentByTag(ProgressDialogFragment.Tag) as DialogFragment)?.Dismiss();
                new TransferErrorDialogFragment(title, text)
                    .Show(Activity.FragmentManager, MessageDialogFragment.Tag);
            }
        }

        private void ShowSuccess()
        {
            if (Activity != null) {
                (Activity.FragmentManager.FindFragmentByTag(ProgressDialogFragment.Tag) as DialogFragment)?.Dismiss();
                new TransferSuccessDialogFragment().Show(Activity.FragmentManager, MessageDialogFragment.Tag);
            }
        }
    }
}