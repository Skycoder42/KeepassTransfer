import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.4

Window {
	visible: true
	width: 640
	height: 480
	title: qsTr("Hello World")

	Label {
		anchors.centerIn: parent
		text: qsTr("Hello World!")
	}
}
