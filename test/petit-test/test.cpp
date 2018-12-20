#include <iostream>
#include "test.hpp"
using namespace std;

int main() {
    cout << "hello" << endl;
    return 0;
}

/* TESTING PARTS */

CircularQueue(size_t init_size) {
    size = init_size;
    data = new uint_8[size];
    front = 0;
    rear = 1;   // point to the next of the last
    _num_free_bytes = size;
}

~CircularQueue() {
    delete[] data;
}

bool CircularQueue::enqueue(const vector<uint_8> &v) {
    if (_num_free_bytes < v.size()) {
        //LOG(Debug) << "queue overflow" << endl;
        cout  << "DEBUG: queue overflow" << endl;
        return false;
    }
    for (const auto &el: v) {
        _num_free_bytes -= 1;
        data[rear] = el;
        rear = (rear++) % size;
    }
    return true;
}

vector<uint_8> CircularQueue::dequeue(size_t dequeue_size) {
    if (_num_free_bytes + dequeue_size > size) {
        //LOG(Debug) << "queue underflow" << endl;
        cout << "DEBUG: queue underflow" << endl;
        return false;
    }
    vector<uint_8> v;
    v.reserve(dequeue_size);
    for (size_t i = 0; i < dequeue_size; i++) {
        _num_free_bytes += 1;
        v.push_back(data[(front++) % size]);
    }
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
    return _num_free_bytes == size;
}

bool CircularQueue::is_full() {
    return _num_free_bytes == 0;
}
