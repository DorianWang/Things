#include "PWDB.h"

using namespace PWDB;

PermissionsDB::PermissionsDB(const std::filesystem::path& newDBPath)
{
   DBPath = newDBPath;
   pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_SCRYPT, NULL);
   if (EVP_PKEY_derive_init(pctx) <= 0) {
      std::cerr << "Failed to init hashing constructor thing\n";
      pctx = nullptr;
   }
   std::filesystem::path rules(RULES_FILE);
   std::filesystem::path commonPasses(COMMON_RULES_FILE);
   std::filesystem::path badPasses(FORBIDDEN_PASSES_FILE);
   passwordFilter.read_password_rules_file(rules);
   passwordFilter.read_common_patterns_file(commonPasses);
   passwordFilter.read_restricted_passwords_file(badPasses);
}

PermissionsDB::~PermissionsDB()
{
   if (pctx != nullptr){
      EVP_PKEY_CTX_free(pctx);
   }
}

void PermissionsDB::set_DB_path(const std::filesystem::path& newDBPath)
{
   DBPath = newDBPath;
}

bool PermissionsDB::scrypt_hashing(std::string& password, uint64_t* saltOut, unsigned char* hashOut, size_t outSize)
{
   if (EVP_PKEY_CTX_set1_pbe_pass(pctx, password.c_str(), password.length()) <= 0) {
      std::cerr << "Failed to set password\n";
      return false;
   }

   unsigned char buffer[8];
   if (RAND_bytes(buffer, 8) == 1){
      *saltOut = 0;
      for (size_t i = 0; i < 8; i++){
         *saltOut += ((uint64_t) buffer) << (i * 8);
      }
   }
   else{
      std::cerr << "Failed to get random salt\n";
      return false;
   }

   if (EVP_PKEY_CTX_set1_scrypt_salt(pctx, buffer, 8) <= 0) {
      std::cerr << "Failed to set salt\n";
      return false;
   }
   if (EVP_PKEY_CTX_set_scrypt_N(pctx, SCRYPT_N) <= 0) {
      std::cerr << "Failed to set N\n";
      return false;
   }
   if (EVP_PKEY_CTX_set_scrypt_r(pctx, SCRYPT_r) <= 0) {
      std::cerr << "Failed to set r\n";
      return false;
   }
   if (EVP_PKEY_CTX_set_scrypt_p(pctx, SCRYPT_p) <= 0) {
      std::cerr << "Failed to set p\n";
      return false;
   }
   // But why a pointer to the size? Does the function change the value?
   if (EVP_PKEY_derive(pctx, hashOut, &outSize) <= 0) {
      std::cerr << "Failed to set output\n";
      return false;
   }
   return true;
}


uint_fast64_t PermissionsDB::add_user(std::string name, std::string username, uint_fast64_t GID, std::string password)
{
   if (groupList.count(GID) == 0){
      return 0; // Bad group.
   }
   if (passwordFilter.check_password(username, password) != 0){
      return 0; // Bad password.
   }
   size_t nudgeCounter = 0;
   while (userList.count(nextUID)){
      // Well it might happen with how fragile this system is. I'll ignore the case that the map is full, 64 bits is a lot of users.
      nextUID++; nudgeCounter++;
      if (nudgeCounter > 64){
         return 0; // Just give up.
      }
   }
   UserEntry temp;
   temp.name = name; temp.GID = GID; temp.username = username; temp.UID = nextUID;
   if (scrypt_hashing(password, &(temp.salt), temp.hashedPass, HASH_OUTPUT_LENGTH)){
      userList.insert(std::pair<uint_fast64_t, UserEntry>(nextUID, temp));
      nextUID++;
      return nextUID - 1;
   }
   else{
      return 0; // SSL stuff failed.
   }
}

uint_fast64_t PermissionsDB::add_group(std::string name){
   size_t nudgeCounter = 0;
   while (groupList.count(nextGID)){
      if (nudgeCounter > 64){
         return 0; // Just give up.
      }
      nextGID++; nudgeCounter++;
   }
   groupList.insert(std::pair<uint_fast64_t, GroupType>(nextGID, GroupType{name, nextGID++}));
}

bool PermissionsDB::add_activity(std::string name, int permissions){
   if (actionList.count(name)){
      return false;
   }
   APRs temp;
   temp.owner_rwe[2] = permissions & 4; temp.owner_rwe[1] = permissions & 2; temp.owner_rwe[0] = permissions & 1;
   temp.name = name;
   actionList.insert(std::pair<std::string, APRs>(name, temp));
};

bool PermissionsDB::add_target(std::string name, uint_fast64_t ownerID){
   if (userList.count(ownerID)){
      patientFiles.push_back(FileData{name, ownerID});
      return true;
   }
   else{
      return false;
   }
};

bool PermissionsDB::add_permission_to_activity(std::string name, uint_fast64_t GID, int permissions){
   bool perms[3];
   perms[2] = permissions & 4; perms[1] = permissions & 2; perms[0] = permissions & 1;
   GroupPermissions temp(GID, perms);
   auto it = actionList.find(name);
   if (it != actionList.end()){
      auto res = it->second.otherPermissions.insert(temp);
      if (res.second == false){
         return false;
      }
      return true;
   }
   return false;
};


bool PermissionsDB::read_DB()
{
   std::fstream in; size_t i;
   size_t userListSize;
   size_t nextUID_file;
   size_t groupListSize;
   size_t nextGID_file;
   size_t patientFilesSize;
   size_t actionListSize;
   in.open(DBPath.string(), std::fstream::in);
   if (in.is_open() && in.good()){
      // Try to read in the 4 numbers at the start of the file.
      in >> userListSize;
      in >> nextUID_file;
      in >> groupListSize;
      in >> nextGID_file;
      in >> patientFilesSize;
      in >> actionListSize;
   }
   else{
      return false;
   }

   for (i = 0; i < userListSize; i++){
      std::string temp;
      in >> temp;
      UserEntry newUser(temp);
      userList.insert(std::pair<uint_fast64_t, UserEntry>(newUser.UID, newUser));
   }

   for (i = 0; i < patientFilesSize; i++){
      std::string tempName; unsigned long long int tempOwner;
      in >> tempName;
      in >> tempOwner;
      FileData tempFD{tempName, tempOwner};
      patientFiles.push_back(tempFD);
   }

   for (i = 0; i < groupListSize; i++){
      std::string tempName; unsigned long long int GID;
      in >> GID;
      in >> tempName;
      GroupType tempG{tempName, GID};
      groupList.insert(std::pair<uint_fast64_t, GroupType>(GID, tempG));
   }

   i = 0; std::string nextLine;
   while(i < actionListSize && std::getline(in, nextLine)){
      if (nextLine.empty()){
         continue;
      }
      size_t firstSpace = nextLine.find(' ');
      APRs temp(nextLine.substr(firstSpace + 1));
      actionList.insert(std::pair<std::string, APRs>(nextLine.substr(0, firstSpace), temp));
   }
   if (i != actionListSize){
      std::cerr << "Number of actions not consistent.\n";
   }
   nextUID = nextUID_file;
   nextGID = nextGID_file;
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

   tempFile << userList.size() << " " << nextUID << " " << groupList.size() << " " << nextGID << " " << patientFiles.size() << " " << actionList.size() << std::endl;
   for (auto iter : userList){
      tempFile << iter.second.to_string() << "\n";
   }

   for (const FileData& iter : patientFiles){
      tempFile << iter.name << " " << iter.ownerID << "\n"; // Just ignore spaces in file names...
   }

   for (auto iter : groupList){
      tempFile << iter.first << " " << iter.second.name << "\n";
   }

   for (auto iter : actionList){
      tempFile << iter.first << " " << iter.second.to_string() << "\n";
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

















