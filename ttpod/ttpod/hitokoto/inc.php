<?php
$filename = "hitokoto.txt";
$handle = fopen($filename, "r");
$arr = array();
while (!feof($handle)) {
    $arr[] = fgets($handle, 1024);
}
$randString = $arr[array_rand($arr)];
fclose($handle);
?>