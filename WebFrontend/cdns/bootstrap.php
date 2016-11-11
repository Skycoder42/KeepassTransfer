<?php require_once(dirname(__FILE__)."/jquery.php"); ?>

<!-- Bootstap CDN -->
<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js" integrity="sha384-0mSbJDEHialfmuBBQP6A4Qrprq5OVfW37PRR3j5ELqxss1yVqOtnepnHVP9aJ7xS" crossorigin="anonymous"></script>	
<link href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7" crossorigin="anonymous">

<!-- Bootstrap JS local fallback -->
<script>if(typeof($.fn.modal) === 'undefined') {document.write('<script src="/libraries/bootstrap.min.js"><\/script>')}</script>
<!-- Bootstrap CSS local fallback -->
<script>
$(document).ready(function() {
	var bodyColor = $('body').css('color');
	if(bodyColor != 'rgb(51, 51, 51)') {
		$("head").prepend('<link rel="stylesheet" href="/libraries/bootstrap.min.css">');
	}
});
</script>

<!-- Bootstap Dialogs CDN -->
<script src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap3-dialog/1.34.7/js/bootstrap-dialog.min.js" integrity="sha384-A1l7AkamUsoFMYzhowbdCHQanwuyYkuwH5lCZr7HrtF++vpLSuMWRoQwFaC3d3dg" crossorigin="anonymous"></script>
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/bootstrap3-dialog/1.34.7/css/bootstrap-dialog.min.css" integrity="sha384-Je6/yfVqKlz/140LeySLv95dqJvjJkRTekqU15aZ9XJF5hRATXzV0sBgDZ2+lWyK" crossorigin="anonymous">