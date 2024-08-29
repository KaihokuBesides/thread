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

//-------------------------------------------------�����֐�-------------------------------------------------
	
	//�|�[�g���J���Ă��邩�̃t���O��Ԃ��֐�
	bool GetStatus();

	//�|�[�g���J����֐�
	bool Open(char* _sOpenSendPortName, char* _sOpenRecvPortName);
	//���͂��ꂽ�|�[�g�ԍ����J���邩�m�F����
	bool CheckOpenPort(int _iOpenSendPortNum, int _iOpenRecvPortNum);
	bool Close();
	bool Send(char* _sInputStr);
	bool Recv(char* _sRecvStr);


private:

	//�ʐM�p�����[�^�[������������֐�
	bool InitializeCommParam();
	//�o�̓o�b�t�@�Ɠ��̓o�b�t�@������������֐�
	bool ClearBuffer();
	//�ʐM�f�o�C�X���\������֐�
	bool SetCommDevice();
	//�^�C���A�E�g�p�����[�^�[��ݒ肷��֐�
	bool SetTimeoutParam();
	
//------------------------------------------------------------------------------------------------------



//-------------------------------------------------�����o�ϐ�-------------------------------------------------
private:
	//Send���̃n���h��
	HANDLE m_hSendPort;

	//Recv���̃n���h��
	HANDLE m_hRecvPort;

	//DCB�\���̂��i�[����ϐ�
	DCB m_dcb;

	//COMMTIMEOUTS�\���̂��i�[����ϐ�
	COMMTIMEOUTS m_timeout;

	//���M����f�[�^�̃T�C�Y
	DWORD m_dwSendBufferSize;

	//��M����f�[�^�̃T�C�Y
	DWORD m_dwRecvBufferSize;

	//�|�[�g���J���Ă��邩�̃t���O �J����Ă���ꍇ��true
	bool m_isOpenPort;

	//Windows API�֐��̖߂�l���i�[����ϐ� �G���[�̏ꍇ��true
	bool m_isApiError;


	//------------------------------------------------------------------------------------------------------
};


//-------------------------------------------------�O���֐�-------------------------------------------------
//���M���镶�������͂���֐�
void InputSendStr();

//�|�[�g�ԍ�����͂���֐�
void InputPortNum();

//��M�����f�[�^���o�͂���֐�
void OutputData();

//------------------------------------------------------------------------------------------------------


//-------------------------------------------------�O���ϐ�-------------------------------------------------

//���M���̃|�[�g�̐���
int iOpenSendPortNum;

//��M���̃|�[�g�̐���
int iOpenRecvPortNum;

char sOpenSendPortName[MAX_ELEMENTS];
char sOpenRecvPortName[MAX_ELEMENTS];

//���͂��ꂽ������
char sInputStr[MAX_ELEMENTS];

//��M���镶����
char sRecvStr[MAX_ELEMENTS];

//�v���O�������I�����邩�̃t���O
bool isInputEnd;

//�O���֐��̖߂�l���G���[�ł��邩 �G���[�̏ꍇ��true
bool isFuncError;


//------------------------------------------------------------------------------------------------------
