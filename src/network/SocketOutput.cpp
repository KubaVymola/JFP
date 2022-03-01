#include "SocketOutput.h"

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <iomanip>


SocketOutput::SocketOutput(const int port, const int precision) {
    this->port = port;
    this->precision = precision;
    
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) return;

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

    // TODO make address a CLI option
    // Convert address to binary form
    if (!inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)) return;

    // Connect to the server
    if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) return;

    std::cout << "Socket connected!" << std::endl;
}

SocketOutput::~SocketOutput() {
    this->Close();
}

void SocketOutput::Close(void) {
    close(socket_fd);
}

void SocketOutput::Clear(void) {
    buffer.str(std::string());
}

void SocketOutput::Clear(const std::string& s) {
    this->Clear();
    buffer << s << ' ';
}

void SocketOutput::Append(const double data) {
    if (buffer.tellp() > 0) buffer << ',';
    buffer << std::setw(12) << std::setprecision(precision) << data; 
}

void SocketOutput::Append(const long data) {
    if (buffer.tellp() > 0) buffer << ',';
    buffer << std::setw(12) << data;
}

void SocketOutput::Append(const char * data) {
    if (buffer.tellp() > 0) buffer << ',';
    buffer << data;
}

void SocketOutput::Send(void) {
    buffer << '\n';
    std::string toSend = buffer.str();

    // if ()
    send(socket_fd, toSend.c_str(), toSend.size(), 0);
}

void SocketOutput::Send(const char * data, int length) {
    send(socket_fd, data, length, 0);
}
