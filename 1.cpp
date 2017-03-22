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
    void standardizeParameters(double lat,double lng)
    {
        char cLat[128] = {0};
        char cLng[128] = {0};
        sprintf(cLat,"%.4f",lat);
        sprintf(cLng,"%.4f",lng);
        double ilat = ((int)(lat*10000 + 0.5))/10000.0;
        double ilng = ((int)(lng*10000 + 0.5))/10000.0;
        //printf("%f\n",ilat);
        //printf("%s\n",cLat);
        //printf("%s\n",cLng);
        //printf("%f\n",ilng);
        char bflat[128] = {0};
        char bflng[128] = {0};
        binCode(cLat,cLng,ilat,ilng,bflat,bflng);
        char geohash1[128] = {0};
        std::vector<std::string> st;
        st.push_back(encode(bflat,bflng));
        char baseLat[128] = {0};      
        char baseLng[128] = {0};
        strncpy(baseLat,bflat,15);
        strncpy(baseLng,bflng,15);
        printf("%s\n",st[0].c_str());
        //std::string downLat = calculate(baseLat,-1);
        std::string upLat = calculate(baseLat,1);
        std::string leftLng = calculate(baseLng,-1);
        //std::string rightLng = calculate(baseLng,-1);
        
        //st.push_back(encode(upLat.c_str(),leftLng.c_str()));
        //printf("%s\n",st[1].c_str());
        //printf("%s\n",upLat.c_str());
        //printf("%s\n",leftLng.c_str());
        /*st.push_back(encode(upLat.c_str(),baseLng));
        //printf("%s\n",st[2].c_str());
        st.push_back(encode(upLat.c_str(),rightLng.c_str()));
        st.push_back(encode(baseLat,leftLng.c_str()));
        st.push_back(encode(baseLat,rightLng.c_str()));
        st.push_back(encode(downLat.c_str(),leftLng.c_str()));
        st.push_back(encode(downLat.c_str(),baseLng));
        st.push_back(encode(downLat.c_str(),rightLng.c_str()));*/
        //printf("%s\n",st[1].c_str());
       /*printf("%s\n",st[2].c_str());
        printf("%d\n",st.size());
        for(int i = 0;i < st.size();i++) {
           std::cout << st[i] << std::endl;
        }*/
    }
private:
    std::string calculate(char *corrdinate,int k) {
        //std::string s;
        int n = 0;
        if(k > 0) {
           n = (bindec(corrdinate)+1+(1 << strlen(corrdinate)));
        } else {
           n =  (bindec(corrdinate)-1+(1 << strlen(corrdinate)));
        }
        printf("%skk\n",corrdinate);
        std::string s = decbin(n);
        printf("%s\n",s.c_str());
        printf("%d\n",strlen(s.c_str()));
        if(strlen(s.c_str()) != strlen(corrdinate)) {
            s.clear();
        }
        
     //   printf("%d\n",n);
        return s;
    }
    std::string decbin(int n) {
     //   printf("%d\n",n);
       //str.erase(str.begin(),str.end());
       //printf("%sigg\n",str.c_str());
        char buff[128] = {0};
        n = 6;
        std::string str;
        int i = 0;
        while(n/2) {
            buff[i] = n%2+48;
            printf("%c\n",(char)(n%2+48));
            n /= 2;
            i++;
            //printf("%d\n",n);
        }
        buff[i++] = '1';
        int k = 0;
       printf("%s\n",buff);
        printf("%d\n",strlen(buff));
        for(int j = strlen(buff)-1;j >= 0;j--) {
            str.insert(buff[j]);
            k++;
            printf("%c\n",buff[j]);
            
        }
        std::cout << str << std::endl;
        //s.reverse(s.begin(),s.end());
        return str;
    }
    int bindec(char *binary) {
        int h = strlen(binary);
        int sum = 0;
        int j = 1;
        for(int i = h-1;i >= 0;i--) {
            sum += (binary[i]-48)*pow(2,j-1);
            j++;
        }
        return sum;
    }

    void substr(char *str,int bg,int n,char *pstr) {

        for(int i = 0;i < n;i++) {
            pstr[i] = str[bg++];
        }

    }

    std::string encode(const char *bflat,const char *bflng) {
        std::string geohash;
        char binary[128] = {0};
        int i = 0;
        int j = 0;
        int k = 0;
        int m = 0;
        int h = strlen(bflat)+strlen(bflng); 
        while(h--) {
            if(k%2 != 0) {
                binary[k++] = bflat[i++];
            } else {
                binary[k++] = bflng[j++];
            }
        }
        char bf[127] = {0};
        for(int n = 0;n < strlen(binary);n += 5) {
       //     printf("%s\n",binary);
            substr(binary,n,5,bf);
         //   printf("%s\n",bf);
           
            int h = bindec(bf);
            memset(bf,0,128);
     //       printf("%d\n",h);
            geohash[m++] = this->coding[h];   
        }
        return geohash;
        //printf("%s\n",geohash.c_str());
    }
         

    void binEncode(char *str,double number,double left, double right,int bitCount) {
        double mid = 0;
        int i = 0;
        while(bitCount) {
            mid = (left + right)/2.0;
            if(compre(number,mid)) {
                str[i] = '1';
                left = mid;
            } else {
                str[i] = '0';
                right = mid;
            }
            bitCount--;
            i++;
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
       
     //   printf("%d\n",precision);
      //  printf("%d\n",n);
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
        //printf("%f\n",pLat);
        int latBits = 1;
        double aErr = 45.0;
        while(compre(aErr,pLat)) {
            latBits++;
            aErr = aErr/2;
     //       printf("%f\n",aErr);
        }
        double pLng = precision(cLng);
       // printf("%f\n",pLng);
        int lngBits = 1;
        double nErr = 90.0;
        while(compre(nErr,pLng)) {
            lngBits++;
            nErr /= 2;
        }
       // printf("%d-%d",latBits,lngBits);
        int bits = max(latBits,lngBits);
       // printf("%d\n",bits);
        int addLong = 1;
        latBits = bits;
        lngBits = bits;
        while((latBits + lngBits) % 5 != 0) {
            lngBits += addLong;
            latBits += !addLong;
            addLong = !addLong;
        }
        //printf("%d-%d\n",latBits,lngBits);
        binEncode(bflat,lat, -90, 90, latBits);
        binEncode(bflng,lng, -180, 180, lngBits);
     //   printf("%s\n",bflat);
      //  printf("%s\n",bflng);
    }
private:
    char coding[33];
};

int main() {

    Geohash st;   
    st.standardizeParameters(30.595093,104.067425);
    return 0;
}
