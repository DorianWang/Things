#include "PWDB.h"

using namespace PWDB;

PermissionsDB::PermissionsDB(const std::filesystem::path& newDBPath)
{
   DBPath = newDBPath;
}

PermissionsDB::~PermissionsDB()
{};

void PermissionsDB::set_DB_path(const std::filesystem::path& newDBPath)
{
   DBPath = newDBPath;
}

// Private functions for simplifying database calls, since my solution will be ugly...
bool PermissionsDB::read_DB()
{
   std::fstream in;
   in.open(DBPath.string(), std::fstream::in);
   if (in.is_open() && in.good()){
      // Try to read in the 4 numbers at the start of the file.
      size_t userListSize;
      size_t nextUID_file;
      size_t patientFilesSize;
      size_t actionListSize;
      in >> userListSize;
      in >> nextUID_file;
      in >> patientFilesSize;
      in >> actionListSize;
   }
   else{
      return false;
   }

   for (size_t i = 0; i < userListSize; i++){
      std::string temp;
      in >> temp;
      UserEntry newUser(temp);
      userList.insert(std::pair<uint_fast64_t, UserEntry>(newUser.UID, newUser));
   }

   for (size_t i = 0; i < patientFilesSize; i++){
      std::string tempName; unsigned long long int tempOwner;
      in >> tempName;
      in >> tempOwner;
      FileData tempFD{tempName, tempOwner};
      patientFiles.push_back(tempFD);
   }

   for (size_t i = 0; i < actionListSize; i++){
      // read lines and parse those instead.
   }
   in.close();
   return true;
}

bool PermissionsDB::write_DB()
{
   std::fstream tempFile; std::filesystem::path tempFilePath("db.temp");
   tempFile.open(tempFilePath.string(), std::fstream::out | std::fstream::trunc);
   if (!(tempFile.is_open()) || tempFile.bad()){
      return false;
   }

   tempFile << userList.size() << " " << nextUID << " " << patientFiles.size() << " " << actionList.size() << std::endl;
   for (auto iter : userList){
      tempFile << iter.second.to_string() << "\n";
   }

   for (const FileData& iter : patientFiles){
      tempFile << iter.name << " " << iter.ownerID << "\n"; // Just ignore spaces in file names...
   }

   for (auto iter : actionList){
      tempFile << iter.to_string() << "\n";
   }
   tempFile << std::endl; // Hopefully flush anything in the buffer?

   try{
      std::filesystem::rename(tempFilePath, DBPath);
   }
   catch ( std::filesystem::filesystem_error& ){
      std::cerr << "Failed to rename temporary file to database file, aborting.\n";
      tempFile.close();
      return false;
   }
   tempFile.close();
   return true;
}


bool PermissionsDB::add_pw_DB(){};
bool PermissionsDB::chg_pw_DB(){};
bool PermissionsDB::rmv_pw_DB(){};






