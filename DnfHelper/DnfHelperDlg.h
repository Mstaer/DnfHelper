
// DnfHelperDlg.h: 头文件
//

#pragma once


#include "Driver.h"
#include "Notice.h"


// CDnfHelperDlg 对话框
class CDnfHelperDlg : public CDialogEx
{
// 构造
public:
	CDnfHelperDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DNFHELPER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void 激活();
	afx_msg void 卸载();
public:
	Driver* driver;

	void 日志公告(CString msg);
};