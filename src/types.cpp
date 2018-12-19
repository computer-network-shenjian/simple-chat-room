#include "../include/types.hpp"

// Used as buffer for transfer layer, instantiated in Clients
class CircularQueue {
    uint8_t *data;
    size_t num_free_bytes;

public:
    CircularQueue(size_t size);
    ~CircularQueue();

    bool enqueue(const vector<uint_8> &v);
    vector<uint_8> dequeue(size_t size);

private:

};
