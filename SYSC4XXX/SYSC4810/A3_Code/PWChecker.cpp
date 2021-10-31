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

// This parses the extra flags at the end so they actually work like javascript regexes.
PWChecker::rStr PWChecker::parseRuleFromLine(std::vector <std::string> inputTokens, std::regex_constants::syntax_option_type* globalFlags)
{
   bool isRequirement; std::regex_constants::syntax_option_type expFlags = *globalFlags;
   if (inputTokens.at(0) == REQUIREMENT_STRING){
      isRequirement = true;
   }
   else if (inputTokens.at(0)  == RESTRICTION_STRING){
      isRequirement = false;
   }
   else{
      return rStr{false, expFlags, std::string()};
   }
   // The regex library doesn't handle expression flags so I will.
   std::string regexString = inputTokens.at(1);
   if (regexString.empty()){
      return rStr{isRequirement, expFlags, std::string()};
   }

   if (regexString.at(0) == '/'){
      // Search for closing slash and then add any extra flags to outputFlags if it is not null.
      // It will try to check if the forward slash is escaped first, but unescaped forward slashes would be irregular regex.
      size_t lastSlash = 0;
      for (size_t i = regexString.size() - 1; i > 0; i--){
         // If I let this grow any more I'm just going to implement a shitty regex engine so I'll stop now >.<
         if (regexString[i] == '/'){
            int backslashCounter = 0;
            if (regexString[i - 1] == '\\')
               break; // Not possible to be a valid flag set.
            lastSlash = i;
            break;
         }
      }
      if (lastSlash){
            bool isFlags = true;
         // Take substring, then check if it looks like a set of expression flags. If so, remove slashes and flags from the string.
         // Not sure if flags are supposed to be case sensitive, so I will assume so for now.
         std::string expressionFlags = regexString.substr(lastSlash + 1);
         for (char in : expressionFlags){
            if (isFlags == false) break;
            switch (in){
            case regexflagchars[0]:
               break;
            case regexflagchars[1]:
               expFlags |= std::regex_constants::icase;
               break;
            case regexflagchars[2]:
            case regexflagchars[3]:
            case regexflagchars[4]:
            case regexflagchars[5]:
               // Currently either unimplemented, not applicable, requires more parsing, or is just a pain in the ass (unicode).
               break;
            default:
               std::cout << "Could not recognize flag: " << (int) in << std::endl;
               isFlags = false;
               break;
            }
         }
         if (isFlags){
            // https://stackoverflow.com/questions/28142011/can-you-assign-a-substring-of-a-stdstring-to-itself
            // Using part of the answer from user "J C Gonzalez" because it's cool, even if libstdc++ should make this operation safe.
            regexString.erase(lastSlash, std::string::npos); // Erase from the last slash to the end.
            regexString.erase(regexString.begin());
         }
      }
   }

   std::string regexTestStr = regexString;
   size_t userMacro = regexString.find(USERNAME_MACRO);
   size_t passMacro = regexString.find(PASSWORD_MACRO);

   if (userMacro != std::string::npos){
      if (passMacro != std::string::npos){
         std::cerr << "Regex \"" << regexString << "\" invalid, contains both username and password macros.\n";
         return rStr{isRequirement, expFlags, std::string()};
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
      std::cerr << "Input regex rule: \"" << regexString
               << "\" is invalid, skipping!\n";
      return rStr{isRequirement, expFlags, std::string()};
   }

   return rStr{isRequirement, expFlags, regexString};
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
      while (getline(in, line)){
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
            std::regex_constants::syntax_option_type outputFlags = std::regex_constants::ECMAScript;
            rStr res = parseRuleFromLine(subStrings, &outputFlags);
            if (res.regStr.length()){
               passwordRegexRules.push_back(res);
            }
         }
      }
   }
   else{
      return false;
   }
   return true;
}

bool PWChecker::read_common_patterns_file(const std::filesystem::path& filePath)
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
      while (getline(in, line)){
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

bool PWChecker::read_restricted_passwords_file(const std::filesystem::path& filePath)
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
      while (getline(in, line)){
         // Accepts whitespace as those characters could be part of a valid password in the future.
         // If I didn't procrastinate so hard I would make this also accept csv files.
         if (line.empty()) continue;
         const auto strBegin = line.find_first_not_of(whitespace);
         std::string cleanString = line.substr(strBegin);
         if (cleanString.at(0) == '#') continue; //Comment, ignore
         restrictedPasswordsFilter.insert(line);
      }
   }
   else{
      return false;
   }
   return true;
}

// Returns 0 if good, 1 if length error, rule index + 2 for regex, -1 for forbidden passwords, -2 - rule index for bad patterns.
// These could be used to give feedback to the user, but that isn't in the requirements and I'm pushing the deadline.
int PWChecker::check_password(const std::string& username, const std::string& password)
{
   std::string cleanUsername; std::string cleanPassword;
   cleanUsername.append(1, '('); cleanPassword.append(1, '(');
   for (size_t i = 0; i < username.length(); i++){
      if (regexchars.find(username[i]) != std::string::npos){
         cleanUsername.append(1, '\\');
      }
      cleanUsername.append(1, username[i]);
   }
   for (size_t i = 0; i < password.length(); i++){
      if (regexchars.find(password[i]) != std::string::npos){
         cleanPassword.append(1, '\\');
      }
      cleanPassword.append(1, password[i]);
   }
   cleanUsername.append(1, ')'); cleanPassword.append(1, ')');

   if (password.length() > maximumLength || password.length() < minimumLength){
      return 1;
   }

   for (size_t i = 0; i < passwordRegexRules.size(); i++){
      rStr& regexStruct = passwordRegexRules[i];
      std::string regexString = regexStruct.regStr;
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

      std::regex newRegex(regexString, regexStruct.expFlags);
      if (checkingPassword){
         bool result = std::regex_match(password, newRegex);
         if (result != regexStruct.expected){
            return i + 2;
         }
         std::cout << password << " " << i << std::endl;
      }
      else{
         bool result = std::regex_match(username, newRegex);
         if (result != regexStruct.expected){
            return i + 2;
         }
      }
   }

   for (size_t i = 0; i < commonPasswordsFilter.size(); i++){
      if (std::regex_match(password, commonPasswordsFilter[i])){
         return (-2 - i);
      }
   }

   for (size_t i = 0; i < restrictedPasswordsFilter.size(); i++){
      if (restrictedPasswordsFilter.count(password)){
         return (-1);
      }
   }

   std::cout << "No owwies..." << std::endl;
   return 0; // Did not hit any filters!
}









