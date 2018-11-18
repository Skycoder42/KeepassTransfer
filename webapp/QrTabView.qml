import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

ScrollView {
	id: qrViewContainer

	Pane {
		id: qrView
		width: qrViewContainer.width
		height: contentLayout.implicitHeight

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
						model: ["secp128r1", "secp256r1", "secp128r2", "secp256k1", "brainpoolP256r1", "brainpoolP512r1"]
						currentIndex: 1
						Layout.fillWidth: true
					}

					Label {
						text: qsTr("Error-Correction:")
						Layout.fillWidth: true
					}

					ComboBox {
						model: ListModel {
							ListElement { text: qsTr("Low (Level L)"); value: 0 }
							ListElement { text: qsTr("Medium (Level M)"); value: 1 }
							ListElement { text: qsTr("Quartile (Level Q)"); value: 2 }
							ListElement { text: qsTr("High (Level H)"); value: 3 }
						}
						textRole: "text"

						currentIndex: 0
						Layout.fillWidth: true
					}

					Label {
						text: qsTr("QR-Code Size:")
						Layout.fillWidth: true
					}

					SpinBox {
						Layout.fillWidth: true
						from: 16
						to: 4096
						value: 512
						editable: true
					}

					Button {
						Layout.columnSpan: 2
						Layout.alignment: Qt.AlignRight
						text: qsTr("Generate QR-Code")
						highlighted: true
					}
				}
			}
		}
	}
}
