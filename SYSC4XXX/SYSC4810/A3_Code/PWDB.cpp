#include "PWDB.h"

PWDB::PWDB()
{
   //ctor
}

PWDB::~PWDB()
{};



// SQLite Callback function. It also sets a lock so that the calling functions know to continue.
// Uses tutorial code from https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
static int callback(void* PWDB_pointer, int argc, char** argv, char** azColName){
   for (size_t i = 0; i < argc; i++){
      std::cout << azColName[i] << " " << (argv[i] ? argv[i] : "NULL") << std::endl;
   }
   return 0;
}



// Private functions for simplifying sqlite functions, for my own benefit.
bool PWDB::write_DB(){};
bool PWDB::read_DB(){};

bool PWDB::add_pw_DB(){};
bool PWDB::chg_pw_DB(){};
bool PWDB::rmv_pw_DB(){};






