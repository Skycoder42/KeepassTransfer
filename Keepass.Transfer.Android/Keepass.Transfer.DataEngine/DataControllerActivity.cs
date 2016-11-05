using System;
using System.Collections.Generic;
using Android.App;
using Android.Content;
using Android.OS;
using Newtonsoft.Json;
using Exception = Java.Lang.Exception;
using Object = Java.Lang.Object;

namespace Keepass.Transfer.DataEngine
{
    public class DataControllerActivity : QrParserActivity
    {
        private const string TransferDataKey = "TRANSFER_DATA";

        private string _transferData;

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
            if (scanData != null)
                new TransferTask(this).Execute(this._transferData, scanData.Secret, scanData.PublicKey);
            else if (!wasAborted)
                this.Finish();
        }
    }
}