#include <iomanip>
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
			string push_message(string account_main, string account_sub); 	 // if no histoy exist, extend the Database
			static DatabaseConnection *get_instance() 			 // return a class instanc	
			{
				if(!obj) {
					obj = new DatabaseConnection;
				}
				return obj;
			}
		private:
			string DatabaseName;
			static DatabaseConnection *obj;
};

// Initialize Mysql connection
// Connect to Database
// For Now provided running on my own virtual machine
void DatabaseConnection::DatabaseInit(string DatabaseName) 
{
        MYSQL *mysql;
        MYSQL_RES *result;
        MYSQL_ROW row;

        if ((mysql = mysql_init(NULL))==NULL) {
        	cout << "mysql_init failed" << endl;
        	return;
        }
    
        if(mysql_real_connect(mysql,"localhost","cyanic", "1985727yyhstc","Simple_chat_room",0, NULL, 0) == NULL) {
            cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
        	return;
        }

        // set up mysql decode to gbk
        mysql_set_character_set(mysql, "gbk");

        if(mysql_query(mysql, "select * from account")) {
        	cout << "debug:" << mysql_real_connect(mysql,"localhost","cyanic", "1985727yyhstc","Database",0, NULL, 0) << endl;
		cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        	return;
        }

        if((result = mysql_store_result(mysql))==NULL) {
        	cout << "mysql_store_result failed" << endl;
        	return;
        }

        // print out select number
        cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;

        // 
        while((row = mysql_fetch_row(result)) != NULL) {
            	cout << setiosflags(ios::left);           
       	     	cout << "account: " << setw(20) << row[0];
            	cout << "account: " << setw(20)  << row[1];     
            	cout << endl;
        }

        mysql_free_result(result);   

        mysql_close(mysql);   
}
