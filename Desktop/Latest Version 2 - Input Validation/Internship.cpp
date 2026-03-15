// implementation file for Internship class
#include "Internship.h"
#include <iostream>

using namespace std;

Internship::Internship()
{
    // organizations where GIS students have interned
    organizations = {
        "Environmental Protection Agency (EPA)",
        "NYC Economic Development Corporation",
        "NYC Department of Transportation",
        "NYC Department of Environmental Protection (DEP)",
        "U.S. Coast Guard",
        "National Intelligence University",
        "Environmental Investigation Agency (EIA)"};

    // internship course information
    courseInfo = {
        "Course Title: Geographic Information Science Internship",
        "Course Number: GIS 325",
        "Credits: 2 credits",
        "Students typically work around 15 hours per week during the semester.",
        "The internship allows students to apply GIS knowledge in a professional environment."};

    // requirements
    requirements = {
        "Students work approximately 15 hours per week.",
        "A term project must be completed during the internship.",
        "Students are evaluated by their internship supervisor.",
        "Students must arrange their schedules to be available during work hours."};

    // resources available at BMCC
    resources = {
        "BMCC Center for Career Development helps with resumes and interview preparation.",
        "Students can use Handshake to search for internships and jobs.",
        "Career advisors and faculty can help students find internship opportunities.",
        "BMCC career events and internship expos help students connect with employers."};
}

// add organization
void Internship::setOrganization(string o)
{
    organizations.push_back(o);
}

// add requirement
void Internship::setRequirement(string r)
{
    requirements.push_back(r);
}

// add resource
void Internship::setResource(string r)
{
    resources.push_back(r);
}

// display organizations
void Internship::showOrganizations() const
{
    cout << "\n========================\n";
    cout << " GIS INTERNSHIP ORGANIZATIONS\n";
    cout << "========================\n\n";

    for (const string &o : organizations)
    {
        cout << "- " << o << endl;
    }

    cout << endl;
}

// display course information
void Internship::showCourseInfo() const
{
    cout << "\n========================\n";
    cout << " GIS INTERNSHIP COURSE\n";
    cout << "========================\n\n";

    for (const string &c : courseInfo)
    {
        cout << "- " << c << endl;
    }

    cout << endl;
}

// display requirements
void Internship::showRequirements() const
{
    cout << "\n========================\n";
    cout << " INTERNSHIP REQUIREMENTS\n";
    cout << "========================\n\n";

    for (const string &r : requirements)
    {
        cout << "- " << r << endl;
    }

    cout << endl;
}

// display resources
void Internship::showResources() const
{
    cout << "\n========================\n";
    cout << " INTERNSHIP RESOURCES AT BMCC\n";
    cout << "========================\n\n";

    for (const string &r : resources)
    {
        cout << "- " << r << endl;
    }

    cout << endl;
}

// show everything together
void Internship::showInternshipInfo() const
{
    showOrganizations();
    showCourseInfo();
    showRequirements();
    showResources();
}