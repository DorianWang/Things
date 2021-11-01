#ifndef PWDB_H
#define PWDB_H

// Hashing and other crypto stuff
#include <openssl/rand.h>
#include <openssl/kdf.h>
#include <openssl/evp.h>

#include "PWChecker.h"

#include <string> // For strings as well as stoll() and to_string functions.
#include <iostream> // For cout, endl, etc
#include <filesystem> // For std::filesystem
#include <sstream> // For the power of stringstreams
#include <cassert> // For assert() statement in constructor.

#include <map> // To hold ID -> object relationships, but I should have made a bidirectional map.
#include <set> // For std::set, used to hold permissions, groups and users.
// I really should be using a sorted vector, but I know the report will take a whole day
// so corners are getting destroyed here.

// Definitely a bit low for scrypt inputs, but it hurts my computer trying to run it at all, so just say it's for testing.
#define HASH_OUTPUT_LENGTH 32
#define SCRYPT_N 262144
#define SCRYPT_r 8
#define SCRYPT_p 1

#define RULES_FILE "rules.txt"
#define COMMON_RULES_FILE "common_passes.txt"
#define FORBIDDEN_PASSES_FILE "forbidden_passes.txt"



namespace PWDB{

const bool defaultPerms[3] = {false, false, false};
constexpr int DEFAULT_PERM = 0; constexpr int NONE_PERM = 0;
constexpr int READ = 4; constexpr int WRITE = 2; constexpr int EXECUTE = 1;

inline int bool_array_to_int(const bool* arrIn) {
   if (arrIn){
      return (arrIn[2] ? 4 : 0) + (arrIn[1] ? 2 : 0) + (arrIn[0] ? 1 : 0);
   }
   return (defaultPerms[2] ? 4 : 0) + (defaultPerms[1] ? 2 : 0) + (defaultPerms[0] ? 1 : 0);
}

inline unsigned char two_hex_to_uchar(unsigned char upper, unsigned char lower)
{
   unsigned char out = 0;
   if (upper >= '0' && upper <= '9'){
      out += (upper - '0') * 16;
   }
   else{
      out += (upper - 'A' + 10) * 16;
   }
   if (lower >= '0' && lower <= '9'){
      out += (lower - '0');
   }
   else{
      out += (lower - 'A' + 10);
   }
   return out;
}

inline std::string hex_str_from_uchar_array(const unsigned char* arrIn, size_t inputSize)
{
   // A bit heavy, but it could be fixed by someone else. Probably. Maybe...
   std::stringstream ss;
   for (size_t i = 0; i < inputSize; i++){
      ss << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (int) arrIn[i];
   }
   return ss.str();
}

inline bool hex_str_to_uchar_array(const std::string& hexInput, unsigned char* arrOut, size_t outputSize)
{
   size_t outputCounter = outputSize;
   if (hexInput.length() % 2 || hexInput.length() < 2) return false;
   for (size_t i = hexInput.length() - 2; i > 0; i = i - 2){
      if (outputCounter == 0){
         std::cerr << hexInput.size() << " " << outputSize << "\n";
         std::cerr << "Hex string input longer than output array capacity.\n";
         break;
      }
      outputCounter--;
      arrOut[outputCounter] = two_hex_to_uchar(hexInput[i], hexInput[i + 1]);
   }
   while(outputCounter > 0){
      outputCounter--;
      arrOut[outputCounter] = 0;
   }
   return true;
}

// These structs might be more efficient with bitsets instead of booleans, but I won't bother.
struct GroupType
{
   GroupType(){};
   GroupType(std::string nname, uint_fast64_t nID){name = nname; ID = nID;};
   std::string name;
   uint_fast64_t ID;
   friend bool operator<(const GroupType& lhs, const GroupType& rhs){
      return lhs.ID < rhs.ID;
   }
};

struct FileData
{
   // This is all dummy stuff to be patched into a real database.
   std::string name;
   uint_fast64_t ownerID;
};

struct UserEntry
{
   UserEntry(std::string DB_string){
      std::vector<std::string> tokens;
      size_t currentComma = DB_string.find_last_of(',');
      size_t lastComma = DB_string.size();
      while(currentComma != std::string::npos){
         // Just going to assume no commas in username or name.
         if (currentComma < lastComma - 1){
            tokens.push_back(DB_string.substr(currentComma + 1, lastComma - currentComma - 1));
         }
         if (currentComma == 0){
            lastComma = 0;
            break;
         }
         lastComma = currentComma;
         currentComma = DB_string.find_last_of(',', currentComma - 1);
      }
      if (lastComma != 0)
         tokens.push_back(DB_string.substr(0, lastComma));

      if (tokens.size() != 6){
         std::cerr << "Given UserEntry \"" << DB_string << "\" is not valid\n";
      }
      else{
         // Format of UID, GID, username, name, salt, and hash.
         try{
            UID = std::stoull(tokens[5]);
            GID = std::stoull(tokens[4]);
            username = tokens[3];
            name = tokens[2];
            salt = std::stoull(tokens[1]);
            hex_str_to_uchar_array(tokens[0], hashedPass, HASH_OUTPUT_LENGTH);
         }
         catch(const std::invalid_argument&){
            std::cerr << "A given UserEntry Argument is not valid\n";
         }
      }
   };
   UserEntry(){};
   UserEntry(uint_fast64_t nUID, uint_fast64_t nGID, std::string nname, std::string nuname){UID = nUID; GID = nGID; name = nname; username = nuname;};
   std::string to_string() const{
      return (std::to_string((unsigned long long) UID) + ',' + std::to_string((unsigned long long) GID) + ','
               + username + ',' + name + ',' + std::to_string(salt) + ',' + hex_str_from_uchar_array(hashedPass, HASH_OUTPUT_LENGTH));
   }
   std::string username;
   std::string name;
   uint_fast64_t UID;
   uint_fast64_t GID;
   unsigned char hashedPass[HASH_OUTPUT_LENGTH];
   uint64_t salt; // 64 bits should be fine if it's random, NIST recommends minimum 32 bits
};

struct GroupPermissions
{
   GroupPermissions(){};
   GroupPermissions(uint_fast64_t newID, const bool perms[] = defaultPerms){
      groupID = newID;
      if (perms){
         rwe[0] = perms[0]; rwe[1] = perms[1]; rwe[2] = perms[2];
      }
   };
   uint_fast64_t groupID;
   bool rwe[3];
   friend bool operator<(const GroupPermissions& lhs, const GroupPermissions& rhs){
      return lhs.groupID < rhs.groupID;
   }
   void getPermissions(const bool perms[]){
      rwe[0] = perms[0]; rwe[1] = perms[1]; rwe[2] = perms[2];
   }
   int getPermissions() const {
      return (rwe[2] ? 4 : 0) + (rwe[1] ? 2 : 0) + (rwe[0] ? 1 : 0);
   }
   std::string to_string() const {
      std::string temp = std::to_string((unsigned long long)groupID);
      temp += ' ';
      temp += std::to_string(getPermissions());
      return temp;
   }
};

//ActionPermissionRequirements
struct APRs
{
   APRs(){};
   APRs(std::string DB_string){
      std::istringstream iss(DB_string);
      std::vector <std::string> subStrings;
      while (iss)
      {
         std::string sub;
         iss >> sub;
         if (sub.empty() == false){
            subStrings.push_back(sub);
         }
      }
      if (subStrings.size() % 2 != 0 || subStrings.size() < 2){
         std::cerr << "Given Requirements \"" << DB_string << "\" is not valid\n";
      }
      else{
         name = subStrings[0];
         int ownerPerms = std::stoul(subStrings[1]);
         owner_rwe[0] = !!(ownerPerms & 1); owner_rwe[1] = !!(ownerPerms & 2); owner_rwe[2] = !!(ownerPerms & 4);
         size_t i = 2; bool newPerms[3];
         while (i < subStrings.size()){
            uint_fast64_t newID = std::stoull(subStrings[i]);
            int permValue = std::stoul(subStrings[i+1]);
            newPerms[0] = !!(permValue & 1); newPerms[1] = !!(permValue & 2); newPerms[2] = !!(permValue & 4);
            GroupPermissions temp(newID, newPerms);
            otherPermissions.insert(temp);
         }
      }
   }

   std::string name; // This struct would be attached to an actual object/caller normally.
   // These are what permissions the owner (set in FileData) has for a file.
   bool owner_rwe[3];
   std::set <GroupPermissions> otherPermissions;
   int getPermissions(uint_fast64_t UID, uint_fast64_t GID, const FileData* target) const {
      if (target != nullptr && UID == target->ownerID){
         return (owner_rwe[2] ? 4 : 0) + (owner_rwe[1] ? 2 : 0) + (owner_rwe[0] ? 1 : 0);
      }
      else{
         GroupPermissions tester(GID);
         const std::set<GroupPermissions>::iterator perms = otherPermissions.find(tester);
         if (perms == otherPermissions.end()){
            return (defaultPerms[2] ? 4 : 0) + (defaultPerms[1] ? 2 : 0) + (defaultPerms[0] ? 1 : 0);
         }
         else{
            return (*perms).getPermissions();
         }
      }
   }
   std::string to_string() const {
      std::string temp = name; temp += ' ';
      temp += std::to_string((owner_rwe[2] ? 4 : 0) + (owner_rwe[1] ? 2 : 0) + (owner_rwe[0] ? 1 : 0));
      for (auto iter : otherPermissions){
         temp += ' ';
         temp += iter.to_string();
      }
      return temp;
   }
};

class PermissionsDB
{
   public:
      PermissionsDB(const std::filesystem::path& newDBPath);
      /** Default destructor */
      virtual ~PermissionsDB();
      void set_DB_path(const std::filesystem::path& newDBPath);

      bool set_restricted_patterns_file(const std::filesystem::path& filePath);
      int check_password();
      bool scrypt_hashing(const std::string& password, uint64_t salt, unsigned char* hashOut, size_t outSize);
      bool scrypt_hash_check(const std::string& password, const UserEntry& usr);
      bool scrypt_generate_hash(const std::string& password, UserEntry& usr);

      // No removal functions, basically done with this.
      // Zero for failure, ID for success.
      uint_fast64_t add_user(std::string name, std::string username, uint_fast64_t GID, std::string password);
      uint_fast64_t add_group(std::string name);
      bool add_activity(std::string name, int permissions = DEFAULT_PERM);
      bool add_target(std::string name, uint_fast64_t ownerID);
      bool add_permission_to_activity(std::string name, uint_fast64_t GID, int permissions);

      // Assume some token is given to the user for secure communications afterwards.
      uint_fast64_t get_UID_from_username(const std::string& username);

      bool toggle_after_hours();
      bool toggle_group_restricted_hours(uint_fast64_t GID);

      uint_fast64_t try_login(const std::string& username, const std::string& password);
      int check_action_permissions(uint_fast64_t UID, const std::string& actionName, size_t target = std::string::npos);

      void print_internal_variables(); // For testing.
      bool read_DB();
      bool write_DB();

   private:
      std::vector <FileData> patientFiles;
      std::map <uint_fast64_t, UserEntry> userList;
      std::map <std::string, APRs> actionList;
      std::map <uint_fast64_t, GroupType> groupList;
      std::set <uint_fast64_t> afterHoursRestrictedGroups;
      uint_fast64_t nextUID; // Just count IDs up for now.
      uint_fast64_t nextGID;
      std::filesystem::path DBPath;
      bool isAfterHours; // For testing.

      PWChecker passwordFilter;

      // https://www.openssl.org/docs/man1.1.1/man7/scrypt.html
      // It's built in and should work for passwords.
      EVP_PKEY_CTX* pctx = nullptr;




};

} // End namespace
#endif // PWDB_H






