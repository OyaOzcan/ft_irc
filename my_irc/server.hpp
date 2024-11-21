#ifndef SERVER_HPP
#define SERVER_HPP
// Maksimum bağlantı sayısı
#define MAX_CLIENTS 1024

#include "common.hpp"


// Sunucu sınıfı
class Server {
private:
    int server_fd; // Sunucu soketi
    std::vector<pollfd> poll_fds; // pollfd listesi
    std::map<std::string, Channel> channels; // Kanal adından kanala erişim

public:
    Server();
    ~Server();
    void setup(int port); // Sunucuyu başlat
    void run();           // Sunucu döngüsü
    void handleJoinCommand(int client_fd, const std::string& channel_name);
    void removeClient(int client_fd);
    void handleWhoisCommand(int client_fd, const std::string& target);
    void handleWhoCommand(int client_fd);
    void handleListCommand(int client_fd);
    void handleTopicCommand(int client_fd, const std::string& channel_name, const std::string& topic);
    void handleInviteCommand(int client_fd, const std::string& target);
    void handleModeCommand(int client_fd, const std::string& channel_name, const std::string& mode);
    void handlePartCommand(int client_fd, const std::string& channel_name);
    void handleNamesCommand(int client_fd, const std::string& channel_name);
    void handleKickCommand(int client_fd, const std::string& target);


    // getter işlemleri
    const std::map<std::string, Channel>& getChannels() const;
    std::map<std::string, Channel>& getChannels();

};

#endif
