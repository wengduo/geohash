#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>

class Geohash {
public:
    //传入经纬度，返回geohash字段
    std::vector<std::string> standardizeParameters(double lat,double lng) const;

private:
    //得到经纬度的二进制
    std::vector binCode(double lat,double lng) const;
private:
    const coding[]="0123456789bcdefghjkmnpqrstuvwxyz";
};