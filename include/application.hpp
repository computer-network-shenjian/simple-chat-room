#include "../src/DatabaseConnection.cpp"
#include "presentation.hpp"
#include "client.hpp"
#include "types.hpp"
#include "log.h"
#include <iostream>

class ApplicationLayer{
        private:
                DatabaseConnection *DatabaseConnectionPtr;
                bool CheckUser(std::string user_name_);
                bool CheckPasswd(std::String user_name_, std::string password_);
        public:
                ApplicationLayer();
                ~ApplicationLayer();
                void MessageToApp(Client *client);
}

