#include "HTTPRequest.h"

HTTPRequest::HTTPRequest(const std::string& data) {
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
