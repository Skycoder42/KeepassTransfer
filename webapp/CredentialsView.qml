import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.3
import de.skycoder42.kpt 1.0

Page {
	id: credPage
	property alias credentials: listView.model
	property string idCred: qsTr("<Unnamed>")

	property real preferredLabelWidth: 0

	header: ToolBar {
		Material.background: Material.primary
		Material.foreground: "black"
		height: 56

		RowLayout {
			anchors.fill: parent

			ToolButton {
				id: backBtn
				flat: true
				icon.name: "draw-arrow-back"
				icon.source: "qrc:/icons/back.svg"
				onClicked: mainStack.pop()

				ToolTip.visible: pressed
				ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
				ToolTip.text:  qsTr("Go back to the setup page")
			}

			Label {
				Layout.fillWidth: true
				Layout.fillHeight: true
				text: qsTr("%1 — Keepass Transfer").arg(idCred)
				font.pointSize: 14
				font.bold: true
				elide: Label.ElideRight
				horizontalAlignment: Qt.AlignLeft
				verticalAlignment: Qt.AlignVCenter
			}
		}
	}

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
						onClicked: {
							cpClear.selectAll();
							cpClear.copy();
						}

						TextField {
							id: cpClear
							visible: false
							text: qsTr("-- Clipboard was cleared --")
						}
					}
				}
			}
		}
	}
}
