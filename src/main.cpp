#include <iostream>
#include "DatabaseConnection.cpp"
using namespace std;

DatabaseConnection *DatabaseConnection::obj = NULL;

int main()
{
        cout << "My ptr:" << DatabaseConnection::get_instance() << endl;        
        DatabaseConnection::get_instance()->DatabaseInit("test");

        return 0;
}
