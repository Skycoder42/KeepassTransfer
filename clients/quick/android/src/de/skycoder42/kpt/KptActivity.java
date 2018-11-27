package de.skycoder42.kpt;

import java.util.Arrays;

import android.content.Intent;
import android.util.Log;

import org.qtproject.qt5.android.bindings.QtActivity;

import com.google.zxing.integration.android.IntentIntegrator;
import com.google.zxing.integration.android.IntentResult;

public class KptActivity extends QtActivity {
	private static native void reportScanResult(String result);

	public void startQrScan() {
		IntentIntegrator integrator = new IntentIntegrator(this);
		integrator.initiateScan(IntentIntegrator.QR_CODE_TYPES);
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent intent) {
		IntentResult scanResult = IntentIntegrator.parseActivityResult(requestCode, resultCode, intent);
		if (scanResult != null) {
			Log.i("KptActivity", scanResult.getFormatName());
			Log.i("KptActivity", scanResult.getContents());
			//reportScanResult(scanResult.getContents());
		} else
			super.onActivityResult(requestCode, resultCode, intent);
	}
}
