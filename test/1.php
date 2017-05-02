<?php

  $pay_file_content = @file_get_contents('point.csv');
	$arr_pay_file = explode("\n",$pay_file_content);
	$arr_line = array();
	$con = new mysqli("localhost","root","wd123456","points");
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
		$sql = "insert into point (address,lat,lng,city_id,display,create_time,update_time) values ('".$co[0]."',"."$co[1],$co[2],$co[3],$co[4],$time,$time)";
		$ret = $con->query($sql);
		} catch (Exception $e) {
			echo $e->getMessage();
		}
    //var_dump($ret);
	}	


