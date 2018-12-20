#include "../include/types.hpp"

CircularQueue::CircularQueue(size_t init_size) {
    size = init_size;
    data = new uint8_t[size];
    front = 0;
    rear = 1;   // point to the next of the last
    _num_free_bytes = size;
}

CircularQueue::~CircularQueue() {
    delete[] data;
}

bool CircularQueue::enqueue(const vector<uint8_t> &v) {
    if (_num_free_bytes < v.size()) {
        LOG(Debug) << "queue overflow" << endl;
        //cerr << "DEBUG: queue overflow" << endl;
        return false;
    }
    for (const auto &el: v) {
        _num_free_bytes -= 1;
        data[rear] = el;
        rear = (rear + 1) % size;
    }
    return true;
}

// dequeue: If underflow, return with size 0.
vector<uint8_t> CircularQueue::dequeue(size_t dequeue_size) {
    vector<uint8_t> v;
    if (_num_free_bytes + dequeue_size > size) {
        LOG(Debug) << "queue underflow" << endl;
        //cerr << "DEBUG: queue underflow" << endl;
        return v;
    }

    v.reserve(dequeue_size);
    for (size_t i = 0; i < dequeue_size; i++) {
        _num_free_bytes += 1;
        front = (front + 1) % size;
        v.push_back(data[front % size]);
    }
    return v;
}

size_t CircularQueue::get_num_free_bytes() {
    return _num_free_bytes;
}

size_t CircularQueue::size() {
    return size;
}

bool CircularQueue::is_empty() {
    return _num_free_bytes == size;
}

bool CircularQueue::is_full() {
    return _num_free_bytes == 0;
}
