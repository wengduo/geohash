/*************************************************************************
    > File Name: point.cpp
    > Author: wengduo
    > Mail: 2292466904@qq.com
    > Created Time: 2017年04月10日 星期一 18时13分45秒
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cassert>
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <climits>
#include "sql.hpp"
#include "geohash.hpp"


class Point {
	private:
		//double类型的数据取出最小值
		double min(double a,double b) {
			if(a - b > 1e-6) {
				return b;
			}
			return a;
		}
		//在地图上，采用步行导航的距离是最好的，但是请求外部服务是非常耗时的所以此处不做处理。
		int getDistance(double fromLat,double fromLng,double toLat,double toLng) {
			int earthRadius = 6378137;

			fromLat = (fromLat*M_PI)/180;
			fromLng = (fromLng*M_PI)/180;

			toLat = (toLat*M_PI)/180;
			toLng = (toLng*M_PI)/180;

			double calcLat = fromLat - toLat;
			double calcLng = fromLng - toLng;

			double stepOne = pow(sin(calcLat/2),2) + cos(toLat)*cos(fromLat)*pow(sin(calcLng/2),2);
			double stepTwo = 2*asin(min(1.0,sqrt(stepOne)));
			return (int)round(earthRadius*stepTwo);
		}
	public:
		Point() {
		}

		//算出四个最近的站点
		std::vector<std::vector<std::string>> popFourRecentPoint(double lat,double lng,std::vector<std::vector<std::string>> &points) {
			std::vector<std::vector<std::string>> data; 
			int n;
			int min;
			int tmp;
			for(int i = 0;i < pointNum;i++) {
				min = INT_MAX;
				n = INT_MAX;
				for(int j = 0;j < points.size();j++) {
					int tmp = getDistance(lat,lng,atof(points[j][2].c_str()),atof(points[j][3].c_str()));
					if(min > tmp) {
						min = tmp;
						n = j;
					}
				}
				if(n >= 0 && n < points.size()) { 
					data.push_back(points[n]);
					points.erase(points.begin()+n);
				}
			}

			return data;
		}

		//得到推荐的站点
		std::vector<std::vector<std::string>> getRecentPoint(double lat,double lng) {
			std::vector<std::vector<std::string>> data;
			std::vector<std::string> geohashs = geo.standardizeParameters(lat,lng);
			char sql[128] = {0};
			char geohash[128] = {0};
			for(int i = 0;i < geohashs.size();i++) {
				std::vector<std::vector<std::string>> oneGeohashData;
				strncpy(geohash,geohashs[i].c_str(),6);
				sprintf(sql,"select id,address,lat,lng from point where geohash like '%s%%' and display = 1",geohash);
				sqlPoint.select(sql,oneGeohashData);
				for(int j = 0;j < oneGeohashData.size();j++) {
					int tmp = getDistance(lat,lng,atof(oneGeohashData[j][2].c_str()),atof(oneGeohashData[j][3].c_str()));
					if(tmp < 1000) {
						data.push_back(oneGeohashData[j]); 
					}
				}
			}
			return popFourRecentPoint(lat,lng,data);
		}

	private:
		const std::string dbName = "points";//存放站点信息的库名
		Mysql sqlPoint;
		Geohash geo;
		const int pointNum = 4;//配置推荐的站点数
};

int main(int argc,char *argv[]) {
	std::string dbName = "sofa_order";
	Mysql order(dbName);
	Point point;
	char sqlHead[128] = {0};
	for(int i = 0;i < 131;i++) {
		std::vector<std::vector<std::string>> data;
		sprintf(sqlHead,"select * from order_list_%d",i);
		order.select(sqlHead,data);	
		for(int j = 0;j < data.size();j++) {
			std::vector<std::vector<std::string>> tmp = point.getRecentPoint(atof(data[j][1].c_str()),atof(data[j][2].c_str()));
			//std::vector<std::vector<std::string>> tmp = point.getRecentPoint(35.04449,11.28414);
			for(int k = 0;k < tmp.size();k++) {
				std::cout << tmp[k][0] << std::endl;
				std::cout << tmp[k][1] << std::endl;
				std::cout << tmp[k][2] << std::endl;
				std::cout << tmp[k][3] << std::endl;
			}
			exit(1);
		}
		exit(1);
	}
	return 0;
}
