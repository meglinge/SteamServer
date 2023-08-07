#include <string>

class Engine
{
private:
    /* data */
    STEAM_GAMESERVER_CALLBACK(Engine, OnNetConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);

public:
    Engine(/* args */);
    ~Engine();

    bool SendMessage(std::string message);

    void CreateListenSocketP2P();

    void ConnectP2PSocket(SteamNetworkingIdentity steamIDRemote);

    void ReceiveMessage();

    int Init();

    void Update();

    void Shutdown();

    std::unique_ptr<std::thread> callback_thread;
    HSteamListenSocket m_hListenSocketServer = 0;
    HSteamNetConnection m_hListenSocketClient = 0;
    HSteamNetConnection m_hConnection = 0;
};