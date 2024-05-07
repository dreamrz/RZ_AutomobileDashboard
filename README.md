# 说明
这是一个Unreal Engine模拟汽车仪表盘的演示,它可以用在任何地方.
> 主要是作者买了`OBD to USB`的线,所以考虑开发类似的东西.

# 文件内容说明
### `AutomobileDashboard` UE引擎工程
这是Unreal Engine 5.3.2项目内容.<br>
通常可直接使用,内部附带了一个UDP客户端,数据使用UDP进行传输.<br>
> 详见下面服务端的程序.<br>
> 这里为什么要做成网络形式?<br>
> 通常仪表盘可以应用在任何目标上,比如真车,游戏,或者自己开发的程序上.所以最方便但不是最好的方法就是使用网络传输.


<br>
如果想要自行处理数据只需要在`RZ_GameMode`的蓝图里断开红色注释部分,然后手动在某处将UMG`Main`添加到场景里即可.<br>
以下是全局的函数接口(在任意蓝图的地方可以调用):<br>

- `SetMeterGear` 设置挡位 0~13
- `SetMeterRev` 设置转速 0~8000
- `SetEngineSpeedometer` 设置时速
- `SetIFC` 设置瞬时油耗 0~100
- `SetTurbine` 设置涡轮压力 -100~300 对应 -1.0和3.0
- `SetEngineTemperatures` 设置发动机温度
- `SetTransmissionTemperature` 设置变速箱温度

### `AD_Test` MFC模拟的服务端
监听端口为`8899`.原理为只接受一个客户端(以最后接收到的客户端发送信息为准),客户端超时大概为10秒,10秒内发送任意数据给服务端即认为存活状态,并会将模拟的数据按每50毫秒发送给引擎.<br>
> 注意这里为了简单演示,所以逻辑非常简单,拥有数据后可自行设计.<br>
> 另外要针对真车试验可以直接读取串口或者Pcan数据,无需使用网络通讯.

### 通讯数据
结构体:
```cpp
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
```
发送的数据直接是这个结构体,未进行任何处理.<br>
可以自行编写服务端模拟或读取数据.
> 如果你未使用C/Cpp语言作为服务端开发语言,请在C/Cpp下查看`size(CPPSTRUCTFORUDP)`结构体的大小,并保持内存对齐即可.

# 演示视频
https://www.bilibili.com/video/BV1gJ4m1K78d/?vd_source=245d55282adf9074f921fe7372d12d75
