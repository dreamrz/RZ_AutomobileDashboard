
// AD_TestDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "AD_Test.h"
#include "AD_TestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CADTestDlg 对话框



CADTestDlg::CADTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AD_TEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CADTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

unsigned int __stdcall CADTestDlg::Thread(void* pM)
{
	CADTestDlg* T = (CADTestDlg*)pM;
	CEdit* Error = (CEdit*)T->GetDlgItem(IDC_EDIT2);
	CString MSG;
	CEdit* ClientText = (CEdit*)T->GetDlgItem(IDC_EDIT1);

	T->ThreadRun = true;

	WSADATA wsd;

	SOCKET sServer;
	SOCKADDR_IN addrServer;

	int sError;

	sError = WSAStartup(MAKEWORD(2, 2), &wsd);
	if (sError != 0)
	{
		T->ThreadRun = false;
		MSG.Format(L"WSAGetLastError()错误:%d", WSAGetLastError());
		Error->SetWindowTextW(MSG);
		return 0;
	}
	sServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == sServer)
	{
		T->ThreadRun = false;
		MSG.Format(L"WSAGetLastError()错误:%d", WSAGetLastError());
		Error->SetWindowTextW(MSG);
		return 0;
	}
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(8899);//端口 
	addrServer.sin_addr.S_un.S_addr = INADDR_ANY;
	sError = bind(sServer, (LPSOCKADDR)&addrServer, sizeof(SOCKADDR_IN));
	if (sError == SOCKET_ERROR)
	{
		T->ThreadRun = false;
		MSG.Format(L"WSAGetLastError()错误:%d", WSAGetLastError());
		Error->SetWindowTextW(MSG);
		return 0;
	}
	int iMode = 1;
	sError = ioctlsocket(sServer, FIONBIO, (u_long FAR*)&iMode);
	if (sError == SOCKET_ERROR)
	{
		T->ThreadRun = false;
		MSG.Format(L"WSAGetLastError()错误:%d", WSAGetLastError());
		Error->SetWindowTextW(MSG);
		return 0;
	}


	SOCKADDR_IN m_addrClient;
	int addrClientlen = sizeof(m_addrClient);
	char buffer[1024];
	int msglength = 0;

	Error->SetWindowTextW(L"服务器运行中...");
	int recvLen = 0;
	int time = 0;
	while (T->ThreadRun)
	{
		recvLen = recvfrom(sServer, buffer, 1024, 0, (SOCKADDR*)&m_addrClient, &addrClientlen);
		if (recvLen >= 0)
		{
			time = 200;//10秒超时
			MSG.Format(L"客户端地址: %d.%d.%d.%d  (超时倒计:%d)", m_addrClient.sin_addr.S_un.S_un_b.s_b1, m_addrClient.sin_addr.S_un.S_un_b.s_b2, m_addrClient.sin_addr.S_un.S_un_b.s_b3, m_addrClient.sin_addr.S_un.S_un_b.s_b4, time);
			ClientText->SetWindowTextW(MSG);
			sendto(sServer, (const char*)&T->SendData, sizeof(CPPSTRUCTFORUDP), 0, (SOCKADDR*)&m_addrClient, addrClientlen);
		}
		else
		{
			if (time >= 0)
			{
				MSG.Format(L"客户端地址: %d.%d.%d.%d  (超时倒计:%d)", m_addrClient.sin_addr.S_un.S_un_b.s_b1, m_addrClient.sin_addr.S_un.S_un_b.s_b2, m_addrClient.sin_addr.S_un.S_un_b.s_b3, m_addrClient.sin_addr.S_un.S_un_b.s_b4, time);
				ClientText->SetWindowTextW(MSG);
				time--;
				sendto(sServer, (const char*)&T->SendData, sizeof(CPPSTRUCTFORUDP), 0, (SOCKADDR*)&m_addrClient, addrClientlen);
			}
			else
			{
				ClientText->SetWindowTextW(L"无客户端连接.");
			}
		}
		Sleep(50);
	}
	closesocket(sServer);
	WSACleanup();

	return 0;
}

BEGIN_MESSAGE_MAP(CADTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CADTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CADTestDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CADTestDlg::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CADTestDlg::OnNMCustomdrawSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &CADTestDlg::OnNMCustomdrawSlider3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER4, &CADTestDlg::OnNMCustomdrawSlider4)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER5, &CADTestDlg::OnNMCustomdrawSlider5)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER6, &CADTestDlg::OnNMCustomdrawSlider6)
END_MESSAGE_MAP()


// CADTestDlg 消息处理程序

BOOL CADTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	memset(&SendData, 0, sizeof(CPPSTRUCTFORUDP));
	THREAD = nullptr;
	THREAD = (HANDLE)_beginthreadex(nullptr, 0, this->Thread, this, 0, nullptr);

	CSliderCtrl* p = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	p->SetRange(0, 8000);
	p->SetLineSize(1);
	p->SetPageSize(10);
	p = (CSliderCtrl*)GetDlgItem(IDC_SLIDER2);
	p->SetRange(0, 400);
	p->SetLineSize(1);
	p->SetPageSize(10);
	p = (CSliderCtrl*)GetDlgItem(IDC_SLIDER3);
	p->SetRange(0, 100);
	p->SetLineSize(1);
	p->SetPageSize(1);
	p = (CSliderCtrl*)GetDlgItem(IDC_SLIDER4);
	p->SetRange(-100, 300);
	p->SetLineSize(1);
	p->SetPageSize(10);
	p = (CSliderCtrl*)GetDlgItem(IDC_SLIDER5);
	p->SetRange(0, 200);
	p->SetLineSize(1);
	p->SetPageSize(10);
	p = (CSliderCtrl*)GetDlgItem(IDC_SLIDER6);
	p->SetRange(0, 200);
	p->SetLineSize(1);
	p->SetPageSize(10);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CADTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CADTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CADTestDlg::OnClose()
{
	if (THREAD != nullptr)ThreadRun = false;

	CDialogEx::OnClose();
}


void CADTestDlg::OnBnClickedButton2()
{
	if (SendData.gear > 0)SendData.gear--;
}


void CADTestDlg::OnBnClickedButton1()
{
	if (SendData.gear < 13)SendData.gear++;
}


void CADTestDlg::OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	CSliderCtrl* p = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	SendData.revs = p->GetPos();
	*pResult = 0;
}


void CADTestDlg::OnNMCustomdrawSlider2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	CSliderCtrl* p = (CSliderCtrl*)GetDlgItem(IDC_SLIDER2);
	SendData.MPH = p->GetPos();
	*pResult = 0;
}


void CADTestDlg::OnNMCustomdrawSlider3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	CSliderCtrl* p = (CSliderCtrl*)GetDlgItem(IDC_SLIDER3);
	SendData.instantaneousFuelConsumption = p->GetPos();
	*pResult = 0;
}


void CADTestDlg::OnNMCustomdrawSlider4(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	CSliderCtrl* p = (CSliderCtrl*)GetDlgItem(IDC_SLIDER4);
	SendData.turbine = p->GetPos();
	*pResult = 0;
}


void CADTestDlg::OnNMCustomdrawSlider5(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	CSliderCtrl* p = (CSliderCtrl*)GetDlgItem(IDC_SLIDER5);
	SendData.engineTemperatures = p->GetPos();
	*pResult = 0;
}


void CADTestDlg::OnNMCustomdrawSlider6(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	CSliderCtrl* p = (CSliderCtrl*)GetDlgItem(IDC_SLIDER6);
	SendData.transmissionTemperature = p->GetPos();
	*pResult = 0;
}