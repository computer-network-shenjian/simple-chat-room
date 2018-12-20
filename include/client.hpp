#include "types.hpp" 
#include <stdint.h> // uint8_t
#include <vector> // uint8_t

class Client {
    CircularQueue recv_buffer;
    CircularQueue send_buffer;

    int socket_fd;
    SessionState state = SessionState::Acceptance;

public:
    // should be always greater than kHeaderSize (reset to this)
    // updated when packet is received and on state change

    size_t pending_recv_size; 

    Client(int socket_fd, size_t buffer_size) : 
        socket_fd(socket_fd),
        recv_buffer(buffer_size), 
        send_buffer(buffer_size)
    {}

    ~Client(); // Should call the destructor of the underlying CircularQueue

    void change_state (SessionState target_state);

private:

};
