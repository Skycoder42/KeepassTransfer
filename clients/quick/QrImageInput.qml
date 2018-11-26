import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.3
import QtMultimedia 5.11
import de.skycoder42.QtMvvm.Core 1.1
import de.skycoder42.QtMvvm.Quick 1.1
import de.skycoder42.kpt 1.0

Rectangle {
	id: imageInput
	property QrCodeConnectorViewModel viewModel: null
	color: "black"

	function focusCamera() {
		camera.focus.focusMode = CameraFocus.FocusContinuous;
		camera.focus.focusPointMode = CameraFocus.FocusPointAuto;
	}

	Camera {
		id: camera
		captureMode: Camera.CaptureVideo
		focus.focusMode: CameraFocus.FocusContinuous
		focus.focusPointMode: CameraFocus.FocusPointAuto
	}

	VideoOutput {
		id: vOut
		source: camera
		anchors.fill: parent
		autoOrientation: true

		readonly property real scanRectLen : Math.min(Math.min(vOut.contentRect.width, vOut.contentRect.height) * 0.8, Math.max(vOut.contentRect.width, vOut.contentRect.height) * 0.5)

		MouseArea {
			anchors.fill: parent
			onClicked: {
				camera.focus.customFocusPoint = Qt.point(mouse.x / width,  mouse.y / height);
				camera.focus.focusMode = CameraFocus.FocusMacro;
				camera.focus.focusPointMode = CameraFocus.FocusPointCustom;
			}
		}

		Item {
			id: contentZone
			width: vOut.contentRect.width
			height: vOut.contentRect.height
			anchors.centerIn: parent

			Rectangle {
				id: captureZone
				border.width: 1
				border.color: "red"
				color: "transparent"
				width: vOut.scanRectLen
				height: vOut.scanRectLen
				anchors.centerIn: parent
			}

			Rectangle {
				id: topVeil
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.top: parent.top
				anchors.bottom: captureZone.top
				opacity: 0.5
				color: "black"
			}

			Rectangle {
				id: bottomVeil
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.top: captureZone.bottom
				anchors.bottom: parent.bottom
				opacity: 0.5
				color: "black"
			}

			Rectangle {
				id: leftVeil
				anchors.left: parent.left
				anchors.right: captureZone.left
				anchors.top: topVeil.bottom
				anchors.bottom: bottomVeil.top
				opacity: 0.5
				color: "black"
			}

			Rectangle {
				id: rightVeil
				anchors.left: captureZone.right
				anchors.right: parent.right
				anchors.top: topVeil.bottom
				anchors.bottom: bottomVeil.top
				opacity: 0.5
				color: "black"
			}
		}
	}

	Label {
		id: errorLabel
		z: 10
		anchors.bottom: parent.bottom
		anchors.bottomMargin: 48
		anchors.horizontalCenter: parent.horizontalCenter

		color: "darkred"
		visible: camera.errorString != ""
		text: qsTr("Camera Error: ") + camera.errorString
		font.bold: true
	}

	Rectangle {
		visible: errorLabel.visible
		z: 9
		height: errorLabel.height * 1.4
		width: errorLabel.width + errorLabel.height
		anchors.centerIn: errorLabel
		radius: height / 2
		color: "grey"
		opacity: 0.8
	}
}
