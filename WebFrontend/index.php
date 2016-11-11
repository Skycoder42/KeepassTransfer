<?php
function getParam($value, $allowed, $defaultIndex) {
	if(isset($value) &&
	   in_array($value, $allowed))
		return $value;
	else
		return $allowed[$defaultIndex];
}

function getKeySize() {
	return getParam($_GET["keySize"], ["512", "1024", "2048", "4096", "8192"], 2);
}

function getErrorLevel() {
	return getParam($_GET["errorLevel"], ["L", "M", "Q", "H"], 0);
}

function getQrSize() {
	if(isset($_GET["qrSize"])) {
		$size = intval($_GET["qrSize"]);
		return min(max($size, 256), 960);
	} else 
		return 512;
}
?>
<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>Keepass Transfer</title>
	<link rel="shortcut icon" href="./launcher_ic.png" />
	
	<?php require_once("./libraries/jquery.php"); ?>
	<script type="text/javascript" src="./jsencrypt.js"></script>
	<script type="text/javascript" src="./qrcode.js"></script>
	<script type="text/javascript" src="./jquery.qrcode.js"></script>
	<script type="text/javascript" src="./libraries/clipboard.min.js"></script>
	<script type="text/javascript" src="./core.js"></script>
	
	<?php require_once("./libraries/bootstrap.php"); ?>
	<link rel="stylesheet" type="text/css" href="./styles.css"></link>

	<script type="text/javascript">
		$(function() {
			setSecret("<?php echo dechex(mt_rand()).dechex(mt_rand())?>");
			generateKeys(<?php echo getKeySize(); ?>, QRErrorCorrectLevel.<?php echo getErrorLevel(); ?>, <?php echo getQrSize(); ?>);
		});
	</script>
</head>
<body>
	<?php $active_item = 2; include('./libraries/navbar.php'); ?>
	
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
</body>
</html>
