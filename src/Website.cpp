#include "Website.h"

Website::Website() {
    mRoutes["/"] = [](const HTTPRequest&) {
        return "<h1>hello, srvr!</h1>";
    };
    
    m404 = [](const HTTPRequest&) {
        return "<h1>404</h1><br><p>route not found</p>";
    };
}

std::string Website::getRoute(const std::string& path, const HTTPRequest& request) const {
    auto it = mRoutes.find(path);
    if (it == mRoutes.end()) {
        return m404(request);
    }
    return it->second(request);
}
