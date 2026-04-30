#include "screens/recommendations.h"
#include "core/screen.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QPushButton>
#include <QStackedWidget>
#include <QSizePolicy>
#include <QLineEdit>
#include <QGridLayout>
#include <QGraphicsDropShadowEffect>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QUrl>
#include <QColor>
#include <functional>

// ─────────────────────────────────────────────────────────────────────────────
// Data
// ─────────────────────────────────────────────────────────────────────────────

struct ResourceInfo {
    QString icon;
    QString color;
    QString title;
    QString description;
    QString details;
    QString url;
    QStringList tags;
    QString url2 = {};
    QString url2Label = {};
    QStringList extraLinks = {}; // label/url pairs for cards with many actions
};

struct TopicInfo {
    QString icon;
    QString accent;     // background color for icon badge
    QString title;
    QString subtitle;   // shown on the landing card
};

struct ResourceSubpageInfo {
    QString key;
    int parentTopicIndex;
    QString icon;
    QString color;
    QString title;
    QString subtitle;
    QString details;
    QString url;
    QString urlLabel;
    QStringList tags;
    QStringList childLinks = {}; // label/url pairs for hub pages
};

static const QList<TopicInfo> TOPICS = {
    { "📚", "#dcfff1", "Exam & Study Stress",        "Tutoring, writing help, academic coaching" },
    { "💰", "#f4ffb8", "Finance",                    "Financial aid, emergency funds, free food"  },
    { "🌐", "#dff6ff", "Immigration",                "Legal help, DACA, undocumented support"     },
    { "💬", "#e9fff3", "Relationships & Family",     "Personal counseling, peer support"          },
    { "🏥", "#e7fbff", "Health & Wellness",          "Medical care, mental health, insurance"     },
};

// ─────────────────────────────────────────────────────────────────────────────
// Resource cards (detail pages — to be filled out later)
// ─────────────────────────────────────────────────────────────────────────────

static const QList<QList<ResourceInfo>> RESOURCES = {

    // 0 — Exam & Study
    {
        { "📚", "#edf7ee", "Learning Resource Center — Tutoring",
          "Free peer tutoring for Math, English, Science, Business, and more. Walk-in or book online. No appointment needed.",
          "📍  199 Chambers St., Room S-510\n"
          "    245 Greenwich St., Fiterman Hall Room F-511\n"
          "🕐  In-person (Chambers): Mon–Thu 10am–6pm  |  Fri–Sat 10am–5pm\n"
          "    In-person (Fiterman): Mon–Wed 10am–3pm  |  Thu 10am–2pm\n"
          "    Remote: Mon–Thu 9am–9pm\n"
          "📞  (212) 220-1383  |  lrctutoring@bmcc.cuny.edu",
          "https://www.bmcc.cuny.edu/students/lrc/in-person-tutoring/",
          {"Free", "Walk-in", "In-person & Remote"} },

        { "🤝", "#eef8f1", "Peer Assisted Learning (PAL)",
          "Small-group study sessions (max 5 students) led by a trained PAL leader for supported courses. Great for processing difficult course material together.",
          "📍  199 Chambers St., Room S-510\n"
          "📞  (212) 220-1383  |  lrctutoring@bmcc.cuny.edu",
          "https://www.bmcc.cuny.edu/students/lrc/in-person-tutoring/peer-assisted-learning/",
          {"Small Group", "Free", "Collaborative"} },

        { "✏️", "#e2f4e8", "Writing Center",
          "One-on-one help with any writing assignment — essays, research papers, reports, and more. All levels welcome.",
          "📍  199 Chambers St., Room S-510\n"
          "🕐  Mon–Thu 10am–6pm  |  Sat 10am–3pm  |  Sun: online only\n"
          "📞  (212) 220-1384  |  writingcenter@bmcc.cuny.edu",
          "https://www.bmcc.cuny.edu/students/writing-center/",
          {"Writing", "Free", "In-person & Online"} },

        { "🎯", "#f1f8e8", "Academic Coaching",
          "Personalized one-on-one coaching for study strategies, time management, organization, and test preparation.",
          "📍  199 Chambers St., Room S-510\n"
          "🕐  Mon–Thu 10am–6pm  |  Fri 10am–5pm\n"
          "📧  AcademicCoaching@bmcc.cuny.edu",
          "https://www.bmcc.cuny.edu/students/lrc/academic-coaching/",
          {"Study Skills", "Test Prep", "Free"} },

        { "💻", "#e8f7ef", "Online Tutoring",
          "Join a live remote tutoring session from home. Covers the same subjects as in-person — no commute needed.",
          "📞  (212) 220-8359  |  onlinetutoring@bmcc.cuny.edu\n"
          "🕐  Mon–Thu 9am–9pm  (extended remote hours)",
          "https://www.bmcc.cuny.edu/students/lrc/online-tutoring/tutoring-schedule/",
          {"Remote", "Flexible Hours", "Free"},
          "https://bmcc.upswing.io/up/dashboard/home", "Upswing →" },

        { "🧰", "#eef8f1", "Free Software for Students — Digital Education Center",
          "Access free BMCC and CUNY digital learning tools for classes, online meetings, assignments, and skill building.",
          "CONTACT: Digital Education Center\n"
          "📧  digitaleducation@bmcc.cuny.edu\n"
          "📞  (212) 220-1243\n"
          "📍  199 Chambers St, Room S-510a, New York, NY 10007\n"
          "🕐  Monday – Friday: 9 a.m.–5 p.m.\n"
          "Includes Brightspace, Zoom, Microsoft Office 365, LinkedIn Learning, VoiceThread, and CUNY Technology Services.",
          "",
          {"Free Software", "Digital Tools", "Student Support"},
          "", "",
          {
              "View Software Tools →", "subpage:free-software"
          } },
    },

    // 1 — Finance
    {
        { "💰", "#e2f4e8", "Financial Aid Office",
          "Grants, loans, work-study, and scholarships. Speak with an advisor.",
          "📞 (212) 220-1430  |  finaid@bmcc.cuny.edu",
          "https://www.bmcc.cuny.edu/finaid/",
          {"Grants", "Scholarships"} },

        { "🆘", "#f1f8e8", "Student Emergency Fund",
          "Emergency grants for textbooks, rent, utilities, and urgent needs.",
          "📍 Room S-230  |  Mon–Fri 8am–6:30pm  |  Sat 10am–1pm\n📞 (212) 220-8195",
          "https://www.bmcc.cuny.edu/student-affairs/arc/student-emergency-fund/",
          {"Emergency", "Fast Support"} },

        { "🍎", "#edf7ee", "Panther Pantry — Free Food",
          "Free food for students facing food insecurity. Walk in, no questions asked.",
          "📍 Room S-230  |  Mon–Fri 8am–6:30pm  |  Sat 10am–1pm",
          "https://www.bmcc.cuny.edu/student-affairs/arc/panther-pantry/",
          {"Free Food", "Walk-in"} },

        { "🏛️", "#e2f4e8", "Advocacy & Resource Center",
          "SNAP assistance, tax prep, housing referrals, and financial counseling.",
          "📍 Room S-230  |  Mon–Fri 8am–6:30pm  |  Sat 10am–1pm",
          "https://www.bmcc.cuny.edu/student-affairs/arc/",
          {"SNAP", "Housing", "Tax Help"} },
    },

    // 2 — Immigration
    {
        { "🌐", "#edf7ee", "Immigrant Resource Center",
          "Confidential support for immigrant and undocumented students.",
          "📞 Albert Lee: (212) 776-6252  |  allee@bmcc.cuny.edu\n📍 Room S-230  |  Mon–Fri 8am–6:30pm",
          "https://www.bmcc.cuny.edu/student-affairs/center-for-belonging-and-inclusion/immigrant-resource-center/",
          {"Confidential", "All Statuses"} },

        { "⚖️", "#e2f4e8", "Free Legal Services — CUNY Citizenship Now!",
          "On-site attorneys monthly for DACA, TPS, family petitions, and naturalization.",
          "📍 Room S-230",
          "https://www.bmcc.cuny.edu/student-affairs/arc/legal-services/",
          {"DACA", "Legal Aid", "Free"} },

        { "🎓", "#f1f8e8", "Scholarships for Immigrant Students",
          "TheDream.US and other scholarships available regardless of status.",
          "",
          "https://www.bmcc.cuny.edu/immigration/scholarships-and-financial-aid/",
          {"Scholarships", "DREAMers"} },

        { "🔒", "#edf7ee", "Know Your Rights at BMCC",
          "CUNY will not share your records with immigration enforcement without a court order.",
          "",
          "qrc:/documents/2025-KYR-Final-01.13.202592.pdf",
          {"Privacy", "Your Rights"} },
    },

    // 3 — Relationships & Family
    {
        { "💬", "#edf7ee", "Personal Counseling — Counseling Center",
          "Free, confidential counseling for relationship, family, and personal issues.",
          "📍 Room S-343  |  Mon, Tue, Thu 9am–6pm  |  Wed, Fri 9am–5pm\n"
          "📞 (212) 220-8140  |  counselingcenter@bmcc.cuny.edu",
          "https://www.bmcc.cuny.edu/student-affairs/counseling/counseling-services/",
          {"Free", "Confidential", "Telehealth OK"} },

        { "🤝", "#e2f4e8", "Togetherall — 24/7 Peer Support",
          "Free anonymous online community for students to support each other.",
          "",
          "https://togetherall.com/en-us/",
          {"Anonymous", "24/7", "Online"} },

        { "📋", "#f1f8e8", "Book a Counseling Appointment",
          "Schedule in person, by phone, or on Zoom. Crisis support also available.",
          "📞 (212) 220-8140",
          "https://www.bmcc.cuny.edu/student-affairs/counseling/request-an-appointment/",
          {"Appointment", "Crisis"} },

        { "🏘️", "#edf7ee", "Community Referrals",
          "Referrals to outside help for housing, domestic support, and legal aid.",
          "",
          "https://www.bmcc.cuny.edu/student-affairs/counseling/counseling-resources/community-resources/",
          {"Housing", "Legal Aid"} },
    },

    // 4 — Health & Wellness
    {
        { "🏥", "#e2f4e8", "Health & Wellness Services",
          "First aid, medical evaluation, health counseling, and wellness workshops.",
          "📍  Room N-380, 199 Chambers St.\n"
          "🕐  Mon – Fri: 9 a.m. – 5 p.m.\n"
          "📞  Tel: (212) 220-8256  |  Fax: (212) 220-2367\n"
          "📧  healthservices@bmcc.cuny.edu",
          "https://www.bmcc.cuny.edu/student-affairs/health-services/",
          {"Medical", "On-campus", "Free"} },

        { "🧠", "#edf7ee", "Mental Health Counseling",
          "Short-term counseling, crisis support, and referrals to community providers.",
          "📍  Room S-343\n"
          "🕐  Mon, Tue, Thu 9am–6pm  |  Wed, Fri 9am–5pm\n"
          "📞  (212) 220-8140  |  counselingcenter@bmcc.cuny.edu",
          "https://www.bmcc.cuny.edu/student-affairs/counseling/",
          {"Mental Health", "Free", "Crisis"} },

        { "🏋️", "#eef7f1", "Campus Fitness",
          "Fitness Center, Swimming Pool, and Health & Wellness Services — accessible exercise options for every student.",
          "CONTACT: BMCC Athletics\n"
          "📞  (212) 220-8260",
          "https://www.bmcc.cuny.edu/students/fitness-center/",
          {"Fitness", "On-campus", "Wellness"},
          "https://bmccathletics.com/sports/2012/4/2/RecreationSchedule.aspx",
          "Schedule →" },

        { "🎖️", "#f3faef", "Veterans Resource Center — Stress & Crisis Support",
          "Dedicated support for veteran students dealing with stress, PTSD, and mental health challenges. Request a virtual or in-person appointment.",
          "📍  199 Chambers St., Room S-115M\n"
          "📞  (212) 220-8035  |  saruiz@bmcc.cuny.edu\n"
          "👤  Samantha Ruiz, LCSW — Manager for Veterans Affairs & Student Development",
          "https://www.bmcc.cuny.edu/student-affairs/veterans-resource-center/dealing-with-stress/",
          {"Veterans", "PTSD", "Free"} },

        { "☎️", "#e9f5ef", "Samaritans — 24-Hour Crisis Hotline",
          "Free and confidential support if you are feeling stressed, overwhelmed, or suicidal — or if you're worried about a friend or family member. Available around the clock.",
          "📞  (212) 673-3000  —  free, confidential, 24 hours a day",
          "",
          {"24/7", "Free", "Confidential"} },

        { "🪖", "#dff1e5", "National Center for PTSD",
          "Information and resources from the U.S. Department of Veterans' Affairs for veterans and their families dealing with PTSD.",
          "",
          "https://www.ptsd.va.gov/",
          {"Veterans", "PTSD", "U.S. VA"} },

        { "💪", "#eef7f1", "Combat Stress Recovery Program",
          "Support program from the Wounded Warrior Project specifically designed to help veterans recover from combat stress and related mental health challenges.",
          "",
          "https://www.woundedwarriorproject.org/programs/mental-wellness",
          {"Veterans", "Combat Stress", "Wounded Warrior"} },

        { "🩺", "#f1f8e8", "Health Insurance Enrollment",
          "Help enrolling in NY State health insurance — open to all students including international and undocumented.",
          "📍  Room S-230",
          "https://www.bmcc.cuny.edu/student-affairs/arc/health-insurance/",
          {"Insurance", "All Students"} },

    },
};

static const QList<ResourceSubpageInfo> RESOURCE_SUBPAGES = {
    { "free-software", 0, "🧰", "#eef8f1", "Free Software for Students",
      "BMCC Digital Education Center software support and student tool directory.",
      "Use this page first when you need the official BMCC list of free and supported student software.\n"
      "Contact Digital Education Center if you are blocked by login, access, setup, or class technology problems.\n"
      "Email: digitaleducation@bmcc.cuny.edu\nPhone: (212) 220-1243\nLocation: 199 Chambers St, Room S-510a\nHours: Monday-Friday 9 a.m.-5 p.m.",
      "https://www.bmcc.cuny.edu/academics/digitaleducation/student-support/software-for-students/",
      "Open Free Software Page →",
      {"Software", "DEC", "Student Support"},
      {
          "Brightspace", "https://brightspace.cuny.edu/d2l/home",
          "Zoom", "https://www.bmcc.cuny.edu/academics/digitaleducation/student-support/zoom-for-students/",
          "Office 365", "https://www.cuny.edu/about/administration/offices/cis/technology-services/microsoft-office-365-for-education/",
          "LinkedIn Learning", "https://www.linkedin.com/learning-login/",
          "VoiceThread", "https://voicethread.com/howto/",
          "CUNY Tech Services", "https://www.cuny.edu/about/administration/offices/cis/technology-services/#student"
      } },
};

static QLabel* makeInfoChip(const QString &text,
                            const QString &bg = "#f5f0e2",
                            const QString &fg = "#3b5645",
                            const QString &border = "#c9d7c2") {
    QLabel *chip = new QLabel(text);
    chip->setStyleSheet(
        QString("font-size:11px; font-weight:600; color:%1; background:%2; "
                "border:1px solid %3; border-radius:12px; padding:5px 10px;")
            .arg(fg, bg, border));
    return chip;
}

static void applySoftShadow(QWidget *widget,
                            int blur = 24,
                            int yOffset = 7,
                            const QColor &color = QColor(121, 154, 112, 20)) {
    auto *shadow = new QGraphicsDropShadowEffect(widget);
    shadow->setBlurRadius(blur);
    shadow->setOffset(0, yOffset);
    shadow->setColor(color);
    widget->setGraphicsEffect(shadow);
}

static bool openResourceTarget(const QString &target) {
    if (target.startsWith("qrc:/")) {
        const QString resourcePath = ":" + target.mid(4);
        QString cacheDirPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        if (cacheDirPath.isEmpty())
            cacheDirPath = QDir::tempPath() + "/MindEase";

        QDir cacheDir(cacheDirPath);
        if (!cacheDir.exists() && !cacheDir.mkpath("."))
            return false;

        const QString outputPath = cacheDir.filePath(QFileInfo(resourcePath).fileName());
        if (QFile::exists(outputPath))
            QFile::remove(outputPath);

        if (!QFile::copy(resourcePath, outputPath))
            return false;

        return QDesktopServices::openUrl(QUrl::fromLocalFile(outputPath));
    }

    const QFileInfo localFile(target);
    if (localFile.exists())
        return QDesktopServices::openUrl(QUrl::fromLocalFile(localFile.absoluteFilePath()));

    return QDesktopServices::openUrl(QUrl(target));
}

static QString normalizedText(const QString &text) {
    return text.simplified().toLower();
}

struct TopicSearchResult {
    int score = -1;
    QStringList matchedSubtopics;
};

static int scoreTextMatch(const QString &text,
                          const QString &query,
                          const QStringList &terms,
                          int exact,
                          int prefix,
                          int contains,
                          int allTermsBase) {
    const QString normalized = normalizedText(text);
    if (normalized.isEmpty()) return -1;

    int score = -1;
    if (normalized == query) score = qMax(score, exact);
    else if (normalized.startsWith(query)) score = qMax(score, prefix);
    else if (normalized.contains(query)) score = qMax(score, contains);

    int termHits = 0;
    for (const QString &term : terms) {
        if (normalized.contains(term))
            termHits++;
    }

    if (!terms.isEmpty() && termHits == terms.size())
        score = qMax(score, allTermsBase + termHits * 10);

    return score;
}

static TopicSearchResult searchTopic(int topicIndex, const QString &queryText) {
    const QString query = normalizedText(queryText);
    if (query.isEmpty()) return {};

    TopicSearchResult result;
    const TopicInfo &topic = TOPICS[topicIndex];
    const QStringList terms = query.split(' ', Qt::SkipEmptyParts);

    result.score = qMax(result.score,
                        scoreTextMatch(topic.title, query, terms, 420, 320, 240, 260));
    result.score = qMax(result.score,
                        scoreTextMatch(topic.subtitle, query, terms, 220, 170, 120, 190));

    for (const ResourceInfo &resource : RESOURCES[topicIndex]) {
        int resourceScore = -1;
        resourceScore = qMax(resourceScore,
                             scoreTextMatch(resource.title, query, terms, 260, 220, 190, 210));
        resourceScore = qMax(resourceScore,
                             scoreTextMatch(resource.description, query, terms, 170, 150, 130, 150));
        resourceScore = qMax(resourceScore,
                             scoreTextMatch(resource.details, query, terms, 110, 100, 90, 100));

        for (const QString &tag : resource.tags) {
            resourceScore = qMax(resourceScore,
                                 scoreTextMatch(tag, query, terms, 210, 180, 150, 170));
        }
        for (int i = 0; i + 1 < resource.extraLinks.size(); i += 2) {
            resourceScore = qMax(resourceScore,
                                 scoreTextMatch(resource.extraLinks[i], query, terms, 190, 160, 130, 150));
        }

        if (resourceScore >= 0) {
            result.score = qMax(result.score, resourceScore);
            result.matchedSubtopics.append(resource.title);
        }
    }

    for (const ResourceSubpageInfo &subpage : RESOURCE_SUBPAGES) {
        if (subpage.parentTopicIndex != topicIndex)
            continue;

        int subpageScore = -1;
        subpageScore = qMax(subpageScore,
                            scoreTextMatch(subpage.title, query, terms, 240, 200, 170, 190));
        subpageScore = qMax(subpageScore,
                            scoreTextMatch(subpage.subtitle, query, terms, 160, 140, 120, 140));
        subpageScore = qMax(subpageScore,
                            scoreTextMatch(subpage.details, query, terms, 110, 100, 90, 100));
        for (const QString &tag : subpage.tags) {
            subpageScore = qMax(subpageScore,
                                scoreTextMatch(tag, query, terms, 190, 160, 130, 150));
        }
        for (int i = 0; i + 1 < subpage.childLinks.size(); i += 2) {
            subpageScore = qMax(subpageScore,
                                scoreTextMatch(subpage.childLinks[i], query, terms, 190, 160, 130, 150));
        }

        if (subpageScore >= 0) {
            result.score = qMax(result.score, subpageScore);
            result.matchedSubtopics.append(subpage.title);
        }
    }

    result.matchedSubtopics.removeDuplicates();
    return result;
}

static QString summarizedSubtopics(const QStringList &subtopics) {
    if (subtopics.isEmpty()) return {};
    if (subtopics.size() == 1)
        return subtopics.first();
    if (subtopics.size() == 2)
        return subtopics.join(", ");
    return QString("%1, %2, +%3 more")
        .arg(subtopics[0], subtopics[1])
        .arg(subtopics.size() - 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// Builders
// ─────────────────────────────────────────────────────────────────────────────

static QWidget* makeResourceCard(const ResourceInfo &r,
                                 std::function<void(const QString&)> onInternalTarget = {}) {
    // Outer wrapper: border + left accent bar. Using border-left rather than a
    // child QFrame means the bar correctly clips to border-radius on all OSes.
    QFrame *wrapper = new QFrame();
    wrapper->setStyleSheet(
        QString("QFrame {"
                "  border: 1px solid #dfece1;"
                "  border-left: 4px solid %1;"
                "  border-radius: 16px;"
                "  background: qradialgradient(cx:0.18, cy:0.12, radius:1.15,"
                "                              fx:0.18, fy:0.12,"
                "                              stop:0 #dff8ff, stop:0.38 #ffffff,"
                "                              stop:0.72 #f8ffff, stop:1 #e7fbff);"
                "}").arg(r.color));
    applySoftShadow(wrapper, 20, 5, QColor(139, 223, 242, 22));
    wrapper->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    QVBoxLayout *cl = new QVBoxLayout(wrapper);
    cl->setContentsMargins(20, 18, 20, 18);
    cl->setSpacing(11);

    // ── Header: icon + title ──────────────────────────────────────────────
    QWidget *header = new QWidget();
    header->setStyleSheet("border:none; background:transparent;");
    QHBoxLayout *hl = new QHBoxLayout(header);
    hl->setContentsMargins(0, 0, 0, 0);
    hl->setSpacing(12);

    QLabel *iconLbl = new QLabel(r.icon);
    iconLbl->setFixedSize(38, 38);
    iconLbl->setAlignment(Qt::AlignCenter);
    iconLbl->setStyleSheet(
        QString("background:%1; border-radius:10px; font-size:19px; border:none;").arg(r.color));

    QLabel *titleLbl = new QLabel(r.title);
    titleLbl->setWordWrap(true);
    titleLbl->setStyleSheet(
        "font-size:15px; font-weight:800; color:#173c2c; border:none; "
        "letter-spacing:-0.1px;");

    hl->addWidget(iconLbl, 0, Qt::AlignTop);
    hl->addWidget(titleLbl, 1);
    cl->addWidget(header);

    // ── Description ───────────────────────────────────────────────────────
    QLabel *descLbl = new QLabel(r.description);
    descLbl->setWordWrap(true);
    descLbl->setStyleSheet(
        "font-size:13px; color:#4f6255; border:none; line-height:165%;");
    cl->addWidget(descLbl);

    // ── Contact / Details block ───────────────────────────────────────────
    if (!r.details.isEmpty()) {
        QFrame *detBox = new QFrame();
        detBox->setStyleSheet(
            "QFrame {"
            "  background: #f8f4ea;"
            "  border: 1px solid #d5e0cf;"
            "  border-radius: 12px;"
            "}");
        QVBoxLayout *dl = new QVBoxLayout(detBox);
        dl->setContentsMargins(12, 10, 12, 10);
        dl->setSpacing(4);

        for (const QString &line : r.details.split('\n')) {
            if (line.trimmed().isEmpty()) continue;
            QLabel *lbl = new QLabel(line.trimmed());
            lbl->setWordWrap(true);
            lbl->setStyleSheet(
                "font-size:12px; color:#4f6255; border:none; background:transparent; "
                "line-height:155%;");
            dl->addWidget(lbl);
        }
        cl->addWidget(detBox);
    }

    // ── Footer: tags + buttons ────────────────────────────────────────────
    QWidget *footer = new QWidget();
    footer->setStyleSheet("border:none; background:transparent;");
    QHBoxLayout *fl = new QHBoxLayout(footer);
    fl->setContentsMargins(0, 4, 0, 0);
    fl->setSpacing(6);

    static const QStringList tagStyles = {
        "background:#edf4e8; color:#355141; border:1px solid #bfd0bb;",
        "background:#f7f1e3; color:#6a7054; border:1px solid #ddd2bb;",
        "background:#eef1e6; color:#597062; border:1px solid #ccd9c8;"
    };
    for (int i = 0; i < r.tags.size(); i++) {
        QLabel *t = new QLabel(r.tags[i]);
        t->setStyleSheet(
            QString("font-size:10px; font-weight:700; padding:3px 10px; "
                    "border-radius:11px; letter-spacing:0.2px; %1")
                .arg(tagStyles[i % tagStyles.size()]));
        fl->addWidget(t);
    }
    fl->addStretch();

    cl->addWidget(footer);

    QStringList buttonLabels;
    QStringList buttonUrls;
    if (!r.url.isEmpty()) {
        buttonLabels << (r.url.startsWith("qrc:/") ? "Open PDF →" : "Visit →");
        buttonUrls << r.url;
    }
    if (!r.url2.isEmpty()) {
        buttonLabels << (r.url2Label.isEmpty() ? "Open →" : r.url2Label);
        buttonUrls << r.url2;
    }
    for (int i = 0; i + 1 < r.extraLinks.size(); i += 2) {
        buttonLabels << r.extraLinks[i];
        buttonUrls << r.extraLinks[i + 1];
    }

    if (!buttonUrls.isEmpty()) {
        QWidget *buttonWrap = new QWidget();
        buttonWrap->setStyleSheet("border:none; background:transparent;");
        QGridLayout *buttonGrid = new QGridLayout(buttonWrap);
        buttonGrid->setContentsMargins(0, 0, 0, 0);
        buttonGrid->setHorizontalSpacing(8);
        buttonGrid->setVerticalSpacing(8);

        auto addUrlBtn = [&](const QString &url, const QString &label, int row, int column) {
            QPushButton *btn = new QPushButton(label);
            const QString u = url;
            if (u.startsWith("subpage:") && onInternalTarget) {
                QObject::connect(btn, &QPushButton::clicked, [u, onInternalTarget]() {
                    onInternalTarget(u.mid(QString("subpage:").length()));
                });
            } else {
                QObject::connect(btn, &QPushButton::clicked, [u]() {
                    openResourceTarget(u);
                });
            }
            btn->setStyleSheet(
                "QPushButton { font-size:12px; font-weight:800; color:#2a4434; "
                "  background:qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                "                              stop:0 #e3efdc, stop:0.58 #d4e6cd, stop:1 #f7efdc);"
                "  border:1px solid #c7d8c1; border-radius:11px; padding:7px 16px; }"
                "QPushButton:hover { background:#e8f1e4; color:#234030; border-color:#a8bf9d; }");
            btn->setCursor(Qt::PointingHandCursor);
            buttonGrid->addWidget(btn, row, column);
        };

        const int columns = buttonUrls.size() > 4 ? 3 : 2;
        for (int i = 0; i < buttonUrls.size(); ++i)
            addUrlBtn(buttonUrls[i], buttonLabels[i], i / columns, i % columns);

        cl->addWidget(buttonWrap);
    }

    return wrapper;
}

// Large clickable topic card for the landing page
static QWidget* makeTopicCard(const TopicInfo &t,
                              std::function<void()> onClick,
                              QLabel **subtitleLabelOut = nullptr) {
    QPushButton *card = new QPushButton();
    card->setFixedSize(268, 268);
    card->setCursor(Qt::PointingHandCursor);
    card->setStyleSheet(
        "QPushButton {"
        "  border: 1px solid #c7d8c1;"
        "  border-radius: 18px;"
        "  background: rgba(255, 255, 255, 0.96);"
        "}"
        "QPushButton:hover {"
        "  border: 1px solid #afc5aa;"
        "  background: rgba(255, 255, 255, 1.0);"
        "}"
        "QPushButton:pressed {"
        "  background: #f5f8ef;"
        "}"
    );
    applySoftShadow(card, 18, 4, QColor(121, 154, 112, 12));

    QHBoxLayout *hl = new QHBoxLayout(card);
    hl->setContentsMargins(24, 22, 24, 22);
    hl->setSpacing(0);
    hl->setAlignment(Qt::AlignCenter);

    QLabel *iconLbl = new QLabel(t.icon);
    iconLbl->setFixedSize(60, 60);
    iconLbl->setAlignment(Qt::AlignCenter);
    iconLbl->setStyleSheet(
        "background:transparent; border:none; font-size:32px;");
    iconLbl->setAttribute(Qt::WA_TransparentForMouseEvents);

    QWidget *textBlock = new QWidget();
    textBlock->setStyleSheet("border:none; background:transparent;");
    textBlock->setAttribute(Qt::WA_TransparentForMouseEvents);
    QVBoxLayout *vl = new QVBoxLayout(textBlock);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(9);
    vl->setAlignment(Qt::AlignCenter);
    QLabel *titleLbl = new QLabel(t.title);
    titleLbl->setWordWrap(true);
    titleLbl->setAlignment(Qt::AlignCenter);
    titleLbl->setStyleSheet(
        "font-size:16px; font-weight:800; color:#1a1a1a; border:none; "
        "letter-spacing:-0.1px;");
    QLabel *subLbl = new QLabel(t.subtitle);
    subLbl->setWordWrap(true);
    subLbl->setAlignment(Qt::AlignCenter);
    subLbl->setMaximumWidth(190);
    subLbl->setStyleSheet("font-size:11px; color:#6d8272; border:none;");
    subLbl->setVisible(true);
    vl->addWidget(titleLbl);
    vl->addWidget(subLbl);

    if (subtitleLabelOut)
        *subtitleLabelOut = subLbl;

    QVBoxLayout *cardColumn = new QVBoxLayout();
    cardColumn->setContentsMargins(0, 0, 0, 0);
    cardColumn->setSpacing(14);
    cardColumn->setAlignment(Qt::AlignCenter);
    cardColumn->addStretch();
    cardColumn->addWidget(iconLbl, 0, Qt::AlignHCenter);
    cardColumn->addWidget(textBlock, 0, Qt::AlignHCenter);
    cardColumn->addStretch();

    hl->addLayout(cardColumn);

    QObject::connect(card, &QPushButton::clicked, onClick);
    return card;
}

// Scrollable list of resource cards
static QWidget* makeResourcePage(int topicIndex,
                                 std::function<void(const QString&)> onInternalTarget = {}) {
    QScrollArea *scroll = new QScrollArea();
    scroll->setObjectName("screenSurface");
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->viewport()->setObjectName("screenViewport");
    QWidget *inner = new QWidget();
    inner->setObjectName("screenSurface");
    QVBoxLayout *il = new QVBoxLayout(inner);
    il->setSpacing(14);
    il->setContentsMargins(2, 8, 10, 10);
    for (const ResourceInfo &r : RESOURCES[topicIndex])
        il->addWidget(makeResourceCard(r, onInternalTarget));
    il->addStretch();
    scroll->setWidget(inner);
    return scroll;
}

static QWidget* makeSubpage(const ResourceSubpageInfo &subpage,
                            std::function<void(int)> onBackToTopic) {
    QWidget *page = new QWidget();
    page->setObjectName("screenSurface");
    QVBoxLayout *pl = new QVBoxLayout(page);
    pl->setContentsMargins(48, 36, 48, 36);
    pl->setSpacing(18);

    QWidget *topRow = new QWidget();
    topRow->setStyleSheet("border:none; background:transparent;");
    QHBoxLayout *trl = new QHBoxLayout(topRow);
    trl->setContentsMargins(0, 0, 0, 0);
    trl->setSpacing(12);

    QPushButton *backBtn = new QPushButton(
        QString("←  Back to %1").arg(TOPICS[subpage.parentTopicIndex].title));
    backBtn->setStyleSheet(
        "QPushButton { font-size:13px; font-weight:600; color:#365143; "
        "  background:#f6f1e5; border:1px solid #c7d8c1; "
        "  border-radius:12px; padding:8px 16px; }"
        "QPushButton:hover { background:#edf4e8; border-color:#a8bf9d; }");
    backBtn->setCursor(Qt::PointingHandCursor);
    QObject::connect(backBtn, &QPushButton::clicked, [onBackToTopic, subpage]() {
        onBackToTopic(subpage.parentTopicIndex);
    });

    QLabel *iconLbl = new QLabel(subpage.icon);
    iconLbl->setFixedSize(42, 42);
    iconLbl->setAlignment(Qt::AlignCenter);
    iconLbl->setStyleSheet(
        QString("background:%1; border-radius:11px; font-size:20px; border:none;").arg(subpage.color));

    QLabel *titleLbl = new QLabel(subpage.title);
    titleLbl->setWordWrap(true);
    titleLbl->setStyleSheet(
        "font-size:24px; font-weight:700; color:#173c2c; border:none; "
        "letter-spacing:-0.3px;");

    trl->addWidget(backBtn);
    trl->addSpacing(4);
    trl->addWidget(iconLbl);
    trl->addWidget(titleLbl, 1);

    QFrame *content = new QFrame();
    content->setStyleSheet(
        "QFrame { background:qradialgradient(cx:0.18, cy:0.12, radius:1.15,"
        "                                  fx:0.18, fy:0.12,"
        "                                  stop:0 #f8f3e8, stop:0.40 #ffffff,"
        "                                  stop:0.76 #f7faf4, stop:1 #eef4ea);"
        "         border:1px solid #c7d8c1; border-radius:24px; }");
    applySoftShadow(content, 24, 7, QColor(121, 154, 112, 18));
    QVBoxLayout *cl = new QVBoxLayout(content);
    cl->setContentsMargins(24, 22, 24, 22);
    cl->setSpacing(14);

    QLabel *subtitle = new QLabel(subpage.subtitle);
    subtitle->setWordWrap(true);
    subtitle->setStyleSheet(
        "font-size:15px; font-weight:600; color:#355141; border:none; line-height:165%;");
    cl->addWidget(subtitle);

    QFrame *detailsBox = new QFrame();
    detailsBox->setStyleSheet(
        "QFrame { background:#f8f4ea; border:1px solid #d5e0cf; border-radius:14px; }");
    QVBoxLayout *dl = new QVBoxLayout(detailsBox);
    dl->setContentsMargins(16, 14, 16, 14);
    dl->setSpacing(6);

    for (const QString &line : subpage.details.split('\n')) {
        if (line.trimmed().isEmpty())
            continue;
        QLabel *lineLbl = new QLabel(line.trimmed());
        lineLbl->setWordWrap(true);
        lineLbl->setStyleSheet(
            "font-size:12px; color:#4f6255; border:none; background:transparent; line-height:165%;");
        dl->addWidget(lineLbl);
    }
    cl->addWidget(detailsBox);

    if (!subpage.childLinks.isEmpty()) {
        QLabel *branchLabel = new QLabel("Choose a tool");
        branchLabel->setStyleSheet(
            "font-size:12px; font-weight:800; color:#6d8272; border:none; "
            "letter-spacing:1.4px; text-transform:uppercase;");
        cl->addWidget(branchLabel);

        QWidget *branchWrap = new QWidget();
        branchWrap->setStyleSheet("border:none; background:transparent;");
        QGridLayout *branchGrid = new QGridLayout(branchWrap);
        branchGrid->setContentsMargins(0, 0, 0, 0);
        branchGrid->setHorizontalSpacing(10);
        branchGrid->setVerticalSpacing(10);

        for (int i = 0; i + 1 < subpage.childLinks.size(); i += 2) {
            const QString label = subpage.childLinks[i];
            const QString url = subpage.childLinks[i + 1];
            QPushButton *toolBtn = new QPushButton(label + " →");
            toolBtn->setCursor(Qt::PointingHandCursor);
            toolBtn->setStyleSheet(
                "QPushButton { font-size:13px; font-weight:800; color:#2a4434; "
                "  background:qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                "                              stop:0 #e3efdc, stop:0.6 #d4e6cd, stop:1 #f7efdc);"
                "  border:1px solid #c7d8c1; border-radius:14px; padding:12px 16px; }"
                "QPushButton:hover { background:#e8f1e4; color:#234030; border-color:#a8bf9d; }");
            QObject::connect(toolBtn, &QPushButton::clicked, [url]() {
                openResourceTarget(url);
            });
            const int itemIndex = i / 2;
            branchGrid->addWidget(toolBtn, itemIndex / 2, itemIndex % 2);
        }

        cl->addWidget(branchWrap);
    }

    QWidget *tagRow = new QWidget();
    tagRow->setStyleSheet("border:none; background:transparent;");
    QHBoxLayout *tl = new QHBoxLayout(tagRow);
    tl->setContentsMargins(0, 0, 0, 0);
    tl->setSpacing(8);
    for (const QString &tag : subpage.tags)
        tl->addWidget(makeInfoChip(tag, "#edf4e8", "#355141", "#bfd0bb"));
    tl->addStretch();
    cl->addWidget(tagRow);

    QPushButton *openBtn = new QPushButton(subpage.urlLabel);
    const QString targetUrl = subpage.url;
    QObject::connect(openBtn, &QPushButton::clicked, [targetUrl]() {
        openResourceTarget(targetUrl);
    });
    openBtn->setCursor(Qt::PointingHandCursor);
    openBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    openBtn->setStyleSheet(
        "QPushButton { font-size:13px; font-weight:800; color:#2a4434; "
        "  background:qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "                              stop:0 #e3efdc, stop:0.58 #d4e6cd, stop:1 #f7efdc);"
        "  border:1px solid #c7d8c1; border-radius:12px; padding:10px 18px; }"
        "QPushButton:hover { background:#e8f1e4; }");
    cl->addWidget(openBtn, 0, Qt::AlignLeft);

    pl->addWidget(topRow);
    pl->addWidget(content);
    pl->addStretch();
    return page;
}

// ─────────────────────────────────────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────────────────────────────────────

// ── Activation hook ───────────────────────────────────────────────────────────
// Resets to the topic-picker home page whenever the user navigates to this
// screen. Overrides the virtual onActivated() from Screen.
void Recommendations::onActivated() {
    if (m_searchInput) {
        m_searchInput->clear();
        applyTopicSearch(QString{});
    }
    m_stack->setCurrentIndex(0);
}

void Recommendations::applyTopicSearch(const QString &query) {
    const QString normalized = normalizedText(query);

    if (normalized.isEmpty()) {
        for (QWidget *card : m_topicCards)
            card->setVisible(true);

        for (int i = 0; i < m_topicSubtitleLabels.size() && i < TOPICS.size(); ++i) {
            m_topicSubtitleLabels[i]->setText(TOPICS[i].subtitle);
            m_topicSubtitleLabels[i]->setStyleSheet(
                "font-size:13px; color:#6d8272; border:none;");
            m_topicSubtitleLabels[i]->setVisible(true);
        }

        m_searchStatus->clear();
        m_searchStatus->setVisible(false);
        return;
    }

    int visibleCount = 0;
    int bestIndex = -1;
    int bestScore = -1;
    QString bestSubtopicSummary;

    for (int i = 0; i < m_topicCards.size(); ++i) {
        const TopicSearchResult result = searchTopic(i, normalized);
        const bool matches = result.score >= 0;
        m_topicCards[i]->setVisible(matches);

        if (i < m_topicSubtitleLabels.size()) {
            if (matches && !result.matchedSubtopics.isEmpty()) {
                m_topicSubtitleLabels[i]->setText(TOPICS[i].subtitle);
                m_topicSubtitleLabels[i]->setStyleSheet(
                    "font-size:11px; color:#6d8272; border:none;");
                m_topicSubtitleLabels[i]->setVisible(true);
            } else {
                m_topicSubtitleLabels[i]->setText(TOPICS[i].subtitle);
                m_topicSubtitleLabels[i]->setStyleSheet(
                    "font-size:11px; color:#6d8272; border:none;");
                m_topicSubtitleLabels[i]->setVisible(matches);
            }
        }

        if (matches) {
            visibleCount++;
            if (result.score > bestScore) {
                bestScore = result.score;
                bestIndex = i;
                bestSubtopicSummary = summarizedSubtopics(result.matchedSubtopics);
            }
        }
    }

    if (visibleCount == 0) {
        m_searchStatus->setText(
            "No topic matched that search. Try words like food, immigration, tutoring, Zoom, or counseling.");
        m_searchStatus->setStyleSheet(
            "font-size:11px; color:#C62828; border:none; background:transparent;");
        m_searchStatus->setVisible(true);
        return;
    }

    if (visibleCount == 1) {
        if (bestSubtopicSummary.isEmpty()) {
            m_searchStatus->setText("1 matching topic found. Press Enter to open it.");
        } else {
            m_searchStatus->setText(
                QString("1 matching topic found via subtopic: %1. Press Enter to open it.")
                    .arg(bestSubtopicSummary));
        }
    } else {
        if (bestSubtopicSummary.isEmpty()) {
            m_searchStatus->setText(
                QString("%1 matching topics found. Press Enter to open the best match.")
                    .arg(visibleCount));
        } else {
            m_searchStatus->setText(
                QString("%1 matching topics found. Best subtopic match: %2. Press Enter to open it.")
                    .arg(visibleCount)
                    .arg(bestSubtopicSummary));
        }
    }

    if (bestIndex >= 0) {
        m_searchStatus->setStyleSheet(
            "font-size:12px; color:#4c5e78; border:none; background:transparent;");
        m_searchStatus->setVisible(true);
    }
}

void Recommendations::openBestTopicMatch() {
    const QString query = m_searchInput ? m_searchInput->text() : QString{};
    const QString normalized = normalizedText(query);
    if (normalized.isEmpty()) return;

    int bestIndex = -1;
    int bestScore = -1;
    for (int i = 0; i < TOPICS.size(); ++i) {
        const TopicSearchResult result = searchTopic(i, normalized);
        if (result.score > bestScore) {
            bestScore = result.score;
            bestIndex = i;
        }
    }

    if (bestIndex >= 0)
        showTopic(bestIndex);
}

// ─────────────────────────────────────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────────────────────────────────────

Recommendations::Recommendations(QWidget *parent) : Screen("BMCC Resources", parent) {
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    m_stack = new QStackedWidget(this);
    m_stack->setObjectName("screenSurface");

    // ── Page 0: Landing / topic picker ───────────────────────────────────
    QScrollArea *homeScroll = new QScrollArea();
    homeScroll->setObjectName("screenSurface");
    homeScroll->setWidgetResizable(true);
    homeScroll->setFrameShape(QFrame::NoFrame);
    homeScroll->viewport()->setObjectName("screenViewport");

    QWidget *homePage = new QWidget();
    homePage->setObjectName("screenSurface");
    QVBoxLayout *hl = new QVBoxLayout(homePage);
    hl->setContentsMargins(48, 28, 48, 48);
    hl->setSpacing(0);

    QWidget *homeInner = new QWidget();
    homeInner->setStyleSheet("border:none; background:transparent;");
    homeInner->setMaximumWidth(1240);
    QVBoxLayout *innerLay = new QVBoxLayout(homeInner);
    innerLay->setContentsMargins(0, 0, 0, 0);
    innerLay->setSpacing(16);

    QLabel *greeting = new QLabel("WELCOME");
    greeting->setStyleSheet(
        "font-size:12px; font-weight:700; color:#5e6d85; "
        "letter-spacing:1.8px;");

    QLabel *heading = new QLabel("What kind of support do you need right now?");
    heading->setStyleSheet(
        "font-size:40px; font-weight:800; color:#111111; "
        "letter-spacing:-0.8px;");

    QLabel *sub = new QLabel(
        "BMCC offers resources that can help you feel supported, connected, and heard.");
    sub->setWordWrap(false);
    sub->setMaximumWidth(900);
    sub->setStyleSheet("font-size:15px; color:#4c5e78; border:none;");

    QWidget *headerWrap = new QWidget();
    headerWrap->setStyleSheet("border:none; background:transparent;");
    QVBoxLayout *headerLay = new QVBoxLayout(headerWrap);
    headerLay->setContentsMargins(0, 0, 0, 0);
    headerLay->setSpacing(10);
    headerLay->addWidget(greeting, 0, Qt::AlignLeft);
    headerLay->addWidget(heading, 0, Qt::AlignLeft);
    headerLay->addWidget(sub, 0, Qt::AlignLeft);

    innerLay->addWidget(headerWrap);
    innerLay->addSpacing(18);

    QFrame *searchCard = new QFrame();
    searchCard->setStyleSheet(
        "QFrame { background:transparent; border:none; }");
    QVBoxLayout *searchOuter = new QVBoxLayout(searchCard);
    searchOuter->setContentsMargins(0, 0, 0, 0);
    searchOuter->setSpacing(8);

    QWidget *searchRow = new QWidget();
    searchRow->setStyleSheet("border:none; background:transparent;");
    QHBoxLayout *searchLay = new QHBoxLayout(searchRow);
    searchLay->setContentsMargins(0, 0, 0, 0);
    searchLay->setSpacing(10);
    searchLay->setAlignment(Qt::AlignLeft);

    QLabel *searchIcon = new QLabel("⌕");
    searchIcon->setAlignment(Qt::AlignCenter);
    searchIcon->setFixedSize(38, 38);
    searchIcon->setStyleSheet(
        "font-size:16px; color:#4c5e78; background:#ffffff; border:1px solid #d7ddd2; "
        "border-radius:13px;");

    m_searchInput = new QLineEdit();
    m_searchInput->setPlaceholderText(
        "Search topics or subtopics, e.g. food, Zoom, fitness, DACA, counseling");
    m_searchInput->setClearButtonEnabled(true);
    m_searchInput->setMaximumWidth(560);
    m_searchInput->setStyleSheet(
        "QLineEdit {"
        "  border:1px solid #d7ddd2;"
        "  border-radius:14px;"
        "  background:#ffffff;"
        "  padding:12px 14px;"
        "  font-size:14px;"
        "  color:#1a1a1a;"
        "}"
        "QLineEdit:focus {"
        "  border:1px solid #b8c8b5;"
        "  background:#ffffff;"
        "}");

    QPushButton *searchBtn = new QPushButton("Search");
    searchBtn->setMinimumHeight(42);
    searchBtn->setCursor(Qt::PointingHandCursor);
    searchBtn->setStyleSheet(
        "QPushButton { font-size:13px; font-weight:700; color:#1a1a1a; "
        "background:#ffffff; border:1px solid #d7ddd2; border-radius:14px; padding:10px 18px; }"
        "QPushButton:hover { background:#f7f7f4; border-color:#c4d1c0; }");

    searchLay->addWidget(searchIcon);
    searchLay->addWidget(m_searchInput);
    searchLay->addWidget(searchBtn);
    searchLay->addStretch();

    m_searchStatus = new QLabel();
    m_searchStatus->setWordWrap(true);
    m_searchStatus->setVisible(false);

    searchOuter->addWidget(searchRow);
    searchOuter->addWidget(m_searchStatus);
    innerLay->addWidget(searchCard);
    innerLay->addSpacing(18);

    connect(m_searchInput, &QLineEdit::textChanged,
            this, &Recommendations::applyTopicSearch);
    connect(m_searchInput, &QLineEdit::returnPressed,
            this, &Recommendations::openBestTopicMatch);
    connect(searchBtn, &QPushButton::clicked,
            this, &Recommendations::openBestTopicMatch);

    QWidget *topicList = new QWidget();
    topicList->setStyleSheet("border:none; background:transparent;");
    QGridLayout *topicListLay = new QGridLayout(topicList);
    topicListLay->setContentsMargins(0, 8, 0, 0);
    topicListLay->setHorizontalSpacing(28);
    topicListLay->setVerticalSpacing(28);

    for (int i = 0; i < TOPICS.size(); i++) {
        int idx = i;
        QLabel *subtitleLabel = nullptr;
        QWidget *card = makeTopicCard(
            TOPICS[i],
            [this, idx]() { showTopic(idx); },
            &subtitleLabel);
        m_topicCards.append(card);
        m_topicSubtitleLabels.append(subtitleLabel);
        topicListLay->addWidget(card, i / 3, i % 3, Qt::AlignCenter);
    }
    for (int col = 0; col < 3; ++col)
        topicListLay->setColumnStretch(col, 1);

    innerLay->addWidget(topicList, 0, Qt::AlignHCenter);
    innerLay->addStretch();

    QHBoxLayout *centerLay = new QHBoxLayout();
    centerLay->setContentsMargins(0, 0, 0, 0);
    centerLay->addStretch();
    centerLay->addWidget(homeInner);
    centerLay->addStretch();
    hl->addLayout(centerLay);
    hl->addStretch();

    homeScroll->setWidget(homePage);
    m_stack->addWidget(homeScroll); // index 0

    applyTopicSearch(QString{});

    // ── Pages 1–5: Resource detail pages ─────────────────────────────────
    for (int i = 0; i < TOPICS.size(); i++) {
        QWidget *page = new QWidget();
        page->setObjectName("screenSurface");
        QVBoxLayout *pl = new QVBoxLayout(page);
        pl->setContentsMargins(56, 42, 56, 42);
        pl->setSpacing(20);

        // Back button + topic title row
        QWidget *topRow = new QWidget();
        topRow->setStyleSheet("border:none;");
        QHBoxLayout *trl = new QHBoxLayout(topRow);
        trl->setContentsMargins(0, 0, 0, 0);
        trl->setSpacing(12);

        QPushButton *backBtn = new QPushButton("←  Back");
        backBtn->setStyleSheet(
            "QPushButton { font-size:13px; font-weight:600; color:#f1e8ad; "
            "  background:#0b2a3c; border:1px solid #8bdff2; "
            "  border-radius:12px; padding:8px 16px; }"
            "QPushButton:hover { background:#123f46; border-color:#f1e8ad; }");
        backBtn->setCursor(Qt::PointingHandCursor);
        connect(backBtn, &QPushButton::clicked, this, &Recommendations::showHome);

        QLabel *iconLbl = new QLabel(TOPICS[i].icon);
        iconLbl->setFixedSize(40, 40);
        iconLbl->setAlignment(Qt::AlignCenter);
        iconLbl->setStyleSheet(
            QString("background:%1; border-radius:10px; font-size:20px; border:none;")
                .arg(TOPICS[i].accent));

        QLabel *titleLbl = new QLabel(TOPICS[i].title);
        titleLbl->setStyleSheet(
            "font-size:23px; font-weight:700; color:#173c2c; border:none; "
            "letter-spacing:-0.3px;");

        trl->addWidget(backBtn);
        trl->addSpacing(4);
        trl->addWidget(iconLbl);
        trl->addWidget(titleLbl, 1);

        QFrame *div = new QFrame();
        div->setFrameShape(QFrame::HLine);
        div->setStyleSheet("color:#dfece1; background:#dfece1; border:none;");

        QLabel *sectionLbl = new QLabel("AVAILABLE AT BMCC");
        sectionLbl->setObjectName("sectionLabel");

        pl->addWidget(topRow);
        pl->addWidget(div);
        pl->addSpacing(4);

        QFrame *summary = new QFrame();
        summary->setStyleSheet(
            "QFrame { background:qradialgradient(cx:0.18, cy:0.12, radius:1.15,"
            "                                  fx:0.18, fy:0.12,"
            "                                  stop:0 #dff8ff, stop:0.38 #ffffff,"
            "                                  stop:0.76 #f8ffff, stop:1 #e7fbff);"
            "         border:1px solid #8bdff2; border-radius:20px; }");
        applySoftShadow(summary, 20, 5, QColor(139, 223, 242, 20));
        QVBoxLayout *sl = new QVBoxLayout(summary);
        sl->setContentsMargins(16, 14, 16, 14);
        sl->setSpacing(8);

        QLabel *summaryText = new QLabel(
            "This section gathers the most relevant BMCC and trusted external support options "
            "for this topic, including direct links, locations, hours, and contact details.");
        summaryText->setWordWrap(true);
        summaryText->setStyleSheet(
            "font-size:13px; color:#4f6255; border:none; line-height:170%;");

        QWidget *metaRow = new QWidget();
        metaRow->setStyleSheet("border:none; background:transparent;");
        QHBoxLayout *ml = new QHBoxLayout(metaRow);
        ml->setContentsMargins(0, 0, 0, 0);
        ml->setSpacing(8);
        ml->addWidget(makeInfoChip(QString("%1 resources").arg(RESOURCES[i].size()),
                                   "#e7fbff", "#073b4c", "#8bdff2"));
        ml->addWidget(makeInfoChip(TOPICS[i].subtitle, "#f8ffff", "#31535f", "#c7eef5"));
        ml->addStretch();

        sl->addWidget(summaryText);
        sl->addWidget(metaRow);

        pl->addWidget(summary);
        pl->addWidget(sectionLbl);
        pl->addWidget(makeResourcePage(i, [this](const QString &key) {
            for (int pageIndex = 0; pageIndex < RESOURCE_SUBPAGES.size(); ++pageIndex) {
                if (RESOURCE_SUBPAGES[pageIndex].key == key) {
                    m_stack->setCurrentIndex(1 + TOPICS.size() + pageIndex);
                    return;
                }
            }
        }), 1);

        m_stack->addWidget(page); // indices 1–5
    }

    for (const ResourceSubpageInfo &subpage : RESOURCE_SUBPAGES) {
        m_stack->addWidget(makeSubpage(subpage, [this](int topicIndex) {
            showTopic(topicIndex);
        }));
    }

    root->addWidget(m_stack);
    m_stack->setCurrentIndex(0);
}

void Recommendations::showTopic(int index) {
    m_stack->setCurrentIndex(index + 1); // +1 because index 0 is home
}

void Recommendations::showHome() {
    m_stack->setCurrentIndex(0);
}
