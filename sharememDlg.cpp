
// sharememDlg.cpp: 实现文件

#include "pch.h"
#include "framework.h"
#include "sharemem.h"
#include "sharememDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CsharememDlg 对话框



CsharememDlg::CsharememDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHAREMEM_DIALOG, pParent)
	, m_send(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsharememDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_send);
}

BEGIN_MESSAGE_MAP(CsharememDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CsharememDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CsharememDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CsharememDlg 消息处理程序

BOOL CsharememDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	 check = OpenFileMapping(         
		FILE_MAP_READ,
		FALSE,
		TEXT("sharemem")
	);
	if (check) {
		MessageBox(L"打开共享内存",L"提示",MB_OK);
	}
	else {
		MessageBox(L"创建共享内存",L"提示",MB_OK);
	}

	//创建映射对象
	HANDLE m_share = CreateFileMapping((HANDLE)0xffffffff,    //因为不是读写文件,所以不需要文件句柄
		nullptr,									   //默认安全属性
		PAGE_READWRITE,								   //可读写
		0,											  //高32位不用	
		0x1000,										  //低32位，设置4k
		TEXT("sharemem")                            //共享数据的名称
	);

	//获取一块内存空间以便写入数据
	m_write = (LPTSTR)MapViewOfFile(m_share, FILE_SHARE_WRITE, 0, 0, 0);
	if (!m_write)
	{
		MessageBox(L"映射数据失败", L"提示", MB_OK);
	}

	//打开映射对象
	openshare = OpenFileMapping(
		FILE_MAP_READ,
		FALSE,
		TEXT("sharemem")
	);
	if (!openshare)
	{
		MessageBox(L"打开映射失败", L"提示", MB_OK);
		return FALSE;
	}

	m_read = (LPTSTR)MapViewOfFile(openshare, FILE_SHARE_READ, 0, 0, 0);
	if (!m_read)
	{
		MessageBox(_T("读取内存空间失败"));
	}
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CsharememDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CsharememDlg::OnPaint()
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
HCURSOR CsharememDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CsharememDlg::OnBnClickedButton1()
{
	
	UpdateData();
	lstrcpy(m_write,m_send);////把编辑框的数据读到内存空间
}

void CsharememDlg::OnBnClickedButton2()
{
	SetDlgItemText(IDC_STATIC, m_read);//将数据写入文本框
	// TODO: 在此添加控件通知处理程序代码
}



