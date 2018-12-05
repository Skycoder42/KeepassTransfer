import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.3
import Qt.labs.settings 1.0
import de.skycoder42.kpt 1.0

ScrollView {
	id: qrViewContainer

	Pane {
		id: qrView
		width: qrViewContainer.width
		height: contentLayout.implicitHeight

		QrEncoder {
			id: encoder
			curve: curveBox.model.get(curveBox.currentIndex).value
			channelId: connector.appId
		}

		Settings {
			id: settings
			category: "qrview"

			property alias curveIndex: curveBox.currentIndex
			property alias ecIndex: ecBox.currentIndex
			property alias qrSize: sizeBox.value
		}

		ColumnLayout {
			id: contentLayout
			anchors.fill: parent
			spacing: 16

			GroupBox {
				id: paramGroup
				title: qsTr("Key-Generation Parameters")
				Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
				Layout.preferredWidth: 400

				GridLayout {
					anchors.fill: parent
					columns: 2
					columnSpacing: 16

					Label {
						text: qsTr("ECC-Curve:")
						Layout.alignment: Qt.AlignLeft
					}

					ComboBox {
						id: curveBox
						Layout.fillWidth: true

						model: ListModel {
							ListElement { key: "secp128r1"; value: DataEncryptor.secp128r1 }
							ListElement { key: "secp256r1"; value: DataEncryptor.secp256r1 }
							ListElement { key: "secp128r2"; value: DataEncryptor.secp128r2 }
							ListElement { key: "secp256k1"; value: DataEncryptor.secp256k1 }
							ListElement { key: "brainpoolP256r1"; value: DataEncryptor.brainpoolP256r1 }
							ListElement { key: "brainpoolP512r1"; value: DataEncryptor.brainpoolP512r1 }
						}
						textRole: "key"

						currentIndex: 4
					}

					Label {
						text: qsTr("Error-Correction:")
						Layout.alignment: Qt.AlignLeft
					}

					ComboBox {
						id: ecBox
						Layout.fillWidth: true

						model: ListModel {
							ListElement { key: qsTr("Low (Level L)"); value: "L" }
							ListElement { key: qsTr("Medium (Level M)"); value: "M" }
							ListElement { key: qsTr("Quartile (Level Q)"); value: "Q" }
							ListElement { key: qsTr("High (Level H)"); value: "H" }
						}
						textRole: "key"

						currentIndex: 0
					}

					Label {
						text: qsTr("QR-Code Size:")
						Layout.alignment: Qt.AlignLeft
					}

					SpinBox {
						id: sizeBox
						Layout.fillWidth: true
						editable: true
						from: 16
						to: 4096
						value: 512
					}

					Button {
						Layout.columnSpan: 2
						Layout.alignment: Qt.AlignRight
						text: qsTr("Generate new Key")
						highlighted: true
						onClicked: encoder.recreateKeys()
					}
				}
			}

			GroupBox {
				id: qrViewBox
				title: qsTr("QR-Code")
				Layout.alignment: Qt.AlignHCenter
				visible: encoder.valid

				ColumnLayout {
					anchors.fill: parent
					spacing: 8

					Image {
						id: qrImage
						source: "image://qrcode/%2?eccLevel=%1"
								.arg(ecBox.model.get(ecBox.currentIndex).value)
								.arg(encoder.qrData)
						sourceSize: Qt.size(sizeBox.value, sizeBox.value)
						cache: false
						smooth: false
						Layout.preferredWidth: sizeBox.value
						Layout.preferredHeight: sizeBox.value
					}

					TextArea {
						id: codeArea
						Layout.fillWidth: true
						Layout.preferredWidth: sizeBox.value
						text: encoder.qrData
						readOnly: true
						selectByMouse: true
						wrapMode: TextArea.WrapAnywhere
						textFormat: TextEdit.PlainText

						onPressed: {
							codeArea.selectAll();
							codeArea.copy();
						}
					}

					Label {
						opacity: 0.5
						text: qsTr("Click the text to copy it to the clipboard")
						font.italic: true
						Layout.fillWidth: true
					}
				}
			}
		}
	}
}
