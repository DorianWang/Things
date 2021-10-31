#include <iostream>

#include "PWDB.h"
#include "PWChecker.h"

#include <filesystem> // For std::filesystem

using std::cout;
using std::endl;

int main()
{
   // Testing PWChecker
   PWChecker tester;
   std::filesystem::path rules("rules.txt");
   std::filesystem::path commonPasses("common_passes.txt");
   std::filesystem::path badPasses("forbidden_passes.txt");

   tester.read_password_rules_file(rules);
   tester.read_common_patterns_file(commonPasses);
   tester.read_restricted_passwords_file(badPasses);
   tester.print_values();

   cout << tester.check_password("A", "Qwerty1!") << endl;
   cout << tester.check_password("A", "Qwer1!") << endl;
   cout << tester.check_password("A", "Qwertysahdfku") << endl;
   cout << tester.check_password("A", "Qwerasdfty1!") << endl;
   cout << tester.check_password("Dorian", "Qwerasdfty1!") << endl;
   cout << tester.check_password("Dorian", "Dorian13?") << endl;
   cout << tester.check_password("Dorian", "dOrian13?") << endl;

   cout << "Hello world!" << endl;
   return 0;
}







