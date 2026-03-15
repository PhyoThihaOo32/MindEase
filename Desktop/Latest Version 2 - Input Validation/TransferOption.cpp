#include "TransferOption.h"
#include <iostream>

using namespace std;

TransferOption::TransferOption()
{
    // -------- Hunter College --------
    Transfer hunter;

    hunter.college = "Hunter College";

    hunter.degree = {
        "BA Geography",
        "BA Environmental Studies",
        "BA/MA Environmental Studies & Earth Science",
        "UGRAD Spatial Data Science Certificate",
        "Post Baccalaureate Advanced Certificate in GIS",
        "MS GeoInformatics",
        "MA Geography",
        "MA TEP Earth Science",
        "MA TEP Social Studies",
        "PhD Earth & Environmental Sciences"};

    hunter.activities = {
        "Field Trips & Guided Tours",
        "Gamma Theta Upsilon",
        "Geography Seminar Series",
        "Graduate Geography Association",
        "Greenbelt Society",
        "Hunter GIS Society",
        "Hunter SEEDs",
        "Hunter Sustainability Project",
        "Undergraduate Geo Club"};

    transferOption.push_back(hunter);

    // -------- Lehman College --------
    Transfer lehman;

    lehman.college = "Lehman College";

    lehman.degree = {
        "MS Geographic Information Science",
        "Graduate Certificate in GIS",
        "Undergraduate GIS Certificate",
        "Accelerated Bachelor's to Master's GIS Program"};

    lehman.activities = {
        "GIS Internships",
        "NASA Research Opportunities",
        "NOAA Fellowship Program",
        "Environmental & Climate Research",
        "GIS Labs and Mapping Projects",
        "Geospatial Data Analysis Workshops"};

    transferOption.push_back(lehman);
}

void TransferOption::setTransfer()
{
    cout << "Enter College Name: ";
    getline(cin, transfer.college);

    int n;

    cout << "How many degree programs? ";
    cin >> n;
    cin.ignore();

    transfer.degree.clear();

    for (int i = 0; i < n; i++)
    {
        string d;
        cout << "Degree " << i + 1 << ": ";
        getline(cin, d);
        transfer.degree.push_back(d);
    }

    cout << "How many activities? ";
    cin >> n;
    cin.ignore();

    transfer.activities.clear();

    for (int i = 0; i < n; i++)
    {
        string a;
        cout << "Activity " << i + 1 << ": ";
        getline(cin, a);
        transfer.activities.push_back(a);
    }
}

void TransferOption::addTransferOption()
{
    transferOption.push_back(transfer);
}

void TransferOption::displayTransferOption() const
{
    for (const Transfer &t : transferOption)
    {
        cout << "\n================================\n";
        cout << "Transfer College: " << t.college << endl;
        cout << "================================\n";

        cout << "\nAvailable Degrees:\n";
        for (const string &d : t.degree)
        {
            cout << "- " << d << endl;
        }

        cout << "\nStudent Activities:\n";
        for (const string &a : t.activities)
        {
            cout << "- " << a << endl;
        }

        cout << endl;
    }
}