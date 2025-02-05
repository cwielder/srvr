#include "HTTPServer.h"

#include "Route.h"

ROUTE("/", []() {
    return "<h1>homepage</h1>";
});

ROUTE("/about", []() {
    return "<h1>about</h1>";
});

int main() {
    HTTPServer server;
    server.start(9999);
    
    return 0;
}
