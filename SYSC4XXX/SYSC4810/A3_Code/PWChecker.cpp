#include "PWChecker.h"

PWChecker::PWChecker()
{
   minimumLength = DEFAULT_MINIMUM_CHARACTERS;
   maximumLength = DEFAULT_MAXIMUM_CHARACTERS;
}

PWChecker::~PWChecker()
{
   //dtor
}

// Debugging function
void PWChecker::print_values()
{
   std::cout << "Number of rules and common passes: " << passwordRegexRules.size() << " " << commonPasswordsFilter.size() << std::endl;
   std::cout << "Rules: " << std::endl;
   for (std::pair<bool, std::string>& rules : passwordRegexRules)
   {
      std::cout << rules.first << " " << rules.second << std::endl;
   }
   std::cout << "Max and Min are: " << maximumLength << " "<< minimumLength << std::endl;

}

bool PWChecker::read_password_rules_file(const std::filesystem::path& filePath)
{
   std::fstream in;
   try{
      std::filesystem::file_status st = std::filesystem::status(filePath);
      if (std::filesystem::exists(st)){
         if(std::filesystem::is_regular_file(st)){
            // Try to read it. If no permissions, just let the exception go.
            in.open(filePath.string(), std::fstream::in);
         }
      }
      else{
         return false;
      }
   }
   catch (std::filesystem::filesystem_error& ){
      return false;
   }

   if (in.is_open() && in.good()){
      std::string line;
      while (getline(in, line))
      {
         //Cleaning up the input a little.
         if (line.empty()) continue;
         const auto strBegin = line.find_first_not_of(whitespace);
         if (strBegin == std::string::npos) continue;
         std::string cleanString = line.substr( strBegin, line.find_last_not_of(whitespace) + 1 );
         if (cleanString.at(0) == '#') continue; //Comment, ignore

         // Only splits on spaces which is lazy, but I don't care enough for that.
         std::istringstream iss(cleanString);
         std::vector <std::string> subStrings;
         while (iss)
         {
            std::string sub;
            iss >> sub;
            if (sub.empty() == false){
               subStrings.push_back(sub);
            }
         }

         if (subStrings.size() < 2){
            // Give an error and return.
            return false;
         }

         if (subStrings[0] == MINIMUM_CHARACTERS_STRING){
            try{
               int result = stoi(subStrings[1]);
               minimumLength = result;
            }
            catch( const std::invalid_argument& ){
               std::cerr << "Given input \"" << subStrings[1] << "\" is not an integer\n";
            }
            catch( const std::out_of_range& ){
              std::cerr << "Given input \"" << subStrings[1] << "\" is out of range\n";
            }
            continue; // Continues the while loop above.
         }
         else if (subStrings[0] == MAXIMUM_CHARACTERS_STRING){
            try{
               int result = stoi(subStrings[1]);
               maximumLength = result;
            }
            catch( const std::invalid_argument& ){
               std::cerr << "Given input \"" << subStrings[1] << "\" is not an integer\n";
            }
            catch( const std::out_of_range& ){
               std::cerr << "Given input \"" << subStrings[1] << "\" is out of range\n";
            }
            continue; // Continues the while loop above.
         }
         else {
            bool isRequirement = false;
            if (subStrings[0] == REQUIREMENT_STRING){
               isRequirement = true;
            }
            if (isRequirement || subStrings[0] == RESTRICTION_STRING){
               std::string regexTestStr = subStrings[1];
               size_t userMacro = subStrings[1].find(USERNAME_MACRO);
               size_t passMacro = subStrings[1].find(PASSWORD_MACRO);

               if (userMacro != std::string::npos){
                  if (passMacro != std::string::npos){
                     std::cerr << "Regex should not have both username and password macros.\n";
                     continue;
                  }
                  regexTestStr.replace(userMacro, std::string(USERNAME_MACRO).length(), DUMMY_USERNAME);
               }
               if (passMacro != std::string::npos){
                  regexTestStr.replace(passMacro, std::string(PASSWORD_MACRO).length(), DUMMY_PASSWORD);
               }

               try{
                  std::regex newRule(regexTestStr);
               }
               catch (const std::regex_error& ) {
                  std::cerr << "Input regex rule: \"" << cleanString
                           << "\" is invalid, skipping!\n";
                  continue;
               }

               passwordRegexRules.push_back(std::pair<bool, std::string>(isRequirement, subStrings[1]));
            }
            else {
               std::cerr << "Given input \"" << subStrings[0] << "\" does not match known keyword\n";
            }
         }
      }
   }
   else{
      return false;
   }
   return true;
}

bool PWChecker::read_common_passwords_file(const std::filesystem::path& filePath)
{
   std::fstream in;
   try{
      std::filesystem::file_status st = std::filesystem::status(filePath);
      if (std::filesystem::exists(st)){
         if(std::filesystem::is_regular_file(st)){
            // Try to read it. If no permissions, just let the exception go.
            in.open(filePath.string(), std::fstream::in);
         }
      }
      else{
         return false;
      }
   }
   catch (std::filesystem::filesystem_error& ){
      return false;
   }

   if (in.is_open() && in.good()){
      std::string line;
      while (getline(in, line))
      {
         //Cleaning up the input a little.
         if (line.empty()) continue;
         const auto strBegin = line.find_first_not_of(whitespace);
         if (strBegin == std::string::npos) continue;
         std::string cleanString = line.substr( strBegin, line.find_last_not_of(whitespace) + 1 );
         if (cleanString.at(0) == '#') continue; //Comment, ignore

         try{
            std::cout << "New pass: " << cleanString << std::endl;
            std::regex newRule(cleanString);
            commonPasswordsFilter.push_back(newRule);
         }
         catch (const std::regex_error& ) {
            std::cerr << "Input regex rule: \"" << cleanString
                     << "\" is invalid, skipping!\n";
         }
      }
   }
   else{
      return false;
   }
   return true;
}

// Returns 0 if good, 1 if length error, positive for regex, negative for common passwords.
int PWChecker::check_password(const std::string& username, const std::string& password)
{
   std::string cleanUsername; std::string cleanPassword;
   cleanUsername.append(1, '('); cleanPassword.append(1, '(');
   for (size_t i = 0; i < username.length(); i++){
      if (regexchars.find(username[i])){
         cleanUsername.append(1, '\\');
      }
      cleanUsername.append(1, username[i]);
   }
   for (size_t i = 0; i < password.length(); i++){
      if (regexchars.find(password[i])){
         cleanPassword.append(1, '\\');
      }
      cleanPassword.append(1, password[i]);
   }
   cleanUsername.append(1, ')'); cleanPassword.append(1, ')');

   if (password.length() > maximumLength || password.length() < minimumLength){
      return 1;
   }

   for (size_t i = 0; i < passwordRegexRules.size(); i++){
      std::string regexString = passwordRegexRules[i].second;
      bool checkingPassword = true;
      size_t userMacro = regexString.find(USERNAME_MACRO);
      size_t passMacro = regexString.find(PASSWORD_MACRO);
      if (userMacro != std::string::npos){
         regexString.replace(userMacro, std::string(USERNAME_MACRO).length(), cleanUsername);
      }
      if (passMacro != std::string::npos){
         regexString.replace(passMacro, std::string(PASSWORD_MACRO).length(), cleanPassword);
         checkingPassword = false;
      }

      std::regex newRegex(regexString);
      if (checkingPassword){
         bool result = std::regex_match(password, newRegex);
         if (result != passwordRegexRules[i].first){
            return i + 2;
         }
      }
      else{
         bool result = std::regex_match(username, newRegex);
         if (result != passwordRegexRules[i].first){
            return i + 2;
         }
      }
   }

   for (size_t i = 0; i < commonPasswordsFilter.size(); i++){
      if (std::regex_match(password, commonPasswordsFilter[i])){
         return (-1 - i);
      }
   }

   return 0; // Did not hit any filters!
}









