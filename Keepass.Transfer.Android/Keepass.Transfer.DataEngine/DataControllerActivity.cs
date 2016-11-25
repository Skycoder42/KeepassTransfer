using System.Collections.Generic;
using Android.OS;
using Newtonsoft.Json;
using System;
using System.Linq;

namespace Keepass.Transfer.DataEngine
{
    public class DataControllerActivity : QrParserActivity
    {
        private const string TransferDataKey = nameof(TransferDataKey);

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

        public bool StartDataTransfer(IEnumerable<DataEntry> transferFields)
        {
            _transferData = transferFields
                .Select(entry => new DataEntry//clone entries
                {
                    Key = entry.Key,
                    Value = entry.Value,
                    Guarded = entry.Guarded
                })
                .ToList();
            if (_transferData.Count > 0)
                return StartQrScan();
            else
                return false;
        }

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            var serData = savedInstanceState?.GetString(TransferDataKey);
            if (!string.IsNullOrEmpty(serData))
                _transferData = JsonConvert.DeserializeObject<List<DataEntry>>(serData);
            TransferEngine.Instance.Activity = this;
        }

        protected override void OnSaveInstanceState(Bundle outState)
        {
            base.OnSaveInstanceState(outState);
            outState.PutString(TransferDataKey, JsonConvert.SerializeObject(_transferData));
        }

        protected override void OnDestroy()
        {
            TransferEngine.Instance.Activity = null;
            base.OnDestroy();
        }

        private async void ScanResultDone(QrData scanData, bool wasAborted)
        {
            if (scanData != null)
                await TransferEngine.Instance.BeginTransfer(_transferData, scanData.Secret, scanData.PublicKey);
            else if (wasAborted)
                Finish();
        }
    }
}