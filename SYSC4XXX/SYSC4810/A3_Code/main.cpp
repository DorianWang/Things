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
   testDB.add_group("Radiologist");
   testDB.add_group("Physician");
   testDB.add_group("Nurse");
   testDB.add_group("Patient");
   testDB.add_group("Administrator");
   testDB.add_group("TechSupport");



   cout << "Hello world!" << endl;
   return 0;
}


















