#pragma once
#include <string>
#include <vector>
#include "CourseHistory.h"

using namespace std;
// 1. The Student Class
// Purpose: This class holds all the information about the user currently talking to the bot
class Student
{
private:
    CourseHistory history;
    string currentMajor;
    int chosenTrack;
    int currentSemester;
    int completedCredits;

public:
    // vector<int> coursesTaken; //this will be a feature for later
    Student();

    // Setters
    void setTrack(int track);
    void setSemester(int semester);
    void setMajor(int major);
    void setCredits();

    // Getters
    int getTrack() const;
    int getSemester() const;
    string getMajor() const;
    int getCredits() const;
    int getRemainingCredits();
    CourseHistory &getCourseHistory(); // Returning by reference otherwise we get a copy.
};
