#ifndef HTTP_H
#define HTTP_H

#include <string>

std::string generateResponse(const std::string& status, const std::string& mimeType, const std::string& content);
std::string getMimeType(const std::string& filePath);
std::string method(std::string request);
std::string path(std::string request);
std::string HTTPVersion(std::string request);
std::string fileRoute(std::string path);

#endif