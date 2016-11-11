var secretString;
var publicKey;
var privateKey;

var editState = {};
var clipManager;

function setSecret(secret) {
	secretString = secret;
}

function showError(errorString) {
	BootstrapDialog.show({
		type:  BootstrapDialog.TYPE_DANGER,
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
			action: function(dialogItself){
				dialogItself.close();
			}
		}]
	});
}

function makeid() {
    var text = "";
    var possible = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    for( var i=0; i < 10; i++ )
        text += possible.charAt(Math.floor(Math.random() * possible.length));

    return text;
}

function escapeHtml(unsafe) {
    return unsafe
         .replace(/&/g, "&amp;")
         .replace(/</g, "&lt;")
         .replace(/>/g, "&gt;")
         .replace(/"/g, "&quot;")
         .replace(/'/g, "&#039;");
 }

function decryptData(data) {	
	var len = data.length;
	var table = $("#entryTable");
	for(var i = 0; i < len; i++) {
		var cData = data[i];
		var crypt = new JSEncrypt();		
		crypt.setPrivateKey(privateKey);
		
		var decr = crypt.decrypt(cData.Value);		
		if(cData.Guarded || cData.Key == "Password") {
			var randID = makeid();
			editState[randID + "Label"] = 0;
			table.append('<tr><td class="labelTD"><label>' + 
						 escapeHtml(cData.Key) + 
						 '</label>&nbsp;&nbsp;</td><td><input id="' + 
						 randID +
						 'Label" type="password" value="' + 
						 escapeHtml(decr) + 
						 '" readonly /></td><td class="buttonTD"><button onClick="togglePw(\'' + 
						 randID +
						 'Label\')">&#128065;</button></td><td class="buttonTD"><button class="clipBtn" data-clipboard-text="' + 
						 escapeHtml(decr) +
						 '">&#128203;</button></td></tr>');
		} else if (cData.Key == "Title") {
			$("#entryLegend").text(decr);
		} else {
			table.append('<tr><td class="labelTD"><label>' +
						 escapeHtml(cData.Key)+ 
						 ':</label>&nbsp;&nbsp;</td><td colspan="2"><input type="text" value="' +
						 escapeHtml(decr) + 
						 '" readonly /></td><td class="buttonTD"><button class="clipBtn" data-clipboard-text="' + 
						 escapeHtml(decr) +
						 '">&#128203;</button></td></tr>');
		}
	}
	
	clipManager = new Clipboard('.clipBtn');
	$(".modal").hide();
	$("#entryPanel").show();
}

function pollForSecret() {
	setTimeout(function() {
		$.ajax({
					url: "./transferEngine.php", 
					dataType: "json",
					data: {
						method: "UPDATE",
						secret: secretString
					},
					method: "POST",
					statusCode: {
						200: function(data) {
							$("#qrPanel").hide();
							$("#busytext").text("Decrypting Data…");
							$(".modal").show();
							decryptData(data);
						},
						204: function() {
							pollForSecret();
						},
					},
					error: function() {
						showError("Failed to update status from server (probably a timeour for security reasons)!");
					}
				});
	}, 10000);
}

function createQrCode(errorLevel, qrSize) {
	var sendKey = publicKey.replace(/\n/g, '');
	sendKey = sendKey.replace("-----BEGIN PUBLIC KEY-----", '');
	sendKey = sendKey.replace("-----END PUBLIC KEY-----", '');
	var senddata = "{\"Secret\":\"" + secretString + "\",\"PublicKey\":\"" + sendKey + "\"}";
	
	$('#qrcodeCanvas').qrcode({
		render: "canvas",
		width: qrSize,
		height: qrSize,
		correctLevel: errorLevel,
		text: senddata
	});
}

function generateKeys(keysize, errorLevel, qrSize) {
	$(".modal").show();
	var crypt = new JSEncrypt({default_key_size: keysize});
	crypt.getKey(function () {
		privateKey = crypt.getPrivateKey();
		publicKey = crypt.getPublicKey();
		
		$.ajax({
					url: "./transferEngine.php", 
					dataType: "json",
					data: {
						method: "REGISTER",
						secret: secretString
					},
					method: "POST",
					success:  function(data) {
						createQrCode(errorLevel, qrSize);
						$(".modal").hide();
						$("#qrPanel").show();
						pollForSecret();
					},
					error: function() {
						$(".modal").hide();
						showError("Failed to register on server!");
					}
				});
	});
};
		
function show(name) {
	$("#" + name).attr("type", "text");
}

function hide(name) {
	$("#" + name).attr("type", "password");
}

function togglePw(name) {
	if (editState[name] == 0) {
		editState[name] = 1;
		show(name);
	} else {
		editState[name] = 0;
		hide(name);
	}
}