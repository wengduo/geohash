/*************************************************************************
    > File Name: point.cpp
    > Author: wengduo
    > Mail: 2292466904@qq.com
    > Created Time: 2017年04月10日 星期一 18时13分45秒
 ************************************************************************/
#include "geohash.h"
#include <iostream>
#include <cstdio>
#include <cassert>
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <sys/time.h>

class Point :public Log {
	private:
		//double类型的数据取出最小值
		double min(double a,double b);
		//在地图上，采用步行导航的距离是最好的，但是请求外部服务是非常耗时的所以此处不做处理。
		int getDistance(double fromLat,double fromLng,double toLat,double toLng);
		//算出四个最近的站点
		std::vector<std::vector<std::string> > popFourRecentPoint(double lat,double lng,std::vector<std::vector<std::string> > &points);
		//得到缓存的key值
		std::string getPointsCacheKey(std::string &geohash);

		std::string getStringValue(std::vector<std::vector<std::string> > &points);

    //得到反序列化的value值
		std::vector<std::string> getVectorValue(std::string &value);

		std::string getPointInfoKey(std::string &pointId);

		std::string getStringPointInfoValue(std::vector<std::string> &pointInfo);
		
		//设置站点详情缓存
		std::vector<std::string> getPointInfoById(std::string &pointId);
	
	public:
		Point() {
		}

		//得到推荐的站点
		std::vector<std::vector<std::string> > getRecentPoint(double lat,double lng);

	private:
		const std::string dbName = "points";//存放站点信息的库名
		Mysql sqlPoint;
		Redis redis; 
		Geohash geo;
		const int pointNum = 4;//配置推荐的站点数
};
