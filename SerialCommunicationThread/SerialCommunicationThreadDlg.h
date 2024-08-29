
// SerialCommunicationThreadDlg.h : ヘッダー ファイル
//

#pragma once


// CSerialCommunicationThreadDlg ダイアログ
class CSerialCommunicationThreadDlg : public CDialogEx
{
// コンストラクション
public:
	CSerialCommunicationThreadDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERIALCOMMUNICATIONTHREAD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	//---------------------------------関数---------------------------------
	//---------------------------------関数---------------------------------


	//---------------------------------変数---------------------------------
	//CComPort comport1;

private:
	HANDLE hMainThread;
	HANDLE hProcessing;
	DWORD dwMainThreadId;
	DWORD dwProcessingThreadId;

	//---------------------------------変数---------------------------------

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedConnection();
	afx_msg void OnBnClickedDisconnection();
	afx_msg void OnBnClickedSend();
};
