// specification file for careerPath

#ifndef CAREER_H
#define CAREER_H

#include <vector>
#include <string>

using namespace std;

struct Alumni
{
    string name;
    string background;
    string update;
    string linkedin;
    string advice;
};

class Career
{
private:
    vector<string> career;
    vector<Alumni> alumni;

public:
    // default construtor will set the list of possible careers and alumni info- (which are currently avaliable on bmcc gis webpage)
    Career();

    // to program should be expandable

    // to set new career list
    void setCareer(string);

    // this will be call in the main
    // this fun will show the avaliable career path to the user
    void showCareer() const;

    // can add more alumni info
    void setAlumni();

    // this function will show the info current alumni working in GIS industry
    void showAlumni() const;
};

#endif