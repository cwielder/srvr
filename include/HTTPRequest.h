#pragma once

#include <unordered_map>
#include <string>

class HTTPRequest {
public:
    HTTPRequest(const std::string& data);
    ~HTTPRequest() = default;
    
    [[nodiscard]] const std::string& getMethod() const { return mMethod; }
    [[nodiscard]] const std::string& getPath() const { return mPath; }
    [[nodiscard]] const std::string& getVersion() const { return mVersion; }
    
private:
    std::string mMethod;
    std::string mPath;
    std::string mVersion;
    std::unordered_map<std::string, std::string> mHeaders;
    std::string mBody;
};
