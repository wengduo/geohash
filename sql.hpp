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
	//std::string dbName = "point";
public:
	Mysql(std::string dbName = "points") {
    //mysql = new MYSQL;
		result = NULL;
		mysql_init(&mysql);
		if(!mysql_real_connect(&mysql,this->host.c_str(),this->userName.c_str()
					,this->passwd.c_str(),dbName.c_str(),this->port,NULL,0)) {
      std::cout << "ral_connect fail" << mysql_error(&mysql) << std::endl;
		} else {
      std::cout << "connect with server succeed" << std::endl; 
		}
		mysql_real_query(&mysql,"SET NAMES UTF8;",15);
  }

	Mysql operator=(const Mysql &data) {
      return data;
	}
	bool query(std::string str) {
		std::cout << str << std::endl;
    bool flag = true;
		if(mysql_query(&mysql,str.c_str())) {
			flag = false;
      std::cout << mysql_error(&mysql) << std::endl;
		}
		return false;
	}

	bool select(const std::string &queryStr,std::vector<std::vector<std::string>> &data) {
      if(0 != mysql_query(&mysql,queryStr.c_str())) {
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
	
	~Mysql() {
	  mysql_close(&mysql);
	}
private:
  void freeResult() {
	  mysql_free_result(this->result);
		while(!mysql_next_result(&mysql)) {
		  result = mysql_store_result(&mysql);
			mysql_free_result(result);
		}
	}

	
private:
  std::string vec;
	MYSQL mysql;
	MYSQL_RES *result;
	MYSQL_FIELD *fd;
	MYSQL_ROW row;

};


/*int main() {
    std::string dbName = "points";
    Mysql dd(dbName);
		char buff[1024] = {0};
		for(int i = 0;i < 131;i++) {
        sprintf(buff,"create table order_list_%d (order_id varchar(64) collate utf8_bin not null default '' comment '订单id',lat double unsigned not null default '0' comment '发单时的纬度',lng double unsigned not null default '0' comment '发单时的经度',primary key (order_id))engine=InnoDB default charset=utf8 collate=utf8_bin",i);
			  dd.query(buff);
		}
    std::vector<std::vector<std::string>> data;
		std::string bu = "select lat,lng from point";
		dd.select(bu,data);

    printf("%d\n",(int)data.size());
    printf("%d\n",(int)data[1].size());
    for(int i = 0;i < data.size();i++) {
        for(int j = 0;j < data[i].size();j++) {
					std::cout << data[i][j] << std::endl;
				}
				printf("\n");
		}

    return 0;
}*/
