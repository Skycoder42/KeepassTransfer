function Core(){}

Core.secret = null;
Core.publicKey = null;
Core.privateKey = null;
Core.connection = null;
Core.normalClosed = false;

Core.init = function() {
	Parameters.extractQueryParameters();
	GuiController.showProgress(true);
	
	Core.secret = Core.generateRandom(64);
	
	Core.connection = new WebSocket("ws://127.0.0.1:11221/backend/");//DEBUG, normally its: ("wss://kpt.skycoder42.de/backend/");
	Core.connection.onopen = Core.sendSecret;
	Core.connection.onmessage = Core.decryptData;
	Core.connection.onerror = Core.socketError;	
	Core.connection.onclose = Core.socketClosed;
	
	Core.generateKeys(Parameters.getKeySize(), Parameters.getErrorLevel(), Parameters.getQrSize());
}

Core.sendSecret = function() {
	Core.connection.send(JSON.stringify({
		"MessageType": "WC",
		"Secret": Core.secret,
	}));
}

Core.socketClosed = function(closeEvent) {
	console.log("Disconnected with close code: " + closeEvent.code)
	if(!Core.normalClosed)
		GuiController.showError("Connection to server was unexpectedly closed!");
}

Core.socketError = function(error) {
	GuiController.showError("Failed to communicate with backend! Some unknown error occured");
}

Core.generateKeys = function(keysize, errorLevel, qrSize) {	
	var crypt = new JSEncrypt({default_key_size: keysize});
	crypt.getKey(function () {
		Core.privateKey = crypt.getPrivateKey();
		Core.publicKey = crypt.getPublicKey();
		
		Core.createQrCode(errorLevel, qrSize);
		GuiController.showQrPanel();
		GuiController.showProgress(false);
	});
};

Core.generateRandom = function(count) {
    var text = "";
    var possible = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    for(var i = 0; i < count; i++) 
        text += possible.charAt(Math.floor(Math.random() * possible.length));

    return text;
}

Core.createQrCode = function(errorLevel, qrSize) {
	var sendKey = Core.publicKey.replace(/\n/g, '');
	sendKey = sendKey.replace("-----BEGIN PUBLIC KEY-----", '');
	sendKey = sendKey.replace("-----END PUBLIC KEY-----", '');
	var sendData = JSON.stringify({
		"Secret": Core.secret,
		"PublicKey": sendKey,
	});
	
	$('#qrcodeCanvas').qrcode({
		render: "canvas",
		width: qrSize,
		height: qrSize,
		correctLevel: errorLevel,
		text: sendData
	});
}

Core.decryptData = function(message) {
	GuiController.setProgressText("Decrypting Data�");
	GuiController.showProgress(true);
	
	var data = message.data;
	Core.normalClosed = true;
	Core.connection.close();
	
	var len = data.length;
	for(var i = 0; i < len; i++) {
		var cData = data[i];
		var crypt = new JSEncrypt();		
		crypt.setPrivateKey(privateKey);
		
		var decr = crypt.decrypt(cData.Value);
		GuiController.addEntry(cData.Key, decr, cData.Guarded, Core.generateRandom(10));
	}
	
	GuiController.showEntryPanel();
}