#include <cstdio>
#include <thread>
#include <iostream>
#include <steam_gameserver.h>
#include "engine.hpp"

void Engine::CreateListenSocketP2P()
{

	m_hListenSocketServer = SteamGameServerNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);
}
void Engine::ConnectP2PSocket(SteamNetworkingIdentity steamIDRemote)
{
	m_hListenSocketClient = SteamGameServerNetworkingSockets()->ConnectP2P(steamIDRemote, 0, 0, nullptr);
}

bool Engine::SendMessage(std::string message)
{
	printf("Message: %s\n", message.c_str());
	EResult res = SteamGameServerNetworkingSockets()->SendMessageToConnection(m_hListenSocketClient, message.data(), message.length() + 1, k_nSteamNetworkingSend_Reliable, 0);
	switch (res)
	{
	case k_EResultOK:
	case k_EResultIgnored:
		break;

	case k_EResultInvalidParam:
		printf("Failed sending data to server: Invalid connection handle, or the individual message is too big\n");
		return false;
	case k_EResultInvalidState:
		printf("Failed sending data to server: Connection is in an invalid state\n");
		return false;
	case k_EResultNoConnection:
		printf("Failed sending data to server: Connection has ended\n");
		return false;
	case k_EResultLimitExceeded:
		printf("Failed sending data to server: There was already too much data queued to be sent\n");
		return false;
	default:
	{
		char msg[256];
		sprintf(msg, "SendMessageToConnection returned %d\n", res);
		printf(msg);
		return false;
	}
	}
}
void Engine::OnNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t *pCallback)
{
	std::cout << "状态改变:" << pCallback->m_info.m_eState << std::endl;
	if (pCallback->m_info.m_eState == k_EResultOK)
	{
		SteamGameServerNetworkingSockets()->AcceptConnection(pCallback->m_hConn);
		m_hConnection = pCallback->m_hConn;
	}
	std::cout << "延迟:" << SteamNetworkingUtils()->GetDirectPingToPOP(pCallback->m_info.m_idPOPRemote) << std::endl;
}
// 初始化
int Engine::Init()
{
	if (!SteamGameServer_Init(0x7f000001, 16801, 16802, eServerModeAuthentication, "1.0"))
	{
		printf("SteamGameServer_Init failed\n");
		return 0;
	}
	printf("SteamGameServer_Init successed\n");
	if (SteamGameServer())
	{

		SteamGameServer()->SetProduct("SteamworksExample");
		SteamGameServer()->SetGameDescription("Steamworks Example");
		SteamGameServer()->LogOnAnonymous();
		SteamNetworkingUtils()->InitRelayNetworkAccess();
	}
	return 1;
}

void Engine::Update()
{
	callback_thread = std::make_unique<std::thread>([&]()
													{
		uint64 id = SteamGameServer()->GetSteamID().ConvertToUint64();
		while(true){
			SteamGameServer_RunCallbacks();
			CSteamID serverSteamID = SteamGameServer_GetSteamID();
			if (id != serverSteamID.ConvertToUint64())
			{
				printf("Server SteamID: %llu\n", serverSteamID.ConvertToUint64());
				id = serverSteamID.ConvertToUint64();
			}



			if (m_hConnection != 0)
			{
				SteamNetworkingMessage_t *pMsg[32];
				int numMsgs = SteamGameServerNetworkingSockets()->ReceiveMessagesOnConnection(m_hConnection, pMsg, 32);
				if (numMsgs > 0)
				{
					printf("numMsgs: %d\n", numMsgs);
					for (size_t i = 0; i < numMsgs; i++)
					{
						SteamNetworkingMessage_t *nets = pMsg[i];
						std::cout << "消息: " << static_cast<const char *>(nets->m_pData) << std::endl;
						nets->Release();
					}
					
				}
			}
			

			
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		} });
}

void Engine::Shutdown()
{
	printf("exit!\n");
	SteamGameServerNetworkingSockets()->CloseListenSocket(m_hListenSocketServer);
	SteamGameServer()->LogOff();
	SteamGameServer_Shutdown();
}

Engine::Engine()
{
	if (Init())
	{
		Update();
	}
}

Engine::~Engine()
{
	Shutdown();
}

int main()
{

	// 创建一个线程以100hz的频率调用SteamGameServer_RunCallbacks
	auto engine = std::make_unique<Engine>();

	engine->CreateListenSocketP2P();

	uint64 idS;
	std::cin >> idS;
	CSteamID steamID(idS);
	SteamNetworkingIdentity identity;
	identity.SetSteamID(steamID);
	engine->ConnectP2PSocket(identity);

	auto t = std::make_unique<std::thread>([&]()
										   {
		for (size_t i = 0; i < 1000; i++)
		{
			engine->SendMessage("SFSFSFSFSF");
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		} });

	std::cin.get();
	std::cin.get();
	std::cin.get();

	return 0;
}