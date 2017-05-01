/*************************************************************************
    > File Name: mysql.cpp
    > Author: wengduo
    > Mail: 2292466904@qq.com
    > Created Time: 2017年04月19日 星期三 08时02分38秒
 ************************************************************************/

#include "mysql.h"

Mysql::Mysql(std::string dbName) {
	result = NULL;
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql,this->host.c_str(),this->userName.c_str()
				,this->passwd.c_str(),dbName.c_str(),this->port,NULL,0)) {
		log_performance(__FILE__,__func__,__LINE__,"mysql_connect fail",mysql_error(&mysql));
	} else {
		log_performance(__FILE__,__func__,__LINE__,"mysql_connect success","");
	}
	mysql_real_query(&mysql,"SET NAMES UTF8;",15);
}

Mysql Mysql::operator=(const Mysql &data) {
  return data;
}

Mysql::~Mysql() {
	mysql_close(&mysql);
}


void Mysql::freeResult() {
  mysql_free_result(this->result);
	while(!mysql_next_result(&mysql)) {
		result = mysql_store_result(&mysql);
		mysql_free_result(result);
	}
}

bool Mysql::select(std::string queryStr,std::vector<std::vector<std::string>> &data) {
	if(!query(queryStr)) {
		return false;
	}
	result = mysql_store_result(&mysql);

	int row = mysql_num_rows(result);
	int field = mysql_num_fields(result);

	MYSQL_ROW line = NULL;
	line = mysql_fetch_row(result);
	std::string temp;

	while(NULL != line) {
		std::vector<std::string> lineData;
		for(int i = 0;i < field;i++) {
			if(line[i]) {
				temp = line[i];
				lineData.push_back(temp);
			} else {
				temp = "";
				lineData.push_back(temp);
			}

		}
		line = mysql_fetch_row(result);
		data.push_back(lineData);
	}
	return true;
}

bool Mysql::query(std::string &str) {
	log_performance(__FILE__,__func__,__LINE__,"mysql command",str);
	bool flag = true;
	if(mysql_query(&mysql,str.c_str())) {
		flag = false;
		log_performance(__FILE__,__func__,__LINE__,"mysql command fail",mysql_error(&mysql));
	}
	return flag;
}

