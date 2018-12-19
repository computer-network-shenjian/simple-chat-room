#include "../include/types.hpp"

CircularQueue(size_t init_size) {
    size = init_size;
    data = new uint_8[size];
    front = 0;
    rear = 1;   // point to the next of the last
    _num_free_bytes = 0;
}

bool CircularQueue::enqueue(const vector<uint_8> &v) {
    if (queue_full() || _num_free_bytes + v.size() > size) {
        LOG(Info) << "queue overflow" << endl;
        return false;
    }
    _num_free_bytes += v.size();
    // TODO: insert vector into buffer, use iterator?
    rear = (rear + v.size()) % size;
    return true;
}

vector<uint_8> CircularQueue::dequeue(size_t dequeue_size) {
    if (queue_empty() || _num_free_bytes < dequeue_size) {
        LOG(Info) << "queue underflow" << endl;
        return false;
    }
    _num_free_bytes -= dequeue_size;
    // TODO: get chars to vector from buffer, use iterator?
    front = (front + dequeue_size) % size;
    return true;
}

size_t CircularQueue::get_num_free_bytes() {
    return _num_free_bytes;
}

size_t CircularQueue::get_size() {
    return size;
}

bool CircularQueue::queue_empty() {
    return _num_free_bytes == 0;
}

bool CircularQueue::queue_full() {
    return _num_free_bytes == size;
}