<?php include 'inc.php'; ?>
<?php
header("Cache-Control: no-cache");
header("Pragma: no-cache");
header("Content-type: text/json");
$field = preg_split("/——/", $randString);
$obj = array('hitokoto' => $field[0], 'source' => $field[1]);
echo json_encode($obj);
?>
