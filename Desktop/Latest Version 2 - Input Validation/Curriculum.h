#pragma once
#include <vector>
#include <string>

using namespace std;

struct Course {
    string courseName;
    string courseCode;
    int credits;
    string major;
    //this is the semester in this specific track
    int semester2Year; // 0 means not in this track
    int semester3Year; // 0 means not in this track
    int id;
    vector<string> prerequisites;
};

class Curriculum{
    public:
        vector<Course> gisCourses;
        int totalRequiredCredits;
        Curriculum();
};
