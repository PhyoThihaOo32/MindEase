#include "app/mainwindow.h"
#include "core/lucideicons.h"
#include "core/screen.h"
#include "screens/home.h"
#include "screens/recommendations.h"
#include "screens/toolkit.h"
#include "screens/journal.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("MindEase — BMCC Wellness Companion");
    setMinimumSize(1120, 760);
    resize(1500, 960);
    setWindowState(Qt::WindowMaximized);

    applyStyle();

    centralWidget = new QWidget(this);
    centralWidget->setObjectName("appShell");
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(24, 20, 24, 20);
    mainLayout->setSpacing(18);

    // Build screens (polymorphic — stored as Screen*)
    Home *home = new Home(this);
    screens.append(home);
    screens.append(new Recommendations(this));
    screens.append(new Toolkit(this));
    screens.append(new Journal(this));

    // Stack
    stack = new QStackedWidget(this);
    stack->setObjectName("screenStack");
    for (Screen *s : screens)
        stack->addWidget(s);

    buildNavigationBar();

    mainLayout->addWidget(navShell);
    mainLayout->addWidget(stack, 1);

    connect(home, &Home::requestScreen, this, [this](const QString &screenId) {
        for (int i = 0; i < screens.size(); ++i) {
            if (screens[i]->screenId() == screenId) {
                switchScreen(i);
                return;
            }
        }
    });

    switchScreen(0);
}

MainWindow::~MainWindow() {}

void MainWindow::buildNavigationBar() {
    navShell = new QWidget(this);
    navShell->setObjectName("navShell");

    QHBoxLayout *shellLayout = new QHBoxLayout(navShell);
    shellLayout->setContentsMargins(10, 6, 10, 6);
    shellLayout->setSpacing(24);

    QWidget *logo = new QWidget();
    logo->setObjectName("logoBox");
    QVBoxLayout *ll = new QVBoxLayout(logo);
    ll->setContentsMargins(0, 0, 0, 0);
    ll->setSpacing(0);

    QLabel *appName = new QLabel("MindEase");
    appName->setObjectName("appName");
    ll->addWidget(appName);

    QWidget *navRow = new QWidget();
    navRow->setObjectName("navRow");
    QHBoxLayout *navLayout = new QHBoxLayout(navRow);
    navLayout->setContentsMargins(0, 0, 0, 0);
    navLayout->setSpacing(24);

    struct NavDef { QString label; };
    const QList<NavDef> navDefs = {
        { "Home"                 },
        { "BMCC Resources"       },
        { "Mental Health Toolkit"},
        { "My Journal"           },
    };

    for (int i = 0; i < navDefs.size(); i++) {
        QPushButton *btn = new QPushButton(navDefs[i].label);
        btn->setObjectName("navBtn");
        btn->setCheckable(true);
        btn->setChecked(i == 0);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setMinimumHeight(40);
        connect(btn, &QPushButton::clicked, this, [this, i]() { switchScreen(i); });
        navButtons.append(btn);
        navLayout->addWidget(btn);
    }
    navLayout->addStretch();

    shellLayout->addWidget(logo, 0, Qt::AlignLeft | Qt::AlignTop);
    shellLayout->addSpacing(28);
    shellLayout->addWidget(navRow, 1);
}

void MainWindow::switchScreen(int index) {
    stack->setCurrentIndex(index);
    for (int i = 0; i < navButtons.size(); i++)
        navButtons[i]->setChecked(i == index);

    // Polymorphic dispatch — each Screen subclass decides what to refresh.
    if (index >= 0 && index < screens.size())
        screens[index]->onActivated();
}

void MainWindow::applyStyle() {
    setStyleSheet(R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #f8f1df, stop:0.46 #f5eedf, stop:0.78 #edf2e6, stop:1 #e2ebde);
        }
        #appShell {
            background: qradialgradient(cx:0.88, cy:0.24, radius:1.2,
                                        fx:0.88, fy:0.24,
                                        stop:0 rgba(205, 229, 201, 0.86),
                                        stop:0.26 rgba(226, 237, 220, 0.62),
                                        stop:0.58 rgba(245, 238, 223, 0.92),
                                        stop:1 rgba(249, 243, 229, 0.98));
        }

        #navShell {
            background: transparent;
            border: none;
        }
        #brandRow, #navRow {
            background: transparent;
            border: none;
        }
        #logoBox {
            background: transparent;
            border: none;
        }
        #appName {
            font-size: 26px;
            font-weight: 800;
            color: #111111;
            letter-spacing: -0.5px;
        }

        #navBtn {
            text-align: center;
            padding: 8px 8px;
            border: none;
            background: transparent;
            font-size: 15px;
            font-weight: 700;
            color: #171717;
            border-radius: 0px;
        }
        #navBtn:hover {
            color: #2f3f31;
        }
        #navBtn:checked {
            background: transparent;
            color: #111111;
            font-weight: 800;
        }
        #navBtn:focus {
            background: transparent;
            color: #111111;
        }

        #screenStack {
            background: transparent;
        }
        QWidget#screenRoot,
        QWidget#screenSurface,
        QWidget#screenViewport {
            background: transparent;
        }

        /* ── Shared screen typography ────────────────────────────────── */
        QLabel#screenTitle {
            font-size: 34px;
            font-weight: 800;
            color: #274334;
            letter-spacing: -0.8px;
        }
        QLabel#sectionLabel {
            font-size: 10px;
            font-weight: 700;
            color: #7d9476;
            letter-spacing: 1.7px;
        }

        /* ── Generic buttons ─────────────────────────────────────────── */
        QPushButton#primaryBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #e3efdc, stop:0.58 #d4e6cd, stop:1 #f7efdc);
            color: #284333;
            border: 1px solid rgba(144, 176, 134, 0.42);
            border-radius: 16px;
            padding: 13px 26px;
            font-size: 14px;
            font-weight: 800;
        }
        QPushButton#primaryBtn:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #eaf4e4, stop:0.54 #deecda, stop:1 #faf3e5);
        }
        QPushButton#primaryBtn:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #d4e3cc, stop:0.54 #cbddc5, stop:1 #ece2ca);
        }
        QPushButton#primaryBtn:disabled {
            background: #d8ddd2;
            color: #92a092;
        }

        QPushButton#outlineBtn {
            background: rgba(249, 244, 232, 0.86);
            color: #365143;
            border: 1px solid rgba(121, 152, 114, 0.34);
            border-radius: 16px;
            padding: 11px 20px;
            font-size: 14px;
            font-weight: 600;
        }
        QPushButton#outlineBtn:hover {
            background: rgba(241, 247, 236, 0.94);
            border-color: rgba(110, 151, 112, 0.42);
            color: #254133;
        }

        /* ── Scrollbars ──────────────────────────────────────────────── */
        QScrollArea {
            border: none;
            background: transparent;
        }
        QScrollBar:vertical {
            width: 10px;
            background: transparent;
            margin: 6px 2px 6px 0;
        }
        QScrollBar::handle:vertical {
            background: rgba(136, 167, 128, 0.72);
            border-radius: 5px;
            min-height: 30px;
        }
        QScrollBar::handle:vertical:hover { background: rgba(109, 144, 105, 0.92); }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0;
            background: transparent;
        }
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: transparent;
        }

        QSplitter::handle:horizontal {
            background: transparent;
            width: 10px;
        }
    )");
}
