/*************************************************************************
    > File Name: geohash.h
    > Author: wengduo
    > Mail: 2292466904@qq.com
    > Created Time: 2017年03月22日 星期三 20时28分03秒
 ************************************************************************/
#include "redis.h"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

class Geohash:public Log {
private:
public:
	Geohash();
	//传入经纬度，返回geohash字段
	std::vector<std::string> standardizeParameters(double lat,double lng);

private:
		std::string getGeohashKey(std::string &geohash);

		//得到序列化的value值
		std::string getStringValue(std::vector<std::string> &geohash);

		//得到反序列化的value值
		std::vector<std::string> getVectorValue(std::string value,std::string &geohash);

		std::vector<std::string> getAroundSquares(std::string geohash,char *bflat,char *bflng);	
		//二进制进行加1,减1的操作
		std::string calculate(char *corrdinate,int n);
		//十进制转二进制
		std::string decbin(int n);
		//二进制数转十进制
		int bindec(const char *binary);
		//模拟php的函数去实现的，支持从begin位置开始拷贝n个字符，到指定空间
		std::string substr(const char *str,int begin,int n,char *tmp);
		//将经纬度字符串转成geohash字符串
		std::string encode(const char *bflat,const char *bflng);
		//将经纬度专成二进制
		void binEncode(char *str,double number,double left, double right,int bitCount);
		//得到某个字符在字符串中的位置
		int strpos(const char *str,const char c);

		double precision(char *str);

		int max(int a,int b);

		int compre(double a,double b);

		//得到经纬度的二进制
		void binCode(char *cLat,char *cLng,double lat,double lng,char *bflat,char *bflng);

	private:
		char coding[33];
		Redis redis;
};
