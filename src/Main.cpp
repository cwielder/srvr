#include <iostream>
#include <memory>
#include <vector>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    const int socketType = SOCK_STREAM;
    const int socketDomain = PF_INET; // IPv4
    const int socketProtocol = 0; // automatic
    
    const int socketHandle = socket(socketDomain, socketType, socketProtocol);
    
    sockaddr_in socketAddress;
    socketAddress.sin_family = socketDomain;
    socketAddress.sin_addr.s_addr = INADDR_ANY;
    socketAddress.sin_port = htons(8080);
    
    int result = bind(socketHandle, reinterpret_cast<const sockaddr*>(&socketAddress), sizeof(socketAddress));
    if (result < 0) {
        std::cerr << "bind failed" << std::endl;
        return 1;
    }
    
    result = listen(socketHandle, 5);
    if (result < 0) {
        std::cerr << "listen failed" << std::endl;
        return 1;
    }
    
    std::cout << "srvr started..." << std::endl;
    
    while (true) {
        sockaddr_in clientAddress;
        socklen_t clientAddressSize = sizeof(clientAddress);
        int establishedConnection = accept(socketHandle, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);
        if (establishedConnection < 0) {
            std::cerr << "accept failed" << std::endl;
            return 1;
        }
        
        std::vector<std::byte> buffer(10240);
        std::memset(buffer.data(), 0, buffer.size());
        ssize_t bytesRead = recv(establishedConnection, buffer.data(), buffer.size(), 0);
        if (bytesRead < 1) {
            std::cerr << "recv failed" << std::endl;
            return 1;
        }
        
        std::cout << "received: " << bytesRead << " bytes" << std::endl;
        buffer[bytesRead] = std::byte{0};
        std::cout << "data: " << std::string(reinterpret_cast<char*>(buffer.data()), bytesRead) << std::endl;
    }
    
    return 0;
}
