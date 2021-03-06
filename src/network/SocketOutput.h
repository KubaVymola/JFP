#ifndef __SOCKET_OUTPUT_H__
#define __SOCKET_OUTPUT_H__

#include <string.h>
#include <sstream>

class SocketOutput {
public:
    SocketOutput(const int port, const int precision = 6);
    ~SocketOutput();
    bool CanSend(void) const;
    void Close(void);
    void Clear(void);
    void Clear(const std::string& s);
    void Append(const double data);
    void Append(const long data);
    void Append(const char * data);
    void Send(void);
    void Send(const char * data, int length);
private:
    bool socketConnected = false;

    int precision;
    int port;
    int socketFd;
    std::ostringstream buffer;
};

#endif