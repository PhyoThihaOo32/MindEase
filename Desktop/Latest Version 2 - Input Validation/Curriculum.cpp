#include "Curriculum.h"

Curriculum::Curriculum() {
    gisCourses = {
        // Both Tracks
        // Seperated the course code from the title to make it easier to find.
     
        // Foundational & English
        {"English Composition", "ENG 101", 3, "GIS", 1, 1,1},
        {"Introduction to Literature", "ENG 201", 3, "GIS", 2, 2,2},
        {"Fundamentals of Public Speaking", "SPE 100", 3, "GIS", 3, 3,3},

        // Math & Computer Science
        {"Precalculus", "MAT 206", 4, "GIS", 1, 1,4},
        {"Statistics", "MAT 209", 4, "GIS", 2, 4,5},
        {"Principles in IT and Computation", "CSC 101", 3, "GIS", 1, 1,6},
        {"Computer Programming I", "CSC 110", 4, "GIS", 2, 2,7},
        {"Database Systems I", "CIS 395", 3, "GIS", 3, 3,8},

        // Geography, Geology & Core GIS
        {"Introduction to Human Geography", "GEO 100", 3, "GIS", 1, 2,9,},
        {"Geology I", "GLY 210", 4, "GIS", 1, 2},
        {"Introduction to Geographic Methods", "GIS 201", 4, "GIS", 2, 3,10},
        {"Environmental Conservation", "GEO 226", 3, "GIS", 3, 3,11},
        {"Introduction to Geographic Info Science", "GIS 261", 3, "GIS", 3, 4,12},
        {"Population Geography", "GEO 241", 3, "GIS", 4, 4,13},
        {"Advanced Geographic Info Science", "GIS 361", 3, "GIS", 4, 5,14},

        // Health & Electives
        {"Health Problems in the Urban Community", "AFL 161", 3, "GIS", 3, 5,15},
        {"Individual and Society", "XXX xxx", 3, "GIS", 4, 4,16},
        {"U.S. Experience in Its Diversity", "XXX xxx", 3, "GIS", 4, 5,17},
        {"Creative Expression", "XXX xxx", 3, "GIS", 4, 5,18}
    };
}



