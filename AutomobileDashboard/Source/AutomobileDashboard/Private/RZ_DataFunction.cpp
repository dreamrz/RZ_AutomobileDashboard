#include "RZ_DataFunction.h"

ARZ_DataFunction::ARZ_DataFunction()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARZ_DataFunction::Socket_UDP_Create(FString IPStr, int32 port, bool& isWork)
{
	SenderSocket = nullptr;
	RemoteAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	RemoteAddr->SetIp(*IPStr, SocketWork);
	RemoteAddr->SetPort(port);

	isWork = SocketWork;

	//udp句柄创建
	SenderSocket = FUdpSocketBuilder(TEXT("default"))
		.AsReusable()
		.WithBroadcast()//广播
		.WithSendBufferSize(512)
		//.BoundToEndpoint(Endpoint)
		;

	int32 SendSize = 512;
	SenderSocket->SetSendBufferSize(SendSize, SendSize);
	SenderSocket->SetReceiveBufferSize(SendSize, SendSize);

	/*
	//GameThread才是安全的,这里是其它线程 传入&等于整个class的this
	AsyncTask(ENamedThreads::GameThread, [&]() {
		TArray<uint8> ReceiveData;
		int32 read = 0;
		while (SocketWork && SenderSocket != nullptr)
		{
			ReceiveData.Init(uint8(0), 512);//数据清空
			SenderSocket->RecvFrom(ReceiveData.GetData(), ReceiveData.Num(), read, *RemoteAddr);//监听
			if (read > 0)//大于0才有数据
			{
				memcpy(&CppStructForUDP, ReceiveData.GetData(), sizeof(CPPSTRUCTFORUDP));
				UDP_Data.revs = CppStructForUDP.revs;
				UDP_Data.MPH = CppStructForUDP.MPH;
				UDP_Data.instantaneousFuelConsumption = CppStructForUDP.instantaneousFuelConsumption;
				UDP_Data.turbine = CppStructForUDP.turbine;
				UDP_Data.engineTemperatures = CppStructForUDP.engineTemperatures;
				UDP_Data.transmissionTemperature = CppStructForUDP.transmissionTemperature;
				UDP_Data.gear = CppStructForUDP.gear;
				SocketEvent.Broadcast(UDP_Data, true);
			}
			else
			{
				SocketEvent.Broadcast(UDP_Data, false);
			}
		}
		});
		*/
}

bool ARZ_DataFunction::Socket_UDP_Send(FString message)
{
	if (SenderSocket == nullptr)return false;
	TCHAR* seriallizedChar = message.GetCharArray().GetData();
	int32 size = FCString::Strlen(seriallizedChar) + 1;
	int32 sent = 0;
	if (SenderSocket->SendTo((uint8*)TCHAR_TO_UTF8(seriallizedChar), size, sent, *RemoteAddr))
		return true;
	return false;

}

void ARZ_DataFunction::BeginPlay()
{
	Super::BeginPlay();
}

void ARZ_DataFunction::Destroyed()
{
	SocketWork = false;
	if (SenderSocket && SocketWork)
	{
		SenderSocket->Close();
		//ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SenderSocket);
	}
	Super::Destroyed();
}

void ARZ_DataFunction::Tick(float DeltaTime)
{
	if (SocketWork && SenderSocket != nullptr)
	{
		ReceiveData.Init(uint8(0), 512);//数据清空
		SenderSocket->RecvFrom(ReceiveData.GetData(), ReceiveData.Num(), read, *RemoteAddr);//监听
		if (read > 0)//大于0才有数据
		{
			memcpy(&CppStructForUDP, ReceiveData.GetData(), sizeof(CPPSTRUCTFORUDP));
			UDP_Data.revs = CppStructForUDP.revs;
			UDP_Data.MPH = CppStructForUDP.MPH;
			UDP_Data.instantaneousFuelConsumption = CppStructForUDP.instantaneousFuelConsumption;
			UDP_Data.turbine = CppStructForUDP.turbine;
			UDP_Data.engineTemperatures = CppStructForUDP.engineTemperatures;
			UDP_Data.transmissionTemperature = CppStructForUDP.transmissionTemperature;
			UDP_Data.gear = CppStructForUDP.gear;
			SocketEvent.Broadcast(UDP_Data, true);
		}
		else
		{
			SocketEvent.Broadcast(UDP_Data, false);
		}
	}
	Super::Tick(DeltaTime);
}

