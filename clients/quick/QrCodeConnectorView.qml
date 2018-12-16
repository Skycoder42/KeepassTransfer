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

	function closeAction() {
		if(scanner.scanning) {
			scanner.scanning = false;
			return true;
		} else
			return false;
	}

	QrCodeScanner {
		id: scanner

		property bool scanning: false

		onScanResultReady: {
			scanning = (scanResult == "");
			bugLoader.codeAreaItem.text = scanResult
			if(viewModel.valid)
				viewModel.transfer();
		}

		function startScan() {
			scanning = true;
			initiateScan();
		}
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
				id: scanButton
				visible: scanner.canScan
				text: qsTr("Take another image")
				icon.name: "camera-photo"
				icon.source: "qrc:/icons/camera.svg"
				onClicked: scanner.startScan()
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

	Component.onCompleted: {
//		if(scanner.canScan)
//			scanner.startScan();
	}

	PresenterProgress {}

	Pane {
		anchors.fill: parent
		GridLayout {
			anchors.fill: parent
			columns: 2

			Component {
				id: normalEditComponent

				ScrollView {
					id: textScroller
					property var codeAreaRef: codeArea

					Flickable {
						TextArea.flickable: TextArea {
							id: codeArea
							selectByMouse: true
							selectByKeyboard: true
							wrapMode: TextArea.WrapAnywhere
							textFormat: TextEdit.PlainText
							placeholderText: qsTr("Scan a QR-Code or paste the\npublic key data here to continue.")

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
			}

			Component {
				id: bugFixComponent

				Label {
					property var codeAreaRef: codeAreaDummy

					text: qrView.viewModel.qrData
					wrapMode: Label.WrapAnywhere
					textFormat: Label.PlainText

					QtObject {
						id: codeAreaDummy
						property string text
						//onTextChanged: qrView.viewModel.qrData = text
						function clear() {
							text = "";
						}
						function paste() {
							text = scanner.pasteData();
						}
					}
				}
			}

			Loader {
				id: bugLoader
				property var codeAreaItem: null

				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.columnSpan: 2

				active: true
				sourceComponent: scanner.canScan ? bugFixComponent : normalEditComponent
				onLoaded: codeAreaItem = item.codeAreaRef
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
					bugLoader.codeAreaItem.clear();
					bugLoader.codeAreaItem.paste();
					if(viewModel.pasteTransfer)
						viewModel.transfer();
				}
			}
		}
	}
}
