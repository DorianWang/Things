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
   //passwordFilter.print_values();
   groupList.insert(std::pair<uint_fast64_t, GroupType>(0, GroupType{std::string("DEFAULT"), 0}));
   userList.insert(std::pair<uint_fast64_t, UserEntry>(0, UserEntry(0, 0, std::string("DEFAULT"), std::string("DEFAULT"))));
   nextUID = 1; nextGID = 1;
   assert(sizeof(uint64_t) == (sizeof(unsigned char) * 8)); // My code will explode if this is false.
   isAfterHours = false;
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

bool PermissionsDB::scrypt_hashing(const std::string& password, uint64_t salt, unsigned char* hashOut, size_t outSize)
{
   // Follows the example given in the docs.
   // https://www.openssl.org/docs/man3.0/man7/EVP_KDF-SCRYPT.html
   if (EVP_PKEY_CTX_set1_pbe_pass(pctx, password.c_str(), password.length()) <= 0) {
      std::cerr << "Failed to set password\n";
      return false;
   }
   unsigned char buffer[8];
   memcpy(buffer, &salt, 8);
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

bool PermissionsDB::scrypt_generate_hash(const std::string& password, UserEntry& usr)
{
   unsigned char buffer[8];
   if (RAND_bytes(buffer, 8) == 1){
      memcpy(&(usr.salt), buffer, 8);
   }
   else{
      std::cerr << "Failed to get random salt\n";
      return false;
   }
   return scrypt_hashing(password, usr.salt, usr.hashedPass, HASH_OUTPUT_LENGTH);
}

bool PermissionsDB::scrypt_hash_check(const std::string& password, const UserEntry& usr)
{
   unsigned char givenHash[HASH_OUTPUT_LENGTH];
   if (scrypt_hashing(password, usr.salt, givenHash, HASH_OUTPUT_LENGTH)){
      return memcmp(givenHash, usr.hashedPass, HASH_OUTPUT_LENGTH) == 0;
   }
   return false; // Hashing failed.
}

uint_fast64_t PermissionsDB::get_UID_from_username(const std::string& username)
{
   // Should have made a bidirectional map >.<
   for (auto iter : userList){
      if (iter.second.username == username){
         return iter.first;
      }
   }
   return 0;
}

uint_fast64_t PermissionsDB::add_user(std::string name, std::string username, uint_fast64_t GID, std::string password)
{
   if (groupList.count(GID) == 0){
      return 0; // Bad group.
   }
   if (passwordFilter.check_password(username, password) != 0){
      return 0; // Bad password.
   }
   if (get_UID_from_username(username)){
      return 0; // User already exists.
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
   if (scrypt_generate_hash(password, temp)){
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
   for (auto iter : groupList){
      if (iter.second.name == name){
         return iter.first; // Give back the same ID, this would be better as a bidirectional map?
      }
      if (iter.first == nextGID) nextGID++;
   }
   while (groupList.count(nextGID)){
      if (nudgeCounter > 64){
         return 0; // Just give up.
      }
      nextGID++; nudgeCounter++;
   }
   GroupType temp(name, nextGID);
   groupList.insert(std::pair<uint_fast64_t, GroupType>(nextGID, temp));
   return nextGID++;
}

bool PermissionsDB::add_activity(std::string name, int permissions){
   if (actionList.count(name)){
      return false;
   }
   APRs temp;
   temp.owner_rwe[2] = permissions & 4; temp.owner_rwe[1] = permissions & 2; temp.owner_rwe[0] = permissions & 1;
   temp.name = name;
   actionList.insert(std::pair<std::string, APRs>(name, temp));
   return true;
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
      auto existingEntry = it->second.otherPermissions.find(temp);
      if (existingEntry != it->second.otherPermissions.end()){
         int existingPerms = existingEntry->getPermissions();
         if ((existingPerms | permissions) == existingPerms){
            return true;
         }
         it->second.otherPermissions.erase(existingEntry);
         temp.setPermissions(existingPerms | permissions);
         it->second.otherPermissions.insert(temp);
      }
      else{
         it->second.otherPermissions.insert(temp);
      }
      return true;
   }
   return false;
};

bool PermissionsDB::toggle_after_hours(){ isAfterHours = !isAfterHours; return isAfterHours;};
bool PermissionsDB::toggle_group_restricted_hours(uint_fast64_t GID){
   if (afterHoursRestrictedGroups.count(GID)){
      afterHoursRestrictedGroups.erase(GID);
      return true;
   }
   afterHoursRestrictedGroups.insert(GID);
   return true;
}

uint_fast64_t PermissionsDB::try_login(const std::string& username, const std::string& password)
{
   uint_fast64_t UID = get_UID_from_username(username);
   if (UID == 0){
      // If username information is also considered PHI, then maybe wait a bit
      // and give a generic "Username or password incorrect" sort of thing.
      std::cout << "Username not found." << std::endl;
      return 0;
   }

   if (scrypt_hash_check(password, userList[UID])){
      if (isAfterHours && afterHoursRestrictedGroups.count(userList[UID].GID)){
         std::cout << "Sorry but you are not allowed access to the system outside business hours." << std::endl;
         return 0;
      }
      return UID; // Using UID as token in this mess.
   }
   else{
      std::cout << "Sorry but we could not verify your password with that username." << std::endl;
      return 0;
   }
}

int PermissionsDB::check_action_permissions(uint_fast64_t UID, const std::string& actionName, size_t target)
{
   uint_fast64_t usrGID;
   try{
      usrGID = userList.at(UID).GID;
   }
   catch (const std::out_of_range&){
      // Also put some sort of error here, it probably shouldn't happen often in a real system.
      // Unless someone is purposely trying to DDOS the system I guess.
      return NONE_PERM;
   }

   if (isAfterHours && afterHoursRestrictedGroups.count(usrGID)){
      std::cout << "Sorry but you are not allowed access to the system outside business hours." << std::endl;
      return NONE_PERM;
   }

   try{
      const APRs& activity = actionList.at(actionName);
      FileData* temp = nullptr;
      if (target < patientFiles.size()){
         temp = patientFiles.data() + target; // C code fixes procrastination!
      }
      return activity.getPermissions(UID, usrGID, temp);
   }
   catch (const std::out_of_range&){
      // Also put some sort of error here, it probably shouldn't happen often in a real system.
      // Unless someone is purposely trying to DDOS the system I guess.
      return NONE_PERM;
   }
   return NONE_PERM;
}

std::vector <std::string> PermissionsDB::getActionsAndFiles()
{
   std::vector <std::string> res;
   for (const auto& iter : actionList){
      res.push_back(iter.first);
   }
   res.push_back("\n Possible targets:");
   for (size_t i = 0; i < patientFiles.size(); i++){
      res.push_back(patientFiles[i].name + ' ' + std::to_string(i));
   }
   return res;
}

// How to debug, just use prints!
void PermissionsDB::print_internal_variables()
{
   std::cout << "There are currently: " << userList.size() - 1 << " Users registered." << std::endl;
   std::cout << "There are currently: " << groupList.size() - 1 << " Groups registered." << std::endl;
   std::cout << "The next UIDs and GIDs to be distributed are: " << nextUID << " and " << nextGID << std::endl;
   std::cout << "Here is the User list:" << std::endl;
   for (auto iter : userList){
      std::cout << iter.second.to_string() << "\n";
   }
   std::cout << "\nHere is the Group list:" << std::endl;
   for (auto iter : groupList){
      std::cout << iter.first << " " << iter.second.name << "\n";
   }

   std::cout << std::endl;
   std::cout << "There are currently: " << patientFiles.size() << " Targets registered." << std::endl;
   std::cout << "There are currently: " << actionList.size() << " Actions registered." << std::endl;
   std::cout << "Here is the target list:" << std::endl;
   for (auto iter : patientFiles){
      std::cout << iter.name << " Owned by: " <<userList[iter.ownerID].name << "\n";
   }
   std::cout << "\nHere is the Action list with additional permissions:" << std::endl;
   for (auto iter : actionList){
      std::cout << "Name: " << iter.first << " Target owner permissions: " << bool_array_to_int(iter.second.owner_rwe) << ", " << iter.second.otherPermissions.size() << " Group Permissions:" << "\n";
      for (const GroupPermissions& permsIter : iter.second.otherPermissions){
         std::cout << "\t" << groupList.at(permsIter.groupID).name << " " << bool_array_to_int(permsIter.rwe) << "\n";
      }
   }
   std::cout << std::endl;
}

bool PermissionsDB::read_DB()
{
   std::fstream in; size_t i = 0; std::string nextLine;
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

   while(i < userListSize && std::getline(in, nextLine)){
      if (nextLine.empty()){
         continue;
      }
      UserEntry newUser(nextLine);
      userList.insert(std::pair<uint_fast64_t, UserEntry>(newUser.UID, newUser));
      i++;
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

   i = 0;
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
   tempFile.close(); // Don't try to move an open file, dummy
   try{
      std::filesystem::rename(tempFilePath, DBPath);
   }
   catch ( std::filesystem::filesystem_error& e ){
      std::cerr << "Failed to rename temporary file to database file, aborting.\n";
      std::cerr << e.what() << std::endl;
      return false;
   }

   return true;
}

















