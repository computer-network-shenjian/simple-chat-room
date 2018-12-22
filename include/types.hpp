#ifndef TYPES_HPP
#define TYPES_HPP


#include <vector>
#include <arpa/inet.h>
#include <stdint.h>
#include <string>

const int MaxHistoryLen = 300;
const int MaxFileLen = 1021;    // 1 KB    

// Constants
const size_t kSessionSetSize = 5; // max number of active sessions
const unsigned int kHeaderSize = 3; // network packet header size
const size_t kMaxPacketLength = 1024; // TODO: double check on this number
const size_t kRecvBufferSize = kMaxPacketLength * 3;

// used as the first byte of data packets
enum class PacketType : uint8_t {
    Info = 0x00,
    InfoResponse = 0x01,
    Password = 0x02,
    PasswordResponse = 0x03,
    Refuse = 0x04,
    Configuration = 0x05,
    HistoryUserName = 0x06,
    History = 0x07,
    SyncEnd = 0x08,
    TextUsername = 0x09,
    Text = 0x0A,
    FileName = 0x0B,
    FileInProgress = 0x0C,
    GroupTextUserlist = 0x0D,
    FileEnd = 0x0E,
    FileUsername = 0x0F,
};

struct DataPacketHeader {
    PacketType type;
    uint16_t payload_size;
};
    
struct DataPacket {
    PacketType type;
    std::vector<uint8_t> data;
};

// status codes
enum class StatusCode : int {
    OK = 0,
    OpenFile = -1,
    LogInit = -2,
    RecvError = -3,
    RecvPartial = -4,
    RecvComplete = -5,
    SendError = -6,
    SendPartial = -7,
    SendComplete = -8,
    Accept = -9,
    CreateSocket = -10,
    Setsockopt = -11,
    Bind = -12,
    Listen = -13,

    //present layer error code : start from -20
    NoCompletePacket = -20
};

// Server response type
enum class ResponseType : uint8_t {
    UserNotExist = 0,
    OK = 1,
    ChangePassword = 2,
    WrongPassword = 3,
};

// State machine definition
// Defined almost sequentially. Actions corresponding to a state are in comments.
enum class SessionState : unsigned int {
    Acceptance,         // On acceptance, create a new client instance
    UserCheck,          // Match user in database, password not received yet
    // If user exists, send a response
    SendResponse,       // Send UserCheck response
    UserExists,         // Branch #1, receive password and match password in database
        PasswordReset,  // First login. Receive new password and update database
        AlreadyLoggedIn,// Kick off the logged in session
    PreferenceSync,     // Merge #1, send preference
    HistorySync,        // Send history
    ServerWaiting,      
        // Branch #2 and Merge #2, branch according to the media_type 
        // of the next packet (either received or sent).
        // Send has priority over read.
        TextUsername,   // Target text username
        Text,           // Text data
        FileUsername,   // Target file username
        FileName,       
        FileInProgress, // Until a FileEnd packet is received
        GroupUsernameList,// Target group username list
        GroupText,      // Target group text data
        // go back to ServerWaiting state
};

// Used as a buffer in transfer layer, instantiated in Clients
class CircularQueue {

public:
    CircularQueue(size_t init_size);
    ~CircularQueue();

    bool enqueue(const uint8_t *buf, const size_t size);
    bool dequeue(uint8_t *buf, const size_t size);
    uint16_t current_packet_size(); // note: this is actually the payload size

    // Also requires a getter method for _num_free_bytes here.
    size_t get_num_free_bytes();
    size_t size();
    bool is_empty();
    bool is_full();
    bool has_complete_packet(); // has at least one complete packet
    DataPacket dequeue_packet(); // return a complete packet

private:
    size_t _size;
    uint8_t *data;
    size_t _num_free_bytes;
    size_t front, rear;
};


class Message_To_App{
    PacketType type_;
    std::string user_name_;
    std::string password_; 
    std::string media_text_;
    std::vector<std::string> user_name_list_;
    std::string file_name_;
    std::string media_file_;
    unsigned short config_; // 2 bytes in TransLayer
};

struct Message_To_Pre{
    PacketType type_;
    ResponseType respond_; 
    int config_;
    std::vector<std::string> history_;
};

// not sure if struct group_text should be kept or just use text[] instead ?
struct group_text{
    std::vector<std::string> user_list;
    std::string data;
};

struct file{
    std::string filePath;
};


#endif
