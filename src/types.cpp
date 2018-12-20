#include "../include/types.hpp"

CircularQueue(size_t init_size) {
    size = init_size;
    data = new uint_8[size];
    front = 0;
    rear = 1;   // point to the next of the last
    _num_free_bytes = 0;
}

// TODO: destructor

bool CircularQueue::enqueue(const vector<uint_8> &v) {
    if (is_full() || _num_free_bytes + v.size() > size) {
        LOG(Debug) << "queue overflow" << endl;
        return false;
    }
    _num_free_bytes += v.size();
    // TODO: insert vector into buffer, use iterator?
    for (const auto &el: v) {
        data[rear] = el;
        rear = (rear + 1) % size;
    }
    //rear = (rear + v.size()) % size;
    return true;
}

vector<uint_8> CircularQueue::dequeue(size_t dequeue_size) {
    if (is_empty() || _num_free_bytes < dequeue_size) {
        LOG(Info) << "queue underflow" << endl;
        return false;
    }
    _num_used_bytes -= dequeue_size;
    // TODO: get chars to vector from buffer, use iterator?
    
    vector<uint_8> v;
    v.reserve(dequeue_size);
    for (size_t i = 0; i < dequeue_size; i++) {
        v.push_back(data[(front++) % size]);
    }
    front--; // undo the last increment
    front %= size; // wrap around

    return v;
}

size_t CircularQueue::get_num_free_bytes() {
    return _num_free_bytes;
}

size_t CircularQueue::get_size() {
    return size;
}

bool CircularQueue::is_empty() {
    return _num_free_bytes == 0;
}

bool CircularQueue::is_full() {
    return _num_free_bytes == size;
}
