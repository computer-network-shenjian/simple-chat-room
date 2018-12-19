#include "types.hpp" 
#include <stdint.h> // uint8_t
#include <vector> // uint8_t

class Client {
    CircularQueue recv_buffer;
    CircularQueue send_buffer;

    int socket_fd;
    SessionState state = SessionState::Acceptance;

public:
    Client(int socket_fd, size_t buffer_size) : 
        socket_fd(socket_fd),
        recv_buffer(buffer_size), 
        send_buffer(buffer_size)
    {}

    ~Client();

private:

};
