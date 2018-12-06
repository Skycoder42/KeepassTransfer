import QtQuick 2.11
import QtQuick.Controls 2.4
import de.skycoder42.kpt 1.0

Dialog {
	id: errorDialog
	x: (appWindow.width - errorDialog.width) / 2
	y: (appWindow.height - errorDialog.height) / 2
	title: qsTr("About Keepass Transfer — Version %1").arg(appVersion)
	standardButtons: Dialog.Ok
	modal: true

	Label {
		id: textLabel
		anchors.fill: parent
		text: qsTr("<p>A tool to securely transfer passwords and other credentials from any device to a remote computer for easy access anywhere.</p>
<p>Qt-Version: <a href=\"https://www.qt.io/\">%1</a></p>
<p>Developed by: Skycoder42<br/>
Project Website: <a href=\"https://github.com/Skycoder42/KeepassTransfer\">https://github.com/Skycoder42/KeepassTransfer</a><br/>
License: <a href=\"https://github.com/Skycoder42/KeepassTransfer/blob/master/LICENSE\">GPL-3.0</a></p>
<p>Attributions:<ul>
<li><a href=\"https://github.com/Skycoder42/QtMvvm\">QtMvvm (BSD-3)</a></li>
<li><a href=\"https://github.com/Skycoder42/QtService\">QtService (BSD-3)</a></li>
<li><a href=\"https://github.com/weidai11/cryptopp\">CryptoPP (BSL-1.0)</a></li>
<li><a href=\"https://github.com/nayuki/QR-Code-generator\">QR-Code-generator (MIT)</a></li>
<li><a href=\"https://github.com/iamantony/qtcsv\">qtcsv (MIT)</a></li>
<li><a href=\"https://github.com/PhilippC/keepass2android\">Keepass2Android Plugin SDK (GPL-3.0)</a></li>
<li><a href=\"https://material.io/tools/icons\">MaterialIO Icons (Apache-2.0)</a></li>
</ul>").arg(qtVersion)

		onLinkActivated: emjscon.openUrl(link)
	}
}
