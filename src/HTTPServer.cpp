#include "HTTPServer.h"

#include "HTTPRequest.h"

#include <iostream>
#include <vector>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>

void HTTPServer::start(int port) {
    const int socketType = SOCK_STREAM;
    const int socketDomain = PF_INET; // IPv4
    const int socketProtocol = 0; // automatic
    
    mSocket = socket(socketDomain, socketType, socketProtocol);
    
    sockaddr_in socketAddress;
    socketAddress.sin_family = socketDomain;
    socketAddress.sin_addr.s_addr = INADDR_ANY;
    socketAddress.sin_port = htons(port);
    
    int result = bind(mSocket, reinterpret_cast<const sockaddr*>(&socketAddress), sizeof(socketAddress));
    if (result < 0) {
        std::cerr << "bind failed" << std::endl;
        return;
    }
    
    result = listen(mSocket, 5);
    if (result < 0) {
        std::cerr << "listen failed" << std::endl;
        return;
    }
    
    std::cout << "srvr started..." << std::endl;
    
    while (true) {
        this->accept();
    }
}

void HTTPServer::accept() const {
    sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    int establishedConnection = ::accept(mSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);
    if (establishedConnection < 0) {
        std::cerr << "accept failed" << std::endl;
        return;
    }
    
    // TODO: Dynamic buffer size
    std::vector<std::byte> buffer(10240);
    std::memset(buffer.data(), 0, buffer.size());
    ssize_t bytesRead = recv(establishedConnection, buffer.data(), buffer.size(), 0);
    
    std::cout << "received: " << bytesRead << " bytes" << std::endl;
    buffer[bytesRead] = std::byte{0};
    if (bytesRead < 1) {
        return;
    }
    
    std::string data(reinterpret_cast<char*>(buffer.data()));
    HTTPRequest request(data);
    
    // Respond!
    std::string body = "";
    body += "<h1>hello, srvr!</h1>";
    body += "<br> <strong>Your request: </strong>" + request.getMethod() + " " + request.getPath() + " " + request.getVersion();
    body += "<p>Current time: " + std::to_string(time(nullptr)) + "</p>";
    
    const std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(body.size()) + "\n\n" + body; 
    
    send(establishedConnection, response.c_str(), response.size(), 0);
}
