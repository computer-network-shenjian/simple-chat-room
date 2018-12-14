#include <iostream>
#include <mysql.h>
#include <string>
using namespace std;

// use singleton design
// example: https://sourcemaking.com/design_patterns/singleton/cpp/1
class DatabaseConnection {
    public:
        void DatabaseInit(string DatabaseName);
        bool check_account(string account_name); // check if an account is registered
        bool check_password(string account_name, string password);
        string retrive_message(string account_main, string account_sub); //     account_main is the corrsponding client
                                                                         // a particular class is directly connected to.
                                                                         //     account_sub is the other guy this 
                                                                         // message is related to.
        string push_message(string account_main, string account_sub); // if no histoy exist, extend the Database
        static DatabaseConnection *get_instance(); // return a class instance
    private:
        string DatabaseName;
        static DatabaseConnection *obj;
}
