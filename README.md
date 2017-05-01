# geohash
此算法是在c++11的基础上开发的，如果在低于c++11上编译，可能会编译不过。。。

Geohash算法原理:http://www.cnblogs.com/dengxinglin/archive/2012/12/14/2817761.html

上面的算法存在两点缺陷:

1:geohash的突变性。（看起来距离很远的点，但它在一个方格中，所以能查询出来）

2:geohash距离很近的点查询不出来。

解决办法:

1:先算出所在地的geohash。

2:再算出次geohash周围的8个方格。

3:再根据所求的范围，进行筛选。
