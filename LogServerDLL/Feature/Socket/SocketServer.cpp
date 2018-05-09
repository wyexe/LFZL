#include "SocketServer.h"
#include <SocketCommon/SocketBuffer.h>
#include <Feature/Socket/Client/RemoteClient.h>
#include <Common/Common.h>
#include <LogLib/Log.h>
#include <Feature/EchoPacket/EchoPacket.h>

#define _SELF L"SocketServer.cpp"

CSocketServer::CSocketServer()
{
	::InitializeCriticalSection(&_LockVecClient);
}

CSocketServer::~CSocketServer()
{
	Stop();
	if (_hWorkThread != NULL)
	{
		::WaitForSingleObject(_hWorkThread, INFINITE);
		_hWorkThread = NULL;
	}

	::DeleteCriticalSection(&_LockVecClient);
}

CSocketServer& CSocketServer::GetInstance()
{
	static CSocketServer Instance;
	return Instance;
}

libTools::CSocketRemoteClient* CSocketServer::CreateNewSocketClient(_In_ UINT_PTR ClientSock)
{
	return new CRemoteClient(ClientSock);
}

VOID CSocketServer::ReleaseSocketClient(_In_ libTools::CSocketRemoteClient* pSocketClient)
{
	pSocketClient->DisConnect();
	pSocketClient->Remove();
	delete dynamic_cast<CRemoteClient *>(pSocketClient);
	pSocketClient = nullptr;
}

BOOL CSocketServer::EchoRecive(_In_ libTools::CSocketRemoteClient* pSocketClient, _In_ libTools::CSocketBuffer* pSocketBuffer)
{
	em_Sock_Msg emSockMsg;
	// Test
	libTools::CSocketBuffer tmpSocketBuffer = *pSocketBuffer;
	auto Error = pSocketBuffer->VerDataPtr(emSockMsg);
	if (Error != libTools::CSocketBuffer::em_SocketBuffer_Error_None)
	{
		LOG_MSG_CF(L"���ݰ�Ч�����!. Err=%d", Error);

		DWORD dwSize, dwValue1, dwValue2, dwValue3 = 0;
		tmpSocketBuffer >> dwSize >> dwValue1 >> dwValue2 >> dwValue3;
		LOG_CF_E(L"dwSize=%d, dwValue1=%X, dwValue2=%X,dwValue3=%X, BufferSize=%d", dwSize, dwValue1, dwValue2, dwValue3, tmpSocketBuffer.size());
		return FALSE;
	}


	CRemoteClient* pRemoteClient = dynamic_cast<CRemoteClient*>(pSocketClient);
	switch (emSockMsg)
	{
	case em_Sock_Msg_KeepLive:
		CEchoPacket::KeepALive(pRemoteClient, pSocketBuffer);
		break;
	case em_Sock_Msg_Log:
		CEchoPacket::SendLog(pRemoteClient, pSocketBuffer);
		break;
	case em_Sock_Msg_DownLoad_File:
		CEchoPacket::KeepALive(pRemoteClient, pSocketBuffer);
		PostSend(pSocketClient, pSocketBuffer);
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL CSocketServer::RunServer()
{
	if (!Run(12345, 1000))
	{
		return FALSE;
	}


	_hWorkThread = ::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_WorkThread, this, NULL, NULL);
	return TRUE;
}


std::wstring CSocketServer::GetOnLineClientArrayText()
{
	std::wstring wsText;
	::EnterCriticalSection(&_LockVecClient);
	for (auto& itm : _VecClient)
	{
		if (itm->IsOnLine())
		{
			wsText += itm->GetClientName();
			wsText += L"\r\n";
		}
	}
	::LeaveCriticalSection(&_LockVecClient);
	return wsText;
}

DWORD WINAPI CSocketServer::_WorkThread(LPVOID lpParam)
{
	CSocketServer* pSocketServer = reinterpret_cast<CSocketServer *>(lpParam);
	while (pSocketServer->_bRun)
	{
		::EnterCriticalSection(&pSocketServer->_LockVecClient);
		for (CONST auto& itm : pSocketServer->_VecClient)
		{
			if (itm->InExist() && itm->IsKeepALiveTimeout())
			{
				itm->DisConnect();
			}
		}

		::LeaveCriticalSection(&pSocketServer->_LockVecClient);
		::Sleep(10 * 1000);
	}
	return 0;
}
