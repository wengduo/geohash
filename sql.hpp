#include <mysql/mysql.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <cstring>


class Mysql {
private:
	std::string host = "localhost";
	const int port = 3306;
	std::string userName = "root";
	std::string passwd = "wd123456";
	std::string dbName = "point"
public:
	Mysql(std::string dbName) {
    mysql = new MYSQL;
		result = NULL;
		mysql_init(mysql);
		if(!mysql_real_connect(mysql,this->host.c_str(),this->userName.c_str()
					,this->passwd.c_str(),this->dbName.c_str(),this->port,NULL,0)) {
      std::cout << "ral_connect fail" << mysql_error(mysql) << std::endl;
		} else {
      std::cout << "connect with server succeed" << std::endl; 
		}
	}
  
	bool query(std::string str) {
    bool flag = true;
		if(mysql_real_query(mysql,str.c_str(),str.size()) {
			flag = false;
      std::cout << mysql_error(mysql) << std::endl;
		}
		return false;
	}
private:
  void freeResult() {
	  mysql_free_result(this->result);
		while(!mysql_next_result(mysql)) {
		  result = mysql_store_result(mysql);
			mysql_free_result(result);
		}
	}

	~Mysql() {
	  mysql_close(mysql);
	}
private:
  std::string vec;
	MYSQL *mysql;
	MYSQL_RES *result;
	MYSQL_FIELD *fd;
	MYSQL_ROW row;

};
