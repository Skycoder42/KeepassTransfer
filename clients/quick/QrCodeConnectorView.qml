import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.3
import de.skycoder42.QtMvvm.Core 1.1
import de.skycoder42.QtMvvm.Quick 1.1
import de.skycoder42.kpt 1.0
import "helpers"

Page {
	id: qrView
	property QrCodeConnectorViewModel viewModel: null

	header: ContrastToolBar {
		height: headerLayout.implicitHeight

		GridLayout {
			id: headerLayout
			anchors.fill: parent
			columnSpacing: 0
			rowSpacing: 0
			columns: 2

			ActionButton {
				icon.name: "open-menu-symbolic"
				icon.source: "qrc:/icons/menu.svg"
				onClicked: QuickPresenter.toggleDrawer()
			}

			ToolBarLabel {
				text: qsTr("QR-Code Transfer")
				Layout.fillWidth: true
				Layout.preferredHeight: 56
			}

			TabBar {
				id: inputBar
				Layout.fillWidth: true
				Layout.columnSpan: 2
				position: TabBar.Header
				Material.background: Material.primary
				currentIndex: swipeView.currentIndex

				TabButton {
					text: qsTr("Take Image")
				}

				TabButton {
					text: qsTr("Enter Text")
				}
			}
		}
	}

	PresenterProgress {}

	SwipeView {
		id: swipeView
		anchors.fill: parent
		currentIndex: inputBar.currentIndex

		Item {}

		QrTextInput {
			id: textInput
			viewModel: qrView.viewModel
		}
	}
}
