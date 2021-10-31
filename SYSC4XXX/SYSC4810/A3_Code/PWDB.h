#ifndef PWDB_H
#define PWDB_H

// Hashing and other crypto stuff
#include <openssl/sha.h>
#include <openssl/evp.h>

#include "PWChecker.h"

#include <string> // For strings as well as stoll() and to_string functions.
#include <iostream> // For cout, endl, etc
#include <filesystem> // For std::filesystem

#include <map>
#include <set> // For std::set, used to hold permissions, groups and users.
// I really should be using a sorted vector, but I know the report will take a whole day
// so corners are getting destroyed here.

namespace PWDB{

const bool defaultPerms[3] = {false, false, false};

struct GroupType
{
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

      if (tokens.size() != 4){
         std::cerr << "Given UserEntry \"" << DB_string << "\" is not valid\n";
      }
      else{
         // Format of UID, GID, username, name. More entries can be added after.
         try{
            UID = std::stoull(tokens[3]);
            GID = std::stoull(tokens[2]);
            username = tokens[1];
            name = tokens[0];
         }
         catch(const std::invalid_argument&){
            std::cerr << "A given UserEntry Argument is not valid\n";
         }
      }
   };
   UserEntry(){};
   std::string to_string() const{
      return (std::to_string((unsigned long long) UID) + ',' + std::to_string((unsigned long long) GID) + ','
               + username + ',' + name);
   }
   std::string username;
   std::string name;
   uint_fast64_t UID;
   uint_fast64_t GID;
};

// These structs might be more efficient with bitsets instead of booleans, but I won't bother.
struct GroupPermissions
{
   GroupPermissions(uint_fast64_t newID, const bool perms[] = defaultPerms){
      groupID = newID;
      if (perms){
         rwe[0] = perms[0]; rwe[1] = perms[1]; rwe[2] = perms[2];
      }
   };
   virtual ~GroupPermissions();
   uint_fast64_t groupID;
   bool rwe[3];
   friend bool operator<(const GroupPermissions& lhs, const GroupPermissions& rhs){
      return lhs.groupID < rhs.groupID;
   }
   int getPermissions() const {
      return (rwe[0] ? 4 : 0) + (rwe[1] ? 2 : 0) + (rwe[0] ? 1 : 0);
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
   std::string name; // This struct would be attached to an actual object/caller normally.
   // These are what permissions the owner (set in FileData) has for a file.
   bool owner_rwe[3];
   std::set <GroupPermissions> otherPermissions;
   int getPermissions(uint_fast64_t UID, uint_fast64_t GID, const FileData& target) const {
      if (UID == target.ownerID){
         return (owner_rwe[0] ? 4 : 0) + (owner_rwe[1] ? 2 : 0) + (owner_rwe[0] ? 1 : 0);
      }
      else{
         GroupPermissions tester(GID);
         const std::set<GroupPermissions>::iterator perms = otherPermissions.find(tester);
         if (perms == otherPermissions.end()){
            return (defaultPerms[0] ? 4 : 0) + (defaultPerms[1] ? 2 : 0) + (defaultPerms[0] ? 1 : 0);
         }
         else{
            return (*perms).getPermissions();
         }
      }
   }
   std::string to_string() const {
      std::string temp = name; temp += ' ';
      temp += std::to_string((owner_rwe[0] ? 4 : 0) + (owner_rwe[1] ? 2 : 0) + (owner_rwe[0] ? 1 : 0));
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

   protected:

   private:
      std::vector <FileData> patientFiles;
      std::map <uint_fast64_t, UserEntry> userList;
      std::map <std::string, APRs> actionList;
      uint_fast64_t nextUID; // Just count UIDs up for now.
      std::filesystem::path DBPath;

      bool read_DB();
      bool write_DB();

      bool add_usr_DB(UserEntry& newUser, std::string salted_pass);
      bool rmv_usr_DB(uint_fast64_t UID);

      bool chg_pw_DB(uint_fast64_t UID, std::string salted_pass);

};

} // End namespace
#endif // PWDB_H



