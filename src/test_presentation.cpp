#include "../include/presentation.hpp"




// enum class PacketType : uint8_t {
//     Info = 0x00,    
//     InfoResponse = 0x01,
//     Password = 0x02,
//     PasswordResponse = 0x03,
//     Refuse = 0x04,
//     Configuration = 0x05,
//     HistoryUserName = 0x06,
//     History = 0x07,
//     SyncEnd = 0x08,
//     TextUsername = 0x09,
//     Text = 0x0A,
//     FileName = 0x0B,
//     FileInProgress = 0x0C,
//     GroupTextUserlist = 0x0D,
//     FileEnd = 0x0E,
//     FileUsername = 0x0F,
// };



// Refuse			PacketType 	
// SyncEnd			PacketType		
// InfoResponse 	PacketType 		ResponseType 
// PasswordResponse PacketType		ResponseType
// Configure		PacketType 		struct configure
// History 			PacketType  	struct chatHistory
// text 			PacketType  	struct text
// file 			PacketType 		struct file
// g_text_usrlist 	PacketType  	vector<string>
// Info 			PacketType 		string
// Password 		PacketType  	string
// file_name 		PacketType 		string 
class Message_To_App{
    PacketType type_;
    std::string user_name_;
    std::string password_; 
    std::string media_text_;
    std::vector<std::string> user_name_list_;
    std::string file_name_;
    std::string media_file_;
    unsigned int config_;
};


class Message_To_Pre{
    PacketType type_;
    ResponseType respond_; 
    int config_;
    vector<string> history_;
};


class PresentLayer {
private:
    uint16_t packet_size;

    vector<uint8_t> * pack_Response(Message_To_Pre message);
    vector<uint8_t> * pack_String(Message_To_Pre message);
   // vector<uint8_t> * pack_Signal(Message_To_Pre message);
    vector<uint8_t> * pack_Config(Message_To_Pre message);
    vector<uint8_t> * pack_HistoryUserName(Message_To_Pre message);

    unsigned char * unpack_String(DataPacket packet);
    Message_To_App  unpack_Configuration(DataPacket packet);
    Message_To_App  unpack_GroupTextUserList(DataPacket packet);

public:
    PresentationLayer() = default;
    ~PresentationLayer();

    StatusCode pack_Message(Client *client);
    // function: pack Message from appLayer
    // status: send
    // precondition: message.type_ matches the state machine
    // return:
    //         OK: pack message and write to client.send_buffer
    //         Error: message.type_ doesn't match the state machine
    
    StatusCode unpack_DataPacket(Client *client);
    // function: unpack DataPacket from transLayer
    // status: recv
    // precondition: client->recv_buffer.dequeue() returns a DataPacket
    // return: 
    //         OK:  unpack a DataPacket from recv_buffer and write Message_To_App succeed
    //         NoCompletePacket:   no complete packet in recv_buffer
};

vector<uint8_t> * PresentLayer::pack_Response(Message_To_Pre message){
    static vector<uint8_t> temp;
    uint8_t descriptor;
    uint16_t length;

    //descriptor
    descriptor = message.type_;
    temp.clear();
    temp.push_back(descriptor);

    switch(message.type_){
        case InfoResponse:             
            //length = 1
            length = htons((uint16_t)1 );    
            temp.push_back((uint8_t)(length >> 8) );
            temp.push_back((uint8_t)(length) );
            temp.push_back(message.respond_);
            break;

        case PasswordResponse:
            //length = 1
            length = htons((uint16_t)1 );    
            temp.push_back((uint8_t)(length >> 8) );
            temp.push_back((uint8_t)(length) );
            temp.push_back(message.respond_);
            break;

        case SyncEnd:
            //length = 0
            temp.push_back((uint8_t)0); 
            temp.push_back((uint8_t)0);
            temp.push_back((uint8_t)0);
    }

    return temp;
}

vector<uint8_t> * PresentLayer::pack_Config(Message_To_Pre message){
    static vector<uint8_t> temp;
    uint8_t descriptor;
    uint16_t length;
    uint16_t conf;

    temp.clear();
    //descriptor
    descriptor = message.type_;
    temp.push_back(descriptor);

    //conf length = 2
    length = htons((uint16_t)2 );    
    temp.push_back((uint8_t)(length >> 8) );
    temp.push_back((uint8_t)(length) );

    //configure
    conf = (uint16_t)message.config_;
    temp.push_back((uint8_t)(conf >> 8) );
    temp.push_back((uint8_t)(conf) );

    return temp;
}

vector<uint8_t> * PresentLayer::pack_String(Message_To_Pre message){
    static vector<uint8_t> temp;
    uint8_t descriptor;
    uint16_t length;
    string str;

    temp.clear();
    //descriptor
    descriptor = message.type_;
    temp.push_back(descriptor);

    //user_name_host
    vector<string>::iterator iter = message.history_.begin();
    str =  *iter;
    if(str == )
    //user_name_receiver
    iter++; 

    //lengh?
    length = htons((uint16_t) );


    return temp;
}

vector<uint8_t> * PresentLayer::pack_HistoryUserName(Message_To_Pre message){

}

StatusCode PresentLayer::pack_Message(Client *client){
    DataPacket packet;
    Message_To_Pre message;
    vector<uint8_t> temp_str;
    unsigned char *temp_data;

    message = client->message_atop;
    //start packing
    if((message.type_ == InfoResponse) || (message.type_ == PasswordResponse) || (message.type_ == SyncEnd) ){
        temp_str = pack_Response(message);
    }

    if(message.type_ == Configuration){
        temp_str = pack_Config(message);
    }

    if((message.type_ == History)
        || (message.type_ == TextUsername) || (message.type_ == Text)
        || (message.type_ == FileName) || (message.type_ == FileInProgress)
        || (message.type_ == FileEnd) || (message.type_ == FileUsername) ){
        
        temp_str = pack_String(message);
    }

    if(message.type_ == HistoryUserName){
        temp_str = pack_HistoryUserName(message);
    }

    client->send_buffer.push(temp_str);

    return StatusCode::OK; 
}

StatusCode PresentLayer::unpack_DataPacket(Client *client){
    AppLayerInstance.MessageToApp(client);
    return StatusCode::OK;


    //check if there is complete packet


    if(!client->recv_buffer.has_complete_packet())
        return StatusCode::NoCompletePacket;
    
    DataPacket packet;
    Message_To_App message;
    unsigned char *temp_data;

    packet = client->recv_buffer.dequeue_packet();
    packet_size = packet.data.size() + 3;

    //start unpacking 
    if((packet.type == Info) || (packet.type == Password)
        || (packet.type == HistoryUserName) || (packet.type == History)
        || (packet.type == TextUsername) || (packet.type == Text)
        || (packet.type == FileUsername) || (packet.type == FileName)
        || (packet.type == FileInProgress) || (packet.type == FileEnd) )
    {
        temp_data = unpack_String(packet);
        switch(packet.type){
            case Info:
                message.user_name_ = (char *)temp_data;
                break;
            case Password:
                message.password_ = (char *)temp_data;
                break;
            case HistoryUserName:
                message.user_name_ = (char *)temp_data;
                break;
            case History:
                message.media_text_ = (char *)temp_data;
                break;
            case TextUsername:
                message.user_name_ = (char *)temp_data;
                break;
            case Text:
                message.media_text_ = (char *)temp_data;
                break;
            case FileUsername:
                message.user_name_ = (char *)temp_data;
                break;
            case FileName:
                message.file_name_ = (char *)temp_data;
                break;
            case FileInProgress:
                message.media_file_ = (char *)temp_data;
                break;
            case FileEnd:                
                message.media_file_ = (char *)temp_data;
                break;
            default:
                break;
        }
    }//end of if
 
    if(packet.type == Configuration)
            message = unpack_Configuration(packet);

    if(packet.type == GroupTextUserlist)
            message = unpack_GroupTextUserList(packet);

    message.type_ = packet.type;
    client->message = message;

    AppLayerInstance.MessageToApp(client);
    return StatusCode::OK;
}

unsigned char * PresentLayer::unpack_String(DataPacket packet){
    vector<uint8_t>::iterator iter;
    static unsigned char temp[kMaxPacketLength];

    int data_len = 0;
    for(iter = packet.data.begin(); iter != packet.data.end(); iter++){
        temp[data_len++] = *iter;
    }
    temp[data_len] = '\0';
    
    return temp;
}

Message_To_App PresentLayer::unpack_Configuration(DataPacket packet){
    Message_To_App message;
    vector<uint8_t>::iterator iter;
    unsigned short st;   

    iter = packet.data.begin();
    st = (unsigned short)(*iter);
    st = st << 8;
    st += (unsigned short)(*(++iter));
    message.config_ = ntohs(st);

    return message;
}

Message_To_App PresentLayer::unpack_GroupTextUserList(DataPacket packet){
    Message_To_App message;
    vector<uint8_t>::iterator iter;
    string temp_str;
    unsigned char temp_ch[30]; //max user name length = 28

    int len = 0;
    iter = packet.data.begin();

    for( ; iter != packet.data.end(); iter++){
        temp_ch[len++] = *iter; 
        //if encountered '\0'
        if((*iter) == 0){
            temp_str = (char *)temp_ch;
            //push_back a new user_name
            message.user_name_list_.push_back(temp_str);
            len = 0;    //reset len = 0, and fetch the next user_name
        }
    }
    //add \0 to the last user_name in the user_name_list
    temp_ch[len] = '\0';
    temp_str = (char *)temp_ch;
    //push_back a new user_name
    message.user_name_list_.push_back(temp_str);

    return message;
}
