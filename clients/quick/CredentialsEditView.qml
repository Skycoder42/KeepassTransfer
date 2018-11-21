import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import de.skycoder42.QtMvvm.Core 1.1
import de.skycoder42.QtMvvm.Quick 1.1
import de.skycoder42.kpt 1.0

Page {
	property CredentialsEditViewModel viewModel: null

	header: ContrastToolBar {
		RowLayout {
			anchors.fill: parent
			spacing: 0

			ToolBarLabel {
				text: qsTr("Enter Credentials")
				Layout.fillWidth: true
			}

			ActionButton {
				text: qsTr("Send")
				display: ToolButton.TextBesideIcon
				icon.name: "mail-send"
				icon.source: "qrc:/icons/send.svg"
				onClicked: viewModel.commitCredentials()
			}
		}
	}

	PresenterProgress {}

	ScrollView {
		id: scrollView
		anchors.fill: parent

		ListView {
			id: listView

			model: viewModel.credentialsModel

			delegate: ItemDelegate {
				width: scrollView.width

				contentItem: RowLayout {
					TextField {
						id: keyField
						placeholderText: qsTr("Key")
						text: key
						onTextEdited: key = text
					}

					TextField {
						id: valueField
						Layout.fillWidth: true
						placeholderText: qsTr("Value")
						text: value
						onTextEdited: value = text
					}

					ToolButton {
						id: confButton
						flat: true
						checkable: true
						icon.name: checked ? "lock" : "unlock"
						checked: confidential
						onCheckedChanged: confidential = checked
					}
				}
			}

			footer: Item {
				height: addButton.height + 2 * addButton.anchors.margins
				width: scrollView.width
			}
		}
	}

	RoundActionButton {
		id: addButton
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.margins: 16

		icon.name: "list-add"
		icon.source: "qrc:/icons/add.svg"
		text: qsTr("Add entry")

		onClicked: viewModel.addEmptyEntry()
	}
}
