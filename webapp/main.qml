import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.4

ApplicationWindow {
	id: appWindow
	visible: true
	width: 960
	height: 720
	title: Qt.application.displayName

	StackView {
		id: mainStack
		anchors.fill: parent
		initialItem: MainView {
		}

		Component {
			id: credentialsViewComponent
			CredentialsView {}
		}

		onCurrentItemChanged: emjscon.setTag(mainStack.depth)

		Connections {
			target: connector
			onCredentialsReceived: {
				mainStack.push(credentialsViewComponent, {
								   credentials: receivedCreds,
								   idCred: entryTitle
							   });
			}
		}

		Connections {
			target: emjscon
			onTagChanged: {
				if(tag != "") {
					var index = Number(tag);
					if(tag < mainStack.depth)
						mainStack.pop(mainStack.get(index - 1, StackView.ForceLoad));
				}
			}
		}
	}

	ErrorDialog {}

	AboutDialog {
		id: aboutDialog
	}
}
