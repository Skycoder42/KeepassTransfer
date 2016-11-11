<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>TEST</title>
	
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.js"></script>
	<script type="text/javascript" src="./jsencrypt.js"></script>
	<script type="text/javascript">
		function transferData() {			
			var crypt = new JSEncrypt();
			crypt.setPublicKey($('#pubkey').val());
			
			var transData = crypt.encrypt($('#data').val());
			
			$.ajax({
						url: "./secretPlacer.php", 
						dataType: "json",
						data: {
							secret: $('#sec').val(),
							data: transData
						},
						method: "POST",
						success:  function() {
							alert("TRANSFERED!!!");
						},
						error: function(jqXHR, textStatus, errorThrown) {
							showError("Failed to transfer!" + "\n" + textStatus + "\n" + errorThrown);
						}
					});
		}
	</script>
</head>
<body>
	Secret:
	<textarea id="sec" rows="1" style="width:100%" ></textarea>
	Public Key:
	<textarea id="pubkey" rows="13" style="width:100%"></textarea>
	Transfer Data:
	<textarea id="data" rows="13" style="width:100%"></textarea>
	<button onClick="transferData()">TRANSFER</button>
</body>
</html>
