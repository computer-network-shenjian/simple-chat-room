#include "../include/application.hpp"

using namespace fly;
const string InitPassword = "123456";

ApplicationLayer::ApplicationLayer()
{
        // initialize DatabaseConnection class
        cout << DatabaseConnection::get_instance() << endl;
        DatabaseConnection::get_instance()->DatabaseInit();

        return;
}

bool ApplicationLayer::CheckUser(std::string user_name_)
{
        return DatabaseConnection::get_instance()->check_account(user_name_);
}

bool ApplicationLayer::CheckPasswd(std::string user_name_, std::string password_)
{
        return DatabaseConnection::get_instance()->check_password(user_name_, password_);
}

void ApplicationLayer::MessageToApp(Client *client_name_)
{
        // main process here
        // finite state machine
        Message_To_App *message_ = &client_name_->message_ptoa;
        Message_To_Pre *respond_ = &client_name_->message_atop;
        switch(client_name_->state)
        {
                case SessionState::Acceptance: {
                        if(message_->type_ != PacketType::Info){
                                // error occurs
                                // client_name_.message_atop.respond_ = ResponseType::ErrorOccurs;
                                respond_->respond_ = ResponseType::ErrorOccurs;
                                LOG(Error) << "Error receive info packet" << std::endl;
                                // stop the connection 
                                // client_name_->state = SessionState::Error;
                                // respond_.type_ = PacketType::InfoResponse;
                        }        
                        // do recv info packet
                        switch(CheckUser(message_->user_name_)){
                               case true: {
                                       // account exists
                                        respond_->type_ = PacketType::InfoResponse;
                                        respond_->respond_ = ResponseType::OK;
                                        client_name_->state = SessionState::WaitForPasswd;
                                        client_name_->host_username_ = message_->user_name_;
                                        LOG(Info) << "Check User Exists" << std::endl;
                                       break;
                               }
                               case false: {
                                       // account not exists
                                        respond_->type_ = PacketType::InfoResponse;
                                        respond_->respond_ = ResponseType::UserNotExist;
                                        client_name_->state = SessionState::Error;
                                        LOG(Error) << "User not Exists" << std::endl;
                                        break;
                               }
                        }
                        break;
                }
                case SessionState::WaitForPasswd: {
                        if(message_->type_ != PacketType::Password) {
                                // error occurs
                                LOG(Error) << "Error receive password packet" << std::endl;
                                // stop the connection
                                client_name_->state = SessionState::Error;
                                respond_->type_ = PacketType::PasswordResponse;
                                respond_->respond_ = ResponseType::ErrorOccurs;
                        }
                        // do recv password packet
                        switch(CheckPasswd(message_->user_name_, message_->password_)) {
                                case true: {
                                        // password correct
                                        if(message_->password_ == InitPassword) {
                                                // need to reset password
                                                client_name_->state = SessionState::WaitForNewPasswd;
                                                respond_->type_ = PacketType::PasswordResponse;
                                                respond_->respond_ = ResponseType::ChangePassword;
                                        }
                                        else {
                                                // do not need to reset
                                                client_name_->state = SessionState::ServerWaiting;
                                                respond_->type_ = PacketType::Configuration;
                                                respond_->history_ = DatabaseConnection::get_instance()->retrive_message(client_name_->host_username_);
                                        }
                                }
                                case false: {
                                        // password error

                                }
                        }
                }
        }
}
