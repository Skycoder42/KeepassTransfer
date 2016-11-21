using System;
using Android.App;
using Android.Content;
using Android.OS;
using Com.Google.Zxing.Integration.Android;
using Newtonsoft.Json;
using System.Linq;

namespace Keepass.Transfer.DataEngine
{
    public class QrParserActivity : Activity
    {
        internal delegate void ScanResultHandler(QrData scanData, bool wasAborted);
        internal event ScanResultHandler ScanResultReady = delegate {};

        private sealed class ScanErrorDialogFragment : DialogFragment
        {
            public new const string Tag = nameof(ScanErrorDialogFragment);

            public ScanErrorDialogFragment()
            {
                Cancelable = false;
            }

            public override Dialog OnCreateDialog(Bundle savedInstanceState)
            {
                var qrActivity = (QrParserActivity)Activity;

                var dialog = new AlertDialog.Builder(qrActivity)
                    .SetTitle(Resource.String.invalid_qr_title)
                    .SetMessage(Resource.String.invalid_qr_text)
                    .SetPositiveButton(Resource.String.retry, (sender, args) => qrActivity.StartQrScan())
                    .SetNegativeButton(Android.Resource.String.Cancel, (sender, args) => qrActivity.ScanResultReady(null, true))
                    .SetCancelable(false)
                    .Create();
                dialog.SetCanceledOnTouchOutside(false);
                return dialog;
            }
        }

        internal QrParserActivity() { }

        protected bool StartQrScan()
        {
            var integrator = new IntentIntegrator(this);
            integrator.AddExtra("SAVE_HISTORY", false);
            var dialog = integrator.InitiateScan(IntentIntegrator.QrCodeTypes.Cast<string>().ToList());
            if(dialog != null)
                dialog.DismissEvent += ZXingDialogDismissed;
            return dialog == null;
        }

        private void ZXingDialogDismissed(object sender, EventArgs e)
        {
            ScanResultReady(null, true);
        }

        protected override void OnActivityResult(int requestCode, Result resultCode, Intent data)
        {
            var zingResult = IntentIntegrator.ParseActivityResult(requestCode, (int)resultCode, data);
            if (zingResult != null)
            {
                if (string.IsNullOrEmpty(zingResult.Contents))
                    ScanResultReady(null, false);
                else
                {
                    try
                    {
                        var codeData = JsonConvert.DeserializeObject<QrData>(zingResult.Contents);
                        ScanResultReady(codeData, false);
                    }
                    catch (Exception)
                    {
                        new ScanErrorDialogFragment().Show(FragmentManager, ScanErrorDialogFragment.Tag);
                    }
                }
            }
            else
                base.OnActivityResult(requestCode, resultCode, data);
        }
    }
}