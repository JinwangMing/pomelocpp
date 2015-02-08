
// ChatDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Chat.h"
#include "ChatDlg.h"
#include "afxdialogex.h"

#include "json\json.h"
#include <string>
#include <functional>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std::placeholders;
CString UTF8ToUnicode( const char* str )
{
 int  len = 0;
 len = strlen(str);
 int  unicodeLen = ::MultiByteToWideChar( CP_UTF8,
            0,
            str,
            -1,
            NULL,
            0 ); 
 wchar_t *  pUnicode; 
 pUnicode = new  wchar_t[unicodeLen+1];
 memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t)); 
 ::MultiByteToWideChar( CP_UTF8,
         0,
         str,
         -1,
         (LPWSTR)pUnicode,
         unicodeLen ); 
 CString  rt;
 rt = ( wchar_t* )pUnicode;
 delete  pUnicode;
   
 return  rt;
}
std::string UnicodeToUTF8( const CString& str )
{  

	char*     pElementText;
	int    iTextLen;  // wide char to multi char  
	iTextLen = WideCharToMultiByte( CP_UTF8,
		0,
		str,
		-1,
		NULL,
		0,
		NULL,
		NULL );
	pElementText = new char[iTextLen + 1];
	memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
	::WideCharToMultiByte( CP_UTF8,
		0,
		str,
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL );
	std::string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatDlg �Ի���




CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatDlg::IDD, pParent)
	, m_input(_T(""))
	, m_currTarget(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_input = _T("");
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT1, input);
	//  D//  DV_MaxChars(p//  DX, input, 256);
	//  DDX_Control(pDX, IDC_COMBO1, m_userList);
	DDX_Text(pDX, IDC_EDIT1, m_input);
	DDV_MaxChars(pDX, m_input, 256);
	//  DDX_Control(pDX, IDC_LIST_MSG, m_listMsg);
	//  DDX_Control(pDX, IDC_LIST1, m_listUsers);
	DDX_CBString(pDX, IDC_COMBO1, m_currTarget);
	DDX_Control(pDX, IDC_COMBO1, m_userBox);
	DDX_Control(pDX, IDC_LIST1, m_userList);
	DDX_Control(pDX, IDC_LIST_MSG, m_msgList);
}

BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CChatDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CChatDlg ��Ϣ�������

BOOL CChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	RegesterListeners();
	m_userBox.SelectString(0,_T("all"));
	PomeloLogin("limenghua","room");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CChatDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CChatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);

	const char * route = "chat.chatHandler.send";
	Json::Value request;
	request["from"] = "limenghua";
	request["rid"] = "room";
	request["content"] = UnicodeToUTF8(m_input);
	std::string to = UnicodeToUTF8(m_currTarget);
	request["target"] = (to =="all") ? "*" : to;

	theApp.pomelo.DoRequest(route,request.toStyledString().c_str(),
		[](const pomelo::Request & req,int rc,const char * msg){
			if(rc != PC_RC_OK){
				::AfxMessageBox(_T("������Ϣ����"));
			}
	});
	//CDialogEx::OnOK();
}

//��½
void CChatDlg::PomeloLogin(std::string name,std::string rid)
{
	//const char * route = "gate.gateHandler.queryEntry";

	const char * route = "connector.entryHandler.enter";
	Json::Value request;
	request["username"] = name;
	request["rid"] = rid;
	theApp.pomelo.DoRequest(route,request.toStyledString().c_str(),
		std::bind(&CChatDlg::InitUserList,this,_3)
		);
	/*
		[](const pomelo::Request & req,int rc,const char * msg){
			if(rc != PC_RC_OK){
				::AfxMessageBox(_T("��½�������"));
			}
			else{
				InitUserList(msg);
			}
	});
	*/
}

void CChatDlg::InitUserList(const char *data){
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(std::string(data), root, false))
    {
        return;
    }
	Json::Value users = root["users"];
	if(!users.isArray()){
		::AfxMessageBox(_T("�û��б���һ������"));
	}
	for(auto it = users.begin();it!= users.end();it++){
		std::string user = (*it).asString();
		m_userList.AddString(UTF8ToUnicode(user.c_str()));
		m_userBox.AddString(UTF8ToUnicode(user.c_str()));
	}
}

void CChatDlg::RegesterListeners()
{
	theApp.pomelo.On("onChat",std::bind(&CChatDlg::OnChat,this,_1));
	theApp.pomelo.On("onAdd",std::bind(&CChatDlg::OnAdd,this,_1));
	theApp.pomelo.On("onLeave",std::bind(&CChatDlg::OnLeave,this,_1));
	theApp.pomelo.On("disconnect",[](const char * msg){
		::AfxMessageBox(_T("���ӶϿ�"));
	});
	theApp.pomelo.On("kicked",[](const char * msg){
		::AfxMessageBox(_T("�������������ݻ�Ӧ"));
	});
}

 

void CChatDlg::OnChat(const char * msg)
{
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(std::string(msg), root, false))
    {
        return;
    }
 
    std::string message = root["msg"].asString();
	std::string from = root["from"].asString();
	std::string target   = root["target"].asString();
	target = (target=="*") ? "all" : target;
	std::string displayStr = from + std::string("  says to ")+ target +":"+ message;

	SYSTEMTIME st;
	CString strTime;
	GetLocalTime(&st); 
	strTime.Format(_T("%02d:%02d  "),st.wMinute,st.wSecond);

	m_msgList.AddString(strTime + UTF8ToUnicode(displayStr.c_str()));
}

void CChatDlg::OnAdd(const char * msg)
{
	Json::Reader reader;
    Json::Value root;
    if (!reader.parse(std::string(msg), root, false))
    {
        return;
    }
	std::string user = root["user"].asString();
	m_userList.AddString(UTF8ToUnicode(user.c_str()));
	m_userBox.AddString(UTF8ToUnicode(user.c_str()));
}
void CChatDlg::OnLeave(const char * msg)
{
	Json::Reader reader;
    Json::Value root;
    if (!reader.parse(std::string(msg), root, false))
    {
        return;
    }
	std::string user = root["user"].asString();
	CString userStr = UTF8ToUnicode(user.c_str());

	int index = m_userList.FindString(0,userStr);
	m_userList.DeleteString(index);
}