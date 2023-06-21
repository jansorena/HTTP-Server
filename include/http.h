#ifndef HTTP_H
#define HTTP_H

#include <string>

std::string generateResponse(const std::string& status, const std::string& mimeType, const std::string& content,bool redirect, const std::string& redirectURL);
std::string getMimeType(const std::string& filePath);
void sendResponse(std::string path, std::string code, int sock, bool redirect, std::string redirectURL);
void sendFile(std::string filePath, int sock);
std::string method(std::string request);
std::string path(std::string request);
std::string HTTPVersion(std::string request);
bool checkMethod(std::string, int sock);
bool checkHTTP(std::string versionString, int sock);
std::string fileRoute(std::string path, int sock);


#endif