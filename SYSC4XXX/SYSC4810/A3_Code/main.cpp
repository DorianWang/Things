#include <iostream>

#include "PWDB.h"
#include "PWChecker.h"

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <filesystem> // For std::filesystem

using std::cout;
using std::endl;
using std::cin;

// This could also use read_DB() on a suitable database file.
// The numbers are hardcoded for testing, the system would obviously need changes.
void setup_DB(PWDB::PermissionsDB& testDB)
{
   testDB.add_group("Radiologist");    // 1
   testDB.add_group("Physician");      // 2
   testDB.add_group("Nurse");          // 3
   testDB.add_group("Patient");        // 4
   testDB.add_group("Administrator");  // 5
   testDB.add_group("TechSupport");    // 6

   testDB.add_user("Howard Linkler", "hlinkler", 1, "Trid142?");
   testDB.add_user("Veronica Perez", "vperez", 1, "Quadd142?");
   testDB.add_user("Winston Callahan", "wcallahan", 2, "Ahufe1@!");
   testDB.add_user("Leslie Stewart", "lstewart", 2, "Trid142?");
   testDB.add_user("Kelsey Chang", "kchang", 3, "Trid142?");
   testDB.add_user("James Preston", "jpreston", 3, "Trid142?");
   testDB.add_user("Rabul Garza", "rgarza", 4, "Trid142?");
   testDB.add_user("Arjun Singh", "asingh", 4, "Trid142?");
   testDB.add_user("Heather Anderson", "handerson", 5, "Trid142?");
   testDB.add_user("Keikilana Kapahu", "kkapahu", 5, "Trid142?");
   testDB.add_user("Harold Zakara", "hzakara", 6, "Trid142?");
   testDB.add_user("Mailina Romanoca", "mromanoca", 6, "Trid142?");

   testDB.add_target("RG_Profile", testDB.get_UID_from_username("rgarza"));
   testDB.add_target("AS_Profile", testDB.get_UID_from_username("asingh"));
   testDB.add_target("RG_History", testDB.get_UID_from_username("rgarza"));
   testDB.add_target("AS_History", testDB.get_UID_from_username("asingh"));
   testDB.add_target("RG_Images", testDB.get_UID_from_username("rgarza"));
   testDB.add_target("AS_Images", testDB.get_UID_from_username("asingh"));
   testDB.add_target("DIAG", 0);

   // Patient gets their own profile, history, I'll assume physician info is attached to profile.
   testDB.add_activity("Patient_Profile", PWDB::READ); // Means that the patient gets read automatically
   testDB.add_activity("Patient_History", PWDB::READ);
   testDB.add_activity("Patient_Images");

   testDB.add_activity("Access_System"); // Dummy activity to restrict Administrators from doing it...

   testDB.add_activity("Diagnose_Patient");
   testDB.add_activity("Prescribe");
   testDB.add_activity("Diagnose_Equipment");

   for (size_t i = 1; i < 7; i++){ // Everyone is allowed to execute "access system"
      testDB.add_permission_to_activity("Access_System", i, PWDB::EXECUTE);
   }

   // For physicians, radiologists and nurses
   for (size_t i = 1; i < 4; i++){
      testDB.add_permission_to_activity("Patient_Profile", i, PWDB::READ);
      testDB.add_permission_to_activity("Patient_History", i, PWDB::READ);
      testDB.add_permission_to_activity("Patient_Images", i, PWDB::READ);
   }
   testDB.add_permission_to_activity("Patient_Profile", 5, PWDB::WRITE + PWDB::READ); // Admins can read and write profiles.
   testDB.toggle_group_restricted_hours(5); // Admins can't work after hours, lucky

   // Radiologists and Physicians can diagnose patients
   testDB.add_permission_to_activity("Diagnose_Patient", 1, PWDB::WRITE + PWDB::READ);
   testDB.add_permission_to_activity("Diagnose_Patient", 2, PWDB::WRITE + PWDB::READ);

   testDB.add_permission_to_activity("Prescribe", 2, PWDB::WRITE);

   testDB.add_permission_to_activity("Diagnose_Equipment", 6, PWDB::EXECUTE);
}



int main()
{
   std::filesystem::path newerPath("newDB.txt");
   PWDB::PermissionsDB testDB(newerPath);
   setup_DB(testDB);
   std::string username; std::string password; uint_fast64_t UID; std::string lineIn;

   cout << "Medview Imaging\n";
   cout << "Medical Information Management System\n";
   cout << "---------------------------------------\n";
   while (true)
   {
      cout << "Enter Username:";
      cin >> username;
      cout << "Enter Password:";
      cin >> password;

      UID = testDB.try_login(username, password);
      password.erase();
      if (UID){
         cout << "ACCESS GRANTED" << endl;
         break;
      }
   }
   cout << "Here are the available commands:" << endl;
   std::vector <std::string> res = testDB.getActionsAndFiles();
   for (const std::string& iter : res){
      cout << iter << "\n";
   }
   cout << std::endl;

   // Ran out of time to either validate files or prevent showing of files the user doesn't have any access to.

   while (true){

      cout << "Please enter your command:";
      cin >> lineIn;
      if (lineIn == "")
      testDB.check_action_permissions()
   }


   //testDB.print_internal_variables();

   return 0;
}


















