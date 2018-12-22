#include <string>
#include <iostream>
#include "../../include/DatabaseConnection.hpp"
using namespace std;

// DatabaseConnection *DatabaseConnection::obj = NULL;

int main()
{
        //cout << "My ptr:" << DatabaseConnection::get_instance() << endl;        
        get_instance()->DatabaseInit();

        get_instance()->push_message("yanic", "Cyanic", "Hello!");
        get_instance()->push_message("Cyanic", "yanic", "Yes!");
        get_instance()->push_message("yanic", "Cyanic", "GOOD!");
        get_instance()->push_message("Dyanic", "Cyanic", "GOOD!");
        
	vector<string> test;
	
	int k;
	k = get_instance()->retrive_history_count("Cyanic");
        cout << k << endl;
        test = get_instance()->retrive_message("Cyanic");
        // int i;
        // for(i = 0; i < 12; i++) {
        //        cout << test[i] <<endl;
       // }

        return 0;
}
