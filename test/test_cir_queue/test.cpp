#include "test.hpp"
using namespace std;

int main() {
    vector<uint8_t> v;
    for (int i = 0; i < 4; i++) {
        v.push_back('a' + i);
    }
    cout << "v.size():\t" << v.size() << endl;
    CircularQueue q(10);
    cout << "queue size:\t" << q.get_size() << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    // enqueue test
    cout << "enqueue:\t" << q.enqueue(v) << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    cout << "enqueue:\t" << q.enqueue(v) << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    cout << "enqueue:\t" << q.enqueue(v) << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    // dequeue test
    vector<uint8_t> v1;
    v1 = q.dequeue(3);
    cout << "v1:\t";
    for (const auto &el: v1) {
        cout << el;
    }
    cout << endl;
    cout << "v1.size():\t" << v1.size() << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    v1 = q.dequeue(3);
    cout << "v1:\t";
    for (const auto &el: v1) {
        cout << el;
    }
    cout << endl;
    cout << "v1.size():\t" << v1.size() << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    v1 = q.dequeue(3);
    cout << "v1:\t";
    for (const auto &el: v1) {
        cout << el;
    }
    cout << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    // enqueue again
    cout << "enqueue:\t" << q.enqueue(v) << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    cout << "enqueue:\t" << q.enqueue(v) << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    cout << "enqueue:\t" << q.enqueue(v) << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;
    // dequeue again
    v1 = q.dequeue(3);
    cout << "v1:\t";
    for (const auto &el: v1) {
        cout << el;
    }
    cout << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    return 0;
}

/* TESTING PARTS */

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
        //LOG(Debug) << "queue overflow" << endl;
        cerr << "DEBUG: queue overflow" << endl;
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
        //LOG(Debug) << "queue underflow" << endl;
        cerr << "DEBUG: queue underflow" << endl;
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

size_t CircularQueue::get_size() {
    return size;
}

bool CircularQueue::is_empty() {
    return _num_free_bytes == size;
}

bool CircularQueue::is_full() {
    return _num_free_bytes == 0;
}
