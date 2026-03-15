#include <vector>
#include <iostream>
#include "Student.h"
#include "Advising.h"
#include "Career.h"
#include "TransferOption.h"
#include "Internship.h"
#include "Research.h"
#include <fstream>

using namespace std;

int main()
{
    // Create objects for different advising modules
    Advising advisor;       // Handles curriculum advising and course planning
    Career career;          // Stores GIS career pathways and alumni information
    TransferOption options; // Stores transfer schools and their programs
    Internship internship;  // Displays internship opportunities
    Research research;      // Displays research opportunities

    char repeat; // variable used to repeat the program

    // ------------------------------------------------------
    // 1. Welcome Message
    // ------------------------------------------------------
    cout << "======================================================\n";
    cout << "Welcome to the BMCC Advising Bot!\n";
    cout << "Geographic Information Science\n";
    cout << "======================================================\n";

    // Main program loop (allows user to continue using the system)
    do
    {
        // ------------------------------------------------------
        // 2. Main Menu (Hub of the advising system)
        // ------------------------------------------------------
        int command;

        cout << "\n[1] Curriculum Advising\n"
             << "[2] Career Pathway\n"
             << "[3] Transfer Options\n"
             << "[4] Internship/Research Opportunity\n"
             << "[0] Exit program\n\n"
             << "Enter your choice: ";

        cin >> command;

        // Validate input using the advising class
        advisor.validateInput(command, 0, 4);

        // 3. Process user selection
        switch (command)
        {

        // ---------------- Curriculum Advising ----------------
        case 1:
            // Start conversation-based curriculum advising
            advisor.conversationController();
            break;

        // ---------------- Career Pathway ----------------
        case 2:
        {
            int option;

            cout << "\n[1] Show GIS Career Paths\n"
                 << "[2] Show BMCC GIS Alumni\n";

            cin >> option;

            // Validate input
            advisor.validateInput(option, 1, 2);

            // Display career information
            if (option == 1)
            {
                career.showCareer();
            }
            else if (option == 2)
            {
                career.showAlumni();
            }

            break;
        }

        // ---------------- Transfer Options ----------------
        case 3:
        {
            // Display transfer schools and programs
            options.displayTransferOption();
            break;
        }

        // ---------------- Internship / Research ----------------
        case 4:
        {
            int select;

            cout << "\nOpportunities\n";
            cout << "1. Internship Opportunity\n";
            cout << "2. Research Opportunity\n";

            cin >> select;

            advisor.validateInput(select, 1, 2);

            // Display internship or research opportunities
            if (select == 1)
            {
                internship.showInternshipInfo();
            }
            else if (select == 2)
            {
                research.showResearchInfo();
            }

            break;
        }

        // ---------------- Exit Program ----------------
        case 0:
            cout << "Exiting program...\n";
            return 0;

        default:
            break;
        }

        // Ask user if they want to continue
        cout << "\nTo see other options press 'y'. To exit press 'n': ";
        cin >> repeat;

    } while (repeat == 'y' || repeat == 'Y');

    return 0;
}