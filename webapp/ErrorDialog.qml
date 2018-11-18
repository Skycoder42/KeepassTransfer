import QtQuick 2.11
import QtQuick.Controls 2.4
import de.skycoder42.kpt 1.0

Dialog {
	id: errorDialog
	x: (appWindow.width - errorDialog.width) / 2
	y: (appWindow.height - errorDialog.height) / 2
	title: qsTr("An Error occured!")
	standardButtons: Dialog.Ok
	modal: true
	closePolicy: Popup.CloseOnEscape

	property alias errorText: textLabel.text

	Connections {
		target: connector
		onServerError: {
			errorDialog.errorText = message;
			errorDialog.open();
		}
	}

	Label {
		id: textLabel
		anchors.fill: parent
	}
}
