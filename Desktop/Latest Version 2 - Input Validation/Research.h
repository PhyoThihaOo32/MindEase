// specification file for Research
#ifndef RESEARCH_H
#define RESEARCH_H

#include <vector>
#include <string>
using namespace std;

class Research
{
private:
    vector<string> researchAreas;
    vector<string> researchActivities;
    vector<string> opportunities;
    vector<string> resources;

public:
    // default constructor will initialize research information
    Research();

    // functions to add more information
    void setResearchArea(string);
    void setOpportunity(string);
    void setResource(string);

    // display functions
    void showResearchAreas() const;
    void showResearchActivities() const;
    void showOpportunities() const;
    void showResources() const;

    // show all research information
    void showResearchInfo() const;
};

#endif
