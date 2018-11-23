import QtQuick 2.11
import QtQuick.Controls 2.4
import de.skycoder42.QtMvvm.Core 1.1
import de.skycoder42.QtMvvm.Quick 1.1
import de.skycoder42.kpt 1.0

ScrollView {
	id: rootDrawer
	property KptRootViewModel viewModel: null
	property Drawer drawer: null

	anchors.fill: parent

	ListView {
		id: listView

		model: [
			qsTr("Select Credentials"),
			qsTr("Transfer Mode")
		]

		delegate: ItemDelegate {
			width: rootDrawer.width
			text: modelData
		}
	}
}
