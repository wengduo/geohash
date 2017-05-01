/*************************************************************************
    > File Name: redis.cpp
    > Author: wengduo
    > Mail: 2292466904@qq.com
    > Created Time: 2017年05月01日 星期一 08时10分17秒
 ************************************************************************/

#include "redis.h"

Redis::Redis() {
	this->redis = redisConnect(redisIp,redisPort);
	if(this->redis != NULL && this->redis->err) {
		log_performance(__FILE__,__func__,__LINE__,"redis connect failure",this->redis->err);
	} else {
		log_performance(__FILE__,__func__,__LINE__,"redis connect success",this->redis->err);
	}
}

std::string Redis::get(std::string key) {
	std::string logData = "GET "+ key;
	std::string res;
	this->result = (redisReply *)redisCommand(this->redis,"GET %s",key.c_str());
	if(this->result != NULL && this->result->str != 0) {
		log_performance(__FILE__,__func__,__LINE__,"redis get success",logData);
		res = this->result->str;
		freeReplyObject(this->result);
	} else {
		log_performance(__FILE__,__func__,__LINE__,"redis get failure",logData);
	}
	return res;
}

void Redis::set(std::string key,std::string value,long cacheTime) {
	std::string logData = "SET "+ key + " " + value;
	log_performance(__FILE__,__func__,__LINE__,"redis set",logData);
	//设置一个k值
	redisCommand(this->redis,"SET %s %s",key.c_str(),value.c_str());   
	//设置过期时间
	log_performance(__FILE__,__func__,__LINE__,"redis set cacheTime",cacheTime);
	redisCommand(this->redis,"EXPIREAT %s %d",key.c_str(),time(NULL)+cacheTime);
}

void Redis::hset(std::string key,std::string field,std::string value,long cacheTime) {
  
   	std::string logData = "HSET "+ key + " " + field + " " + value;
		log_performance(__FILE__,__func__,__LINE__,"redis hset",logData);

		//
    redisCommand(this->redis,"HSET %s %s %s",key.c_str(),field.c_str(),value.c_str());
	  //设置过期时间
		log_performance(__FILE__,__func__,__LINE__,"redis hset cacheTime",cacheTime);
		redisCommand(this->redis,"EXPIREAT %s %d",key.c_str(),time(NULL)+cacheTime);

}

std::vector<std::string> Redis::hGetAll(std::string key) {
	std::string logData = "HGETALL " + key;
	std::vector<std::string> res;
	this->result = (redisReply *)redisCommand(this->redis,"HGETALL %s",key.c_str());
	if(this->result != NULL && this->result->elements != 0) {
		for(int i = 0;i < this->result->elements;i++) {
			res.push_back(this->result->element[i]->str);
		}
		log_performance(__FILE__,__func__,__LINE__,"redis hgetall success",logData);
		freeReplyObject(this->result);
	} else {
		log_performance(__FILE__,__func__,__LINE__,"redis hgetall failure",logData);
	}
	return res;

}

void Redis::lPush(std::string queueName,std::string value) {

	this->result = (redisReply *)redisCommand(this->redis,"LPUSH %s %s",queueName.c_str(),value.c_str());

}

std::string Redis::lPop(std::string queueName) {
  std::string tmp;
	this->result = (redisReply *)redisCommand(this->redis,"LPOP %s",queueName.c_str());
	return tmp;
}

  
