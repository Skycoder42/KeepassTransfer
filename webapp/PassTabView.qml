import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.3
import Qt.labs.settings 1.0
import de.skycoder42.kpt 1.0

Pane {
	id: passView

	PassEncoder {
		id: encoder
	}

	ColumnLayout {
		id: contentLayout
		anchors.fill: parent

		GroupBox {
			id: passGroup
			title: qsTr("Passphrase and Identity")
			Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
			Layout.preferredWidth: 500
			Layout.minimumWidth: 0

			GridLayout {
				anchors.fill: parent
				columns: 4
				columnSpacing: 16

				Label {
					text: qsTr("Identity:")
					Layout.alignment: Qt.AlignLeft
				}

				TextField {
					id: idField
					Layout.fillWidth: true
					Layout.columnSpan: 3
					readOnly: true
					selectByMouse: true
					text: connector.appIdStr
				}

				Label {
					text: qsTr("Passphrase:")
					Layout.alignment: Qt.AlignLeft
				}

				TextField {
					id: passField
					Layout.fillWidth: true
					selectByMouse: true
					echoMode: visButton.checked ? TextField.Normal : TextField.Password

					text: encoder.passphrase
					onTextEdited: encoder.passphrase = text
				}

				ToolButton {
					id: visButton
					flat: true
					checkable: true
					icon.name: checked ? "show_table_column" : "hide_table_column"
					icon.source: checked ? "qrc:/icons/show.svg" : "qrc:/icons/hide.svg"
					checked: false

					ToolTip.visible: pressed
					ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
					ToolTip.text:  qsTr("Toggle the visibility of the password")
				}

				ToolButton {
					id: generateButton
					flat: true
					icon.name: "password-generate"
					icon.source: "qrc:/icons/key.svg"
					onClicked: encoder.generateRandom()

					ToolTip.visible: pressed
					ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
					ToolTip.text:  qsTr("Generate a new, 16 character password")
				}
			}
		}
	}
}
