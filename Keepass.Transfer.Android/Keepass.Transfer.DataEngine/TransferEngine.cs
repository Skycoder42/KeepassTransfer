using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Newtonsoft.Json;
using Android.Content;
using Android.App;
using Android.Util;
using Com.Neovisionaries.WS.Client;

namespace Keepass.Transfer.DataEngine
{
    internal class TransferEngine
    {
        private const string Tag = "Keepass.Transfer.DataEngine.TransferEngine";

        private class TransferMessage
        {
            public string MessageType => "MC";//MobileClient
            public string Secret { get; set; }
            public IList<DataEntry> TransferData { get; set; }
        }

        private class ReplyMessage
        {
            public bool Successful { get; set; }
            public string Error { get; set; }
        }

        private class TransferErrorDialogFragment : MessageDialogFragment
        {
            public TransferErrorDialogFragment() { }
            public TransferErrorDialogFragment(int title, int message) : base(title, message) { }
            public TransferErrorDialogFragment(int title, string message) : base(title, message) { }

            public override EventHandler<DialogClickEventArgs> DialogReadyHandler => (sender, args) => this.Activity.Finish();
        }

        private class TransferSuccessDialogFragment : MessageDialogFragment
        {
            public TransferSuccessDialogFragment() : base(Resource.String.completed_title, Resource.String.completed_text) { }

            public override EventHandler<DialogClickEventArgs> DialogReadyHandler => (sender, args) => this.Activity.Finish();
        }

        private class WebSocketListener : WebSocketAdapter
        {
            private readonly TransferMessage _message;

            public WebSocketListener(TransferMessage message)
            {
                _message = message;
            }

            public override void OnConnected(WebSocket socket, IDictionary<string, IList<string>> headers)
            {
                socket.SendText(JsonConvert.SerializeObject(_message));
                Instance.UpdateProgressText(Resource.String.progress_text_wait);
            }

            public override void OnTextMessage(WebSocket socket, string message)
            {
                try {
                    var response = JsonConvert.DeserializeObject<ReplyMessage>(message);
                    if (response.Successful)
                        Instance.ShowSuccess();
                    else
                        Instance.ShowError(Resource.String.client_error_title, response.Error);
                } catch (Exception e) {
                    Log.Error(Tag, e.ToString());
                    Console.WriteLine(e.ToString());
                    Instance.ShowError(Resource.String.network_error_title, Resource.String.network_error_text);
                } finally {
                    socket.SendClose();
                }
            }

            public override void OnDisconnected(WebSocket socket, WebSocketFrame serverCloseFrame, WebSocketFrame clientCloseFrame, bool closedByServer)
            {
                if(closedByServer)
                    Instance.ShowError(Resource.String.network_error_title, Resource.String.network_error_text);
                Instance._currentSocket = null;//remove myself
            }

            public override void OnError(WebSocket socket, WebSocketException exception)
            {
                Log.Error(Tag, exception.ToString());
                Instance.ShowError(Resource.String.network_error_title, Resource.String.network_error_text);
            }
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

        private WebSocket _currentSocket;

        public Uri BackendUri { get; set; } = new Uri("wss://kpt.skycoder42.de/backend/");
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
                await DataEncryptor.EncryptDataAsync(message.TransferData, publicKey);//TODO conf await
            } catch (Exception e) {
                Log.Error(Tag, e.ToString());
                ShowError(Resource.String.invalid_key_title, Resource.String.invalid_key_text);
                return;
            }

            //connect
            UpdateProgressText(Resource.String.progress_text_transfer);
            _currentSocket = new WebSocketFactory().CreateSocket(BackendUri.ToString());
            _currentSocket.AddListener(new WebSocketListener(message));
            _currentSocket.ConnectAsynchronously();
        }

        private void UpdateProgressText(int? textId = null)
        {
            Activity?.RunOnUiThread(() => {
                var fragment = Activity.FragmentManager.FindFragmentByTag(ProgressDialogFragment.Tag) as ProgressDialogFragment;
                if (fragment == null) {
                    fragment = new ProgressDialogFragment();
                    fragment.Show(Activity.FragmentManager, ProgressDialogFragment.Tag);
                }
                if (textId.HasValue)
                    fragment.TextId = textId.Value;
            });
        }

        private void ShowError(int title, int text)
        {
            Activity?.RunOnUiThread(() => {
                (Activity.FragmentManager.FindFragmentByTag(ProgressDialogFragment.Tag) as DialogFragment)?.Dismiss();
                new TransferErrorDialogFragment(title, text)
                    .Show(Activity.FragmentManager, MessageDialogFragment.Tag);
            });
        }

        private void ShowError(int title, string text)
        {
            Activity?.RunOnUiThread(() => {
                (Activity.FragmentManager.FindFragmentByTag(ProgressDialogFragment.Tag) as DialogFragment)?.Dismiss();
                new TransferErrorDialogFragment(title, text)
                    .Show(Activity.FragmentManager, MessageDialogFragment.Tag);
            });
        }

        private void ShowSuccess()
        {
            Activity?.RunOnUiThread(() => {
                (Activity.FragmentManager.FindFragmentByTag(ProgressDialogFragment.Tag) as DialogFragment)?.Dismiss();
                new TransferSuccessDialogFragment().Show(Activity.FragmentManager, MessageDialogFragment.Tag);
            });
        }
    }
}