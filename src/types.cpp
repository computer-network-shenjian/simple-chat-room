#include "../include/types.hpp"

class CircularQueue {
    uint8_t *data;
    size_t _num_free_bytes;

public:
    CircularQueue(size_t size);
    ~CircularQueue();

    bool enqueue(const vector<uint_8> &v);
    vector<uint_8> dequeue(size_t size);

    // Also requires a getter method for _num_free_bytes here.

private:

};
