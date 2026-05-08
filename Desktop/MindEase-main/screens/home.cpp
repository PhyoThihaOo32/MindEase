#include "screens/home.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

// ─────────────────────────────────────────────────────────────────────────────
// Shared stylesheet factory — returns the correct palette for dark / light mode
// ─────────────────────────────────────────────────────────────────────────────
namespace {
QString homeStylesheet(bool dark) {
    if (dark) {
        return R"(
            QLabel#homeEyebrow {
                color: #4a7a43;
                font-size: 13px;
                font-weight: 600;
                letter-spacing: 2.3px;
                background: transparent;
                border: none;
            }
            QLabel#homeTitle {
                color: #C8ECC2;
                font-size: 52px;
                font-weight: 800;
                background: transparent;
                border: none;
            }
            QLabel#homeBody {
                color: #6a9c6a;
                font-size: 18px;
                font-weight: 500;
                background: transparent;
                border: none;
            }
            QLabel#homeCredit {
                color: #4a7a43;
                font-size: 12px;
                font-weight: 600;
                letter-spacing: 0.8px;
                background: transparent;
                border: none;
            }
            QPushButton#homeActionBtn {
                font-size: 15px;
                font-weight: 700;
                color: #C8ECC2;
                background: rgba(20, 40, 22, 0.88);
                border: 1px solid rgba(74, 122, 67, 0.45);
                border-radius: 24px;
                padding: 14px 24px;
            }
            QPushButton#homeActionBtn:hover {
                background: rgba(30, 61, 32, 0.95);
                color: #A8D8A0;
            }
            QPushButton#homeActionBtn:pressed {
                background: rgba(15, 30, 16, 0.98);
            }
        )";
    }
    return R"(
        QLabel#homeEyebrow {
            color: #5e6d85;
            font-size: 13px;
            font-weight: 600;
            letter-spacing: 2.3px;
            background: transparent;
            border: none;
        }
        QLabel#homeTitle {
            color: #111111;
            font-size: 52px;
            font-weight: 800;
            background: transparent;
            border: none;
        }
        QLabel#homeBody {
            color: #4c5e78;
            font-size: 18px;
            font-weight: 500;
            background: transparent;
            border: none;
        }
        QLabel#homeCredit {
            color: #7a8fa0;
            font-size: 12px;
            font-weight: 600;
            letter-spacing: 0.8px;
            background: transparent;
            border: none;
        }
        QPushButton#homeActionBtn {
            font-size: 15px;
            font-weight: 700;
            color: #171717;
            background: rgba(255, 255, 255, 0.88);
            border: none;
            border-radius: 24px;
            padding: 14px 24px;
        }
        QPushButton#homeActionBtn:hover {
            background: rgba(255, 255, 255, 0.96);
        }
        QPushButton#homeActionBtn:pressed {
            background: rgba(245, 245, 245, 0.96);
        }
    )";
}
} // namespace

namespace {
QPushButton *makeHomeAction(const QString &text) {
    auto *button = new QPushButton(text);
    button->setObjectName("homeActionBtn");
    button->setCursor(Qt::PointingHandCursor);
    button->setMinimumHeight(50);
    return button;
}
}

Home::Home(QWidget *parent)
    : Screen("Home", parent) {
    setStyleSheet(homeStylesheet(false));   // light theme default

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(72, 30, 72, 72);
    root->setSpacing(0);
    root->addStretch();

    auto *contentRow = new QHBoxLayout();
    contentRow->setContentsMargins(0, 0, 0, 0);
    contentRow->addStretch();

    auto *content = new QWidget();
    content->setObjectName("homeContent");
    content->setStyleSheet("background: transparent; border: none;");
    content->setMaximumWidth(1160);

    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(28);
    contentLayout->setAlignment(Qt::AlignHCenter);

    auto *eyebrow = new QLabel("MINDEASE");
    eyebrow->setObjectName("homeEyebrow");
    eyebrow->setAlignment(Qt::AlignHCenter);

    auto *title = new QLabel("A calm wellness companion for BMCC\nstudents");
    title->setObjectName("homeTitle");
    title->setAlignment(Qt::AlignHCenter);
    title->setWordWrap(true);

    auto *body = new QLabel(
        "MindEase helps you find campus resources, take care small self-care steps, engage calming\n"
        "tools, and write positive future plans.");
    body->setObjectName("homeBody");
    body->setAlignment(Qt::AlignHCenter);
    body->setWordWrap(true);

    auto *actionsRow = new QHBoxLayout();
    actionsRow->setContentsMargins(0, 0, 0, 0);
    actionsRow->setSpacing(18);
    actionsRow->setAlignment(Qt::AlignHCenter);

    auto *resourcesBtn = makeHomeAction("BMCC Resources");
    auto *toolkitBtn = makeHomeAction("Mental Health Toolkit");
    auto *journalBtn = makeHomeAction("Journal");

    // ── Course credit line ────────────────────────────────────────────────
    auto *credit = new QLabel(
        "CSC211H Honors Project  ·  Phyo Thiha Oo  ·  Instructor: Dr. Azhar");
    credit->setObjectName("homeCredit");
    credit->setAlignment(Qt::AlignHCenter);
    credit->setWordWrap(true);

    actionsRow->addWidget(resourcesBtn);
    actionsRow->addWidget(toolkitBtn);
    actionsRow->addWidget(journalBtn);

    contentLayout->addWidget(eyebrow);
    contentLayout->addWidget(title);
    contentLayout->addWidget(body);
    contentLayout->addSpacing(8);
    contentLayout->addLayout(actionsRow);
    contentLayout->addSpacing(18);
    contentLayout->addWidget(credit);

    contentRow->addWidget(content);
    contentRow->addStretch();

    root->addLayout(contentRow);
    root->addStretch();

    connect(resourcesBtn, &QPushButton::clicked, this, [this]() { emit requestScreen("resources"); });
    connect(toolkitBtn, &QPushButton::clicked, this, [this]() { emit requestScreen("toolkit"); });
    connect(journalBtn, &QPushButton::clicked, this, [this]() { emit requestScreen("journal"); });
}

