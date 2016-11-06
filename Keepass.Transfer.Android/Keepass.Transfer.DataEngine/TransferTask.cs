using System;
using System.Collections.Generic;
using Android.OS;
using Java.Net;
using System.Text;
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
            var entryList = JsonConvert.DeserializeObject<List<DataEntry>>(@params[0]);
            var secret = @params[1];
            var publicKey = @params[2];

            try
            {
                DataEncryptor.EncryptData(entryList, publicKey);
            }
            catch (Exception)
            {
                return ResultCode.EncryptError;
            }

            try
            {
                //var ksTrust = KeyStore.GetInstance("BKS");
                //ksTrust.Load(this._activity.Resources.OpenRawResource(Resource.Raw.skynetKeystore), "skycoder42".ToCharArray());
                //var tmf = TrustManagerFactory.GetInstance(KeyManagerFactory.DefaultAlgorithm);
                //tmf.Init(ksTrust);
                //var sslContext = SSLContext.GetInstance("TLS");
                //sslContext.Init(null, tmf.GetTrustManagers(), new SecureRandom());

                var url = new URL("https://kpt.skycoder42.de/secretPlacer.php");
                var con = (HttpsURLConnection)url.OpenConnection();
                //con.SSLSocketFactory = sslContext.SocketFactory;

                con.RequestMethod = "POST";
                con.DoOutput = true;
                var os = con.OutputStream;
                var postData = Encoding.UTF8.GetBytes($"secret={URLEncoder.Encode(secret, "UTF-8")}&data={URLEncoder.Encode(JsonConvert.SerializeObject(entryList), "UTF-8")}");
                os.Write(postData, 0, postData.Length);
                os.Flush();
                os.Close();

                var status = con.ResponseCode;
                if ((int)status / 100 == 2)
                    return ResultCode.Success;
                else
                    return ResultCode.TransferError;
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