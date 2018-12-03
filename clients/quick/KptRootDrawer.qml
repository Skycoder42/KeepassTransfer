import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.3
import de.skycoder42.QtMvvm.Core 1.1
import de.skycoder42.QtMvvm.Quick 1.1
import de.skycoder42.kpt 1.0

ColumnLayout {
	id: rootDrawer
	property KptRootViewModel viewModel: null
	property Drawer drawer: null

	anchors.fill: parent
	spacing: 0

	ColorHelper {
		id: helper
	}

	Rectangle {
		id: headerRect
		Layout.fillWidth: true
		Layout.preferredHeight: width * 0.666
		color: QuickPresenter.currentStyle == "Material" ? Material.primary : helper.highlight
	}

	Item {
		id: spacer
		Layout.fillHeight: true
		Layout.fillWidth: true
	}

	MenuSeparator {
		Layout.fillWidth: true
		topPadding: 0
		bottomPadding: 0
	}

	ItemDelegate {
		Layout.fillWidth: true
		display: Button.TextBesideIcon
		icon.name: "settings-configure"
		icon.source: "qrc:/de/skycoder42/qtmvvm/icons/settings.svg"
		text: qsTr("Settings")
		onClicked: {
			viewModel.showSettings();
			drawer.close();
		}
	}

	MenuSeparator {
		Layout.fillWidth: true
		topPadding: 0
		bottomPadding: 0
	}

	ItemDelegate {
		Layout.fillWidth: true
		display: Button.TextBesideIcon
		icon.name: "help-about"
		icon.source: "qrc:/icons/about.svg"
		text: qsTr("About this App")
		onClicked: viewModel.about()
	}
}
