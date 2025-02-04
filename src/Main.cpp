#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <unordered_map>

#include <sys/socket.h>
#include <netinet/in.h>

class HTTPRequest {
public:
    HTTPRequest(const std::string& data) {
        // first word before space is the method
        size_t wordDelimiter = data.find(' ');
        mMethod = data.substr(0, wordDelimiter);
        
        // second word before space is the path
        size_t pathDelimiter = data.find(' ', wordDelimiter + 1);
        mPath = data.substr(wordDelimiter + 1, pathDelimiter - wordDelimiter - 1);
        
        // third word before newline is the version
        size_t versionDelimiter = data.find('\n', pathDelimiter + 1);
        mVersion = data.substr(pathDelimiter + 1, versionDelimiter - pathDelimiter - 1);
    }
    
    [[nodiscard]] const std::string& getMethod() const {
        return mMethod;
    }
    
    [[nodiscard]] const std::string& getPath() const {
        return mPath;
    }
    
    [[nodiscard]] const std::string& getVersion() const {
        return mVersion;
    }
    
private:
    std::string mMethod;
    std::string mPath;
    std::string mVersion;
    std::unordered_map<std::string, std::string> mHeaders;
    std::string mBody;
};

int main() {
    const int socketType = SOCK_STREAM;
    const int socketDomain = PF_INET; // IPv4
    const int socketProtocol = 0; // automatic
    
    const int socketHandle = socket(socketDomain, socketType, socketProtocol);
    
    sockaddr_in socketAddress;
    socketAddress.sin_family = socketDomain;
    socketAddress.sin_addr.s_addr = INADDR_ANY;
    socketAddress.sin_port = htons(1113);
    
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
        
        // TODO: Dynamic buffer size
        std::vector<std::byte> buffer(10240);
        std::memset(buffer.data(), 0, buffer.size());
        ssize_t bytesRead = recv(establishedConnection, buffer.data(), buffer.size(), 0);
        
        std::cout << "received: " << bytesRead << " bytes" << std::endl;
        buffer[bytesRead] = std::byte{0};
        if (bytesRead < 1) {
            continue;
        }
        
        std::string data(reinterpret_cast<char*>(buffer.data()));
        HTTPRequest request(data);
        
        // Respond!
        std::string body = "";
        body += "<h1>hello, srvr!</h1>";
        body += "<br> <strong>Your request: </strong>" + request.getMethod() + " " + request.getPath() + " " + request.getVersion();
        
        const std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(body.size()) + "\n\n" + body; 
        
        send(establishedConnection, response.c_str(), response.size(), 0);
    }
    
    return 0;
}
