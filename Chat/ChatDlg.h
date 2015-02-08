
// ChatDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CChatDlg �Ի���
class CChatDlg : public CDialogEx
{
// ����
public:
	CChatDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CHAT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void PomeloLogin(std::string name,std::string rid);
	void RegesterListeners();
	void OnChat(const char * msg);
	void OnAdd(const char * msg);
	void OnLeave(const char * msg);
	void InitUserList(const char  * data);

	CString m_input;
//	CListBox m_listMsg;
	CString m_currTarget;
	afx_msg void OnBnClickedOk();
	CComboBox m_userBox;
	CListBox m_userList;
	CListBox m_msgList;
};
