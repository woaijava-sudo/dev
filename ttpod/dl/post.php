<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb18030" />
<meta http-equiv="Content-Language" content="zh-cn" />

<meta name="robots" content="no" />

<meta name="Copyright" content="Copyright Bajdcc All Rights Reserved." />
<meta name="MSSmartTagsPreventParsing" content="true" />

<link rel="shortcut icon" href="/favicon.ico" type="image/x-icon" />
<title>离线下载</title>

</head>

<body>

	<?php
		$url = $_REQUEST['url'];
		$name = $_REQUEST['name'];
		$k = 'python dl.py "'.$url.'" "'.$name.'"';
		system($k);
	?>
	<p>命令已提交</p>
	<p><?php echo $k; ?></p>

</body>

</html>