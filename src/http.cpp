#include <string>
#include <sstream>

std::string generateResponse(const std::string& status, const std::string& mimeType, const std::string& content) {
    std::stringstream response;
    response << "HTTP/1.1 " << status << "\r\n";
    response << "Content-Length: " << content.length() << "\r\n";
    response << "Connection: close\r\n";
    response << "Content-Type: " << mimeType << "\r\n";
    response << "\r\n";
    response << content;
    return response.str();
}

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


std::string method(std::string request){
    std::size_t methodEnd = request.find(' ');
    return request.substr(0, methodEnd);
}

std::string path(std::string request){
    std::stringstream ss(request);
    std::string path;
    ss >> path; // GET
    ss >> path; // path
    return path;
}

std::string HTTPVersion(std::string request){
    std::stringstream ss(request);
    std::string version;
    ss >> version; // GET
    ss >> version; // path
    ss >> version; // HTTPVersion
    return version;
}

std::string fileRoute(std::string path){
    std::string basePath = "/home/jansorena/development/Redes-Lab2/http";
    
    if(path == "/" || path == "/index.html") {
        basePath += "/index.html";
        return basePath;
    }
    
    if(path.back() == '/'){
        return basePath+path+"index.html";
    }

    return basePath+path;
}

