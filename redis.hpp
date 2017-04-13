/*************************************************************************
    > File Name: redis.cpp
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

//redis配置ip和端口
#define redisIp "127.0.0.1"
#define redisPort 6379

class Redis {

public:
  Redis() {
    this->redis = redisConnect(redisIp,redisPort);
		if(this->redis != NULL && this->redis->err) {
      printf("connect error:%s\n",this->redis->errstr);
		}
	}

	//得到缓存的值
	std::string get(std::string key) {
		std::string res;
		this->result = (redisReply *)redisCommand(this->redis,"GET %s",key.c_str());
		if(this->result != NULL && this->result->str != 0) {
			res = this->result->str;
		  freeReplyObject(this->result);
		}
		return res;
	}

	//设置key值，默认缓存时间为10秒
	void set(std::string key,std::string value,long cacheTime = 10) {
		//设置一个k值
    redisCommand(this->redis,"SET %s %s",key.c_str(),value.c_str());   
		//设置过期时间
		redisCommand(this->redis,"EXPIREAT %s %d",key.c_str(),time(NULL)+cacheTime);
	}

	void hset(std::string key,std::string field,std::string value,long cacheTime = 10) {
    //
    redisCommand(this->redis,"HSET %s %s %s",key.c_str(),field.c_str(),value.c_str());
	  //设置过期时间
		redisCommand(this->redis,"EXPIREAT %s %d",key.c_str(),time(NULL)+cacheTime);
	}

	//得到缓存的值
	std::string hGet(std::string key,std::string field) {
		std::string res;
		this->result = (redisReply *)redisCommand(this->redis,"HGET %s %s",key.c_str(),field.c_str());
		if(this->result != NULL && this->result->str != 0) {
			res = this->result->str;
		  freeReplyObject(this->result);
		}
		return res;
	}

	//
	std::vector<std::string> hGetAll(std::string key) {
		std::vector<std::string> res;
    this->result = (redisReply *)redisCommand(this->redis,"HGETALL %s",key.c_str());
		if(this->result != NULL && this->result->elements != 0) {
      for(int i = 0;i < this->result->elements;i++) {
        res.push_back(this->result->element[i]->str);
			}
		  freeReplyObject(this->result);
		}
		return res;
	}
private:
  redisContext *redis;
	redisReply *result;
};

/*int main(int argc,char *argv[])
{
  Redis st;	
	//st.hset("wengduo","boy","2");
	//st.hset("wengduo","girl","3");
	std::vector<std::string> res = st.hGetAll("wengduo");
	for(int i = 0;i < res.size();i++) {
    std::cout << res[i] << std::endl;
	}
	return 0;
}*/
