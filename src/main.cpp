/*************************************************************************
    > File Name: main.cpp
    > Author: wengduo
    > Mail: 2292466904@qq.com
    > Created Time: 2017年05月01日 星期一 09时28分12秒
 ************************************************************************/

#include "point.h"

int main(int argc,char *argv[]) {
	std::string dbName = "sofa_order";
	Mysql order(dbName);
	Point point;
	struct timeval start,end;
	gettimeofday(&start,NULL);
	char sqlHead[128] = {0};
	for(int i = 0;i < 131;i++) {
		std::vector<std::vector<std::string> > data;
		sprintf(sqlHead,"select * from order_list_%d",i);
		order.select(sqlHead,data);	
		std::cout <<start.tv_sec*1000 + start.tv_usec/1000 << std::endl;
		std::cout << data.size() << std::endl;
		for(int j = 1000;j < data.size();j++) {
			std::vector<std::vector<std::string> > tmp = point.getRecentPoint(atof(data[j][1].c_str()),atof(data[j][2].c_str()));
			point.log_notice(__FILE__,__func__,__LINE__,"points",tmp);
			for(int k = 0;k < tmp.size();k++) {
				std::cout << tmp[k][0] << std::endl;
				std::cout << tmp[k][1] << std::endl;
				std::cout << tmp[k][2] << std::endl;
				std::cout << tmp[k][3] << std::endl;
			}
			gettimeofday(&end,NULL);
			std::cout <<end.tv_sec*1000 + end.tv_usec/1000<< std::endl;
			exit(1);
		}
		gettimeofday(&end,NULL);
		std::cout <<end.tv_sec*1000 +end.tv_usec/1000 << std::endl;
		exit(1);
	}

	return 0;
}
