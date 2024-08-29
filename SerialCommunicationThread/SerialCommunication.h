#pragma once

#include <Windows.h>

#define BUFFER_SIZE 1024
#define BIT_SIZE 8
#define WAIT_TIMEOUT_MILLISECOND 500
#define COMMUNICATION_SPEED 9600
#define MAX_ELEMENTS 32


class CComPort
{
public:
	CComPort(void);
	~CComPort();

//-------------------------------------------------内部関数-------------------------------------------------
	
	//ポートが開いているかのフラグを返す関数
	bool GetStatus();

	//ポートを開ける関数
	bool Open(char* _sOpenSendPortName, char* _sOpenRecvPortName);
	//入力されたポート番号が開けるか確認する
	bool CheckOpenPort(int _iOpenSendPortNum, int _iOpenRecvPortNum);
	bool Close();
	bool Send(char* _sInputStr);
	bool Recv(char* _sRecvStr);


private:

	//通信パラメーターを初期化する関数
	bool InitializeCommParam();
	//出力バッファと入力バッファを初期化する関数
	bool ClearBuffer();
	//通信デバイスを構成する関数
	bool SetCommDevice();
	//タイムアウトパラメーターを設定する関数
	bool SetTimeoutParam();
	
//------------------------------------------------------------------------------------------------------



//-------------------------------------------------メンバ変数-------------------------------------------------
private:
	//Send側のハンドル
	HANDLE m_hSendPort;

	//Recv側のハンドル
	HANDLE m_hRecvPort;

	//DCB構造体を格納する変数
	DCB m_dcb;

	//COMMTIMEOUTS構造体を格納する変数
	COMMTIMEOUTS m_timeout;

	//送信するデータのサイズ
	DWORD m_dwSendBufferSize;

	//受信するデータのサイズ
	DWORD m_dwRecvBufferSize;

	//ポートが開いているかのフラグ 開かれている場合はtrue
	bool m_isOpenPort;

	//Windows API関数の戻り値を格納する変数 エラーの場合はtrue
	bool m_isApiError;


	//------------------------------------------------------------------------------------------------------
};


//-------------------------------------------------外部関数-------------------------------------------------
//送信する文字列を入力する関数
void InputSendStr();

//ポート番号を入力する関数
void InputPortNum();

//受信したデータを出力する関数
void OutputData();

//------------------------------------------------------------------------------------------------------


//-------------------------------------------------外部変数-------------------------------------------------

//送信側のポートの数字
int iOpenSendPortNum;

//受信側のポートの数字
int iOpenRecvPortNum;

char sOpenSendPortName[MAX_ELEMENTS];
char sOpenRecvPortName[MAX_ELEMENTS];

//入力された文字列
char sInputStr[MAX_ELEMENTS];

//受信する文字列
char sRecvStr[MAX_ELEMENTS];

//プログラムを終了するかのフラグ
bool isInputEnd;

//外部関数の戻り値がエラーであるか エラーの場合はtrue
bool isFuncError;


//------------------------------------------------------------------------------------------------------
