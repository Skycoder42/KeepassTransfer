import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import de.skycoder42.kpt 1.0

Page {
	id: credPage
	property alias credentials: listView.model

	property real preferredLabelWidth: 0

	ScrollView {
		id: scrollView
		anchors.fill: parent

		ListView {
			id: listView

			delegate: ItemDelegate {
				width: scrollView.width

				contentItem: RowLayout {
					Label {
						Layout.column: 0
						Layout.row: 0
						Layout.preferredWidth: Math.max(implicitWidth, credPage.preferredLabelWidth)
						text: qsTr("%1: ").arg(modelData.key)

						Component.onCompleted: credPage.preferredLabelWidth = Math.max(implicitWidth, credPage.preferredLabelWidth)
					}

					TextField {
						id: valueField
						Layout.fillWidth: true
						readOnly: true
						selectByMouse: true
						echoMode: visButton.checked ? TextField.Normal : TextField.Password
						text: modelData.value
					}

					ToolButton {
						id: visButton
						visible: modelData.confidential
						flat: true
						checkable: true
						icon.name: checked ? "show_table_column" : "hide_table_column"
						icon.source: checked ? "qrc:/icons/show.svg" : "qrc:/icons/hide.svg"
						checked: !modelData.confidential

						ToolTip.visible: pressed
						ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
						ToolTip.text:  qsTr("Toggle the visibility of the confidential content")
					}

					ToolButton {
						id: copyButton
						flat: true
						icon.name: "edit-copy"
						icon.source: "qrc:/icons/copy.svg"
						onClicked: {
							valueField.selectAll(); //TODO wont work, do via cpp
							valueField.copy();
						}

						ToolTip.visible: pressed
						ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
						ToolTip.text:  qsTr("Copy the data to the clipboard")
					}
				}
			}

			footerPositioning: ListView.InlineFooter
			footer: Pane {
				width: scrollView.width

				RowLayout {
					anchors.fill: parent

					Button {
						Layout.alignment: Qt.AlignRight
						text: qsTr("Clear Clipboard")
					}
				}
			}
		}
	}
}
