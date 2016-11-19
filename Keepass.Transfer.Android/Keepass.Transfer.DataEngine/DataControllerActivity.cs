using System.Collections.Generic;
using Android.OS;
using Newtonsoft.Json;
using System;

namespace Keepass.Transfer.DataEngine
{
    public class DataControllerActivity : QrParserActivity
    {
        private const string TransferDataKey = "TRANSFER_DATA";

        private IList<DataEntry> _transferData;

        public Uri BackendUri
        {
            get { return TransferEngine.Instance.BackendUri; }
            set { TransferEngine.Instance.BackendUri = value; }
        }

        public DataControllerActivity()
        {
            ScanResultReady += ScanResultDone;
        }

        public bool StartDataTransfer(IList<DataEntry> transferFields)
        {
            _transferData = transferFields;
            return StartQrScan();
        }

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            var serData = savedInstanceState?.GetString(DataControllerActivity.TransferDataKey);
            if (!string.IsNullOrEmpty(serData))
                _transferData = JsonConvert.DeserializeObject<List<DataEntry>>(serData);
            TransferEngine.Instance.Activity = this;
            //TODO "reload dialogs" ???
        }

        protected override void OnSaveInstanceState(Bundle outState)
        {
            base.OnSaveInstanceState(outState);
            outState.PutString(DataControllerActivity.TransferDataKey, JsonConvert.SerializeObject(_transferData));
        }

        protected override void OnDestroy()
        {
            TransferEngine.Instance.Activity = null;
            base.OnDestroy();
        }

        private async void ScanResultDone(QrData scanData, bool wasAborted)
        {
            if (scanData != null)//TODO not like this
                await TransferEngine.Instance.BeginTransfer(_transferData, scanData.Secret, scanData.PublicKey);
            else if (wasAborted)
                Finish();
        }
    }
}