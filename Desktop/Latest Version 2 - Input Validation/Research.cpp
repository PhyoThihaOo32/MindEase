// implementation file for Research class
#include "Research.h"
#include <iostream>

using namespace std;

Research::Research()
{
    // areas where GIS research is commonly applied
    researchAreas = {
        "Urban planning and city development",
        "Environmental science and sustainability",
        "Transportation and infrastructure planning",
        "Public health and spatial data analysis",
        "Disaster management and emergency response",
        "Natural resource management"};

    // research activities students may participate in
    researchActivities = {
        "Spatial data analysis using GIS software",
        "Mapping and visualization of geographic data",
        "Working with satellite imagery and spatial datasets",
        "Collecting and analyzing geographic information",
        "Supporting faculty research projects involving GIS"};

    // opportunities available for students
    opportunities = {
        "Students may assist faculty members with GIS-related research projects.",
        "Students can participate in research activities connected to GIS coursework.",
        "GIS projects completed in classes can contribute to research experience.",
        "Students may collaborate with organizations using GIS for research purposes."};

    // BMCC resources that support research
    resources = {
        "BMCC faculty members guide students in GIS projects and research activities.",
        "GIS coursework provides hands-on experience with spatial data and mapping.",
        "Students can build research skills through class projects and data analysis.",
        "GIS software tools used in classes help students perform geographic research."};
}

// add research area
void Research::setResearchArea(string a)
{
    researchAreas.push_back(a);
}

// add opportunity
void Research::setOpportunity(string o)
{
    opportunities.push_back(o);
}

// add resource
void Research::setResource(string r)
{
    resources.push_back(r);
}

// display research areas
void Research::showResearchAreas() const
{
    cout << "\n========================\n";
    cout << " GIS RESEARCH AREAS\n";
    cout << "========================\n\n";

    for (const string &a : researchAreas)
    {
        cout << "- " << a << endl;
    }

    cout << endl;
}

// display research activities
void Research::showResearchActivities() const
{
    cout << "\n========================\n";
    cout << " GIS RESEARCH ACTIVITIES\n";
    cout << "========================\n\n";

    for (const string &a : researchActivities)
    {
        cout << "- " << a << endl;
    }

    cout << endl;
}

// display research opportunities
void Research::showOpportunities() const
{
    cout << "\n========================\n";
    cout << " GIS RESEARCH OPPORTUNITIES\n";
    cout << "========================\n\n";

    for (const string &o : opportunities)
    {
        cout << "- " << o << endl;
    }

    cout << endl;
}

// display research resources
void Research::showResources() const
{
    cout << "\n========================\n";
    cout << " RESEARCH RESOURCES AT BMCC\n";
    cout << "========================\n\n";

    for (const string &r : resources)
    {
        cout << "- " << r << endl;
    }

    cout << endl;
}

// show all research information
void Research::showResearchInfo() const
{
    showResearchAreas();
    showResearchActivities();
    showOpportunities();
    showResources();
}