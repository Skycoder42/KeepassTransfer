import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.4

Page {
	id: mainView

	header: ToolBar {
		Material.background: Material.primary
		Material.foreground: "black"

		GridLayout {
			anchors.fill: parent
			columns: 2

			Label {
				Layout.preferredHeight: 56
				Layout.fillWidth: true
				text: qsTr("Keepass Transfer")
				font.pointSize: 14
				font.bold: true
				elide: Label.ElideRight
				horizontalAlignment: Qt.AlignLeft
				verticalAlignment: Qt.AlignVCenter
				leftPadding: 16
			}

			ToolButton {
				id: aboutBtn
				flat: true
				icon.name: "help-about"
				icon.source: "qrc:/icons/about.svg"
				onClicked: aboutDialog.open()

				ToolTip.visible: pressed
				ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
				ToolTip.text:  qsTr("About Keepass Transfer")
			}

			TabBar {
				id: modeBar
				currentIndex: modeView.currentIndex
				Layout.fillWidth: true
				Layout.columnSpan: 2

				TabButton {
					text: qsTr("QR-Code")
				}

				TabButton {
					text: qsTr("Password")
				}
			}
		}
	}

	SwipeView {
		id: modeView
		anchors.fill: parent
		currentIndex: modeBar.currentIndex

		QrTabView {}

		PassTabView {}
	}
}
