#include <stdint.h> // uint8_t
#include <vector> // uint8_t

class CircularQueue {
    uint8)t *data;
    size_t num_free_bytes;

public:
    CircularQueue(size_t size);
    ~CircularQueue();

    bool enqueue(const vector<uint_8> &v);
    vector<uint_8> dequeue(size_t size);

private:

};

class Client {
    CircularQueue recv_buffer;
    CircularQueue send_buffer;

public:
    Client(size_t buffer_size) : 
        recv_buffer(buffer_size), 
        send_buffer(buffer_size)
    {}

    ~Client();

private:

};
