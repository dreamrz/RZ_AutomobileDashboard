#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "GameFramework/Actor.h"
#include "RZ_DataFunction.generated.h"

//����ͨѶ�ṹ��
struct CPPSTRUCTFORUDP
{
	short revs; //ת��
	short MPH; //����ÿСʱ
	short instantaneousFuelConsumption; //˲ʱ�ͺ�
	short turbine; //����ѹ��
	short engineTemperatures; //�������¶�
	short transmissionTemperature; //�������¶�
	short gear; //��λ 0=P 1=N 2=R 3=D 4=1 5=2 6=3 7=4 8=5 9=6 10=7 11=8 12=9 13=10
};

USTRUCT(BlueprintType)
struct FUDPDATA
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 revs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MPH;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 instantaneousFuelConsumption;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 turbine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 engineTemperatures;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 transmissionTemperature;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 gear;
};


UCLASS()
class AUTOMOBILEDASHBOARD_API ARZ_DataFunction : public AActor
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOCKETUDPMSG, FUDPDATA, data, bool, isConnects);
public:	
	ARZ_DataFunction();
	UFUNCTION(BlueprintCallable, Category = "RZ_Socket_UDP")
	void Socket_UDP_Create(FString IPStr, int32 port, bool& isWork);
	UFUNCTION(BlueprintCallable, Category = "RZ_Socket_UDP")
	bool Socket_UDP_Send(FString message);
	UPROPERTY(BlueprintAssignable, Category = "RZ_Socket_UDP")
	FSOCKETUDPMSG SocketEvent;

private:
	bool SocketWork;
	FSocket* SenderSocket;
	TSharedPtr<FInternetAddr> RemoteAddr;

	TArray<uint8> ReceiveData;
	int32 read = 0;

	FUDPDATA UDP_Data;
	CPPSTRUCTFORUDP CppStructForUDP;


protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
