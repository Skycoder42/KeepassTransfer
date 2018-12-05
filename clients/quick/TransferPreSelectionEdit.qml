import QtQuick 2.11
import QtQuick.Controls 2.4

ScrollView {
	id: preSelView

	property var inputValue

	implicitHeight: dummyDelegate.height * (elementList.model ? elementList.model.length : 1)

	ListView {
		id: elementList
		model: preSelView.inputValue

		CheckDelegate {
			id: dummyDelegate
			visible: false
			text: "dummy"
		}

		delegate: CheckDelegate {
			id: delegate
			width: preSelView.width
			text: modelData.key
			checked: modelData.selected
			onCheckedChanged: inputValue[index].selected = delegate.checked
		}
	}
}
