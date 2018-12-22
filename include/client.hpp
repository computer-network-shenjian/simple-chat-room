#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h> // uint8_t
#include <vector>
#include <queue>

#include "types.hpp" 

class Client {
    CircularQueue recv_buffer;
    std::queue< std::vector<uint8_t> > send_buffer;

    int socket_fd;
    SessionState state = SessionState::Acceptance;
    std::string host_username_;
    Message_To_App message_ptoa;
    Message_To_Pre message_atop;

public:
    // should be always greater than kHeaderSize (reset to this)
    // updated when packet is received and on state change

    Client(int socket_fd, size_t buffer_size) : 
        socket_fd(socket_fd),
        recv_buffer(buffer_size)
    {}

    ~Client(); // Should call the destructor of the underlying CircularQueue

    void change_state (SessionState target_state);

private:

};

#endif
