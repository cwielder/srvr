#pragma once

#include "HTTPRequest.h"
#include <string>
#include <unordered_map>
#include <functional>

class HTTPRequest;

class Website {
private:
    Website();
    
public:
    using RouteProvider = std::function<std::string(const HTTPRequest&)>;
    
public:
    static Website* getInstance() {
        static Website instance;
        return &instance;
    }

    std::string getRoute(const std::string& path, const HTTPRequest& request) const;
    
    void setRoute(const std::string& path, const RouteProvider& provider) {
        mRoutes[path] = provider;
    }
    
private:
    std::unordered_map<std::string, RouteProvider> mRoutes;
    RouteProvider m404;
};
