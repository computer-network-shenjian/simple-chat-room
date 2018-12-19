
class TransferLayer {

public:
    TransferLayer() = default;
    ~TransferLayer();

    // send a packet to data
    void send_to_client(int target_socket_fd, const DataPacket packet);

private:
    
};
