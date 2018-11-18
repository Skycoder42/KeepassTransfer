import QtQuick 2.10
import QtQuick.Controls 2.4

Page {
	id: mainView

	header: TabBar {
		id: modeBar
		currentIndex: modeView.currentIndex

		TabButton {
			text: qsTr("QR-Code")
		}

		TabButton {
			text: qsTr("Password")
		}
	}

	SwipeView {
		id: modeView
		anchors.fill: parent
		currentIndex: modeBar.currentIndex

		QrTabView {

		}

		Item {}
	}
}
