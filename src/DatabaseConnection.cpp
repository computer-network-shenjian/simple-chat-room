#include "boost/lexical_cast.hpp" 
#include <unistd.h>
#include <iomanip>
#include <iostream>
#include <mysql.h>
#include <string>
using namespace std;

#define DatabaseName "Simple_chat_room"
#define DatabaseUserId "cyanic"
#define DatabasePassword "1985727yyhstc"

// const string kDatabaseName = "Simple_chat_room";

// use singleton design
// example: https://sourcemaking.com/design_patterns/singleton/cpp/1
class DatabaseConnection {
	public:
		void DatabaseInit();
	   	bool check_account(string account_name);	 		 // check if an account is registered
		bool check_password(string account_name, string password);
		string retrive_message(string account_main, string account_sub); //     account_main is the corrsponding client
               	                                                         	 // a particular class is directly connected to.
                      	                                                	 //     account_sub is the other guy this 
                              	                                         	 // message is related to.
		void push_message(string account_main, string account_sub, string message, uint64_t id); // push into history table
		static DatabaseConnection *get_instance() 		  	 // return a class instance	
		{
			if(!obj) {
				obj = new DatabaseConnection;
			}
			return obj;
		}
		MYSQL_RES *MysqlExecCommand(string command);
	private:
		MYSQL *MysqlHandler;
		static DatabaseConnection *obj;
};

// Initialize Mysql connection
// Connect to Database
// For Now provided running on my own virtual machine
void DatabaseConnection::DatabaseInit() 
{
	// initialize mysql handler
        while((this->MysqlHandler = mysql_init(NULL))==NULL) {
        	cout << "mysql_init failed" << endl;
		cout << "retry after 3 seconds" << endl;
		sleep(3);
        }
    
	// connect to mysql server
        while(mysql_real_connect(this->MysqlHandler, "localhost", DatabaseUserId, DatabasePassword, DatabaseName, 0, NULL, 0) == NULL) {
            	cout << "mysql_real_connect failed(" << mysql_error(this->MysqlHandler) << ")" << endl;
		cout << "retry after 3 seconds" << endl;
		sleep(3);
        }

        // set up mysql decode to gbk
        mysql_set_character_set(this->MysqlHandler, "gbk");

}

// a tool function for mysql operation
MYSQL_RES* DatabaseConnection::MysqlExecCommand(string command)
{
	MYSQL_RES *result;
	while(mysql_query(this->MysqlHandler, command.c_str()) != 0) {
		cout << "mysql_query failed(" << mysql_error(this->MysqlHandler) << ")" << endl;
		cout << "reconnect after 3 seconds" << endl;
		sleep(3);
		DatabaseInit();
	}

	string empty = "";
	while((result = mysql_store_result(this->MysqlHandler)) == NULL) {
		if(mysql_error(this->MysqlHandler) == empty) {
			mysql_free_result(result);
			break;
		}
        	cout << "mysql_store_result failed(" << mysql_error(this->MysqlHandler) << ")" << endl;
		cout << "reconnect after 3 seconds" << endl;
		sleep(3);
		DatabaseInit();
		// reconnect to mysql server
		mysql_query(this->MysqlHandler, command.c_str());
	}

	return result;
}

bool DatabaseConnection::check_account(string account_name)
{
        MYSQL_RES *result;
	MYSQL_ROW row;

	// form command	
	string command = "select count(*) from account where username = '" + account_name + "'";

	// exec command and store return value
	result = MysqlExecCommand(command);
	row = mysql_fetch_row(result);

	if(atoi(row[0]) == 1) {
		mysql_free_result(result);
		return true;
	}
	else {
		mysql_free_result(result);
		return false;
	} 
}

bool DatabaseConnection::check_password(string account_name, string password)
{
	MYSQL_RES *result;
	MYSQL_ROW row;

	// form command
	string command = "select count(*) from account where username = '" + account_name + "' and password = '" + password + "';";

	// exec command and store return value
	result = MysqlExecCommand(command);
	row = mysql_fetch_row(result);

	if(atoi(row[0]) == 1) {
		mysql_free_result(result);
		return true;
	}
	else {
		mysql_free_result(result);
		return false;
	}
}

void DatabaseConnection::push_message(string account_main, string account_sub, string message, uint64_t id)
{
	// form command
	string command = "insert into history values('";
	command += boost::lexical_cast<std::string>(id);
	command += "', '" + account_main + "','" + account_sub + "','" + message + "')";

	MysqlExecCommand(command);
}

string DatabaseConnection::retrive_message(string account_main, string account_sub)
{
	MYSQL_RES *result;
	MYSQL_ROW row;

	// form command
	string command = "select * from history where username_main = '" + account_main + "' and username_sub = '" + account_sub + "'";

	result = MysqlExecCommand(command);
	string message_all;
	while((row = mysql_fetch_row(result)) != NULL) {
		message_all += row[3];
		// TODO: vector push_back
        }

	mysql_free_result(result);
	return message_all;
}
