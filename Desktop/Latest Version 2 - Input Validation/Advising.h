#pragma once
#include "Student.h"
#include "Curriculum.h"
#include "CourseHistory.h"

class Advising
{
private:
    Student student;
    Curriculum curriculum;

public:
    bool validateInput(int &, int, int);
    void conversationController();
    void askForMajor();
    void askForTrack();
    void askForCourseHistory();
    void askForSemester();
    // void adviseOnClasses();
    // void calculateCredits();
    // void showcareerPathway();
    // void showinterResearchopp();
    void loadSchedule(int);
    int creditsRemaining();
    Advising();
};
