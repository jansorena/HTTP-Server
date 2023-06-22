#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "tcp.h"

// Generar respuesta a la request
std::string generateResponse(const std::string& status,
                             const std::string& mimeType,
                             const std::string& content, bool redirect,
                             const std::string& redirectURL) {
  std::stringstream response;
  response << "HTTP/1.1 " << status << "\r\n";
  response << "Content-Length: " << content.length() << "\r\n";
  response << "Connection: close\r\n";
  response << "Content-Type: " << mimeType << "\r\n";
  if (redirect) {
    response << "Location: " << redirectURL << "\r\n";
  }
  response << "\r\n";
  response << content;
  return response.str();
}

// Obtener MIME
std::string getMimeType(const std::string& filePath) {
  std::size_t dotPos = filePath.find_last_of('.');
  if (dotPos != std::string::npos) {
    std::string extension = filePath.substr(dotPos + 1);
    if (extension == "html" || extension == "htm") {
      return "text/html";
    } else if (extension == "css") {
      return "text/css";
    } else if (extension == "js") {
      return "application/javascript";
    } else if (extension == "png") {
      return "image/png";
    } else if (extension == "jpeg" || extension == "jpg") {
      return "image/jpeg";
    }
  }
  return "application/octet-stream";
}

// Enviar respuesta
void sendResponse(std::string path, std::string code, int sock, bool redirect,
                  std::string redirectURL) {
  std::ifstream file(path);
  std::string content;
  std::string responseContent;
  std::ostringstream contentStream;

  contentStream << file.rdbuf();
  responseContent = contentStream.str();
  std::string mimeType = getMimeType(path);
  std::string response =
      generateResponse(code, mimeType, responseContent, redirect, redirectURL);
  tcp_send(sock, response.c_str(), response.length());
}

// Enviar archivo solicitado
void sendFile(std::string filePath, int sock) {
  std::ifstream file(filePath);
  if (file) {
    sendResponse(filePath, "200 OK", sock, false, "");
  } else {
    sendResponse("/home/jansorena/development/Redes-Lab2/error-pages/404.html",
                 "404 Not Found", sock, false, "");
  }
}

// Obtener metodo de la request
std::string method(std::string request) {
  std::size_t methodEnd = request.find(' ');
  return request.substr(0, methodEnd);
}

// Obtener ruta de la request
std::string path(std::string request) {
  std::stringstream ss(request);
  std::string path;
  ss >> path;  // GET
  ss >> path;  // path
  return path;
}

// Obtener version HTTP de la request
std::string HTTPVersion(std::string request) {
  std::stringstream ss(request);
  std::string version;
  ss >> version;  // GET
  ss >> version;  // path
  ss >> version;  // HTTPVersion
  return version;
}

// Verificar metodo de la request
bool checkMethod(std::string method, int sock) {
  if (method == "GET")
    return true;
  else if (method == "BREW") {
    sendResponse("/home/jansorena/development/Redes-Lab2/error-pages/418.html",
                 "418 I'm a teapot", sock, false, "");
    return false;
  } else {
    sendResponse("/home/jansorena/development/Redes-Lab2/error-pages/405.html",
                 "405 Method Not Allowed", sock, false, "");
    return false;
  }
}

// Verificar HTTP de la request
bool checkHTTP(std::string versionString, int sock) {
  if (versionString == "HTTP/1.1")
    return true;
  sendResponse("/home/jansorena/development/Redes-Lab2/error-pages/505.html",
               "505 HTTP Version Not Supported", sock, false, "");
  return false;
}

// Parsear la ruta
std::string fileRoute(std::string path, int sock) {
  if (path[0] != '/') {
    sendResponse("/home/jansorena/development/Redes-Lab2/error-pages/400.html",
                 "400 Bad Request", sock, false, "");
    return "400";
  }

  std::string basePath = "/home/jansorena/development/Redes-Lab2/http";

  if (path == "/" || path == "/index.html") {
    basePath += "/index.html";
    return basePath;
  }

  if (path.back() == '/') {
    return basePath + path + "index.html";
  }

  if (path.back() != '/' && path.find_last_of('.') == std::string::npos) {
    std::string redirectURL = "http://localhost:10000" + path + "/";
    sendResponse("/home/jansorena/development/Redes-Lab2/error-pages/301.html",
                 "301 Moved Permanently", sock, true, redirectURL);
    return "301";
  }

  return basePath + path;
}
