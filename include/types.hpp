// used as the first byte of data packets
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
    FileEnd = 0x0E,
    FileUsername = 0x0F,
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
};


