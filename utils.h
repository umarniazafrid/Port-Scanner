#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <unistd.h>

inline std::string grabBanner(int sock) {
    char buffer[1024];
    std::string banner = "";
    send(sock, "\r\n", 2, 0); // provoke response
    int n = recv(sock, buffer, sizeof(buffer)-1, 0);
    if (n > 0) {
        buffer[n] = '\0';
        banner = "(" + std::string(buffer) + ")";
    }
    return banner;
}

#endif
