/*************************************************************************
    > File Name: log.h
    > Author: wengduo
    > Mail: 2292466904@qq.com
    > Created Time: 2017年04月14日 星期五 17时04分31秒
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <sys/time.h>
#include <cmath>
#include <unistd.h>

class Log {
private:

	void operator=(const Log &tmp);

public:
	Log();
	
	~Log();
	
	void log_notice(std::string file,std::string function,int line,std::string title,std::string &data);

	void log_error(std::string file,std::string function,int line,std::string title,std::string &data);
	
	void log_performance(std::string file,std::string function,int line,std::string title,std::string &data);
  
	void log_error(std::string file,std::string function,int line,std::string title,std::vector<std::vector<std::string> > &data);

  void log_error(std::string file,std::string function,int line,std::string title,std::vector<std::string> &data);

	void log_error(std::string file,std::string function,int line,std::string title,const char *data);
  
	void log_error(std::string file,std::string function,int line,std::string title,int data);
	
	void log_notice(std::string file,std::string function,int line,std::string title,std::vector<std::vector<std::string> > &data);

  void log_notice(std::string file,std::string function,int line,std::string title,std::vector<std::string> &data);
	
	void log_notice(std::string file,std::string function,int line,std::string title,const char *data);

  void log_notice(std::string file,std::string function,int line,std::string title,int data);
	
	void log_performance(std::string file,std::string function,int line,std::string title,std::vector<std::vector<std::string> > &data);

  void log_performance(std::string file,std::string function,int line,std::string title,std::vector<std::string> &data);
  
	void log_performance(std::string file,std::string function,int line,std::string title,const char *data);
  
	void log_performance(std::string file,std::string function,int line,std::string title,int data);

private:
	std::string basePreformanceLogPath = "../logs/performance.log.";
	std::string baseNoticeLogPath = "../logs/point.log."; 
	std::string baseErrorLogPath = "../logs/point.log.wf.";
	std::string path;
};

