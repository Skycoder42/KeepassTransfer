<?php
if(session_start()) {
	switch($_POST["method"]) {
	case "REGISTER":
		$_SESSION["secret"] = $_POST["secret"];
		$_SESSION["timout"] = time() + 180;
		http_response_code(204);
		break;
	case "UPDATE":
		if($_SESSION["secret"] == $_POST["secret"] &&
		   $_SESSION["timout"] >= time()) {
			$fileName = "/tmp/keepass_transfer/".$_SESSION["secret"].".tcd";
			if(file_exists($fileName)) {
				echo file_get_contents($fileName);
				unlink($fileName);
			} else
				http_response_code(204);
		} else
			http_response_code(410);
		break;
	default:
		http_response_code(400);
	}
	if(http_response_code() != 204) {
		unset($_SESSION);
		session_destroy();
	}		
} else 
	http_response_code(500);
?>