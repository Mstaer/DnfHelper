
// DnfHelperDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "DnfHelper.h"
#include "DnfHelperDlg.h"
#include "afxdialogex.h"

#include "Driver.h"
#include "Helper.h"
#include "VoouerDrv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDnfHelperDlg 对话框

CDnfHelperDlg::CDnfHelperDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DNFHELPER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(DNF_ICO);
}

void CDnfHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDnfHelperDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDnfHelperDlg::激活)
	ON_BN_CLICKED(IDC_BUTTON2, &CDnfHelperDlg::卸载)
END_MESSAGE_MAP()


// CDnfHelperDlg 消息处理程序
BOOL CDnfHelperDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, TRUE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 设置窗口标题
	SetWindowText(_TEXT("DnfHelper"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CDnfHelperDlg::OnPaint()
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
HCURSOR CDnfHelperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDnfHelperDlg::日志公告(CString msg)
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDCANCEL);
	int nLength = pEdit->GetWindowTextLength();

	//选定当前文本的末端
	pEdit->SetSel(nLength, nLength);
	//l追加文本
	CString data;

	data = data + "\r\n" + msg;
	pEdit->ReplaceSel(data);
}

// 激活按钮点击处理
void CDnfHelperDlg::激活()
{
	//MessageBoxA(NULL, "激活按钮事件", "激活", MB_OK);
	driver = new Driver(); //实例化类的对象
	notice = new Notice(); //实例化类的对象

	//构建驱动文件路径
	CString szPath;

	TCHAR szDrvPath[MAX_PATH];
	GetModuleFileName(NULL, szDrvPath, MAX_PATH);
	*(_tcsrchr(szDrvPath, _T('\\')) + 1) = _T('\0');
	_tcscat_s(szDrvPath, MAX_PATH, _T("voouerDrv.sys"));

	// 初始化驱动服务
	if (szDrvPath == NULL)
	{
		日志公告(_T("驱动文件不存在"));
		return;
	}

	if (!driver->Install(szDrvPath, L"vuDrv", L"vuDrv"))
	{
		日志公告(_T("驱动服务安装失败"));
		return;
	}
	
	// 启动驱动服务
	if (!driver->Start()) {
		日志公告(_T("驱动服务启动失败"));
		return;
	}
	日志公告(_T("驱动服务启动成功"));

	//打开驱动的符号链接  使用期间只需要打开一次,请勿重复调用,此处仅仅为了示例
	driver->Open(L"\\\\.\\vuDrv");

	设置驱动句柄(driver->m_hDriver);
	CHAR* key = "B0A856B7AE45C406644FA76BBE156B63";

	//激活驱动->校验驱动有效性(激活驱动)
	LONG 结果 = 效验有效性(key);

	if (结果 <= 0)
	{
		CString resMsg;
		switch (结果)
		{
			case 0:
				resMsg = "参数错误";
				break;
			case -1:
				resMsg = "激活码不存在";
				break;
			case -2:
				resMsg = "账户余额不足";
				break;
			case -3:
				resMsg = "扣费失败";
				break;
			case -4:
				resMsg = "网络错误";
				break;
			case -5:
				resMsg = "驱动安装失败";
				break;
			case -6:
				resMsg = "有效性效验失败";
				break;
			default:
				resMsg = "未知错误";
		}
		日志公告(_T("加载驱动失败：") + resMsg);
		return;
	}

	// 保护_安装();
	// 键鼠_安装();

	日志公告(_T("加载驱动结果"));

	//关闭符号链接句柄   使用期间请勿关闭句柄,否则驱动将会失效
	// CloseHandle(driver->m_hDriver);
	driver->m_hDriver = INVALID_HANDLE_VALUE;
	return;
}


void abc() {
}

// 卸载按钮点击处理
void CDnfHelperDlg::卸载()
{
	//MessageBoxA(NULL, "卸载按钮事件", "激活", MB_OK);
	// 停止驱动服务
	if (driver->Stop())
	{
		SetDlgItemText(IDCANCEL, L"驱动服务停止成功!");

	}
	else {
		SetDlgItemText(IDCANCEL, L"驱动服务停止失败!");
	}

	// 卸载驱动服务
	if (driver->Remove())
	{
		SetDlgItemText(IDCANCEL, L"驱动服务卸载成功!");
	}
	else
	{
		SetDlgItemText(IDCANCEL, L"驱动服务卸载失败!");
	}


	Sleep(1000);


	// 关闭窗口界面
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}