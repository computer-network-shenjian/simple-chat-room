#include "utils.hpp"

template<std::size_t N>
void generate_init_vector(uint8_t (&IV_buff)[N]) {
    using bytes_randomizer = std::independent_bits_engine<std::default_random_engine, CHAR_BIT, uint8_t>;
    std::default_random_engine rd;
    bytes_randomizer bytes(rd);

    std::generate(std::begin(IV_buff), std::end(IV_buff), std::ref(bytes));
}

void write_bytes_to_file(string fn, uint8_t *ptr_bytes, int num_bytes) {
    ofstream ofile(fn, ios::binary);
    ofile.write(ptr_bytes, num_bytes);
    ofile.close();
}

ErrorCode log_init(std::ofstream &log_stream, const std::string log_name, const Level level) {
    // log_stream must not be opened before calling this function.
    if (log_stream.is_open()) return ErrorCode::LogInit;

    log_stream.open(log_name, ios::out|ios::trunc);
    if (!log_stream.is_open()) return ErrorCode::LogInit;

    Log::get().setLogStream(log_stream);
    Log::get().setLevel(level);
    return ErrorCode::OK;
}
