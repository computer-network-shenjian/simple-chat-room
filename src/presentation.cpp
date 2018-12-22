#ifndef PRESENT_H
#define PRESENT_H
#include "../include/presentation.hpp"
#include "../include/application.hpp"

extern ApplicationLayer AppLayerInstance;

using namespace std;

vector<uint8_t> PresentationLayer::pack_Response(Message_To_Pre message){
    static vector<uint8_t> temp;
    uint8_t* descriptor;
    uint16_t length;

    //descriptor
    descriptor = (uint8_t *)&message.type_;
    temp.clear();
    temp.push_back(*descriptor);

    switch(message.type_){
        case PacketType::InfoResponse:             
            //length = 1
            length = htons((uint16_t)1 );    
            temp.push_back((uint8_t)(length >> 8) );
            temp.push_back((uint8_t)(length) );
            temp.push_back(*((uint8_t*)&message.respond_));
            break;

        case PacketType::PasswordResponse:
            //length = 1
            length = htons((uint16_t)1 );    
            temp.push_back((uint8_t)(length >> 8) );
            temp.push_back((uint8_t)(length) );
            temp.push_back(*((uint8_t*)&message.respond_));
            break;

        case PacketType::SyncEnd:
            //length = 0
            temp.push_back((uint8_t)0); 
            temp.push_back((uint8_t)0);
            temp.push_back((uint8_t)0);
    }

    return temp;
}

vector<uint8_t> PresentationLayer::pack_Config(Message_To_Pre message){
    static vector<uint8_t> temp;
    uint8_t descriptor;
    uint16_t length;
    uint16_t conf;

    temp.clear();

    //descriptor
    temp.push_back(*((uint8_t*)&message.type_));

    //conf length = 2
    length = htons((uint16_t)2 );    
    temp.push_back((uint8_t)(length >> 8) );
    temp.push_back((uint8_t)(length) );

    //configure
    conf = htons((uint16_t)message.config_);
    temp.push_back((uint8_t)(conf >> 8) );
    temp.push_back((uint8_t)(conf) );

    return temp;
}

vector<uint8_t> PresentationLayer::pack_TextUserName(Client * client){
    vector<uint8_t> temp;
    uint8_t descriptor;
    uint16_t length;

    Message_To_App message = client->message_ptoa;

    //push back: descriptor
    temp.push_back((uint8_t)PacketType::TextUsername);

    //push back: user name length   (user name = client->host_username_)
    length = htons((uint16_t)client->host_username_.length() );
    temp.push_back((uint8_t)(length >> 8) );
    temp.push_back((uint8_t)(length) );

    //push back: user name
    const char* c;
    c = client->host_username_.c_str();
    while((*c) != '\0'){
        temp.push_back((uint8_t)(*c) );
        c++;
    }

    return temp;
}

vector<uint8_t> PresentationLayer::pack_Text(Client * client){
    vector<uint8_t> temp;
    uint8_t descriptor;
    uint16_t length;

    Message_To_App message = client->message_ptoa;

    //push back: descriptor
    temp.push_back((uint8_t)PacketType::Text); 

    //push back: text length
    length = htons((uint16_t)message.media_text_.length() );
    temp.push_back((uint8_t)(length >> 8) );
    temp.push_back((uint8_t)(length) );

    //push back: text
    const char* c;
    c = message.media_text_.c_str();
    while((*c) != '\0'){
        temp.push_back((uint8_t)(*c) );
        c++;
    }

    return temp;
}

vector<uint8_t> PresentationLayer::pack_HistoryUserName(Message_To_Pre * message, string host_name){
    uint8_t direct;
    vector<uint8_t> temp;
    uint16_t length;
    string str;

    //push back: descriptor
    temp.push_back((uint8_t)PacketType::HistoryUserName);

    //direct
    str = *message->history_.begin();
    if(str == host_name)
        direct = 0; //me to others
    else
        direct = 1; //others to me

    //me to others
    if(direct == 0){    
        //erase host name
        message->history_.erase(message->history_.begin());     
        
        //push_back: user_name length
        str = *message->history_.begin(); 
        length = htons((uint16_t)(str.length()) );
        temp.push_back((uint8_t)(length >> 8) );
        temp.push_back((uint8_t)(length) );

        //push_back: direct
        temp.push_back(direct);

        //push_back: user_name
        const char* c;
        c = str.c_str();
        while((*c) != '\0'){
            temp.push_back((uint8_t)(*c) );
            c++;
        }
    }
    //others to me
    else{    
        //push_back: user_name length
        str = *message->history_.begin(); 
        length = htons((uint16_t)(str.length()) );
        temp.push_back((uint8_t)(length >> 8) );
        temp.push_back((uint8_t)(length) );

        //push_back: user_name
        const char* c;
        c = str.c_str();
        while((*c) != '\0'){
            temp.push_back((uint8_t)(*c) );
            c++;
        }

        //push_back: direct
        temp.push_back(direct);

        //erase host name
        message->history_.erase(message->history_.begin());     
    }

    return temp;
}

vector<uint8_t> PresentationLayer::pack_History(Message_To_Pre * message){
    vector<uint8_t> temp;
    uint8_t descriptor;
    uint16_t length;
    string str;

    //push back: descriptor
    if(message->type_ == PacketType::Configuration)
        temp.push_back((uint8_t)PacketType::History);

    if(message->type_ == PacketType::Text)
        temp.push_back((uint8_t)PacketType::Text);

    //push back: text length
    str = *message->history_.begin();
    length = htons((uint16_t)(str.length()) );
    temp.push_back((uint8_t)(length >> 8) );
    temp.push_back((uint8_t)(length) );

    //push back: text
    const char* c;
    c = str.c_str();
    while((*c) != '\0'){
        temp.push_back((uint8_t)(*c) );
        c++;
    }

    //erase text
    message->history_.erase(message->history_.begin());     

    return temp;
}

StatusCode PresentationLayer::pack_Message(Client *client){
    Client *recv_client;
    DataPacket packet;
    Message_To_Pre message;
    vector<uint8_t> temp_str;
    unsigned char *temp_data;

    message = client->message_atop;

    //start packing:
    //WaitForPasswd or WaitForNewPasswd: send info/passwd response
    if((client->state == SessionState::WaitForPasswd) 
            || (client->state == SessionState::WaitForNewPasswd) 
            || (client->state == SessionState::Error)) {
        temp_str = pack_Response(message);
        client->send_buffer.push(temp_str);
    }

    //ServerWaiting: 
    if((client->state == SessionState::ServerWaiting) ){
        
        //slog in succeed, now synchronize 
        if(message.type_ == PacketType::Configuration){
            //sync config
            temp_str = pack_Config(message);
            client->send_buffer.push(temp_str);

            //sync history
            while(message.history_.size() != 0){
                //history user name
                temp_str = pack_HistoryUserName(&message, client->host_username_);
                client->send_buffer.push(temp_str);
                //history
                temp_str = pack_History(&message);
                client->send_buffer.push(temp_str);
            }
        }//end of Configuration

        //send Text to some other client
        if(message.type_ == PacketType::Text){
            //use message_ptoa to find the receiver client
            Message_To_App message_ptoa = client->message_ptoa;
         
            /*
                use message_ptoa.user_name_ to 
                find the recv_client
            */

            temp_str = pack_TextUserName(client);
            recv_client->send_buffer.push(temp_str);

            temp_str = pack_Text(client);
            recv_client->send_buffer.push(temp_str);

            return StatusCode::OK;
        }//end of Text
    }

    //WaitForText:
    if((client->state == SessionState::WaitForText) )
        return StatusCode::OK;


    //file
    // if((message.type_ == FileName) || (message.type_ == FileInProgress)
    //     || (message.type_ == FileEnd) || (message.type_ == FileUsername) ){
    //     temp_str = pack_String(message);
    // }
    return StatusCode::OK; 
}

StatusCode PresentationLayer::unpack_DataPacket(Client *client){
  while( client->recv_buffer.has_complete_packet()){
        //client->recv_buffer.has_complete_packet()
        DataPacket packet;
        Message_To_App message;
        unsigned char *temp_data;

        packet = client->recv_buffer.dequeue_packet();
        packet_size = packet.data.size() + 3;

        //start unpacking 
        if((packet.type == PacketType::Info) || (packet.type == PacketType::Password)
            || (packet.type == PacketType::HistoryUserName) || (packet.type == PacketType::History)
            || (packet.type == PacketType::TextUsername) || (packet.type == PacketType::Text)
            || (packet.type == PacketType::FileUsername) || (packet.type == PacketType::FileName)
            || (packet.type == PacketType::FileInProgress) || (packet.type == PacketType::FileEnd) )
        {
            temp_data = unpack_String(packet);
            switch(packet.type){
                case PacketType::Info:
                    message.user_name_ = (char *)temp_data;
                    break;
                case PacketType::Password:
                    message.password_ = (char *)temp_data;
                    break;
                case PacketType::HistoryUserName:
                    message.user_name_ = (char *)temp_data;
                    break;
                case PacketType::History:
                    message.media_text_ = (char *)temp_data;
                    break;
                case PacketType::TextUsername:
                    message.user_name_ = (char *)temp_data;
                    break;
                case PacketType::Text:
                    message.media_text_ = (char *)temp_data;
                    break;
                case PacketType::FileUsername:
                    message.user_name_ = (char *)temp_data;
                    break;
                case PacketType::FileName:
                    message.file_name_ = (char *)temp_data;
                    break;
                case PacketType::FileInProgress:
                    message.media_file_ = (char *)temp_data;
                    break;
                case PacketType::FileEnd:                
                    message.media_file_ = (char *)temp_data;
                    break;
                default:
                    break;
            }
        }//end of if
     
        if(packet.type == PacketType::Configuration)
                message = unpack_Configuration(packet);

        if(packet.type == PacketType::GroupTextUserlist)
                message = unpack_GroupTextUserList(packet);

        message.type_ = packet.type;
        client->message_ptoa = message;

        AppLayerInstance.MessageToApp(client);
    }
    return StatusCode::OK;
}

unsigned char * PresentationLayer::unpack_String(DataPacket packet){
    vector<uint8_t>::iterator iter;
    static unsigned char temp[kMaxPacketLength];

    int data_len = 0;
    for(iter = packet.data.begin(); iter != packet.data.end(); iter++){
        temp[data_len++] = *iter;
    }
    temp[data_len] = '\0';
    
    return temp;
}

Message_To_App PresentationLayer::unpack_Configuration(DataPacket packet){
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

Message_To_App PresentationLayer::unpack_GroupTextUserList(DataPacket packet){
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

#endif