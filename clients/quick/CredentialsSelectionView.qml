import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import de.skycoder42.QtMvvm.Core 1.1
import de.skycoder42.QtMvvm.Quick 1.1
import de.skycoder42.kpt 1.0

Page {
	property CredentialsSelectionViewModel viewModel: null

	header: ContrastToolBar {
		RowLayout {
			anchors.fill: parent
			spacing: 0

			ActionButton {
				icon.name: "open-menu-symbolic"
				icon.source: "qrc:/icons/menu.svg"
				onClicked: QuickPresenter.toggleDrawer()
			}

			ToolBarLabel {
				text: qsTr("Select Credentials")
				Layout.fillWidth: true
			}

			ActionButton {
				text: qsTr("Send")
				display: ToolButton.TextBesideIcon
				icon.name: "mail-send"
				icon.source: "qrc:/icons/send.svg"
				onClicked: viewModel.commitCredentials()
			}

			MenuButton {
				MenuItem {
					text: qsTr("Select all")
					onClicked: viewModel.selectAll()
				}

				MenuItem {
					text: qsTr("Deselect all")
					onClicked: viewModel.deselectAll()
				}
			}
		}
	}

	PresenterProgress {}

	ScrollView {
		id: scrollView
		anchors.fill: parent

		ListView {
			id: listView

			model: viewModel.credModel

			delegate: CheckDelegate {
				width: scrollView.width
				text: key
				icon.name: iconName
				icon.source: iconPath
				checkable: true
				checkState: selected
				onCheckStateChanged: selected = checkState
			}
		}
	}
}
