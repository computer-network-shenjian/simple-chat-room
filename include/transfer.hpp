
class TransferLayer {

public:
    TransferLayer() = default;
    ~TransferLayer();

    // send a packet to data
    void send_to_client(Client &target_client, const DataPacket packet);

private:
    
};
