#include <iostream>

#include "PWDB.h"
#include "PWChecker.h"

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <filesystem> // For std::filesystem

using std::cout;
using std::endl;

int main()
{
   // Testing PWChecker
   PWChecker tester;

   tester.print_values();

   std::filesystem::path newerPath("newDB.txt");
   PWDB::PermissionsDB testDB(newerPath);


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











