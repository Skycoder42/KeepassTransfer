function Parameters(){}

Parameters.urlParams = {};

Parameters.extractQueryParameters = function() {	
    var match,
        pl     = /\+/g,  // Regex for replacing addition symbol with a space
        search = /([^&=]+)=?([^&]*)/g,
        decode = function (s) { return decodeURIComponent(s.replace(pl, " ")); },
        query  = window.location.search.substring(1);

    Parameters.urlParams = {};
    while (match = search.exec(query))
       Parameters.urlParams[decode(match[1])] = decode(match[2]);
}

Parameters.getKeySize = function() {
	var value = Parameters.urlParams["keySize"];
	var allowed = ["512", "1024", "2048", "4096", "8192"];
	var index = allowed.indexOf(value);
	if(index < 0)
		return allowed[2];
	else
		return value;
}

Parameters.getErrorLevel = function() {	
	switch(Parameters.urlParams["errorLevel"]) {
    case "L":
		return 1;
    case "M":
    default:
		return 0;
    case "Q":
		return 3;
    case "H":
		return 2;
	}
}

Parameters.getQrSize = function() {
	var num = parseInt(Parameters.urlParams["qrSize"]);
	if(isNaN(num))
		return 512;
	else
		return Math.min(Math.max(Parameters.urlParams["qrSize"], 256), 960);
}