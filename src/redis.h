/*************************************************************************
    > File Name: redis.h
    > Author: wengduo
    > Mail: 2292466904@qq.com
    > Created Time: 2017年04月12日 星期三 15时08分51秒
 ************************************************************************/

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cassert>
#include <hiredis/hiredis.h>
#include <vector>
#include "mysql.h"

//redis配置ip和端口
#define redisIp "127.0.0.1"
#define redisPort 6379
#define defineCacheTime 10
class Redis:public Log {

public:
  Redis();
	//得到缓存的值
	std::string get(std::string key);
	//设置key值，默认缓存时间为10秒
	void set(std::string key,std::string value,long cacheTime = defineCacheTime) ;
	//redis的hashtable
	void hset(std::string key,std::string field,std::string value,long cacheTime = defineCacheTime);
	//得到缓存的值
	std::string hGet(std::string key,std::string field);
	//
	std::vector<std::string> hGetAll(std::string key);
	//redis队列生产日志
	void lPush(std::string queueName,std::string value);
	//redis队列消费日志
	std::string lPop(std::string queueName);

private:
  redisContext *redis;
	redisReply *result;
};

