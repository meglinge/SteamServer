#include <cstdio>
#include <thread>
#include <iostream>
#include <steam_gameserver.h>
#include "engine.hpp"
#include <sstream>
#include <vector>
#include <spdlog/spdlog.h>
void Engine::CreateListenSocketP2P() {
     m_hListenSocketServer = SteamGameServerNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);
}
void Engine::ConnectP2PSocket(uint64 steamID64) {
     CSteamID                steamID(steamID64);
     SteamNetworkingIdentity identity;
     identity.SetSteamID(steamID);
     m_hListenSocketClient = SteamGameServerNetworkingSockets()->ConnectP2P(identity, 0, 0, nullptr);
}

bool Engine::SendMessage(std::string message) {
     spdlog::info("Message: {}", message);
     EResult res = SteamGameServerNetworkingSockets()->SendMessageToConnection(
         m_hListenSocketClient, message.data(), message.length() + 1, k_nSteamNetworkingSend_Reliable, 0
     );
     switch (res) {
          case k_EResultOK:
          case k_EResultIgnored: break;

          case k_EResultInvalidParam:
               spdlog::info("Failed sending data to server: Invalid connection handle, or the individual message is too big\n");
               return false;
          case k_EResultInvalidState:
               spdlog::info("Failed sending data to server: Connection is in an invalid state\n");
               return false;
          case k_EResultNoConnection: spdlog::info("Failed sending data to server: Connection has ended\n"); return false;
          case k_EResultLimitExceeded:
               spdlog::info("Failed sending data to server: There was already too much data queued to be sent\n");
               return false;
          default: {
               char msg[256];
               sprintf(msg, "SendMessageToConnection returned %d\n", res);
               spdlog::info(msg);
               return false;
          }
     }
}
void Engine::OnNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pCallback) {
     spdlog::info("状态改变: {}", (int)pCallback->m_info.m_eState);
     if (pCallback->m_info.m_eState == k_EResultOK) {
          SteamGameServerNetworkingSockets()->AcceptConnection(pCallback->m_hConn);
          m_hConnection = pCallback->m_hConn;
     }
     auto ping = SteamNetworkingUtils()->GetDirectPingToPOP(pCallback->m_info.m_idPOPRemote);
     if (ping > 0) {
          spdlog::info("延迟: {}", ping);
     }
}
// 初始化
int Engine::Init() {
     printf("开始初始化");
     if (!SteamGameServer_Init(0x7f000001, 16801, 16802, eServerModeAuthentication, "1.0")) {
          spdlog::info("SteamGameServer_Init failed");
          return 0;
     }

     spdlog::info("SteamGameServer_Init successed");
     if (SteamGameServer()) {

          SteamGameServer()->SetProduct("SteamworksExample");
          SteamGameServer()->SetGameDescription("Steamworks Example");
          SteamGameServer()->LogOnAnonymous();
          SteamNetworkingUtils()->InitRelayNetworkAccess();
     }
     return 1;
}

void Engine::Update() {
     callback_thread = std::make_unique<std::thread>([&]() {
          uint64 id = SteamGameServer()->GetSteamID().ConvertToUint64();
          while (true) {
               SteamGameServer_RunCallbacks();
               serverSteamID = SteamGameServer_GetSteamID();
               if (id != serverSteamID.ConvertToUint64()) {
                    id = serverSteamID.ConvertToUint64();
               }

               if (m_hConnection != 0) {
                    SteamNetworkingMessage_t* pMsg[32];
                    int numMsgs = SteamGameServerNetworkingSockets()->ReceiveMessagesOnConnection(m_hConnection, pMsg, 32);
                    if (numMsgs > 0) {
                         spdlog::info("numMsgs: {}", numMsgs);
                         for (size_t i = 0; i < numMsgs; i++) {
                              SteamNetworkingMessage_t* nets = pMsg[i];
                              spdlog::info("消息: {}", static_cast<const char*>(nets->m_pData));
                              nets->Release();
                         }
                    }
               }

               std::this_thread::sleep_for(std::chrono::milliseconds(10));
          }
     });
}

void Engine::Shutdown() {
     spdlog::info("exit!");
     SteamGameServerNetworkingSockets()->CloseListenSocket(m_hListenSocketServer);
     SteamGameServer()->LogOff();
     SteamGameServer_Shutdown();
}

Engine::Engine() {
     if (Init()) {
          Update();
     }
}

Engine::~Engine() {
     Shutdown();
}

int main() {

     // 创建一个线程以100hz的频率调用SteamGameServer_RunCallbacks
     auto engine = std::make_unique<Engine>();
     while (true) {
          std::cout << ">";
          std::string idS;
          std::getline(std::cin, idS);
          if (idS == "exit") {
               return 0;
          }
          if (idS == "server") {
               engine->CreateListenSocketP2P();
          }
          if (idS == "steamid") {
               spdlog::info("SteamID: {}", engine->serverSteamID.ConvertToUint64());
          }

          if (idS.contains(" ")) {
               auto                     iss = std::istringstream(idS);
               std::vector<std::string> tokens;
               std::string              token;
               while (std::getline(iss, token, ' ')) {
                    tokens.push_back(token);
               }
               if (tokens[0] == "connect") {
                    engine->ConnectP2PSocket(std::stoull(tokens[1]));
               }
               if (tokens[0] == "send") {
                    engine->SendMessage(tokens[1]);
               }
          }
          if (idS == "help") {
               spdlog::info("exit: 退出程序");
               spdlog::info("server: 创建服务器");
               spdlog::info("steamid: 显示服务器SteamID");
               spdlog::info("connect [steamid64]: 连接服务器");
               spdlog::info("send [message]: 发送消息");
          }
     }

     return 0;
}