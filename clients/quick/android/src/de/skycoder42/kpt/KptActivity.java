package de.skycoder42.kpt;

import java.util.Arrays;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;

import android.content.Intent;

import android.os.Bundle;

import android.util.Log;

import org.qtproject.qt5.android.bindings.QtActivity;

import com.google.zxing.integration.android.IntentIntegrator;
import com.google.zxing.integration.android.IntentResult;

public class KptActivity extends QtActivity {
	public static class KptEntry {
		public String key;
		public String value;
		public boolean guarded;

		public KptEntry(String key, String value, boolean guarded) {
			this.key = key;
			this.value = value;
			this.guarded = guarded;
		}
	}

	public static final String DataEntriesExtra = "de.skycoder42.kpt.KptActivity.DataEntriesExtra";
	public static final String GuardedEntriesExtra = "de.skycoder42.kpt.KptActivity.GuardedEntriesExtra";

	private ArrayList<KptEntry> _cachedFields;
	private synchronized void setTransferEntries(ArrayList<KptEntry> entries) {
		_cachedFields = entries;
	}

	private native void reportScanResult(String result);

	public void startQrScan() {
		IntentIntegrator integrator = new IntentIntegrator(this);
		integrator.addExtra("SAVE_HISTORY", false);
		integrator.initiateScan(IntentIntegrator.QR_CODE_TYPES);
	}

	public synchronized List<KptEntry> getLastTransferEntries() {
		return _cachedFields;
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		Intent intent = getIntent();
		if(intent != null && intent.hasExtra(DataEntriesExtra) && intent.hasExtra(GuardedEntriesExtra)) {
			HashMap<String, String> entries = (HashMap<String, String>)intent.getSerializableExtra(DataEntriesExtra);
			List<String> guarded = Arrays.asList(intent.getStringArrayExtra(GuardedEntriesExtra));
			ArrayList<KptEntry> fields = new ArrayList<>(entries.size());
			for (Map.Entry<String, String> entry : entries.entrySet()) {
				fields.add(new KptEntry(
					entry.getKey(),
					entry.getValue(),
					guarded.contains(entry.getKey())));
			}
			setTransferEntries(fields);
		} else
			setTransferEntries(null);
		super.onCreate(savedInstanceState);
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent intent) {
		IntentResult scanResult = IntentIntegrator.parseActivityResult(requestCode, resultCode, intent);
		if (scanResult != null)
			reportScanResult(scanResult.getContents());
		else
			super.onActivityResult(requestCode, resultCode, intent);
	}
}
