// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"
#include <process.h> 
#include <winsock2.h> 
#pragma comment(lib, "ws2_32.lib") 

#endif //PCH_H

struct CPPSTRUCTFORUDP
{
	short revs; //转速
	short MPH; //公里每小时
	short instantaneousFuelConsumption; //瞬时油耗
	short turbine; //涡轮压力
	short engineTemperatures; //发动机温度
	short transmissionTemperature; //变速箱温度
	short gear; //挡位 0=P 1=N 2=R 3=D 4=1 5=2 6=3 7=4 8=5 9=6 10=7 11=8 12=9 13=10
};
