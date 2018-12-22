#include "../include/transfer.hpp"

using namespace std;

StatusCode TransferLayer::try_recv(const Client &client) {
    uint8_t tmp_buffer[kMaxPacketLength] {};

    // at least receive more than header so we can determine the data length
    if (client.recv_buffer.size() < kHeaderSize) { // doesn't have header currently
        int received_bytes = 0;
        while (client.recv_buffer.size() < kHeaderSize) {
            int num_bytes = recv(client.socket_fd, tmp_buffer, client.recv_buffer.get_num_free_bytes, 0);
            // error handling
            if (num_bytes <= 0) {
                LOG(Error) << "RecvError 1\n";
                return StatusCode::RecvError;
            } else {
                // recv correct
                received_bytes += num_bytes;
                client.recv_buffer.enqueue(tmp_buffer, received_bytes); // TODO
            }
        }
    } 

    if (!client.recv_buffer.is_full() && client.recv_buffer.current_packet_size()) {
        int num_bytes = recv(client.socket_fd, tmp_buffer, client.recv_buffer.get_num_free_bytes(), 0);
        // error handling
        if (num_bytes < 0) {
            LOG(Error) << "RecvError 2\n";
            return StatusCode::RecvError;
        } else {
            client.recv_buffer.enqueue(tmp_buffer, num_bytes); 
        }
    }

    return StatusCode::OK;
}

StatusCode TransferLayer::try_send(const Client &client) {
    vector<uint8_t> &v = client.send_buffer.front();
    size_t size_before = v.size();
    int num_bytes = send(client.socket_fd, v.data(), size_before, MSG_NOSIGNAL);
    if (num_bytes <= 0) {
        LOG(Error) << "SendError, send returned " << num_bytes << endl;
        return StatusCode::SendError;
    }

    if (num_bytes < size_before) {
        // partial send
        v.erase(v.begin(), v.begin() + num_bytes);
    } else { // complete send
        client.send_buffer.pop();
    }

    return StatusCode::OK;
}

void TransferLayer::select_loop(int listener, const PresentationLayer &presentation_layer) {
    fd_set read_fds, write_fds;

    for (;;) {
        int fdmax = reset_rw_fd_sets(fd_set &read_fds, fd_set &write_fds);
        FD_SET(listener, read_fds); // also listen for new connections

        int rv = select(fdmax+1, read_fds, write_fds, NULL, NULL);
        switch (rv) {
            case -1:
                LOG(Error) << "Select in main loop\n";
                break;
            case 0;
                // TODO: remove sockets that haven't responded in certain amount of time, exept for listener socket
                break;
            default:
                // firstly, iterate through map and process clients in session
                for (auto &el : session_set) {
                    if (FD_ISSET(el.socket_fd, &read_fds)) {
<<<<<<< HEAD
                        if (try_recv(el) != StatusCode::OK) {
                            presentation_layer.transfer_to_presentation(el);
                        } else {
                            // remove client here
                        }
                    }
                    
                    if (FD_ISSET(el.socket_fd, &write_fds)) {
                        if (try_send(el) != StatusCode::OK) {
                            presentation_layer.transfer_to_presentation(el);
                            if (el.state == SessionState::Error) {
                                // remove client
                            }
                        } else {
                            // remove client here
                        }
                    }
                }

                // and lastly, check for new connections 
                if (FD_ISSET(listener, &read_fds)) {
                    accept_new_client(listener);
                }
                
                break;
        } // end of switch
    } // end of main loop
}

int TransferLayer::reset_rw_fd_sets(fd_set &read_fds, fd_set &write_fds) {
    int maxfd = 0;
    for (const Client &client : session_set) {
        // set read_fds if have enough buffer size to receive at least the header
        if (client.recv_buffer.get_num_free_bytes() > kHeaderSize) {
            FD_SET(client.socket_fd, &read_fds);
            maxfd = max(maxfd, client_socket_fd);
        }

        // set write_fds if has data in send_buffer
        if (!client.send_buffer.is_empty()) {
            FD_SET(client.socket_fd, &write_fds);
            maxfd = max(maxfd, client_socket_fd);
        }
    }
    return maxfd;
}

// helper function
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

StatusCode accept_new_client(int listener, list<Client> &session_set) {
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen = sizeof(remoteaddr);

    int newfd = accept(listener, (sockaddr *) &remoteaddr, &addrlen);
    
    if (newfd == -1) {
        LOG(Error) << "accept\n";
        return StatusCode::Accept;
    } else {
        // set non-blocking connection
        int val = fcntl(newfd, F_GETFL, 0);
        if (val < 0) {
            close(newfd);
            LOG(Error) << "fcntl, GETFL\n";
        }
        if (fcntl(newfd, F_SETFL, val|O_NONBLOCK) < 0) {
            close(newfd);
            LOG(Error) << "fcntl, SETFL\n";
        }

        session_set.emplace_back(newfd, kRecvBufferSize);

        char remoteIP[INET6_ADDRSTRLEN];
        std::cout << "New connection from " << inet_ntop(remoteaddr.ss_family,
                get_in_addr((struct sockaddr*) &remoteaddr),
                remoteIP, INET6_ADDRSTRLEN)
            << " on socket " << newfd << std::endl;
    }
    return newfd;
}

bool is_client_active(int client_id) {
    return find_if(session_set.begin(), session_set.end(), 
            [client_id](const Client &c){ return c.client_id = client_id; });
}

int TransferLayer::get_listener(const ServerConf &conf) {
    // AF_INET: IPv4 protocol
    // SOCK_STREAM: TCP protocol
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        LOG(Error) << "Server socket init error" << endl;
        graceful_return("socket", StatusCode::CreateSocket);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        LOG(Error) << "Server setsockopt error" << endl;
        graceful_return("setsockopt", StatusCode::Setsockopt);
    }

    int flags = fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, flags|O_NONBLOCK);

    struct sockaddr_in server_addr; 
    server_addr.sin_family = AF_INET; 
    // INADDR_ANY means 0.0.0.0(localhost), or all IP of local machine.
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(conf.port); 
    int server_addrlen = sizeof(server_addr);
    if (bind(server_fd, (struct sockaddr *) &server_addr, server_addrlen) < 0) {
        LOG(Error) << "Server bind error" << endl;
        graceful_return("bind", StatusCode::Bind);
    }

    if (listen(server_fd, 10) < 0) {
        LOG(Error) << "Server listen error" << endl;
        graceful_return("listen", StatusCode::Listen); 
    }
    LOG(Info) << "Server socket init ok with port: " << conf.port << endl;
    return server_fd;
}

