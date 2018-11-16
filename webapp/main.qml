import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Window {
	visible: true
	width: 640
	height: 480
	title: qsTr("Hello World")

	ColumnLayout {
		anchors.centerIn: parent

		Label {
			id: hLabel
			text: qsTr("Hello World!")
		}

		Image {
			source: "image://QZXing/encode/" + hLabel.text;
			cache: false;
		}
	}
}
