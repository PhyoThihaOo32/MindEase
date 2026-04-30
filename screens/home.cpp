#include "screens/home.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

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
    setStyleSheet(R"(
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
    )");

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
    auto *assistantBtn = makeHomeAction("Assistant");
    auto *journalBtn = makeHomeAction("Journal");

    actionsRow->addWidget(resourcesBtn);
    actionsRow->addWidget(toolkitBtn);
    actionsRow->addWidget(assistantBtn);
    actionsRow->addWidget(journalBtn);

    contentLayout->addWidget(eyebrow);
    contentLayout->addWidget(title);
    contentLayout->addWidget(body);
    contentLayout->addSpacing(8);
    contentLayout->addLayout(actionsRow);

    contentRow->addWidget(content);
    contentRow->addStretch();

    root->addLayout(contentRow);
    root->addStretch();

    connect(resourcesBtn, &QPushButton::clicked, this, [this]() { emit requestScreen("resources"); });
    connect(toolkitBtn, &QPushButton::clicked, this, [this]() { emit requestScreen("toolkit"); });
    connect(assistantBtn, &QPushButton::clicked, this, [this]() { emit requestScreen("assistant"); });
    connect(journalBtn, &QPushButton::clicked, this, [this]() { emit requestScreen("journal"); });
}
