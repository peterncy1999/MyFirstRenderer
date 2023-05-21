# MyFirstRenderer
## 学习与构建过程写在[项目的wiki](https://github.com/peterncy1999/MyFirstRenderer/wiki)里了
由于写前没有想好要实现啥功能，写的过程就是想到啥写啥，所以代码的结构很乱，接口也没有好好设计,等之后有时间整理一下<br/>
目前暂时实现的基本功能有<br/>
* Bresenham直线算法画线
* 读取OBJ、TGA
* 光栅化(扫描线和重心坐标)
* Zbuffer
* 纹理映射、UV插值
* MVP变换
* 透视插值
* Blinn-Phong光照
* 光照贴图+法线贴图
* Shadowmap<br/>
模型使用的是[tinyrenderer](https://github.com/ssloy/tinyrenderer)里的迪亚波罗，目前实现的效果：<br/>
![207228d015051da1a047ee65eaa1f9b](https://github.com/peterncy1999/MyFirstRenderer/assets/37617739/a71b555b-4bc1-4590-ae91-14151fb67f70)

