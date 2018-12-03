package de.skycoder42.kpt;

import android.content.Context;
import android.content.Intent;

import android.os.Bundle;

import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;

import keepass2android.pluginsdk.PluginActionBroadcastReceiver;
import keepass2android.pluginsdk.PluginAccessException;
import keepass2android.pluginsdk.Strings;

public class TransferActionReceiver extends PluginActionBroadcastReceiver {
	private Intent _currentIntent;

	@Override
	protected void openEntry(OpenEntryAction oe) {
		try {
			oe.addEntryAction(oe.getContext().getString(R.string.action_entry),
				R.mipmap.ic_foreground,
				new Bundle());
		} catch (PluginAccessException e) {
			Log.getStackTraceString(e);
		}
	}

	@Override
	protected void actionSelected(ActionSelectedAction actionSelected) {
		Intent intent = new Intent(actionSelected.getContext(), KptActivity.class);

		intent.putExtra(KptActivity.DataEntriesExtra, actionSelected.getEntryFields());

		String[] protFields = actionSelected.getProtectedFieldsList();
		if (protFields == null) {//in case it was fixed -> only if not successful (see https://github.com/PhilippC/keepass2android/issues/627)
			try {
				JSONArray fieldData = new JSONArray(_currentIntent.getStringExtra(Strings.EXTRA_PROTECTED_FIELDS_LIST));
				protFields = new String[fieldData.length()];
				for(int i = 0; i < fieldData.length(); i++)
					protFields[i] = fieldData.getString(i);
			} catch(JSONException e) {
				Log.getStackTraceString(e);
				protFields = new String[0];
			}
		}
		intent.putExtra(KptActivity.GuardedEntriesExtra, protFields);

		intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
		actionSelected.getContext().startActivity(intent);
	}

	@Override
	public void onReceive(Context context, Intent intent) {
		_currentIntent = intent;
		super.onReceive(context, intent);
		_currentIntent = null;
	}
}
