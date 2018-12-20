#include "../include/transfer.hpp"

using namespace std;

bool TransferLayer::try_recv(Client &client) {
    uint8_t tmp_buffer[kMaxPacketLength] {};

    // at least receive more than header so we can determine the data length
    if (client.recv_buffer.size() < kHeaderSize) { // doesn't have header currently
        int received_bytes = 0;
        while (client.recv_buffer.size() < kHeaderSize) {
            int num_bytes = recv(client.socket_fd, tmp_buffer, client.pending_recv_size, 0);
            // error handling
            if (num_bytes <= 0) {
                return StatusCode::RecvError;
            }

            // recv correct
            received_bytes += num_bytes;
            client.recv_buffer.enqueue(tmp_buffer, received_bytes); // TODO
        }
        client.pending_recv_size = client.recv_buffer.current_packet_size();
    } else { // 

    }
}

bool TransferLayer::try_send(Client &client) {

}
