import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.4

ApplicationWindow {
	id: appWindow
	visible: true
	width: 960
	height: 720
	title: qsTr("KeePass Transfer")

	StackView {
		id: mainStack
		anchors.fill: parent
		initialItem: MainView {
		}
	}

	ErrorDialog {}
}
