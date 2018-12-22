#include "types.hpp" 
#include <stdint.h> // uint8_t
#include <vector>
#include <queue>

class Client {

    int client_id;

    CircularQueue recv_buffer;
    std::queue< std::vector<uint8_t> > send_buffer;

    int socket_fd;
    SessionState state = SessionState::Acceptance;

public:
    // should be always greater than kHeaderSize (reset to this)
    // updated when packet is received and on state change

    Client(int socket_fd, size_t buffer_size) : 
        socket_fd(socket_fd),
        recv_buffer(buffer_size), 
        send_buffer(buffer_size)
    {}

    ~Client(); // Should call the destructor of the underlying CircularQueue

    void change_state (SessionState target_state);

private:

};
