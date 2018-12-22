#include <string>
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

        DatabaseConnection::get_instance()->push_message("yanic", "Cyanic", "Hello!", 7);
        DatabaseConnection::get_instance()->push_message("yanic", "Cyanic", "Yes!", 8);
        DatabaseConnection::get_instance()->push_message("yanic", "Cyanic", "GOOD!", 9);
        
	vector<string> test;

        test = DatabaseConnection::get_instance()->retrive_message("yanic", "Cyanic");
        int i;
        for(i = 0; i < 9; i++) {
                cout << test[i] <<endl;
        }

        return 0;
}
