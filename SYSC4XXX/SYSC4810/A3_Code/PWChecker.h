#ifndef PWCHECKER_H
#define PWCHECKER_H

#include <iostream> // For std::cout and std::endl, mostly for testing
#include <regex> // For std::regex and regex objects
#include <filesystem> // For std::filesystem::path
#include <fstream> // For fileIO
#include <sstream> // For stringstream and tokenization of inputs
#include <vector> // For vectors
#include <stdexcept> // For exception classes
#include <set> // For set

// These are used for rules that involve the username or password as variables in the regex.
// The dummy vales are to check that a given regex with variables in them are valid.
// The minimum length is to ignore username based rules when the given username length is equal
// or less than that value, to avoid vexatious blocking of valid passwords.
#define USERNAME_MACRO "$(USERNAME)"
#define PASSWORD_MACRO "$(PASSWORD)"
#define MIN_USERNAME_LENGTH 2
#define DUMMY_USERNAME "(DummyUser)"
#define DUMMY_PASSWORD "(DummyPass)"

// These are defaults for certain variables used if not given in the password rules.
// The values are inclusive unless specified otherwise.
#define DEFAULT_MINIMUM_CHARACTERS 8
#define DEFAULT_MAXIMUM_CHARACTERS 12

#define MINIMUM_CHARACTERS_STRING "MINIMUM_CHARACTERS"
#define MAXIMUM_CHARACTERS_STRING "MAXIMUM_CHARACTERS"

#define REQUIREMENT_STRING "REQUIRE"
#define RESTRICTION_STRING "PROHIBIT"

namespace fs = std::filesystem;

// Thanks to https://regex101.com/ for telling me why my regex is bad and I should feel bad.
// https://stackoverflow.com/questions/19605150/regex-for-password-must-contain-at-least-eight-characters-at-least-one-number-a
// Uses regex modified from answer by user 'anubhava'





class PWChecker
{
   public:
      /** Default constructor */
      PWChecker();
      /** Default destructor */
      virtual ~PWChecker();

      // This just simplifies the vector templates.
      struct rStr{
         bool expected;
         std::regex_constants::syntax_option_type expFlags;
         std::string regStr;
      };

      void print_values(); // For debugging
      // Not compatable with non-ascii whitespace, but unicode support is pain.
      // Sure hope I got all those escape characters...
      const std::string whitespace = " \t\r\n\v\f";
      const std::string regexchars = "-()[]{}*+?.\\^$|#,/:!<=";
      static constexpr const char* regexflagchars = "gimsuy"; // Only the i flag is implemented at this time, regex is too hard to learn in a day :|

      rStr parseRuleFromLine(std::vector <std::string> inputTokens, std::regex_constants::syntax_option_type* globalFlags = nullptr);
      bool read_password_rules_file(const std::filesystem::path& filePath);
      bool read_common_patterns_file(const std::filesystem::path& filePath);
      bool read_restricted_passwords_file(const std::filesystem::path& filePath);

      // Returns 0 if good, a positive value corresponding to the failed regex rule,
      // or a negative value for a failed common password check.
      int check_password(const std::string& username, const std::string& password);

   protected:

   private:
      std::vector <rStr> passwordRegexRules; // These are the rules all passwords must follow, has macros for usernames and passwords
      std::vector <std::regex> commonPasswordsFilter; // These are rules that disqualify passwords, they are not allowed to match.
      std::set <std::string> restrictedPasswordsFilter; // These are explicitly disallowed passwords, they are matched 1 to 1.
      int minimumLength;
      int maximumLength;


};

#endif // PWCHECKER_H










