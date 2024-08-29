
// SerialCommunicationThread.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CSerialCommunicationThreadApp:
// このクラスの実装については、SerialCommunicationThread.cpp を参照してください
//

class CSerialCommunicationThreadApp : public CWinApp
{
public:
	CSerialCommunicationThreadApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CSerialCommunicationThreadApp theApp;
