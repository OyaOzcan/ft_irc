to_sting fonksiyonu

/*std::string Server::toString(int value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}*/


//server.hppye ctime eklenmeli

 ping işlevi
/*

void Server :: PING(Client &client) {
    if (this->commands.size() < 2) {
        client.print(":" + this->getServerIP() + " 409 " + client.getNick() + " :No origin specified\r\n");
        return;
    }
    client.print("PONG :" + this->commands[1] + "\r\n");
}

---> message.cpp'ye eklenmeli ---> &Server::PING*/

notice için

/*
void Server :: NOTICE(Client &client) {
    if (this->commands.size() < 3) {
        // NOTICE komutunu sessizce yok say
        return;
    }

    std::string target = this->commands[1];
    std::string message = this->commands[2].substr(1);

    // Eğer hedef bir kanal veya kullanıcı ise loglayabiliriz
    std::cout << "NOTICE from " << client.getNick() << " to " << target << ": " << message << std::endl;
}

 ---> message.cpp'ye eklenmeli ---> &Server::NOTICE
*/
