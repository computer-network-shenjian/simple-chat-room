#include <algorithm>    // std::max
#include <map>
#include <set>
#include "types.hpp"
#include "utils.hpp"

#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/prctl.h>

class TransferLayer {

public:
    TransferLayer() = default;
    ~TransferLayer();

    // main loop. May never return.
    void select_loop(int listener);

    // send a packet to client
    StatusCode send_to_client(int target_socket_fd, const DataPacket packet);

    // kick client if password error
    StatusCode remove_client(Client &client);

    // Try to read as much as possible from the client's socket_fd and write to the 
    // underlying circular queue buffer.
    //
    // Precondition: client's socket_fd is readable 
    // Postcondition: the buffer is possibly left with packet data only partial received
    bool try_recv(const Client &client);

    bool try_send(const Client &client);

    // find client by client_id in session_set
    bool is_client_active(int client_id);

private:

    list<Client> session_set;

    // loop accept client
    StatusCode loop_accept_client();

    int reset_rw_fd_sets(fd_set &read_fds, fd_set &write_fds);

    // If false, return -1, else return socket code.
    int get_listener(const ServerConf &conf);

};

