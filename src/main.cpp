#include <iostream>
#include "DatabaseConnection.cpp"
using namespace std;

DatabaseConnection *DatabaseConnection::obj = NULL;

int main()
{
        cout << "My ptr:" << DatabaseConnection::get_instance() << endl;        
        DatabaseConnection::get_instance()->DatabaseInit();

        cout << DatabaseConnection::get_instance()->check_password("Cyanic", "123456") << endl;
        cout << DatabaseConnection::get_instance()->check_password("cyanic", "123456") << endl;

        DatabaseConnection::get_instance()->push_message("yanic", "Cyanic", "Hello!", 4);
        DatabaseConnection::get_instance()->push_message("yanic", "Cyanic", "Yes!", 5);
        DatabaseConnection::get_instance()->push_message("yanic", "Cyanic", "GOOD!", 6);
        
        cout << DatabaseConnection::get_instance()->retrive_message("yanic", "Cyanic");
        return 0;
}