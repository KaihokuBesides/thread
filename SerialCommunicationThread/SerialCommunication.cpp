// SerialCommunication.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "SerialCommunication.h"

using namespace std;


//送信側のポートの数字
int iOpenSendPortNum = 0;

//受信側のポートの数字
int iOpenRecvPortNum = 0;

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


CComPort::CComPort(void)
	: m_hSendPort(NULL)
	, m_hRecvPort(NULL)
	, m_dcb(m_dcb)
	, m_timeout(m_timeout)
	, m_dwSendBufferSize(0)
	, m_dwRecvBufferSize(0)
	, m_isOpenPort(false)
	, m_isApiError(false)
{


}


CComPort::~CComPort()
{

}


int main()
{
	isInputEnd = false;


	//Endが入力されるまで、繰り返す。
	while (isInputEnd == false)
	{
		CComPort comport1;

		//外部変数を初期化
		isFuncError = false;
		iOpenSendPortNum = 0;
		iOpenRecvPortNum = 0;

		memset(sOpenSendPortName, 0, sizeof(sOpenSendPortName));
		memset(sOpenRecvPortName, 0, sizeof(sOpenRecvPortName));
		memset(sInputStr, 0, sizeof(sInputStr));
		memset(sRecvStr, 0, sizeof(sRecvStr));


		//送信する文字列を入力する関数
		InputSendStr();

		//Endが入力されたらプログラムを終了
		if (isInputEnd == true)
		{
			cout << "プログラムを終了します。" << endl;
			break;
		}
		//ポート番号を入力する関数
		InputPortNum();

		//ポートが開けるかどうかチェックする関数を呼び出す
		isFuncError = comport1.CheckOpenPort(iOpenSendPortNum, iOpenRecvPortNum);

		//ポートを開く関数を呼び出す
		if (isFuncError == false)
		{
			isFuncError = comport1.Open(sOpenSendPortName, sOpenRecvPortName);
		}

		//データを送信する関数を呼び出す
		if (isFuncError == false)
		{
			isFuncError = comport1.Send(sInputStr);
		}
		
		//データを受信する関数を呼び出す
		if (isFuncError == false)
		{
			isFuncError = comport1.Recv(sRecvStr);
		}

		if (isFuncError == false)
		{
			//受信した文字列を出力
			OutputData();
		}

		//ポートを閉じる
		isFuncError = comport1.Close();
	}
}



// func		:ポートが開いているかのフラグを返す関数
// brief	:
// param	:
// return	:ポートが開いているかのフラグ
bool CComPort::GetStatus()
{
	return m_isOpenPort;
}


// func		:送信する文字列を入力する関数
// brief	:
// param	:
// return	:
void InputSendStr()
{
	cout << endl << "送信するデータを入力していください。" << endl
		<< "プログラムを終了する場合は「End」を入力してください。" << endl;

	//送信するデータを入力
	cin >> setw(MAX_ELEMENTS) >> sInputStr;
	cin.clear();
	cin.ignore(1000, '\n');


	if (strcmp(sInputStr, "End") == 0)
	{
		isInputEnd = true;
		return;
	}
}


// func		:ポート番号を入力する関数
// brief	:
// param	:
// return	:
void InputPortNum()
{
	//cinの例外処理を有効にします。
	cin.exceptions(ios::failbit);

	//送信側のポート番号を入力
	cout << "開くポートの番号を入力していください。" << endl;
	while (true)
	{
		try
		{
			string tempStr = " ";
			istringstream ss1;


			cout << "送信側：";
			cin >> tempStr;

			//COM+番号で入力されたら番号だけを取得する
			if (tempStr[0] == 'C' && tempStr[1] == 'O' && tempStr[2] == 'M' && isdigit(tempStr[3]) != 0)
			{
				ss1 = istringstream(tempStr.substr(3));
				ss1 >> iOpenSendPortNum;
			}
			//文字が入力されたらエラー
			else if (!std::all_of(tempStr.cbegin(), tempStr.cend(), isdigit))
			{
				throw "書式エラー";
			}
			//数値のみの入力をiOpenSendPortNumに代入
			else
			{
				ss1 = istringstream(tempStr);
				ss1 >> iOpenSendPortNum;
			}
			//iOpenSendPortNumがマイナスの時
			if (iOpenSendPortNum < 0)
			{
				throw "マイナス";
			}


			cout << "受信側：";
			cin >> tempStr;

			//COM+番号で入力されたら番号だけを取得する
			if (size(tempStr) >= 4 && tempStr[0] == 'C' && tempStr[1] == 'O' && tempStr[2] == 'M')
			{
				ss1 = istringstream(tempStr.substr(3));
				ss1 >> iOpenRecvPortNum;
			}
			//文字が入力されたらエラー
			else if (!std::all_of(tempStr.cbegin(), tempStr.cend(), isdigit))
			{
				throw "書式エラー";
			}
			//数値のみの入力をiOpenRecvPortNumに代入
			else
			{
				ss1 = istringstream(tempStr);
				ss1 >> iOpenRecvPortNum;
			}
			//iOpenRecvPortNumがマイナスの時
			if (iOpenRecvPortNum < 0)
			{
				throw "マイナス";
			}
			cout << endl;
		}
		catch (...)
		{
			cout << "不正な値が入力されました。再入力していください。" << endl;
			cin.clear();
			cin.ignore(1000, '\n');
			continue;
		}
		break;
	}

	//入力されたポート番号を"COM"とくっつける
	sprintf(sOpenSendPortName, "\\\\.\\COM%d", iOpenSendPortNum);
	sprintf(sOpenRecvPortName, "\\\\.\\COM%d", iOpenRecvPortNum);
}



// func		:受信したデータを出力する関数
// brief	:
// param	:
// return	:
void OutputData()
{
	cout << "受信したデータ :" << sRecvStr << endl;
}


// func		:受信したデータを出力する関数
// brief	:
// param	:(_sOpenSendPortName) 送信側のポート名、(_sOpenRecvPortName)受信側のポート名
// return	:関数が成功したかのフラグ trueの場合は失敗、falseの場合は成功
bool CComPort::Open(char* _sOpenSendPortName, char* _sOpenRecvPortName)
{
	//ポートが開いているなら関数失敗
	if (GetStatus() == true)
	{
		return true;
	}


	//送信側のポートを開く
	m_hSendPort = CreateFile(
		_sOpenSendPortName,
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);


	//関数が失敗したらtrueを返す
	if (m_hSendPort == INVALID_HANDLE_VALUE)
	{
		cout << "送信側のポートが開けませんでした。" << endl;
		return true;
	}


	//受信側のポートを開く
	m_hRecvPort = CreateFile(
		_sOpenRecvPortName,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);


	//関数が失敗したらtrueを返す。
	if (m_hRecvPort == INVALID_HANDLE_VALUE)
	{
		cout << "受信側のポートが開けませんでした。" << endl;
		return true;
	}
	else
	{
		//ポートが開いているかのフラグをtrueにする
		m_isOpenPort = true;	
	}



	//通信パラメーターを初期化する関数を呼び出して、戻り値がtrueだったらtrueを返す。
	if (InitializeCommParam() == true)
	{
		return true;
	}


	//出力バッファと入力バッファを初期化する関数を呼び出して、戻り値がtrueだったらtrueを返す。
	if (ClearBuffer() == true)
	{
		return true;
	}


	//通信デバイスを構成する関数を呼び出して、戻り値がtrueだったらtrueを返す。
	if (SetCommDevice() == true)
	{
		return true;
	}


	//タイムアウトパラメーターを設定する関数を呼び出して、戻り値がtrueだったらtrueを返す。
	if (SetTimeoutParam() == true)
	{
		return true;
	}


	//ここまでエラーが無かったので、関数成功としてfalseを返す
	return false;
}


// func		:開きたいポートが設定されているかチェックする関数
// brief	:
// param	:(_iOpenSendPortNum) 送信側のポート番号、 (_iOpenRecvPortNum) 受信側のポート番号
// return	:関数が成功したかのフラグ trueの場合は失敗、falseの場合は成功
bool CComPort::CheckOpenPort(int _iOpenSendPortNum, int _iOpenRecvPortNum)
{
	//ポートが開いているなら関数失敗
	if (GetStatus() == true)
	{
		return true;
	}


	unsigned long resPort;                               //ポートを取得しようとした結果
	constexpr size_t uPortNumbersCount = 10;			//10個分検索をする
	unsigned long lpPortNumbers[uPortNumbersCount]{ 0 }; //取得したポート番号の配列
	unsigned long puPortNumbersFound;                    //取得したポートの数
	bool isHit = false;									//検索結果を格納する変数

	//設定されているポート番号を取得 lpPortNumbersに格納
	resPort = GetCommPorts(lpPortNumbers, uPortNumbersCount, &puPortNumbersFound);

	

	//送信側のポート番号を検索
	isHit = find(lpPortNumbers, lpPortNumbers + uPortNumbersCount, _iOpenSendPortNum)
		!= lpPortNumbers + uPortNumbersCount;


	//ポートが設定されていない場合はtrueを返す。
	if (isHit == false)
	{
		cout << "送信側のポートが設定されていません。" << endl;
		return true;
	}


	//受信側のポート番号を検索
	isHit = find(lpPortNumbers, lpPortNumbers + uPortNumbersCount, _iOpenRecvPortNum)
		!= lpPortNumbers + uPortNumbersCount;


	//ポートが 設定されている場合はfalseを返す。設定されていない場合はtrueを返す。
	if (isHit == true)
	{
		return false;
	}
	else
	{
		cout << "受信側のポートが設定されていません。" << endl;
		return true;
	}
}


// func		:ポートを閉じる関数
// brief	:
// param	:
// return	:関数が成功したかのフラグ trueの場合は失敗、falseの場合は成功
bool CComPort::Close()
{
	//ポートが開いていないなら関数失敗
	if (GetStatus() != true)
	{
		return true;
	}


	//送信側のポートを閉じる
	m_isApiError = CloseHandle(
		m_hSendPort
	);

	//ポートが閉じられない場合はtrueを返す。
	if (m_isApiError == 0)
	{
		cout << "ポートが閉じられませんでした。。" << endl;
		return true;
	}


	//受信側のポートを閉じる
	m_isApiError = CloseHandle(
		m_hRecvPort
	);


	//ポートが閉じられない場合はtrueを返す。閉じられた場合はfalseを返す。
	if (m_isApiError == 0)
	{
		cout << "ポートが閉じられませんでした。。" << endl;
		return true;
	}
	else
	{
		return false;
	}

}


// func		:データを送信する関数
// brief	:
// param	:(_sInputStr)送信するデータ
// return	:関数が成功したかのフラグ trueの場合は失敗、falseの場合は成功
bool CComPort::Send(char* _sInputStr)
{
	//ポートが開いていないなら関数失敗
	if (GetStatus() != true)
	{
		return true;
	}

	//送信するデータをチェックする処理
	if (strlen(_sInputStr) > BUFFER_SIZE)
	{
		cout << "バッファの制限サイズを超えています。" << endl;
		cout << "WriteFile関数が失敗しました。" << endl;
		return true;
	}

	//データを送信する
	m_isApiError = WriteFile(
		m_hSendPort,
		_sInputStr,
		strlen(_sInputStr),
		&m_dwSendBufferSize,
		NULL
	);

	//データの送信に失敗したので、trueを返す
	if (m_isApiError == 0)
	{
		cout << "WriteFile関数が失敗しました。" << endl;
		return true;
	}
	//データの送信に成功したので、falseを返す
	else
	{
		return false;
	}
}


// func		:データを受信する関数
// brief	:
// param	:(_sRecvStr)受信したデータを格納するアドレス
// return	:関数が成功したかのフラグ trueの場合は失敗、falseの場合は成功
bool CComPort::Recv(char* _sRecvStr)
{
	//ポートが開いていないなら関数失敗
	if (GetStatus() != true)
	{
		return true;
	}

	
	DWORD dwErrors = 0;
	COMSTAT ComStat;
	DWORD dwCount = 0;

	//受信するデータのバイト数を取得
	ClearCommError(m_hRecvPort, &dwErrors, &ComStat);
	dwCount = ComStat.cbInQue;


	//受信するデータのサイズをチェックする処理
	if (dwCount > BUFFER_SIZE)
	{
		cout << "バッファの制限サイズを超えています。" << endl;
		cout << "ReadFile関数が失敗しました。" << endl;
		return true;
	}
	else if (dwCount == 0)
	{
		cout << "データの受信に失敗しました。ポートのペアリングを確認してください。" << endl;
		cout << "ReadFile関数が失敗しました。" << endl;
		return true;
	}


	//データを受信する
	m_isApiError = ReadFile(
		m_hRecvPort,
		_sRecvStr,
		dwCount,
		&m_dwRecvBufferSize,
		NULL
	);


	//データの受信に失敗したので、trueを返す
	if (m_isApiError == 0)
	{
		cout << "ReadFile関数が失敗しました。" << endl;
		return true;
	}
	//データの受信に成功したので、falseを返す
	else
	{
		return false;
	}
}



// func		:通信パラメーターを初期化する関数
// brief	:
// param	:
// return	:関数が成功したかのフラグ trueの場合は失敗、falseの場合は成功
bool CComPort::InitializeCommParam()
{
	//ポートが開いていないなら関数失敗
	if (GetStatus() != true)
	{
		return true;
	}

	//送信側の通信パラメーターを初期化
	m_isApiError = SetupComm(
		m_hSendPort,
		BUFFER_SIZE,
		BUFFER_SIZE
	);

	//通信パラメーターの初期化に失敗したのでtrueを返す
	if (m_isApiError == 0)
	{
		cout << "SetupComm関数が失敗しました。" << endl;
		return true;
	}


	//受信側の通信パラメーターを初期化
	m_isApiError = SetupComm(
		m_hRecvPort,
		BUFFER_SIZE,
		BUFFER_SIZE
	);


	//通信パラメーターの初期化に失敗したのでtrueを返す
	if (m_isApiError == 0)
	{
		cout << "SetupComm関数が失敗しました。" << endl;
		return true;
	}
	//通信パラメーターの初期化に成功したのでfalseを返す
	else
	{
		return false;
	}
}



// func		:出力バッファと入力バッファを初期化する関数
// brief	:
// param	:
// return	:関数が成功したかのフラグ trueの場合は失敗、falseの場合は成功
bool CComPort::ClearBuffer()
{
	//ポートが開いていないなら関数失敗
	if (GetStatus() != true)
	{
		return true;
	}

	//送信側の出力バッファと入力バッファを初期化する
	m_isApiError = PurgeComm(
		m_hSendPort,
		PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
	);


	//出力バッファと入力バッファの初期化に失敗したのでtrueを返す
	if (m_isApiError == 0)
	{
		cout << "PurgeComm関数が失敗しました。" << endl;
		return true;
	}


	//受信側の出力バッファと入力バッファを初期化する
	m_isApiError = PurgeComm(
		m_hRecvPort,
		PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
	);


	//出力バッファと入力バッファの初期化に失敗したのでtreuを返す
	if (m_isApiError == 0)
	{
		cout << "PurgeComm関数が失敗しました。" << endl;
		return true;
	}
	//出力バッファと入力バッファの初期化に成功したのでfalseを返す
	else
	{
		return false;
	}
}


// func		:通信デバイスを構成する関数
// brief	:
// param	:
// return	:関数が成功したかのフラグ trueの場合は失敗、falseの場合は成功
bool CComPort::SetCommDevice()
{
	//ポートが開いていないなら関数失敗
	if (GetStatus() != true)
	{
		return true;
	}

	//dcb構造体のパラメーターを取得
	m_isApiError = GetCommState(
		m_hSendPort,
		&m_dcb
	);


	//dcb構造体のパラメーターを取得に失敗したのでtreuを返す
	if (m_isApiError == 0)
	{
		cout << "GetCommState関数が失敗しました。" << endl;
		return true;
	}


	//dcb構造体のパラメーターを一部変更する。
	m_dcb.BaudRate = COMMUNICATION_SPEED;
	m_dcb.fBinary = TRUE;
	m_dcb.ByteSize = BIT_SIZE;
	m_dcb.Parity = NOPARITY;
	m_dcb.StopBits = ONESTOPBIT;


	//変更したdcb構造体のパラメーターを送信側のポートにセットする。
	m_isApiError = SetCommState(
		m_hSendPort,
		&m_dcb
	);

	//dcb構造体のパラメーターのセットに失敗したのでtrueを返す
	if (m_isApiError == 0)
	{
		cout << "SetCommState関数が失敗しました。" << endl;
		return true;
	}


	//変更したdcb構造体のパラメーターを受信側のポートにセットする。
	m_isApiError = SetCommState(
		m_hRecvPort,
		&m_dcb
	);


	//dcb構造体のパラメーターのセットに失敗したのでtrueを返す
	if (m_isApiError == 0)
	{
		cout << "SetCommState関数が失敗しました。" << endl;
		return true;
	}
	//dcb構造体のパラメーターのセットに成功したのでtrueを返す
	else
	{
		return false;
	}
}


// func		:タイムアウトパラメーターを設定する関数
// brief	:
// param	:
// return	:関数が成功したかのフラグ trueの場合は失敗、falseの場合は成功
bool CComPort::SetTimeoutParam()
{
	//ポートが開いていないなら関数失敗
	if (GetStatus() != true)
	{
		return true;
	}

	//タイムアウトパラメーターを変更する。
	m_timeout.ReadIntervalTimeout = WAIT_TIMEOUT_MILLISECOND;
	
	m_timeout.ReadTotalTimeoutMultiplier = 0;
	m_timeout.ReadTotalTimeoutConstant = WAIT_TIMEOUT_MILLISECOND;

	m_timeout.WriteTotalTimeoutMultiplier = 0;
	m_timeout.WriteTotalTimeoutConstant = WAIT_TIMEOUT_MILLISECOND;


	//送信側のポートにタイムアウトパラメーターをセットする。
	m_isApiError = SetCommTimeouts(
		m_hSendPort,
		&m_timeout
	);


	//タイムアウトパラメーターのセットに失敗したのでtrueを返す
	if (m_isApiError == 0)
	{
		cout << "SetTimeoutParam関数が失敗しました。" << endl;
		return true;
	}


	//受信側のポートにタイムアウトパラメーターをセットする。
	m_isApiError = SetCommTimeouts(
		m_hRecvPort,
		&m_timeout
	);


	//タイムアウトパラメーターのセットに失敗したのでtrueを返す
	if (m_isApiError == 0)
	{
		cout << "SetTimeoutParam関数が失敗しました。" << endl;
		return true;
	}
	//タイムアウトパラメーターのセットに成功したのでfalseを返す
	else
	{
		return false;
	}
}
