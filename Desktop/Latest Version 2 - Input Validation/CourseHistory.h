#pragma once
#include "Curriculum.h"

struct CourseRecord {
    string name;
    string couseCode;
    int semester;
};

class CourseHistory {
    private:
        string filename;
        vector<string> completedCourses;
    public:
        CourseHistory();
        bool fileExists();
        void loadCourses();
        void promptEnterCourses(Curriculum& curriculum);
        void enterCourses(Curriculum& curriculum);
        bool validateCourse(string course);
        bool validateInput(int& input, int min, int max);
        void saveCourses();
        bool hasCompleted(string course);
        vector<string> getCourses();
};
