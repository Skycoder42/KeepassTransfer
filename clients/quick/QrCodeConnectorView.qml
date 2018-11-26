import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.3
import de.skycoder42.QtMvvm.Core 1.1
import de.skycoder42.QtMvvm.Quick 1.1
import de.skycoder42.kpt 1.0

Page {
	id: qrView
	property QrCodeConnectorViewModel viewModel: null

	function captureImage() {
		console.log("UNIMPLEMENTED");
	}

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
				text: qsTr("QR-Code Transfer")
				Layout.fillWidth: true
				Layout.preferredHeight: 56
			}

			ActionButton {
				text: qsTr("Take another image")
				icon.name: "camera-photo"
				icon.source: "qrc:/icons/camera.svg"
				onClicked: captureImage()
			}
		}
	}

	Component.onCompleted: captureImage()

	PresenterProgress {}

	Pane {
		anchors.fill: parent
		GridLayout {
			anchors.fill: parent
			columns: 3

			ScrollView {
				id: textScroller
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.columnSpan: 3

				Flickable {
					TextArea.flickable: TextArea {
						id: codeArea
						selectByMouse: true
						selectByKeyboard: true
						wrapMode: TextArea.WrapAnywhere
						textFormat: TextEdit.PlainText

						MvvmBinding {
							viewModel: qrView.viewModel
							viewModelProperty: "qrData"
							view: codeArea
							viewProperty: "text"
							type: MvvmBinding.OneWayToViewModel
						}
					}
				}
			}

			Label {
				Layout.fillWidth: true
				text: viewModel.valid ? qsTr("Content is valid") : qsTr("Content is invalid")
				font.italic: true
				color: viewModel.valid ? "green" : "red"
			}

			Button {
				id: pasteBtn
				text: qsTr("Paste")
				onClicked: {
					codeArea.clear();
					codeArea.paste();
				}
			}

			Button {
				id: acceptBtn
				enabled: viewModel.valid
				text: qsTr("Transfer")
				highlighted: true
				onClicked: viewModel.transfer()
			}
		}
	}
}
