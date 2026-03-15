// specification file for Internship
#ifndef INTERNSHIP_H
#define INTERNSHIP_H

#include <vector>
#include <string>
using namespace std;

class Internship
{
private:
    vector<string> organizations;
    vector<string> courseInfo;
    vector<string> requirements;
    vector<string> resources;

public:
    // default constructor will initialize internship data
    Internship();

    // functions to add more information if needed
    void setOrganization(string);
    void setRequirement(string);
    void setResource(string);

    // display functions
    void showOrganizations() const;
    void showCourseInfo() const;
    void showRequirements() const;
    void showResources() const;

    // show all internship information together
    void showInternshipInfo() const;
};

#endif