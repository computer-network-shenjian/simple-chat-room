enum class packet_type: uint8_t {
    info = 0x00,
    infoRespond = 0x01,
    passwd = 0x02,
    passwdRespond = 0x03,
    refuse = 0x04,
    conf = 0x05,
    historyUserName = 0x06,
    history = 0x07,
    syncEnd = 0x08,
    textUser = 0x09,
    text = 0x0A,
    fileName = 0x0B,
    fileInProgress = 0x0C,
    groupTextUserlist = 0x0D,
    fileEnd = 0x0E
};
struct data_packet {
    packet_type type;
    std::vector<uint8_t> data;
};
