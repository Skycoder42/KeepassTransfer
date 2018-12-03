package de.skycoder42.kpt;

import java.util.ArrayList;

import keepass2android.pluginsdk.PluginAccessBroadcastReceiver;
import keepass2android.pluginsdk.Strings;

public class TransferAccessReceiver extends PluginAccessBroadcastReceiver {
	@Override
	public ArrayList<String> getScopes() {
		ArrayList<String> scopes = new ArrayList<String>();
		scopes.add(Strings.SCOPE_CURRENT_ENTRY);
		scopes.add(Strings.SCOPE_QUERY_CREDENTIALS);
		return scopes;
	}
}
