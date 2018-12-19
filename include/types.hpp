enum class PacketType: uint8_t {
    Info = 0x00,
    InfoRespond = 0x01,
    Passwd = 0x02,
    PasswdRespond = 0x03,
    Refuse = 0x04,
    Conf = 0x05,
    HistoryUserName = 0x06,
    History = 0x07,
    SyncEnd = 0x08,
    TextUsername = 0x09,
    Text = 0x0A,
    FileName = 0x0B,
    FileInProgress = 0x0C,
    GroupTextUserlist = 0x0D,
    FileEnd = 0x0E
};

struct DataPacket {
    Packet_type type;
    std::vector<uint8_t> data;
};
