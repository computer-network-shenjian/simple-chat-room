#include <unordered_set>
#include "types.hpp"

// Session layer maintains a set of active client sessions.
// From the lower layer, CircularQueues are passed through this layer, creating new sessions
// if a new connection is established
class SessionLayer {
    Client* session_set;

public:
    SessionLayer() = default; // TODO: Allocate session_set to have size kSessionSetSize
    ~SessionLayer();


    // Public methods called by the lower layer (transfer layer).

    // This method process a raw frame starting from the head of the circular queue
    // Implementation node: Because the session layer knows the set of active client sessions,
    // this method should automatically match source_socket_fd in argument
    // with the corresponding active client session.
    // The queue is passed as pointer (newed) because the memory should be reused and referenced
    // by the client instance
    void process_packet(const int source_socket_fd, const CircularQueue *q);

    // Returns a client bound to source_socket_fd, creating a new client if not found in session set
    Client* get_client_instance(const int source_socket_fd);


    // Public methods called by the high layer (presentation layer)

    // Send a packet to the target client bound to target_socket_fd
    void send_packet(const int target_socket_fd, const DataPacket packet);

}
