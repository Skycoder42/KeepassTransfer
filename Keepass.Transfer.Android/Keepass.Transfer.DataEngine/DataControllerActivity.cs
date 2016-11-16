using System.Collections.Generic;
using Android.OS;
using Newtonsoft.Json;
using System;

namespace Keepass.Transfer.DataEngine
{
    public class DataControllerActivity : QrParserActivity
    {
        private const string TransferDataKey = "TRANSFER_DATA";

        private string _transferData;

        public Uri BackendUri
        {
            get { return TransferEngine.Instance.BackendUri; }
            set { TransferEngine.Instance.BackendUri = value; }
        }

        public DataControllerActivity()
        {
            this.ScanResultReady += ScanResultDone;
        }

        public bool StartDataTransfer(IList<DataEntry> transferFields)
        {
            this._transferData = JsonConvert.SerializeObject(transferFields);
            return this.StartQrScan();
        }

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            this._transferData = savedInstanceState?.GetString(DataControllerActivity.TransferDataKey);
        }

        protected override void OnSaveInstanceState(Bundle outState)
        {
            base.OnSaveInstanceState(outState);
            outState.PutString(DataControllerActivity.TransferDataKey, this._transferData);
        }

        private void ScanResultDone(QrData scanData, bool wasAborted)
        {
            if (scanData != null)//TODO not like this
                new TransferTask(this).Execute(this._transferData, scanData.Secret, scanData.PublicKey);
            else if (wasAborted)
                this.Finish();
        }
    }
}