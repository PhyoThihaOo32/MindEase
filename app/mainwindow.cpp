#include "app/mainwindow.h"
#include "core/fallingleafoverlay.h"
#include "core/screen.h"
#include "screens/recommendations.h"
#include "screens/toolkit.h"
#include "screens/assistantchat.h"
#include "screens/journal.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      leafOverlay(nullptr)
{
    setWindowTitle("MindEase — BMCC Wellness Companion");
    setMinimumSize(1120, 760);
    resize(1500, 960);
    setWindowState(Qt::WindowMaximized);

    applyStyle();

    centralWidget = new QWidget(this);
    centralWidget->setObjectName("appShell");
    setCentralWidget(centralWidget);

    mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Build screens (polymorphic — stored as Screen*)
    screens.append(new Recommendations(this));
    screens.append(new Toolkit(this));
    screens.append(new AssistantChat(this));
    screens.append(new Journal(this));

    // Stack
    stack = new QStackedWidget(this);
    stack->setObjectName("screenStack");
    for (Screen *s : screens)
        stack->addWidget(s);

    buildSidebar();

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stack);
    mainLayout->setStretch(1, 1);

    leafOverlay = new FallingLeafOverlay(centralWidget);
    leafOverlay->setGeometry(centralWidget->rect());
    leafOverlay->raise();

    switchScreen(0);
}

MainWindow::~MainWindow() {}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    if (leafOverlay && centralWidget) {
        leafOverlay->setGeometry(centralWidget->rect());
        leafOverlay->raise();
    }
}

void MainWindow::buildSidebar() {
    sidebar = new QWidget(this);
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(288);

    QVBoxLayout *sl = new QVBoxLayout(sidebar);
    sl->setContentsMargins(0, 0, 0, 0);
    sl->setSpacing(0);

    // ── Logo block ───────────────────────────────────────────────────────────
    QWidget *logo = new QWidget();
    logo->setObjectName("logoBox");
    QVBoxLayout *ll = new QVBoxLayout(logo);
    ll->setContentsMargins(26, 26, 26, 22);
    ll->setSpacing(2);

    QLabel *appName = new QLabel("MindEase");
    appName->setObjectName("appName");
    QLabel *appSub  = new QLabel("BMCC Wellness Companion");
    appSub->setObjectName("appSub");
    ll->addWidget(appName);
    ll->addWidget(appSub);
    sl->addWidget(logo);

    // Small section label above nav
    QLabel *navHeader = new QLabel("MENU");
    navHeader->setObjectName("navHeader");
    navHeader->setContentsMargins(26, 18, 26, 8);
    sl->addWidget(navHeader);

    // ── Nav items (index matches stack order) ────────────────────────────────
    struct NavDef { QString icon; QString label; };
    const QList<NavDef> navDefs = {
        { "✦", "BMCC Resources"         },
        { "◆", "Mental Health Toolkit"  },
        { "☼", "MindEase Assistant"      },
        { "✎", "My Journal"             },
    };

    for (int i = 0; i < navDefs.size(); i++) {
        QPushButton *btn = new QPushButton(
            QString("  %1   %2").arg(navDefs[i].icon, navDefs[i].label));
        btn->setObjectName("navBtn");
        btn->setCheckable(true);
        btn->setChecked(i == 0);
        btn->setCursor(Qt::PointingHandCursor);
        connect(btn, &QPushButton::clicked, this, [this, i]() { switchScreen(i); });
        navButtons.append(btn);
        sl->addWidget(btn);
    }

    sl->addStretch();

    // ── Footer block ─────────────────────────────────────────────────────────
    QFrame *footerDiv = new QFrame();
    footerDiv->setObjectName("footerDiv");
    footerDiv->setFrameShape(QFrame::HLine);
    sl->addWidget(footerDiv);

    QWidget *footer = new QWidget();
    QVBoxLayout *fl = new QVBoxLayout(footer);
    fl->setContentsMargins(26, 16, 26, 20);
    fl->setSpacing(2);

    QLabel *footerTitle = new QLabel("Honors Project");
    footerTitle->setObjectName("footerTitle");
    QLabel *ver = new QLabel("v1.0  ·  CSC211H  ·  Qt 6");
    ver->setObjectName("verLabel");
    QLabel *footNote = new QLabel("Private, local-first wellness support");
    footNote->setObjectName("footerNote");

    fl->addWidget(footerTitle);
    fl->addWidget(ver);
    fl->addWidget(footNote);
    sl->addWidget(footer);
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

        /* ── Sidebar ──────────────────────────────────────────────────── */
        #sidebar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 rgba(247, 241, 227, 0.96),
                                        stop:0.56 rgba(239, 238, 225, 0.94),
                                        stop:1 rgba(228, 235, 220, 0.96));
            border-right: 1px solid rgba(118, 148, 112, 0.24);
        }
        #logoBox {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 rgba(247, 241, 227, 0.98),
                                        stop:0.62 rgba(235, 241, 228, 0.96),
                                        stop:1 rgba(220, 231, 214, 0.96));
            border-bottom: 1px solid rgba(118, 148, 112, 0.24);
        }
        #appName {
            font-size: 30px;
            font-weight: 800;
            color: #2b4a39;
            letter-spacing: -0.8px;
        }
        #appSub {
            font-size: 13px;
            color: #6c8170;
            margin-top: 3px;
            letter-spacing: 0.35px;
        }
        #navHeader {
            font-size: 10px;
            font-weight: 700;
            color: #7f9577;
            letter-spacing: 1.8px;
        }

        /* ── Unified nav buttons ─────────────────────────────────────── */
        #navBtn {
            text-align: left;
            padding: 18px 22px;
            border: 1px solid rgba(146, 171, 139, 0.06);
            border-left: 4px solid transparent;
            background: rgba(255, 255, 255, 0.28);
            font-size: 15px;
            color: #385442;
            border-radius: 0px 22px 22px 0px;
            margin: 6px 16px 6px 0;
        }
        #navBtn:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                        stop:0 rgba(233, 240, 227, 0.96), stop:1 rgba(221, 233, 216, 0.96));
            color: #284635;
            border: 1px solid rgba(133, 166, 125, 0.22);
        }
        #navBtn:checked {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                        stop:0 rgba(233, 240, 227, 0.98), stop:1 rgba(216, 229, 209, 0.98));
            color: #1f3a2c;
            border-left: 4px solid #6e9770;
            border-top: 1px solid rgba(122, 151, 116, 0.24);
            border-right: 1px solid rgba(122, 151, 116, 0.24);
            border-bottom: 1px solid rgba(122, 151, 116, 0.24);
            font-weight: 800;
        }
        #navBtn:focus {
            background: rgba(232, 239, 226, 0.92);
            color: #274332;
        }

        /* ── Sidebar footer ──────────────────────────────────────────── */
        #footerDiv {
            color: rgba(118, 148, 112, 0.22);
            background: rgba(118, 148, 112, 0.22);
            max-height: 1px;
            border: none;
        }
        #footerTitle {
            font-size: 11px;
            font-weight: 700;
            color: #405846;
            letter-spacing: 0.3px;
        }
        #verLabel { font-size: 10px; color: #738573; letter-spacing: 0.25px; }
        #footerNote { font-size: 10px; color: #6d8476; }

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
