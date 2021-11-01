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
   std::filesystem::path newerPath("newDB.txt");
   PWDB::PermissionsDB testDB(newerPath);
   std::cout << "Done initializing testDB" << std::endl;

   testDB.add_group("Radiologist");
   testDB.add_group("Physician");
   testDB.add_group("Nurse");
   testDB.add_group("Patient");
   testDB.add_group("Administrator");
   testDB.add_group("TechSupport");

   testDB.add_user("Howard Linkler", "hlinkler", 1, "Trid142?");
   testDB.add_user("Veronica Perez", "vperez", 1, "Trid142?");
   testDB.add_user("Winston Callahan", "wcallahan", 2, "Trid142?");
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

   // Patient gets their own profile, history, I'll assume physician is attached to profile.
   testDB.add_activity("Patient_Profile", PWDB::READ); // Means that the patient gets read automatically
   testDB.add_activity("Patient_History", PWDB::READ);
   testDB.add_activity("Patient_Images");

   testDB.add_activity("Access_System"); // Dummy activity to restrict Administrators from doing it...

   testDB.add_activity("Diagnose_Patient");
   testDB.add_activity("Prescribe");
   testDB.add_activity("Diagnose_Equipment");

   // Everyone except IT.
   for (size_t i = 1; i < 6; i++){
      testDB.add_permission_to_activity("Patient_Profile", i, PWDB::READ);
   }
   testDB.add_permission_to_activity("Patient_Profile", 5, PWDB::WRITE); // Admins can write.
   testDB.toggle_group_restricted_hours(5); // Admins can't work after hours, lucky




   testDB.print_internal_variables();
   //testDB.read_DB();
   //testDB.print_internal_variables();

   return 0;
}


















