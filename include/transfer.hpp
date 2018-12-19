class TransferLayer {

public:
    TransferLayer() = default;
    ~TransferLayer();

    // send a packet to data
    StatusCode send_to_client(int target_socket_fd, const DataPacket packet);
    // kick client if password error
    StatusCode kick_client(int target_client_fd);

private:
    int server_fd;
    // init socket, with conf
    StatusCode socket_init(const Conf config);
    // loop accept client
    StatusCode loop_accept_client();
};
