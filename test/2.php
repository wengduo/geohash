<?php

for($i = 120;$i < 131;$i++) {
	$pay_file_content = @file_get_contents("or/order_list_$i.csv");
	$arr_pay_file = explode("\n",$pay_file_content);
	$arr_line = array();
	$con = new mysqli("localhost","root","wd123456","sofa_order");
	if(!$con) {
		die('Could not connect: '.mysql_error());
	}
	foreach($arr_pay_file as $k => $v) {
		$line = trim($v);
		if(empty($line)) {
			continue;
		}
		$co = explode(',',$line);
		$time = time();
		try {
			$con->set_charset('utf8');
			$sql = "insert into order_list_$i (order_id,lat,lng) values ('".$co[0]."',"."$co[1],$co[2])";
			$ret = $con->query($sql);
		} catch (Exception $e) {
			echo $e->getMessage();
		}
	}
 var_dump($i);	
}


