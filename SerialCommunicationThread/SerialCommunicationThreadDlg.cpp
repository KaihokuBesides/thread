
// SerialCommunicationThreadDlg.cpp : 実装ファイル
//
#define _CRT_SECURE_NO_WARNINGS


#include "pch.h"
#include "framework.h"
#include "SerialCommunicationThread.h"
#include "SerialCommunicationThreadDlg.h"
#include "SerialCommunication.h"
#include <thread>
#include <string>
#include "afxdialogex.h"
#include <windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW

#endif



CComPort comport;

// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
public:
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


// CSerialCommunicationThreadDlg ダイアログ



CSerialCommunicationThreadDlg::CSerialCommunicationThreadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERIALCOMMUNICATIONTHREAD_DIALOG, pParent)
	, hMainThread(NULL)
	, hProcessing(NULL)
	, dwMainThreadId(0)
	, dwProcessingThreadId(0)
	, sSendMessage(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CSerialCommunicationThreadDlg::DoDataExchange(CDataExchange* pDX)
{

	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, sSendMessage);
}

BEGIN_MESSAGE_MAP(CSerialCommunicationThreadDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OK, &CSerialCommunicationThreadDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL, &CSerialCommunicationThreadDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CONNECTION, &CSerialCommunicationThreadDlg::OnBnClickedConnection)
	ON_BN_CLICKED(IDC_DISCONNECTION, &CSerialCommunicationThreadDlg::OnBnClickedDisconnection)
	ON_BN_CLICKED(IDC_SEND, &CSerialCommunicationThreadDlg::OnBnClickedSend)
END_MESSAGE_MAP()


// CSerialCommunicationThreadDlg メッセージ ハンドラー

BOOL CSerialCommunicationThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
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

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}



void CSerialCommunicationThreadDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CSerialCommunicationThreadDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CSerialCommunicationThreadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CSerialCommunicationThreadDlg::OnBnClickedOk()
{
	UpdateData();

	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	std::thread MainThread(&CSerialCommunicationThreadDlg::MainThreadFunc, this);
	MainThread.detach();

}


void CSerialCommunicationThreadDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


void CSerialCommunicationThreadDlg::OnBnClickedConnection()
{

}


void CSerialCommunicationThreadDlg::OnBnClickedDisconnection()
{
	
}


void CSerialCommunicationThreadDlg::OnBnClickedSend()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}

void CSerialCommunicationThreadDlg::MainThreadFunc()
{
	comport.CheckOpenPort(1, 2);

	comport.Open("COM1", "COM2");

	char* pChar = new char[sSendMessage.GetLength() + 1];
	char* recvChar = "";
	strcpy(pChar, sSendMessage);

	comport.Send(pChar);

	comport.Recv(recvChar);

	delete[] pChar; // newした場合は忘れずに削除

}


