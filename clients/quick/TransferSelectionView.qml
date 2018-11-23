import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import de.skycoder42.QtMvvm.Core 1.1
import de.skycoder42.QtMvvm.Quick 1.1
import de.skycoder42.kpt 1.0

Page {
	id: mainView
	property TransferSelectionViewModel viewModel: null

	header: ContrastToolBar {
		ToolBarLabel {
			text: qsTr("Select a mode")
			anchors.fill: parent
		}
	}

	PresenterProgress {}

	ScrollView {
		id: scrollView
		anchors.fill: parent

		ListView {
			id: listView

			model: viewModel.modeModel

			delegate: ItemDelegate {
				width: scrollView.width

				onClicked: viewModel.startTransfer(index)

				Component.onCompleted: height = Qt.binding(function(){ return grid.implicitHeight + padding; })

				contentItem: GridLayout {
					id: grid
					rows: 2
					columns: 2
					columnSpacing: 16

					Label {
						id: titleLabel
						text: edit
						Layout.row: 0
						Layout.column: 0
						font.bold: true
						elide: Label.ElideRight
						Layout.fillWidth: true
					}

					Label {
						id: textLabel
						Layout.row: 1
						Layout.column: 0
						text: toolTip
						wrapMode: Text.WordWrap
						Layout.fillWidth: true
					}

					TintIcon {
						id: openIcon
						icon.source: "qrc:/de/skycoder42/qtmvvm/quick/icons/ic_chevron_right.svg"
						Layout.row: 0
						Layout.column: 2
						Layout.rowSpan: 2
						Layout.fillHeight: true
						Layout.preferredWidth: implicitWidth
						Layout.preferredHeight: implicitHeight
						Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
					}
				}
			}
		}
	}
}
