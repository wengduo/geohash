/*************************************************************************
    > File Name: 2.cpp
    > Author: wengduo
    > Mail: 2292466904@qq.com
    > Created Time: 2017年03月22日 星期三 20时28分03秒
 ************************************************************************/


#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>


class Geohash {
public:
    Geohash() {
        strcpy(this->coding,"0123456789bcdefghjkmnpqrstuvwxyz");
    }
    //传入经纬度，返回geohash字段
		std::vector<std::string> standardizeParameters(double lat,double lng) {
        char cLat[128] = {0};
        char cLng[128] = {0};
        sprintf(cLat,"%.4f",lat);
        sprintf(cLng,"%.4f",lng);
				//进行经纬度4舍5入
        double ilat = ((int)(lat*10000 + 0.5))/10000.0;
        double ilng = ((int)(lng*10000 + 0.5))/10000.0;
				//將经纬度转成相应的二进制位
        char bflat[128] = {0};
        char bflng[128] = {0};
        binCode(cLat,cLng,ilat,ilng,bflat,bflng);
				//定义geohash的容器，为vector内包string容器
        std::vector<std::string> geohash;
        geohash.push_back(encode(bflat,bflng));
				//得到二进制经纬度的前15位,这个可变，范围为1200米
        char baseLat[128] = {0};      
        char baseLng[128] = {0};
        strncpy(baseLat,bflat,15);
        strncpy(baseLng,bflng,15);
				//得到上下左右4个经纬度的二进制位
        std::string downLat = calculate(baseLat,-1);
        std::string upLat = calculate(baseLat,1);
        std::string leftLng = calculate(baseLng,-1);
        std::string rightLng = calculate(baseLng,1);
        //为了解决geohash算法的缺陷，从而得到周围的八个方格
        geohash.push_back(encode(upLat.c_str(),leftLng.c_str()));
        geohash.push_back(encode(upLat.c_str(),baseLng));
        geohash.push_back(encode(upLat.c_str(),rightLng.c_str()));
        geohash.push_back(encode(baseLat,leftLng.c_str()));
        geohash.push_back(encode(baseLat,rightLng.c_str()));
        geohash.push_back(encode(downLat.c_str(),leftLng.c_str()));
        geohash.push_back(encode(downLat.c_str(),baseLng));
        geohash.push_back(encode(downLat.c_str(),rightLng.c_str()));
				return geohash;
    }
private:

		//二进制进行加1,减1的操作
    std::string calculate(char *corrdinate,int n) {
        int sum = 0;
        if(n > 0) {
           sum = bindec(corrdinate)+1+(1 << strlen(corrdinate));
        } else {
           sum = bindec(corrdinate)-1+(1 << strlen(corrdinate));
        }
        std::string s = decbin(sum);
				//如果字符加超，返回为空
        if(strlen(s.c_str()) != strlen(corrdinate)) {
					  std::string tmp;
						tmp[0] = 0;
            return tmp;
        }
        return s;
    }

		//十进制转二进制
    std::string decbin(int n) {
        char buff[128] = {0};
        std::string str;
        int i = 0;
        while(n/2) {
            buff[i] = n%2+48;
            n /= 2;
            i++;
        }
        buff[i++] = '1';
        int k = 0;
        for(int i = strlen(buff)-2;i >= 0;i--) {
            str[k++] = buff[i];
        }
				str[k] = 0;
        return str;
    }

		//二进制数转十进制
    int bindec(const char *binary) {
        int sum = 0;
        int n = 1;
        for(int i = strlen(binary)-1;i >= 0;i--) {
            sum += (binary[i]-48)*pow(2,n-1);
            n++;
        }
        return sum;
    }
    //模拟php的函数去实现的，支持从begin位置开始拷贝n个字符，到指定空间
		std::string substr(const char *str,int begin,int n,char *tmp) {
				int i;
        for(i = 0;i < n;i++) {
            tmp[i] = str[begin++];
        }
				tmp[i] = 0;
        return tmp;
    }
    //将经纬度字符串转成geohash字符串
    std::string encode(const char *bflat,const char *bflng) {
        std::string geohash;
        char binary[128] = {0};
        int i = 0;
        int length = strlen(bflat)+strlen(bflng); 
        while(length--) {
            if(i%2 != 0) {
                binary[i++] = *bflat++;
            } else {
                binary[i++] = *bflng++;
            }
        }
				char buff[128] = {0};
        int m = 0;
        for(int i = 0;i < strlen(binary);i += 5) {
            substr(binary,i,5,buff);
            geohash[m++] = this->coding[bindec(buff)];   
            memset(buff,0,128);
        }
				geohash[m] = 0; 
        return geohash;
    }
         
    //将经纬度专成二进制
    void binEncode(char *str,double number,double left, double right,int bitCount) {
        double mid = 0;
        while(bitCount) {
            mid = (left + right)/2.0;
            if(compre(number,mid)) {
                *str++ = '1';
                left = mid;
            } else {
                *str++ = '0';
                right = mid;
            }
            bitCount--;
        }
    }
    //得到某个字符在字符串中的位置
    int strpos(const char *str,const char c) {
        if(NULL == str) {
            return -1;
        }
        const char *s = str;
        while(*s) {
            if(*s == c) {
                return s-str;
            }
            s++;
        }
        return -1;
    }

    double precision(char *str) {
        int precision = 0;
        int n = strpos(str,'.');
        if(n != -1) {
            precision = -(strlen(str)-n-1);
        }
       
        return pow(10,precision)/2;
    }

    int max(int a,int b) {
        return a > b ? a : b;
    }

    int compre(double a,double b) {
        if(a - b > 1e-6) {
            return 1;
        }
        return 0;
    }
 
    //得到经纬度的二进制
    void binCode(char *cLat,char *cLng,double lat,double lng,char *bflat,char *bflng) {
        double pLat = precision(cLat);
        int latBits = 1;
        double aErr = 45.0;
        while(compre(aErr,pLat)) {
            latBits++;
            aErr = aErr/2;
        }
        double pLng = precision(cLng);
        int lngBits = 1;
        double nErr = 90.0;
        while(compre(nErr,pLng)) {
            lngBits++;
            nErr /= 2;
        }
        int bits = max(latBits,lngBits);
        int addLong = 1;
        latBits = bits;
        lngBits = bits;
        while((latBits + lngBits) % 5 != 0) {
            lngBits += addLong;
            latBits += !addLong;
            addLong = !addLong;
        }
        binEncode(bflat,lat, -90, 90, latBits);
        binEncode(bflng,lng, -180, 180, lngBits);
    }
private:
    char coding[33];
};

int main() {

    Geohash st;   
		std::vector<std::string> geohash = st.standardizeParameters(30.595093,104.067425);
    for(int i = 0;i < geohash.size();i++) {
        std::cout << geohash[i].c_str()  << std::endl;
    }
    return 0;
}
