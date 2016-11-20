function GuiController(){}

GuiController.clipManager = null;
GuiController.isErrorState = false;
GuiController.editState = {};

GuiController.escapeHtml = function(unsafe) {
	return unsafe
		.replace(/&/g, "&amp;")
        .replace(/</g, "&lt;")
        .replace(/>/g, "&gt;")
        .replace(/"/g, "&quot;")
        .replace(/'/g, "&#039;");
}

GuiController.showError = function(errorString) {
	if(!GuiController.isErrorState) {
		GuiController.showProgress(false);
		GuiController.isErrorState = true;
		
		BootstrapDialog.show({
			type:  BootstrapDialog.TYPE_DANGER,
			closable: false,
			title: "Error",
			message: errorString + "\n\nPlease refresh the website to start a new session to try again.",
			buttons: [{
				label: "Reload",
				cssClass: 'btn-primary',
				hotkey: 13, //Enter
				action: function(dialogItself){
					location.reload();
				}
			}, {
				label: "Close",
				hotkey: 27, //Esc
				action: function(dialogItself){
					dialogItself.close();
				}
			}]
		});
	}
}

GuiController.showProgress = function(show) {
	if(!GuiController.isErrorState) {
		if(show)
			$(".modal").show();
		else
			$(".modal").hide();
	}
}

GuiController.setProgressText = function(progText) {
	if(!GuiController.isErrorState) 
		$("#busytext").text(progText);
}

GuiController.showQrPanel = function() {
	if(!GuiController.isErrorState)
		$("#qrPanel").show();
}

GuiController.addEntry = function(key, value, guarded, randID) {
	var table = $("#entryTable");
	
	if(guarded) {
		GuiController.editState[randID + "Edit"] = 0;
		table.append('<tr>' + 
					 ' <td class="labelTD"><label>' + GuiController.escapeHtml(key) + ':</label>&nbsp;&nbsp;</td>' + 
					 ' <td><input id="' + randID + 'Edit" type="password" value="' + GuiController.escapeHtml(value) + '" readonly /></td>' +
					 ' <td class="buttonTD"><button onClick="GuiController.togglePw(\'' + randID + 'Edit\')">&#128065;</button></td>' + 
					 ' <td class="buttonTD"><button class="clipBtn" data-clipboard-text="' + GuiController.escapeHtml(value) + '">&#128203;</button></td>' + 
					 '</tr>');
	} else if (key == "Title") {
		$("#entryLegend").text(value);
	} else {
		table.append('<tr>' + 
					 ' <td class="labelTD"><label>' + GuiController.escapeHtml(key)+ ':</label>&nbsp;&nbsp;</td>' + 
					 ' <td colspan="2"><input id="' + randID + 'Edit" type="text" value="' + GuiController.escapeHtml(value) + '" readonly /></td>' + 
					 ' <td class="buttonTD"><button class="clipBtn" data-clipboard-text="' + GuiController.escapeHtml(value) + '">&#128203;</button></td>' + 
					 '</tr>');
	}
}

GuiController.showEntryPanel = function() {	
	$("#entryTable").append('<tr>' + 
							' <td class="labelTD"><label></label>&nbsp;&nbsp;</td>' + 
							' <td colspan="3"><button id="clearButton" class="clipBtn" data-clipboard-text="Some garbage, because I was not able to actually clear the data, just overwrite it...">Clear Clipboard</button></td>' + 
							'</tr>');
	
	if(!GuiController.isErrorState) {
		$("#qrPanel").hide();
		$(".modal").hide();
		$("#entryPanel").show();
		GuiController.clipManager = new Clipboard('.clipBtn');
	}
}

GuiController.togglePw = function(name) {
	if (GuiController.editState[name] == 0) {
		GuiController.editState[name] = 1;
		$("#" + name).attr("type", "text");
	} else {
		GuiController.editState[name] = 0;
		$("#" + name).attr("type", "password");
	}
}