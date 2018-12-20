#include <algorithm>    // std::max

class TransferLayer {

public:
    TransferLayer() = default;
    ~TransferLayer();

    // send a packet to data
    StatusCode send_to_client(int target_socket_fd, const DataPacket packet);
    StatusCode send_to_client(int target_socket_fd, const DataPacket packet);

    // kick client if password error
    StatusCode kick_client(int target_client_fd);

    // Try to read as much as possible from the client's socket_fd and write to the 
    // underlying circular queue buffer.
    //
    // Precondition: client's socket_fd is readable and there doesn't exist a unprocessed
    //  complete packet in the client's buffer
    // Postcondition: the data in buffer is possibly left at the middle of a packet
    bool try_recv(Client &client);

    bool try_send(Client &client);


private:
    int server_fd;
    // init socket, with conf
    StatusCode socket_init(const Conf config);
    // loop accept client
    StatusCode loop_accept_client();
};
