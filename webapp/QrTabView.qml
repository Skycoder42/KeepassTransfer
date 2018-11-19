import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
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
		}

		BrowserStorage {
			id: storage
		}

		ColumnLayout {
			id: contentLayout
			anchors.fill: parent

			GroupBox {
				id: paramGroup
				title: qsTr("Key-Generation Parameters")
				Layout.alignment: Qt.AlignCenter

				GridLayout {
					anchors.fill: parent
					columns: 2
					columnSpacing: 16

					Label {
						text: qsTr("ECC-Curve:")
						Layout.fillWidth: true
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

						currentIndex: storage.value("curveIndex", 2)
						onCurrentIndexChanged: storage.setValue("curveIndex", currentIndex)
					}

					Label {
						text: qsTr("Error-Correction:")
						Layout.fillWidth: true
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

						currentIndex: storage.value("ecIndex", 0)
						onCurrentIndexChanged: storage.setValue("ecIndex", currentIndex)
					}

					Label {
						text: qsTr("QR-Code Size:")
						Layout.fillWidth: true
					}

					SpinBox {
						id: heightBox
						Layout.fillWidth: true
						editable: true
						from: 16
						to: 4096
						value: storage.value("qrSize", 512)
						onValueChanged: storage.setValue("qrSize", value)
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
				Layout.alignment: Qt.AlignCenter
				visible: encoder.valid

				Image {
					source: "image://QZXing/encode/%2?format=qrcode&corretionLevel=%1"
							.arg(ecBox.model.get(ecBox.currentIndex).value)
							.arg(encoder.publicKey)
					sourceSize: Qt.size(heightBox.value, heightBox.value)
					cache: false
					height: heightBox.value
					width: height
				}
			}
		}
	}
}
