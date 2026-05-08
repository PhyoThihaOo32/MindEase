#include "screens/toolkit.h"
#include "core/lucideicons.h"
#include <QColor>
#include <QDesktopServices>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QUrl>
#include <QVBoxLayout>
#include <functional>

struct ToolItem {
    QString icon;
    QString title;
    QString body;
    QString url;
    QString urlLabel;
    QString url2 = {};
    QString url2Label = {};
};

struct FolderDef {
    QString id;
    QString icon;
    QString accent;
    QString accentBorder;
    QString name;
    QString tagline;
    QString intro;
    QList<ToolItem> items;
};

static const QList<FolderDef> FOLDERS = {
    {   "academic", "📚", "#dcfff1", "#8ee2a8",
        "Academic Stress",
        "Deadlines, studying, tutoring, and staying focused.",
        "School can feel heavy fast. This section brings together study tools, academic support, and small strategies for handling pressure with more structure and less panic.",
        {
            { "🗓️", "Weekly Planner",
              "Break tasks into manageable chunks so you always know what to prioritize. A weekly planner can reduce semester overwhelm by giving you a clear, day-by-day roadmap.",
              "https://www.bmcc.cuny.edu/student-affairs/counseling/your-mental-health-toolkit/",
              "Open Toolkit →" },
            { "🧩", "Procrastination Worksheet",
              "Identify what is holding you back and create actionable steps to move forward. Pinpoint your procrastination triggers and build a plan that actually works for you.",
              "https://freetodolist.com/", "Open Worksheet →" },
            { "☕", "Smart Study Break Ideas",
              "• 5-minute refresh: stretch or take a short walk\n"
              "• Creative break: doodle or free-write in a journal\n"
              "• Snack smart: fruit, nuts, or a protein bar\n"
              "• Mindful moment: try a 5-minute guided meditation\n"
              "• Social recharge: check in with a friend or study buddy", "", "" },
            { "🏫", "BMCC Academic Support",
              "Free tutoring, Writing Center, academic coaching, library quiet spaces, and the Study Space Guide are all available on campus.",
              "https://www.bmcc.cuny.edu/learning-assistance/", "Visit →" },
            { "🧠", "BMCC Counseling — Academic Stress",
              "Counselors can help with stress reduction, time management, and academic success strategies.\n📍 Room S-343   📞 (212) 220-8140",
              "https://www.bmcc.cuny.edu/student-affairs/counseling/academics/", "Book Appointment →" },
        }
    },
    {   "connection", "🤝", "#e7fbff", "#8bdff2",
        "Connection & Community",
        "Peer support, groups, clubs, and finding belonging.",
        "You do not have to do college alone. This section is for support groups, workshops, clubs, and community resources that can help you feel more connected.",
        {
            { "🫂", "Counseling Groups & Workshops",
              "BMCC offers group spaces and workshops where students can share experiences, practice coping skills, and build connection.",
              "https://www.bmcc.cuny.edu/student-affairs/counseling/groups-and-workshops/groups/",
              "View Groups →" },
            { "📅", "Upcoming Workshops",
              "Drop-in and pop-up workshops on stress management, art-based stress relief, mindfulness, and more.",
              "https://www.bmcc.cuny.edu/student-affairs/counseling/groups-and-workshops/workshops/",
              "View Schedule →" },
            { "🎓", "Student Clubs & Activities",
              "Join or start a club to meet people who share your interests and build community on campus.",
              "https://www.bmcc.cuny.edu/student-affairs/student-activities/clubs/",
              "Find a Club →" },
            { "📞", "NYC Crisis Resources (24/7)",
              "Hotlines and texting services for emergency mental health support, available 24/7.",
              "https://www.bmcc.cuny.edu/student-affairs/counseling/counseling-resources/nyc-resources/",
              "See All Hotlines →" },
            { "🌐", "Community Referrals",
              "Outside referrals for housing support, domestic assistance, legal aid, and substance use services curated by BMCC.",
              "https://www.bmcc.cuny.edu/student-affairs/counseling/counseling-resources/community-resources/",
              "Visit →" },
        }
    },
    {   "identity", "🌈", "#fbf4d0", "#f1e8ad",
        "Empowered Identity & Care",
        "Racial trauma, LGBTQIA+, disability justice, and identity support.",
        "This section gathers affirming resources for students navigating identity, discrimination, and belonging. Support should make room for every part of who you are.",
        {
            { "✊", "Racial Trauma Resources",
              "Resources for learning, validation, and healing around racial trauma and repeated racism.",
              "https://www.bmcc.cuny.edu/about-bmcc/race-equity-and-inclusion/resources/racial-trauma-resources/",
              "Open BMCC Resources →" },
            { "📖", "Anti-Racist Resources",
              "Articles, talks, and infographics that support learning, allyship, and reflection.", "", "" },
            { "🏳️‍🌈", "LGBTQIA+ Support & Hotlines",
              "Trans Lifeline: (877) 565-8860\nLGBT National Hotline: (888) 843-4564\nTrevor Lifeline: (866) 488-7386", "", "" },
            { "♿", "Disability Justice — 10 Principles",
              "A framework for understanding disability in relation to race, gender, and other intersecting identities.",
              "https://www.sinsinvalid.org/blog/10-principles-of-disability-justice", "Read →" },
            { "💬", "BMCC Counseling — Identity Support",
              "BMCC counselors provide affirming, culturally competent support for all students.\n📍 Room S-343   📞 (212) 220-8140",
              "https://www.bmcc.cuny.edu/student-affairs/counseling/counseling-services/", "Visit →" },
        }
    },
    {   "gratitude", "✏️", "#fbf4d0", "#f1e8ad",
        "Gratitude & Journaling",
        "Prompts, reflection, and gentle daily writing.",
        "A few minutes of reflection can help slow racing thoughts and make room for perspective. This section offers prompts and simple journaling ideas you can start right away.",
        {
            { "🔬", "Why It Works",
              "Gratitude and journaling are linked to lower stress, improved sleep, stronger relationships, and greater resilience.", "", "" },
            { "📝", "Journal Prompts to Try Today",
              "1. What are three things that happened today that brought you peace?\n"
              "2. What went well this week, even if something else was hard?\n"
              "3. What everyday object near you are you grateful for?\n"
              "4. Who made a positive difference in your life this month?", "", "" },
            { "🗓️", "7-Day Gratitude Challenge",
              "A small guided challenge to help you reflect on what is steady, good, or meaningful in your week.",
              "https://www.bmcc.cuny.edu/student-affairs/counseling/your-mental-health-toolkit/",
              "Open Toolkit →" },
            { "📚", "Recommended Reading",
              "The Artist's Way — Julia Cameron\n"
              "The Great Book of Journaling — Eric Maisel\n"
              "Thanks! — Robert Emmons", "", "" },
        }
    },
    {   "mindfulness", "🧘", "#dcfff1", "#8ee2a8",
        "Mindfulness & Meditation",
        "Breathing, guided meditation, and grounding practices.",
        "Mindfulness does not need to be perfect or long. Even a short pause can help settle your body and make the next moment feel more manageable.",
        {
            { "💛", "Lovingkindness Meditation",
              "A practical guide with recorded sessions and beginner-friendly support.",
              "https://www.elliemillertherapy.com/blog/loving-kindness-meditation-a-complete-guide",
              "Open Guide →" },
            { "🌿", "Calm — Free Guided Meditations",
              "Open-access guided meditations for stress, anxiety, focus, and sleep.",
              "https://www.youtube.com/@calm", "Watch on YouTube →" },
            { "🎧", "Headspace Meditation Classes",
              "Structured meditation classes for stress, sleep, focus, and anxiety.",
              "https://www.headspace.com/meditation", "Visit →" },
            { "▶️", "Free YouTube Guided Meditation",
              "A simple guided meditation that is easy to try even if this is new for you.",
              "https://www.youtube.com/watch?v=8x-QmEB4REg", "Watch →" },
            { "📖", "Verywell Mind — Meditation Library",
              "A curated library of guided meditations for anxiety, sleep, grief, and more.",
              "https://www.verywellmind.com/guided-meditations-for-anxiety-stress-and-more-4692781",
              "Visit →" },
        }
    },
    {   "music", "🎵", "#dff6ff", "#8bdff2",
        "Music for Your Mind",
        "Lo-fi, calm music, and playlists for focus or rest.",
        "The right sound can shift your energy fast. This section collects calming and focus-friendly music resources you can use while studying, resting, or decompressing.",
        {
            { "😴", "Calming & Sleep Music",
              "For winding down, reducing anxiety, or falling asleep with something soft in the background.",
              "https://www.youtube.com/watch?v=1ZYbU82GVz4", "YouTube: Sleep Music →" },
            { "☕", "Study & Focus Music",
              "Background sound for concentration: lo-fi beats, coffee shop ambiance, and mellow instrumental sets.",
              "https://www.youtube.com/watch?v=5qap5aO4i9A", "YouTube: Lo-Fi Hip Hop →" },
            { "🎵", "Spotify — Peaceful Piano",
              "A calm instrumental playlist for studying or slowing down after a long day.",
              "https://open.spotify.com/playlist/37i9dQZF1DX3rxVfibe1L0", "Open Spotify →" },
            { "🌿", "Lofi Station",
              "A clean, simple lofi music station for studying, relaxing, and staying focused.",
              "https://www.lofi.cafe/", "Open Lofi Station →" },
        }
    },
    {   "nutrition", "🥗", "#e9fff3", "#8ee2a8",
        "Nutrition & Energy",
        "Food access, campus fitness, and steady energy.",
        "Food and movement shape energy, mood, and focus. This section brings together practical campus resources for nourishment, movement, and everyday energy support.",
        {
            { "🧠", "Food & Your Mind",
              "Leafy greens, protein, hydration, and steadier meals can support energy, concentration, and mood balance.", "", "" },
            { "🍎", "Panther Pantry (Free, On Campus)",
              "Non-perishable, refrigerated, and frozen food items for students facing food insecurity.\n📍 Room S-230   🕐 Mon–Fri 8am–6:30pm  |  Sat 10am–1pm",
              "https://www.bmcc.cuny.edu/student-affairs/arc/panther-pantry/", "Visit →" },
            { "🏋️", "Campus Fitness",
              "On campus: Fitness Center, Swimming Pool, and Health & Wellness Services.\nCONTACT: BMCC Athletics\n📞 (212) 220-8260",
              "https://bmccathletics.com/sports/2012/4/2/RecreationSchedule.aspx",
              "Schedule →",
              "https://www.bmcc.cuny.edu/students/fitness-center/",
              "Visit →" },
            { "🌳", "NYC Free Outdoor Fitness",
              "Free outdoor fitness classes in NYC parks, adaptive programs, and community exercise options.",
              "https://www.nycgovparks.org/programs/recreation/free-fitness-classes",
              "NYC Parks →" },
        }
    },
    {   "sleep", "🌙", "#e7fbff", "#8bdff2",
        "Sleep & Relaxation",
        "Better rest, bedtime tools, and winding down.",
        "Rest affects everything: mood, focus, memory, and patience. This section offers practical sleep tools and wind-down ideas that fit student life.",
        {
            { "⏰", "Sleep Calculator",
              "Find a bedtime based on your wake-up time so you can line up fuller sleep cycles.",
              "https://sleepeducation.org/healthy-sleep/bedtime-calculator/", "Calculate →" },
            { "📘", "Young Adult Sleep Toolkit",
              "Covers sleep cycles, routines, and habits that support more consistent rest.",
              "https://www.thensf.org/sleep-in-adolescents/", "Visit →" },
            { "💡", "Tips for Better Sleep",
              "• Get daylight early in the day\n"
              "• Reduce bright screens before bed\n"
              "• Keep meals and sleep times steadier\n"
              "• Make the room cool, dark, and quiet",
              "https://www.sleepfoundation.org/sleep-hygiene", "Sleep Foundation →" },
            { "📵", "Stop Doom-Scrolling",
              "Blue light and endless scrolling can make it harder to fall asleep. Try a phone-free wind-down for 30 minutes before bed.", "", "" },
            { "🎧", "Relaxation Audio",
              "Rain sounds, white noise, or guided sleep meditations can help quiet a busy mind.",
              "https://www.youtube.com/watch?v=1ZYbU82GVz4", "YouTube: Sleep Music →" },
        }
    },
};

static void applyToolkitShadow(QWidget *widget,
                               int blur = 18,
                               int yOffset = 4,
                               const QColor &color = QColor(121, 154, 112, 12)) {
    auto *shadow = new QGraphicsDropShadowEffect(widget);
    shadow->setBlurRadius(blur);
    shadow->setOffset(0, yOffset);
    shadow->setColor(color);
    widget->setGraphicsEffect(shadow);
}

static QString normalizedText(const QString &text) {
    QString out;
    out.reserve(text.size());
    for (const QChar c : text.toLower()) {
        if (c.isLetterOrNumber()) {
            out.append(c);
        } else {
            out.append(' ');
        }
    }
    return out.simplified();
}

static QString folderSearchText(const FolderDef &fd) {
    QString text = fd.name + " " + fd.tagline + " " + fd.intro;
    for (const ToolItem &item : fd.items)
        text += " " + item.title + " " + item.body;
    return normalizedText(text);
}

static int folderSearchScore(const FolderDef &fd, const QString &normalizedQuery) {
    if (normalizedQuery.isEmpty())
        return -1;

    const QStringList tokens = normalizedQuery.split(' ', Qt::SkipEmptyParts);
    const QString name = normalizedText(fd.name);
    const QString tagline = normalizedText(fd.tagline);
    const QString haystack = folderSearchText(fd);

    int score = 0;
    if (name.contains(normalizedQuery))
        score += 80;
    if (tagline.contains(normalizedQuery))
        score += 55;
    if (haystack.contains(normalizedQuery))
        score += 40;

    int matchedTokens = 0;
    for (const QString &token : tokens) {
        if (name.contains(token)) {
            score += 20;
            matchedTokens++;
        } else if (tagline.contains(token)) {
            score += 14;
            matchedTokens++;
        } else if (haystack.contains(token)) {
            score += 8;
            matchedTokens++;
        }
    }

    if (matchedTokens == 0)
        return -1;
    if (matchedTokens == tokens.size())
        score += 16;
    return score;
}

static QWidget* makeToolItem(const ToolItem &item) {
    QFrame *row = new QFrame();
    row->setStyleSheet("QFrame { border:none; background:transparent; }");
    QHBoxLayout *hl = new QHBoxLayout(row);
    hl->setContentsMargins(0, 12, 0, 12);
    hl->setSpacing(14);

    QLabel *iconLbl = new QLabel();
    iconLbl->setFixedSize(40, 40);
    setLucideIcon(iconLbl, item.icon, QColor("#173c2c"), 23);
    iconLbl->setStyleSheet(
        "background:#edf4e8; border:1px solid #c7d8c1; border-radius:12px;");

    QWidget *textBlock = new QWidget();
    textBlock->setStyleSheet("background:transparent; border:none;");
    QVBoxLayout *vl = new QVBoxLayout(textBlock);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(5);

    QLabel *titleLbl = new QLabel(item.title);
    titleLbl->setStyleSheet(
        "font-size:14px; font-weight:800; color:#173c2c; border:none;");
    vl->addWidget(titleLbl);

    if (!item.body.isEmpty()) {
        QLabel *bodyLbl = new QLabel(lucideCleanText(item.body));
        bodyLbl->setWordWrap(true);
        bodyLbl->setStyleSheet(
            "font-size:13px; color:#4f6255; border:none; line-height:168%;");
        vl->addWidget(bodyLbl);
    }

    if (!item.url.isEmpty() || !item.url2.isEmpty()) {
        QWidget *buttonRow = new QWidget();
        buttonRow->setStyleSheet("background:transparent; border:none;");
        QHBoxLayout *buttonLayout = new QHBoxLayout(buttonRow);
        buttonLayout->setContentsMargins(0, 2, 0, 0);
        buttonLayout->setSpacing(8);

        auto addLinkButton = [&](const QString &url, const QString &label) {
            if (url.isEmpty())
                return;

            QPushButton *linkBtn = new QPushButton(label.isEmpty() ? "Visit →" : label);
            const QString targetUrl = url;
            QObject::connect(linkBtn, &QPushButton::clicked, linkBtn, [targetUrl]() {
                QDesktopServices::openUrl(QUrl(targetUrl));
            });
            linkBtn->setStyleSheet(
                "QPushButton { font-size:12px; font-weight:800; color:#2a4434; "
                "background:qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                "stop:0 #e3efdc, stop:0.58 #d4e6cd, stop:1 #f7efdc); "
                "border:1px solid #c7d8c1; border-radius:11px; padding:6px 14px; margin-top:3px; }"
                "QPushButton:hover { background:#e8f1e4; color:#234030; border-color:#a8bf9d; }");
            linkBtn->setCursor(Qt::PointingHandCursor);
            buttonLayout->addWidget(linkBtn);
        };

        addLinkButton(item.url, item.urlLabel);
        addLinkButton(item.url2, item.url2Label);
        buttonLayout->addStretch();
        vl->addWidget(buttonRow);
    }

    hl->addWidget(iconLbl, 0, Qt::AlignTop);
    hl->addWidget(textBlock, 1);
    return row;
}

static QFrame* makeDivider() {
    QFrame *f = new QFrame();
    f->setFrameShape(QFrame::HLine);
    f->setStyleSheet("background:#dfece1; border:none;");
    f->setFixedHeight(1);
    return f;
}

static QWidget* makeFolderCard(const FolderDef &fd,
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
    applyToolkitShadow(card);

    QHBoxLayout *hl = new QHBoxLayout(card);
    hl->setContentsMargins(24, 22, 24, 22);
    hl->setSpacing(0);
    hl->setAlignment(Qt::AlignCenter);

    QLabel *iconLbl = new QLabel();
    iconLbl->setFixedSize(60, 60);
    setLucideIcon(iconLbl, fd.icon, QColor("#173c2c"), 34);
    iconLbl->setStyleSheet("background:transparent; border:none;");
    iconLbl->setAttribute(Qt::WA_TransparentForMouseEvents);

    QWidget *textBlock = new QWidget();
    textBlock->setStyleSheet("border:none; background:transparent;");
    textBlock->setAttribute(Qt::WA_TransparentForMouseEvents);
    QVBoxLayout *vl = new QVBoxLayout(textBlock);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(9);
    vl->setAlignment(Qt::AlignCenter);

    QLabel *titleLbl = new QLabel(fd.name);
    titleLbl->setWordWrap(true);
    titleLbl->setAlignment(Qt::AlignCenter);
    titleLbl->setStyleSheet(
        "font-size:16px; font-weight:800; color:#1a1a1a; border:none; letter-spacing:-0.1px;");

    QLabel *subLbl = new QLabel(fd.tagline);
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

    QObject::connect(card, &QPushButton::clicked, card, [onClick]() { onClick(); });
    return card;
}

static QWidget* makeFolderDetailPage(const FolderDef &fd, std::function<void()> onBack) {
    QWidget *page = new QWidget();
    page->setObjectName("screenSurface");
    QVBoxLayout *pl = new QVBoxLayout(page);
    pl->setContentsMargins(48, 28, 48, 40);
    pl->setSpacing(18);

    QWidget *topRow = new QWidget();
    topRow->setStyleSheet("border:none; background:transparent;");
    QHBoxLayout *trl = new QHBoxLayout(topRow);
    trl->setContentsMargins(0, 0, 0, 0);
    trl->setSpacing(12);

    QPushButton *backBtn = new QPushButton("←  Back");
    backBtn->setObjectName("detailFolderBack");
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet(
        "QPushButton { font-size:13px; font-weight:700; color:#1a1a1a; "
        "background:#ffffff; border:1px solid #d7ddd2; border-radius:14px; padding:10px 16px; }"
        "QPushButton:hover { background:#f7f7f4; border-color:#c4d1c0; }");
    QObject::connect(backBtn, &QPushButton::clicked, backBtn, [onBack]() { onBack(); });

    QLabel *iconLbl = new QLabel();
    iconLbl->setFixedSize(42, 42);
    setLucideIcon(iconLbl, fd.icon, QColor("#173c2c"), 25);
    iconLbl->setStyleSheet(
        QString("background:%1; border-radius:12px; border:none;")
            .arg(fd.accent));

    QWidget *titleBlock = new QWidget();
    titleBlock->setStyleSheet("border:none; background:transparent;");
    QVBoxLayout *titleLay = new QVBoxLayout(titleBlock);
    titleLay->setContentsMargins(0, 0, 0, 0);
    titleLay->setSpacing(2);

    QLabel *eyebrow = new QLabel("MENTAL HEALTH TOOLKIT");
    eyebrow->setObjectName("detailFolderEyebrow");
    eyebrow->setStyleSheet(
        "font-size:12px; font-weight:700; color:#5e6d85; letter-spacing:1.8px;");

    QLabel *titleLbl = new QLabel(fd.name);
    titleLbl->setObjectName("detailFolderTitle");
    titleLbl->setStyleSheet(
        "font-size:28px; font-weight:800; color:#111111; letter-spacing:-0.5px;");

    QLabel *tagLbl = new QLabel(fd.tagline);
    tagLbl->setObjectName("detailFolderTagline");
    tagLbl->setStyleSheet("font-size:14px; color:#4c5e78; border:none;");

    titleLay->addWidget(eyebrow);
    titleLay->addWidget(titleLbl);
    titleLay->addWidget(tagLbl);

    trl->addWidget(backBtn, 0, Qt::AlignTop);
    trl->addSpacing(4);
    trl->addWidget(iconLbl, 0, Qt::AlignTop);
    trl->addWidget(titleBlock, 1);
    pl->addWidget(topRow);

    QScrollArea *scroll = new QScrollArea();
    scroll->setObjectName("screenSurface");
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->viewport()->setObjectName("screenViewport");

    QWidget *inner = new QWidget();
    inner->setObjectName("screenSurface");
    QVBoxLayout *il = new QVBoxLayout(inner);
    il->setContentsMargins(2, 8, 10, 10);
    il->setSpacing(14);

    QFrame *introCard = new QFrame();
    introCard->setStyleSheet(
        QString("QFrame { background:rgba(255, 255, 255, 0.96); border:1px solid %1; border-radius:18px; }")
            .arg(fd.accentBorder));
    applyToolkitShadow(introCard, 16, 4, QColor(121, 154, 112, 10));
    QVBoxLayout *introLay = new QVBoxLayout(introCard);
    introLay->setContentsMargins(20, 18, 20, 18);
    introLay->setSpacing(8);

    QLabel *introLbl = new QLabel(fd.intro);
    introLbl->setWordWrap(true);
    introLbl->setStyleSheet(
        "font-size:14px; color:#4f6255; border:none; line-height:170%;");
    introLay->addWidget(introLbl);
    il->addWidget(introCard);

    QFrame *listCard = new QFrame();
    listCard->setStyleSheet(
        "QFrame { background:rgba(255, 255, 255, 0.96); border:1px solid #c7d8c1; border-radius:22px; }");
    applyToolkitShadow(listCard, 18, 4, QColor(121, 154, 112, 10));
    QVBoxLayout *listLay = new QVBoxLayout(listCard);
    listLay->setContentsMargins(22, 16, 22, 18);
    listLay->setSpacing(0);

    for (int i = 0; i < fd.items.size(); ++i) {
        if (i > 0)
            listLay->addWidget(makeDivider());
        listLay->addWidget(makeToolItem(fd.items[i]));
    }

    il->addWidget(listCard);
    il->addStretch();

    scroll->setWidget(inner);
    pl->addWidget(scroll, 1);

    return page;
}

Toolkit::Toolkit(QWidget *parent) : Screen("Mental Health Toolkit", parent) {
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    m_stack = new QStackedWidget();
    m_stack->setObjectName("screenSurface");

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

    m_greeting = new QLabel("WELCOME");
    m_greeting->setStyleSheet(
        "font-size:12px; font-weight:700; color:#5e6d85; letter-spacing:1.8px;");

    m_heading = new QLabel("A quiet space to pause, reflect, and reset");
    m_heading->setStyleSheet(
        "font-size:40px; font-weight:800; color:#111111; letter-spacing:-0.8px;");

    m_sub = new QLabel(
        "Choose a toolkit topic for calming tools, reflection prompts, and supportive practices you can try at your own pace.");
    m_sub->setWordWrap(false);
    m_sub->setMaximumWidth(900);
    m_sub->setStyleSheet("font-size:15px; color:#4c5e78; border:none;");

    QWidget *headerWrap = new QWidget();
    headerWrap->setStyleSheet("border:none; background:transparent;");
    QVBoxLayout *headerLay = new QVBoxLayout(headerWrap);
    headerLay->setContentsMargins(0, 0, 0, 0);
    headerLay->setSpacing(10);
    headerLay->addWidget(m_greeting, 0, Qt::AlignLeft);
    headerLay->addWidget(m_heading,  0, Qt::AlignLeft);
    headerLay->addWidget(m_sub,      0, Qt::AlignLeft);

    innerLay->addWidget(headerWrap);
    innerLay->addSpacing(18);

    QFrame *searchCard = new QFrame();
    searchCard->setStyleSheet("QFrame { background:transparent; border:none; }");
    QVBoxLayout *searchOuter = new QVBoxLayout(searchCard);
    searchOuter->setContentsMargins(0, 0, 0, 0);
    searchOuter->setSpacing(8);

    QWidget *searchRow = new QWidget();
    searchRow->setStyleSheet("border:none; background:transparent;");
    QHBoxLayout *searchLay = new QHBoxLayout(searchRow);
    searchLay->setContentsMargins(0, 0, 0, 0);
    searchLay->setSpacing(10);
    searchLay->setAlignment(Qt::AlignLeft);

    m_searchIcon = new QLabel();
    m_searchIcon->setAlignment(Qt::AlignCenter);
    m_searchIcon->setFixedSize(38, 38);
    setLucideIcon(m_searchIcon, "search", QColor("#4c5e78"), 18);
    m_searchIcon->setStyleSheet(
        "font-size:16px; color:#4c5e78; background:#ffffff; border:1px solid #d7ddd2; border-radius:13px;");

    m_searchInput = new QLineEdit();
    m_searchInput->setPlaceholderText(
        "Search toolkit topics or subtopics, e.g. sleep, journaling, music, focus");
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

    m_searchBtn = new QPushButton("Search");
    m_searchBtn->setMinimumHeight(42);
    m_searchBtn->setCursor(Qt::PointingHandCursor);
    m_searchBtn->setStyleSheet(
        "QPushButton { font-size:13px; font-weight:700; color:#1a1a1a; "
        "background:#ffffff; border:1px solid #d7ddd2; border-radius:14px; padding:10px 18px; }"
        "QPushButton:hover { background:#f7f7f4; border-color:#c4d1c0; }");

    searchLay->addWidget(m_searchIcon);
    searchLay->addWidget(m_searchInput);
    searchLay->addWidget(m_searchBtn);
    searchLay->addStretch();

    m_searchStatus = new QLabel();
    m_searchStatus->setWordWrap(true);
    m_searchStatus->setVisible(false);

    searchOuter->addWidget(searchRow);
    searchOuter->addWidget(m_searchStatus);
    innerLay->addWidget(searchCard);
    innerLay->addSpacing(18);

    QWidget *folderList = new QWidget();
    folderList->setStyleSheet("border:none; background:transparent;");
    QGridLayout *folderListLay = new QGridLayout(folderList);
    folderListLay->setContentsMargins(0, 8, 0, 0);
    folderListLay->setHorizontalSpacing(28);
    folderListLay->setVerticalSpacing(28);

    for (int i = 0; i < FOLDERS.size(); ++i) {
        int idx = i;
        QLabel *subtitleLabel = nullptr;
        QWidget *card = makeFolderCard(
            FOLDERS[i],
            [this, idx]() { showFolder(idx); },
            &subtitleLabel);
        m_folderCards.append(card);
        m_folderSubtitleLabels.append(subtitleLabel);
        folderListLay->addWidget(card, i / 3, i % 3, Qt::AlignCenter);
    }
    for (int col = 0; col < 3; ++col)
        folderListLay->setColumnStretch(col, 1);

    innerLay->addWidget(folderList, 0, Qt::AlignHCenter);
    innerLay->addStretch();

    QHBoxLayout *centerLay = new QHBoxLayout();
    centerLay->setContentsMargins(0, 0, 0, 0);
    centerLay->addStretch();
    centerLay->addWidget(homeInner);
    centerLay->addStretch();
    hl->addLayout(centerLay);
    hl->addStretch();

    homeScroll->setWidget(homePage);
    m_stack->addWidget(homeScroll);

    connect(m_searchInput, &QLineEdit::textChanged,
            this, &Toolkit::applyFolderSearch);
    connect(m_searchInput, &QLineEdit::returnPressed,
            this, &Toolkit::openBestFolderMatch);
    connect(m_searchBtn, &QPushButton::clicked,
            this, &Toolkit::openBestFolderMatch);

    for (int i = 0; i < FOLDERS.size(); ++i) {
        m_stack->addWidget(makeFolderDetailPage(FOLDERS[i], [this]() { showHome(); }));
    }

    applyFolderSearch(QString{});
    root->addWidget(m_stack);
}

void Toolkit::onActivated() {
    if (m_searchInput) {
        m_searchInput->clear();
        applyFolderSearch(QString{});
    }
    showHome();
}

void Toolkit::applyFolderSearch(const QString &query) {
    const QString normalized = normalizedText(query);

    if (normalized.isEmpty()) {
        for (QWidget *card : m_folderCards)
            card->setVisible(true);

        for (int i = 0; i < m_folderSubtitleLabels.size() && i < FOLDERS.size(); ++i) {
            m_folderSubtitleLabels[i]->setText(FOLDERS[i].tagline);
            m_folderSubtitleLabels[i]->setStyleSheet(
                "font-size:11px; color:#6d8272; border:none;");
            m_folderSubtitleLabels[i]->setVisible(true);
        }

        m_searchStatus->clear();
        m_searchStatus->setVisible(false);
        return;
    }

    int visibleCount = 0;
    int bestIndex = -1;
    int bestScore = -1;

    for (int i = 0; i < m_folderCards.size(); ++i) {
        const int score = folderSearchScore(FOLDERS[i], normalized);
        const bool matches = score >= 0;
        m_folderCards[i]->setVisible(matches);

        if (i < m_folderSubtitleLabels.size()) {
            m_folderSubtitleLabels[i]->setText(FOLDERS[i].tagline);
            m_folderSubtitleLabels[i]->setStyleSheet(
                "font-size:11px; color:#6d8272; border:none;");
            m_folderSubtitleLabels[i]->setVisible(matches);
        }

        if (matches) {
            visibleCount++;
            if (score > bestScore) {
                bestScore = score;
                bestIndex = i;
            }
        }
    }

    if (visibleCount == 0) {
        m_searchStatus->setText(
            "No toolkit topic matched that search. Try words like sleep, music, journaling, focus, or meditation.");
        m_searchStatus->setStyleSheet(
            "font-size:11px; color:#C62828; border:none; background:transparent;");
        m_searchStatus->setVisible(true);
        return;
    }

    if (visibleCount == 1) {
        m_searchStatus->setText("1 matching toolkit topic found. Press Enter to open it.");
    } else {
        m_searchStatus->setText(
            QString("%1 matching toolkit topics found. Press Enter to open the best match.")
                .arg(visibleCount));
    }

    if (bestIndex >= 0) {
        m_searchStatus->setStyleSheet(
            "font-size:12px; color:#4c5e78; border:none; background:transparent;");
        m_searchStatus->setVisible(true);
    }
}

void Toolkit::openBestFolderMatch() {
    const QString normalized = normalizedText(m_searchInput ? m_searchInput->text() : QString{});
    if (normalized.isEmpty())
        return;

    int bestIndex = -1;
    int bestScore = -1;
    for (int i = 0; i < FOLDERS.size(); ++i) {
        const int score = folderSearchScore(FOLDERS[i], normalized);
        if (score > bestScore) {
            bestScore = score;
            bestIndex = i;
        }
    }

    if (bestIndex >= 0)
        showFolder(bestIndex);
}

void Toolkit::showFolder(int index) {
    if (index < 0 || index >= FOLDERS.size())
        return;
    m_stack->setCurrentIndex(index + 1);
}

void Toolkit::showHome() {
    if (m_stack)
        m_stack->setCurrentIndex(0);
}

