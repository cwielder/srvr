#pragma once

class HTTPServer {
public:
    void start(int port);
    
private:
    void accept() const;
    
private:
    int mSocket;
};
