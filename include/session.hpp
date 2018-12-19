#include <unordered_set>
#include "types.hpp"

class SessionLayer {
    unordered_set<Client> session_set;

public:
    SessionLayer() = default;
    ~SessionLayer();

    // Public methods called by the lower layer (transfer layer).

    // This method process a raw frame starting from the head of the circular queue
    // Implementation node: Because the session layer knows the set of active client sessions,
    // this method should automatically match source_socket_fd in argument
    // with the corresponding active client session.
    void process_packet(const int source_socket_fd, const CircularQueue &q);


    // Public methods called by the high layer (presentation layer)

    // Send a packet to the target client bound to target_socket_fd
    void send_packet(const int target_socket_fd, const DataPacket packet);

}
