<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>Keepass Transfer</title>
	<link rel="shortcut icon" href="./launcher_ic.png" />
	
	<!-- jQuery CDN -->
	<script src="https://code.jquery.com/jquery-2.2.4.min.js" integrity="sha256-BbhdlvQf/xTY9gja0Dq3HiwQF8LaCRTXxZKRutelT44=" crossorigin="anonymous"></script>
	<!-- jQuery local fallback -->
	<script>window.jQuery || document.write('<script src="./cdns/jquery.min.js"><\/script>')</script>
	
	<!-- Bootstap CDN -->
	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js" integrity="sha384-Tc5IQib027qvyjSMfHjOMaLkfuWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa" crossorigin="anonymous"></script>
	<link href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u" crossorigin="anonymous">
	
	<!-- Bootstrap JS local fallback -->
	<script>if(typeof($.fn.modal) === 'undefined') {document.write('<script src="./cdns/bootstrap.min.js"><\/script>')}</script>
	<!-- Bootstrap CSS local fallback -->
	<script>
	$(document).ready(function() {
		var bodyColor = $('body').css('color');
		if(bodyColor != 'rgb(51, 51, 51)') {
			$("head").prepend('<link rel="stylesheet" href="/cdns/bootstrap.min.css">');
		}
	});
	</script>
	
	<!-- Bootstap Dialogs CDN -->
	<script src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap3-dialog/1.35.3/js/bootstrap-dialog.min.js" integrity="sha256-8qVHcBsZcW0BFgt8DWh3qVJeci+3rhb4eqtzKAzqb7o=" crossorigin="anonymous"></script>
	<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/bootstrap3-dialog/1.35.3/css/bootstrap-dialog.min.css" integrity="sha256-wstTM1F5dOf7cgnlRHIW3bmoRAAGh6jL7tMIvqTuFZE=" crossorigin="anonymous" />
	
	<!-- Bootstrap Dialogs JS local fallback -> do both -->
	<script>
	if(typeof(BootstrapDialog.TYPE_DANGER) === 'undefined') {
		document.write('<script src="./cdns/bootstrap-dialog.min.js"><\/script>');
		document.write('<link rel="stylesheet" href="/cdns/bootstrap-dialog.min.css">');
	}
	</script>
	
	<link rel="stylesheet" type="text/css" href="./styles.css"></link>
</head>
<body>	
	<div>
		<div class="panel panel-info" id="qrPanel" style="display: none">
			<div class="panel-heading">
				<h3 class="panel-title">QR-Code</h3>
			</div>
			<div class="panel-body">
				<div class="qrcenter">
					<div class="qrcontainer" id="qrcodeCanvas"></div>
				</div>
				<br/>Just scan this code with the Keepass Transfer Plugin and your data will be transfered here.
			</div>
		</div>
	</div>
	
	<div>
		<div class="panel panel-info" id="entryPanel" style="display: none">
			<div class="panel-heading">
				<h3 class="panel-title" id="entryLegend">Password Entry</h3>
			</div>
			<div class="panel-body">
				<table class="table table-borderless" id="entryTable">
				</table>
			</div>
		</div>
	</div>
	
	<div>
		<div class="panel panel-primary">
			<div class="panel-heading">
				<h3 class="panel-title">Usage</h3>
			</div>
			<div class="panel-body">
				<p>The usage is pretty straight-foreward. Open the website, wait for the QR-Code, scan it in the app and wait for the result to be shown.<br/>
				However, in some cases you have to customize how the QR-Code is generated. For now, there are 2 Parameters that allow you to configure the code:</p>
				<ul class="list-group">
					<li class="list-group-item">
						<h4 class="list-group-item-heading">keySize</h4>
						<p class="list-group-item-text">The keySize parameter can be used to change the size of the generateded key. The bigger the keySize, the more secure
						it is. In addition to that, the maximum size of the data to encrypt increases. However, if you choose a bigger key, the generation will take much longer.<br>
						The following sizes are supported: <b>[512, 1024, 2048, 4096, 8192]</b>. The default
						size (if you leave out the parameter) is <u>2048</u>.</p>
					</li>
					<li class="list-group-item">
						<h4 class="list-group-item-heading">errorLevel</h4>
						<p class="list-group-item-text">The errorLevel specifies how much error-correction should be used. The higher the error correction, the more damaged the
						QR-Code can be, without beeing a problem for readers. More information about the levels can be found at <a href="http://blog.qrstuff.com/2011/12/14/qr-code-error-correction">QRStuff.com</a>.<br/>
						The following levels are supported: <b>[L, M, Q, H]</b>. Typically, a QR-Code on the screen is not damaged. To reduce the complexity of the code, the default error correction
						level is <u>L</u>.</p>
					</li>
					<li class="list-group-item">
						<h4 class="list-group-item-heading">qrSize</h4>
						<p class="list-group-item-text">The qrSize parameter allows you to adjust the size of the generated QR-Code. it specifies the height and width in pixels.
						Depending on the keySize and errorLevel, as well as the screen-size, you may have to adjust this parameter to get a readable QR-Code.<br/>
						All sizes from <b>256 to 960</b> are supported. If you enter a lower or higher value, the closest possible one will be choosen. The default size are <u>512</u> pixels.</p>
					</li>
				</ul>
				<p>How to apply these parameters? Just add them as get-parameters to the url! The link below provides a simple example:<br/>
				<a href="./index.php?keySize=1024&errorLevel=H&qrSize=420">.../index.php?<b>keySize=1024</b>&amp;<b>errorLevel=H</b>&amp;<b>qrSize=420</b></a></p>
			</div>
		</div>
	</div>
	
	<!-- A window-modal busy-indicator -->
	<div class="modal" style="display: none">
		<div class="indicator">
			<img alt="Please wait, key is beeing generated" src="./loader.gif" />
			<div class="centertext" id="busytext">Generating Key&#8230;</div>
		</div>
	</div>
	
	<!-- Put Logical scripts at the end -->
	<!-- JS Encrypt CDN -->
	<script src="https://cdnjs.cloudflare.com/ajax/libs/jsencrypt/2.3.1/jsencrypt.min.js" integrity="sha256-WgvkBqG9+UolqdFC1BJOPcy961WTzXj7C9I034ndc4k=" crossorigin="anonymous"></script>
	<!-- JS Encrypt CDN local fallback
	<script>if(typeof(bnpCopyTo) === 'undefined') {document.write('<script src="./cdns/jsencrypt.min.js"><\/script>')}</script> -->
	<!-- jQuery qrcode CDN -->
	<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery.qrcode/1.0/jquery.qrcode.min.js" integrity="sha256-9MzwK2kJKBmsJFdccXoIDDtsbWFh8bjYK/C7UjB1Ay0=" crossorigin="anonymous"></script>
	<!-- jQuery qrcode CDN local fallback
	<script type="text/javascript" src="./libraries/qrcode.js"></script>
	<script type="text/javascript" src="./libraries/jquery.qrcode.js"></script> -->
	<!-- clipboard CDN -->
	<script src="https://cdnjs.cloudflare.com/ajax/libs/clipboard.js/1.5.15/clipboard.min.js" integrity="sha256-COWXDc7n7PAqsE3y1r4CVopxWU9JI0kenz6K4zBqhT8=" crossorigin="anonymous"></script>
	<!-- clipboard CDN local fallback
	<script type="text/javascript" src="./libraries/clipboard.min.js"></script>-->

	<script type="text/javascript" src="./parameters.js"></script>
	<script type="text/javascript" src="./core.js"></script>
	
	<script type="text/javascript">
		$(function() {
			extractQueryParameters();
		});
	</script>
</body>
</html>
