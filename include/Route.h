#pragma once

#include "Website.h"

#include <functional>

class Route {
public:
    Route(const std::string& path, const Website::RouteProvider& provider) {
        Website::getInstance()->setRoute(path, provider);
    }
    
    Route(const std::string& path, const std::function<std::string()>& provider) {
        Website::getInstance()->setRoute(path, [provider](const HTTPRequest&) {
            return provider();
        });
    }
};

#define TOKENPASTE(x, y) x ## y
#define CONCAT(x, y) TOKENPASTE(x, y)

#define ROUTE(path, function) static Route CONCAT(route, __LINE__) (path, function)
