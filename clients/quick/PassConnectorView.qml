import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.3
import de.skycoder42.QtMvvm.Core 1.1
import de.skycoder42.QtMvvm.Quick 1.1
import de.skycoder42.kpt 1.0

Page {
	id: passView
	property PassConnectorViewModel viewModel: null

	header: ContrastToolBar {
		height: headerLayout.implicitHeight

		RowLayout {
			id: headerLayout
			anchors.fill: parent
			spacing: 0

			ActionButton {
				icon.name: "open-menu-symbolic"
				icon.source: "qrc:/icons/menu.svg"
				onClicked: QuickPresenter.toggleDrawer()
			}

			ToolBarLabel {
				text: qsTr("Passphrase Transfer")
				Layout.fillWidth: true
				Layout.preferredHeight: 56
			}

			ActionButton {
				text: qsTr("Send")
				display: ToolButton.TextBesideIcon
				enabled: viewModel.valid
				icon.name: "mail-send"
				icon.source: "qrc:/icons/send.svg"
				onClicked: viewModel.transfer()
			}
		}
	}

	PresenterProgress {}

	Pane {
		anchors.fill: parent
		GridLayout {
			anchors.fill: parent
			columns: 4

			DecorLabel {
				Layout.fillWidth: true
				text: qsTr("Channel-ID:")
				edit: channelIdField
			}

			Label {
				Layout.fillWidth: true
				Layout.columnSpan: 3
				text: viewModel.valid ? qsTr("Channel-ID is valid") : qsTr("Channel-ID is invalid")
				font.italic: true
				color: viewModel.valid ? "green" : "red"
			}

			TextField {
				id: channelIdField
				Layout.fillWidth: true
				Layout.columnSpan: 4
				validator: viewModel.channelIdValidator

				MvvmBinding {
					viewModel: passView.viewModel
					viewModelProperty: "channelIdStr"
					view: channelIdField
					viewProperty: "text"
					type: MvvmBinding.OneWayToViewModel
				}
			}

			DecorLabel {
				Layout.fillWidth: true
				Layout.columnSpan: 4
				text: qsTr("Passphrase:")
				edit: passphraseField
			}

			TextField {
				id: passphraseField
				Layout.fillWidth: true
				Layout.columnSpan: 2
				selectByMouse: true
				echoMode: visButton.checked ? TextField.Normal : TextField.Password

				MvvmBinding {
					viewModel: passView.viewModel
					viewModelProperty: "passphrase"
					view: passphraseField
					viewProperty: "text"
					viewChangeSignal: "editingFinished()"
				}
			}

			ActionButton {
				id: visButton
				flat: true
				checkable: true
				icon.name: checked ? "show_table_column" : "hide_table_column"
				icon.source: checked ? "qrc:/icons/show.svg" : "qrc:/icons/hide.svg"
				text:  qsTr("Toggle the visibility of the password")
				checked: false
			}

			ActionButton {
				id: generateButton
				flat: true
				icon.name: "password-generate"
				icon.source: "qrc:/icons/key.svg"
				text:  qsTr("Generate a new, 16 character password")

				onClicked: viewModel.generateRandomPassphrase()
			}

			Item {
				Layout.columnSpan: 4
				Layout.fillHeight: true
				Layout.fillWidth: true
			}
		}
	}
}
