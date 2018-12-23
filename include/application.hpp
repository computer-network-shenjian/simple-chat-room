#ifndef APPLICA_H
#define APPLICA_H
#include "DatabaseConnection.hpp"
// #include "client.hpp"
#include "types.hpp"
#include "Log.h"
// #include <iostream>


class ApplicationLayer{
        private:
                // DatabaseConnection *DatabaseConnection::obj;
                bool CheckUser(std::string user_name_);
                bool CheckPasswd(std::string user_name_, std::string password_);
        public:
                ApplicationLayer();
                void MessageToApp(Client *client);
};



#endif