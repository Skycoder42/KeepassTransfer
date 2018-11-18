import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.4

Window {
	visible: true
	width: 640
	height: 480
	title: qsTr("Hello World")

	ColumnLayout {
		anchors.centerIn: parent

		Label {
			id: hLabel
			text: qsTr("Current status: %1\nAppId: %2")
				.arg(connector.connected ? "connected" : "disconnected")
				.arg(connector.appId)
		}

		Image {
			source: "image://QZXing/encode/" + hLabel.text;
			cache: false;
		}
	}
}
