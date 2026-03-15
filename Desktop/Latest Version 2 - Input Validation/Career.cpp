// implementation file for Career class
#include "Career.h"
#include <iostream>

using namespace std;

Career::Career()
{
    // initialize career paths
    career = {
        "Health and Human Services",
        "Manufacturing",
        "Sustainability",
        "Transportation",
        "Government",
        "Real Estate",
        "Emergency and Public Safety",
        "Natural Resource Management",
        "Business Intelligence Using Location Analytics",
        "Crime Mapping and Hot Spot Analysis"};

    // alumni data
    alumni.push_back({"Bruno Luna",
                      "A.S. Geographic Information Science (GIS), BMCC (2019)",
                      "Unmanned Aerial Systems (UAS) Program Coordinator for Flagler County, Florida. "
                      "Supports drone missions and collaborates with GIS teams for mapping.",
                      "https://www.linkedin.com/in/bruno-luna43/",
                      "Explore industries you enjoy and discover how GIS can be applied in those fields."});

    alumni.push_back({"Naisiaae Kenana",
                      "A.S. Geographic Information Science (GIS), BMCC (2017)\n"
                      "B.A. Geography and Geographic Information Science, Hunter College (2019)",
                      "GIS Technician for the Government of Kenya, Nairobi County.\n"
                      "Works on surveying policy review, cadastral and boundary surveys, geospatial "
                      "technology research, map production, and technical reporting.",
                      "https://www.linkedin.com/in/naisiaae-k-b19618116/",
                      "GIS is used in many fields including environmental science, urban planning, "
                      "healthcare, and business analytics. Gain experience through projects, "
                      "internships, and professional networking."});
    alumni.push_back({"Juan F. Martinez",
                      "A.S. Multimedia Programming and Design, BMCC (2017); "
                      "B.A. Sustainable Development, Columbia University (2020); "
                      "MPA Urban Studies, Columbia University (2025)",
                      "Senior Staff Research Assistant at CIESIN, Columbia Climate School, Columbia University. "
                      "Works with GIS products, spatial data analysis, and research support for scientists and research teams.",
                      "https://www.linkedin.com/in/juan-f-martinez-aba3854a/",
                      "Develop strong data analysis and GIS skills. Sustainability and spatial data science are growing fields."});
    alumni.push_back({"Jocelyn Rajaballey",
                      "A.S. Public Health, BMCC (2021); "
                      "B.S. Urban Health Geography, Lehman College (2021); "
                      "GIS Certificate, Lehman College (2022)",
                      "City Service Corps member at NYC Emergency Management. "
                      "Works on community engagement data and GIS mapping for research and public safety.",
                      "https://www.linkedin.com/in/jocelyn-rajaballey/",
                      "GIS can be applied to public health, urban planning, and community research."});
    alumni.push_back({"Jada Grandchamps",
                      "A.S. Environmental Science, BMCC (2017); "
                      "B.A. Environmental Earth Science, City College (2019); "
                      "M.S. Geography, Hunter College (2021)",
                      "Field Hydrogeologist at PW Grosser working on environmental and geospatial studies.",
                      "https://www.linkedin.com/in/jada-grandchamps-567a0a141/",
                      "GIS plays an important role in environmental science and ecological research."});
    alumni.push_back({"Lisa S. Palmer",
                      "A.S. Geographic Information Science, BMCC; "
                      "B.A. Geography and GIS, Hunter College (2017)",
                      "Career Advisor in the BMCC Computer Information Systems Department "
                      "helping students find internships, apprenticeships, and careers.",
                      "https://www.linkedin.com/in/lspalmer",
                      "The GIS workforce is small, so start networking early and attend GIS events."});
    alumni.push_back({"Pizeme Tchara",
                      "A.S. Multimedia Programming/Computer Programming, BMCC (2008); "
                      "B.A. Communications and Media Studies/Computer Programming, NYU (2012); "
                      "M.S. Geographic Information Science and Cartography, Lehman College (2019)",
                      "Emergency Management Specialist and Geospatial Data Analyst at FEMA.",
                      "https://www.linkedin.com/in/pizeme/",
                      "GIS combines well with programming, statistics, and data analysis."});
    alumni.push_back({"Alexandrea Fortin",
                      "A.A. Liberal Arts, BMCC (1998); "
                      "B.A. English/Geology, Queens College (2010); "
                      "M.S. Geology and GIS, University of Rhode Island (2013)",
                      "Works in environmental science performing surveys, sampling, and environmental reporting.",
                      "https://www.linkedin.com/in/alexandrea-fortin-b0449720/",
                      "GIS is widely used in environmental science, government, and mapping industries."});
    alumni.push_back({"Juanita Corea",
                      "A.A. Liberal Arts, BMCC; "
                      "BSc Oceanography, University of Washington; "
                      "MSc Geographic Information Science, University of Denver",
                      "PhD student in Geography and Geoinformatics with over 20 years of professional "
                      "experience in geospatial analysis and cartography.",
                      "https://www.linkedin.com/in/juanita-corea-map-lady/",
                      "Combine GIS with other fields such as urban planning, conservation, disaster "
                      "management, or database systems."});
}

// add new career pathway
void Career::setCareer(string c)
{
    career.push_back(c);
}

// display career paths
void Career::showCareer() const
{
    cout << "\n========================\n";
    cout << "     GIS CAREER PATHS\n";
    cout << "========================\n\n";

    cout << "GIS is used in many industries including law enforcement, " << endl
         << "urban planning, environmental science, healthcare, " << endl
         << "transportation, and business analytics.\n\n";

    for (const string &c : career)
    {
        cout << "- " << c << endl;
    }

    cout << endl;
}

// add alumni interactively
void Career::setAlumni()
{
    Alumni alumnus;

    cout << "Name: ";
    getline(cin, alumnus.name);

    cout << "Academic Background: ";
    getline(cin, alumnus.background);

    cout << "Update: ";
    getline(cin, alumnus.update);

    cout << "LinkedIn: ";
    getline(cin, alumnus.linkedin);

    cout << "Advice: ";
    getline(cin, alumnus.advice);

    alumni.push_back(alumnus);
}

// display alumni
void Career::showAlumni() const
{
    string heading =
        "Geographic Information Science (GIS) is a fast-growing field used in many areas "
        "such as urban planning, environmental science, disaster management, public health, "
        "and government services. Below are some BMCC alumni working in GIS-related careers.\n";

    cout << "\n==============================\n";
    cout << "       BMCC GIS ALUMNI\n";
    cout << "==============================\n\n";

    cout << heading << endl;

    for (const Alumni &a : alumni)
    {
        cout << "---------------------------------\n";
        cout << "Name: " << a.name << endl;
        cout << "Academic Background:\n"
             << a.background << endl;
        cout << "Current Role:\n"
             << a.update << endl;
        cout << "LinkedIn: " << a.linkedin << endl;
        cout << "Advice: " << a.advice << endl;
        cout << endl;
    }
}