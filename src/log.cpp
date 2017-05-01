/*************************************************************************
    > File Name: log.cpp
    > Author: wengduo
    > Mail: 2292466904@qq.com
    > Created Time: 2017年05月01日 星期一 08时34分27秒
 ************************************************************************/

#include "log.h"

Log::Log() {

	time_t timep;
	struct tm *p;
	time(&timep);
	p = gmtime(&timep);
	char buff[128] = {0};
	sprintf(buff,"%d%02d%02d",1900+p->tm_year,1+p->tm_mon,p->tm_mday);
	this->baseErrorLogPath += buff;
	this->baseNoticeLogPath += buff;
	this->basePreformanceLogPath += buff;
	//获取文件目录
	memset(buff,0,128);
	getcwd(buff,127);
	this->path = buff;

}

Log::~Log() {
}

void Log::log_notice(std::string file,std::string function,int line,std::string title,std::string &data) {
	time_t timep;
	struct tm *p;
	time(&timep);
	struct timeval now;
	gettimeofday(&now,NULL);
	p = localtime(&timep);
	char log[1024] = {0};
	FILE *fd = fopen(this->baseNoticeLogPath.c_str(),"a+");
	assert(NULL != fd);
	sprintf(log,"[NOTICE][%d-%02d-%02dT%02d:%02d:%02d.%03ld][line=%s/%s]:%d-[functon=%s]-[title=%s]|",1900+p->tm_year,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec,now.tv_usec/1000,path.c_str(),file.c_str(),line,function.c_str(),title.c_str());
	std::string logData = log;
	logData += "data:{" + data + "}\n";
	fwrite(logData.c_str(),1,strlen(logData.c_str()),fd);
	fclose(fd);
	logData.clear();
}

void Log::log_error(std::string file,std::string function,int line,std::string title,std::string &data) {

	FILE *fd = fopen(this->baseErrorLogPath.c_str(),"a+");
	assert(NULL != fd);
	time_t timep;
	struct tm *p;
	time(&timep);
	struct timeval now;
	gettimeofday(&now,NULL);
	p = localtime(&timep);
	char log[1024] = {0};
	sprintf(log,"[ERROR][%d-%02d-%02dT%02d:%02d:%02d.%03ld][line=%s/%s]:%d-[functon=%s]-[title=%s]|",1900+p->tm_year,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec,now.tv_usec/1000,path.c_str(),file.c_str(),line,function.c_str(),title.c_str());
	std::string logData = log;
	logData += "{" + data + "}\n";
	fwrite(logData.c_str(),1,strlen(logData.c_str()),fd);
	fclose(fd);
	logData.clear();
}

void Log::log_performance(std::string file,std::string function,int line,std::string title,std::string &data) {
	FILE *fd = fopen(this->baseErrorLogPath.c_str(),"a+");
	assert(NULL != fd);
	time_t timep;
	struct tm *p;
	time(&timep);
	struct timeval now;
	gettimeofday(&now,NULL);
	p = localtime(&timep);
	char log[1024] = {0};
	sprintf(log,"[PERFORMANCE][%d-%02d-%02dT%02d:%02d:%02d.%03ld][line=%s/%s]:%d-[functon=%s]-[title=%s]|",1900+p->tm_year,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec,now.tv_usec/1000,path.c_str(),file.c_str(),line,function.c_str(),title.c_str());
	std::string logData = log;
	logData += "{" + data + "}\n";
	fwrite(logData.c_str(),1,strlen(logData.c_str())+1,fd);
	fclose(fd);
	logData.clear();
}

void Log::log_error(std::string file,std::string function,int line,std::string title,std::vector<std::vector<std::string> > &data) {
    std::string jsonData;
    for(int i = 0;i < data.size();++i) {
      for(int j = 0;j < data[i].size();++j) {
        jsonData += data[i][j];
				if(i != data.size()-1 || j != data[i].size()-1) {
					jsonData += "||";
				}
			}
		}
		log_error(file,function,line,title,jsonData);
}

void Log::log_error(std::string file,std::string function,int line,std::string title,std::vector<std::string> &data) {
	std::string jsonData;
	for(int i = 0;i < data.size();++i) {
		jsonData += data[i];
		if(i != data.size()-1) {
			jsonData += "||";
		}
	}
	log_error(file,function,line,title,jsonData);
}

void Log::log_error(std::string file,std::string function,int line,std::string title,const char *data) {
	std::string jsonData = data;
	log_error(file,function,line,title,jsonData);
}

void Log::log_error(std::string file,std::string function,int line,std::string title,int data) {
	char buff[127] = {0};
	sprintf(buff,"%d",data);
	std::string jsonData = buff;
	log_error(file,function,line,title,jsonData);
}

void Log::log_notice(std::string file,std::string function,int line,std::string title,std::vector<std::vector<std::string> > &data) {
	std::string jsonData;
	for(int i = 0;i < data.size();++i) {
		for(int j = 0;j < data[i].size();++j) {
			jsonData += data[i][j];
			if(i != data.size()-1 || j != data[i].size()-1) {
				jsonData += "||";
			}
		}
	}
	log_notice(file,function,line,title,jsonData);
}

void Log::log_notice(std::string file,std::string function,int line,std::string title,std::vector<std::string> &data) {
	std::string jsonData;
	for(int i = 0;i < data.size();++i) {
		jsonData += data[i];
		if(i != data.size()-1) {
			jsonData += "||";
		}
	}
	log_notice(file,function,line,title,jsonData);
}

void Log::log_notice(std::string file,std::string function,int line,std::string title,const char *data) {
	std::string jsonData = data;
	log_notice(file,function,line,title,jsonData);
}

void Log::log_notice(std::string file,std::string function,int line,std::string title,int data) {
	char buff[127] = {0};
	sprintf(buff,"%d",data);
	std::string jsonData = buff;
	log_notice(file,function,line,title,jsonData);
}

void Log::log_performance(std::string file,std::string function,int line,std::string title,std::vector<std::vector<std::string> > &data) {
	std::string jsonData;
	for(int i = 0;i < data.size();++i) {
		for(int j = 0;j < data[i].size();++j) {
			jsonData += data[i][j];
			if(i != data.size()-1 || j != data[i].size()-1) {
				jsonData += "||";
			}
		}
	}
	log_performance(file,function,line,title,jsonData);
}

void Log::log_performance(std::string file,std::string function,int line,std::string title,std::vector<std::string> &data) {
	std::string jsonData;
	for(int i = 0;i < data.size();++i) {
		jsonData += data[i];
		if(i != data.size()-1) {
			jsonData += "||";
		}
	}
	log_performance(file,function,line,title,jsonData);
}

void Log::log_performance(std::string file,std::string function,int line,std::string title,const char *data) {
	std::string jsonData = data;
	log_performance(file,function,line,title,jsonData);
}

void Log::log_performance(std::string file,std::string function,int line,std::string title,int data) {
	char buff[127] = {0};
	sprintf(buff,"%d",data);
	std::string jsonData = buff;
	log_performance(file,function,line,title,jsonData);
}

