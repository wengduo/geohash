/*************************************************************************
    > File Name: point.cpp
    > Author: wengduo
    > Mail: 2292466904@qq.com
    > Created Time: 2017年05月01日 星期一 09时23分04秒
 ************************************************************************/

#include "point.h"

double Point::min(double a,double b) {
	if(a - b > 1e-6) {
		return b;
	}
	return a;
}
//在地图上，采用步行导航的距离是最好的，但是请求外部服务是非常耗时的所以此处不做处理。
int Point::getDistance(double fromLat,double fromLng,double toLat,double toLng) {
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
//算出四个最近的站点
std::vector<std::vector<std::string> > Point::popFourRecentPoint(double lat,double lng,std::vector<std::vector<std::string> > &points) {
	std::vector<std::vector<std::string> > data; 
	int n;
	int min;
	int toPointDistance;
	log_notice(__FILE__,__func__,__LINE__,"Total points",points);	
	for(int i = 0;i < pointNum;i++) {
		min = INT_MAX;
		n = INT_MAX;
		for(int j = 0;j < points.size();j++) {
			toPointDistance = getDistance(lat,lng,atof(points[j][2].c_str()),atof(points[j][3].c_str()));
			if(min > toPointDistance) {
				min = toPointDistance;
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

//得到缓存的key值
std::string Point::getPointsCacheKey(std::string &geohash) {
	std::string key = "pointIds1:";
	key += geohash;
	return key;
}

std::string Point::getStringValue(std::vector<std::vector<std::string> > &points) {
	std::string value;
	for(int i = 0;i < points.size();i++) {
		value += points[i][0];
		if(i != points.size() - 1) {
			value += ",";
		}
	}
	return value;
}

//得到反序列化的value值
std::vector<std::string> Point::getVectorValue(std::string &value) {
	char tmp[128] = {0};
	std::vector<std::string> res;
	strcpy(tmp,value.c_str());
	char *p = strtok(tmp,",");
	while(p != NULL) {
		res.push_back(p);
		p = strtok(NULL,",");
	}
	return res;
}

std::string Point::getPointInfoKey(std::string &pointId) {
	std::string key = "pointInfo1:";
	key += pointId;
	return key;
}

std::string Point::getStringPointInfoValue(std::vector<std::string> &pointInfo) {
	std::string value;
	for(int i = 0;i < pointInfo.size();i++) {
		value += pointInfo[i];
		value += ",";
	}
	return value;
}

//设置站点详情缓存
std::vector<std::string> Point::getPointInfoById(std::string &pointId) {
	std::string key = getPointInfoKey(pointId);
	log_notice(__FILE__,__func__,__LINE__,"get redis key",key);
	std::string strPointInfo = redis.get(key);
	std::vector<std::string> pointInfo = getVectorValue(strPointInfo);
	log_notice(__FILE__,__func__,__LINE__,"get redis pointInfo",pointInfo);
	//如果从缓存拿不到就从数据库拿
	if(pointInfo.size() == 0) {
		char sql[128] = {0};
		std::vector<std::vector<std::string> > data;
		sprintf(sql,"select * from point where id = %s",pointId.c_str());
		bool selectRes = sqlPoint.select(sql,data);
		if(selectRes) {
			log_notice(__FILE__,__func__,__LINE__,"select success",sql);
		} else {
			log_error(__FILE__,__func__,__LINE__,"select failure",sql);
		}
		pointInfo = data[0];
		std::string value = getStringPointInfoValue(pointInfo);
		log_notice(__FILE__,__func__,__LINE__,"set redis pointInfo",value);
		redis.set(key,value);
	}
	return pointInfo;
}

//得到推荐的站点
std::vector<std::vector<std::string> > Point::getRecentPoint(double lat,double lng) {
	std::vector<std::vector<std::string> > data;
	std::vector<std::vector<std::string> > points;

	std::vector<std::string> geohashs = geo.standardizeParameters(lat,lng);
	log_notice(__FILE__,__func__,__LINE__,"geohash值",geohashs);
	//此处可以考虑加入缓存， 可以加快查询速度
	std::string key = getPointsCacheKey(geohashs[0]);
	std::string value = redis.get(key);
	log_notice(__FILE__,__func__,__LINE__,"get redis pointIds",value);
	//看缓存中是否有，如果没有的话，就从数据库中再算一遍			
	if(value.length() > 0) {
		std::vector<std::string> pointIds = getVectorValue(value);
		for(int i = 0;i < pointIds.size();i++) {
			points.push_back(getPointInfoById(pointIds[i]));
		}

	} else {
		char sql[128] = {0};
		char geohash[128] = {0};
		for(int i = 0;i < geohashs.size();i++) {
			std::vector<std::vector<std::string> > oneGeohashData;
			strncpy(geohash,geohashs[i].c_str(),6);
			sprintf(sql,"select id,address,lat,lng from point where geohash like '%s%%' and display = 1",geohash);
			bool selectRes = sqlPoint.select(sql,oneGeohashData);
			if(selectRes) {
				log_notice(__FILE__,__func__,__LINE__,"select success",sql);
			} else {
				log_error(__FILE__,__func__,__LINE__,"select failure",sql);
			}
			for(int j = 0;j < oneGeohashData.size();j++) {
				int tmp = getDistance(lat,lng,atof(oneGeohashData[j][2].c_str()),atof(oneGeohashData[j][3].c_str()));
				if(tmp < 1000) {
					data.push_back(oneGeohashData[j]); 
				}
			}
		}
		points = popFourRecentPoint(lat,lng,data);
		log_notice(__FILE__,__func__,__LINE__,"points",value);
		std::string value = getStringValue(points);
		log_notice(__FILE__,__func__,__LINE__,"set redis pointIds",value);
		redis.set(key,value);
	}
	return points;
}


