#ifndef PWDB_H
#define PWDB_H

// Hashing and other crypto stuff
#include <openssl/sha.h>
#include <openssl/evp.h>

#include "PWChecker.h"

#include <iostream> // For cout, endl, etc
#include <vector> // For vector of regexs (sp?)
#include <filesystem> // For std::filesystem

class PWDB
{
   public:
      /** Default constructor */
      PWDB();
      /** Default destructor */
      virtual ~PWDB();

      bool set_restricted_patterns_file(const std::filesystem::path& filePath);
      size_t check_password();

   protected:

   private:
      std::vector <std::string> passwordRules;

      bool write_DB();
      bool read_DB();

      bool add_pw_DB();
      bool chg_pw_DB();
      bool rmv_pw_DB();


};

#endif // PWDB_H



