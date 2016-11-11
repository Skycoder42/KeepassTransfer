var urlParams;

function extractQueryParameters() {	
    var match,
        pl     = /\+/g,  // Regex for replacing addition symbol with a space
        search = /([^&=]+)=?([^&]*)/g,
        decode = function (s) { return decodeURIComponent(s.replace(pl, " ")); },
        query  = window.location.search.substring(1);

    urlParams = {};
    while (match = search.exec(query))
       urlParams[decode(match[1])] = decode(match[2]);
}

function getRealParam(value, allowed, defaultIndex) {
	var index = allowed.indexOf(value);
	if(index < 0)
		return allowed[defaultIndex];
	else
		return value;
}

function getKeySize() {
	return getRealParam(urlParams["keySize"], ["512", "1024", "2048", "4096", "8192"], 2);
}

function getErrorLevel() {
	return getRealParam(urlParams["errorLevel"], ["L", "M", "Q", "H"], 1);
}

function getQrSize() {
	var num = parseInt(urlParams["qrSize"]);
	if(isNaN(num))
		return 512;
	else
		return Math.min(Math.max(urlParams["qrSize"], 256), 960);
}