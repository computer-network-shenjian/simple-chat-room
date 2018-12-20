class CircularQueue {

public:
    CircularQueue(size_t init_size);
    ~CircularQueue();

    bool enqueue(const std::vector<uint_8> &v);
    std::vector<uint_8> dequeue(size_t dequeue_size);

    // Also requires a getter method for _num_free_bytes here.
    size_t get_num_free_bytes();
    size_t get_size();
    bool is_empty();
    bool is_full();

private:
    size_t size;
    uint8_t *data;
    size_t _num_free_bytes;
    size_t front, rear;
};