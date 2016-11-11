<?php
$basePath = "/tmp/keepass_transfer/";
if (!is_dir($basePath) && !mkdir($basePath, 0700, true)) {
	http_response_code(500);
	exit;
}

if(!isset($_POST["secret"])) {
	echo "400";
	http_response_code(400);
	exit;
}	

if(file_put_contents($basePath.$_POST["secret"].".tcd", $_POST["data"], LOCK_EX) == false)
	http_response_code(500);
else
	http_response_code(204);
?>