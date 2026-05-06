#include "screens/settings.h"
#include <QButtonGroup>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QSettings>
#include <QSizePolicy>
#include <QTimer>
#include <QVBoxLayout>

// ─────────────────────────────────────────────────────────────────────────────
// Inline style helpers
// ─────────────────────────────────────────────────────────────────────────────
namespace {

const QString kCardStyle =
    "QFrame {"
    "  background:#ffffff;"
    "  border:1px solid #dce8d9;"
    "  border-radius:16px;"
    "}";

const QString kSectionLabelStyle =
    "font-size:11px; font-weight:700; color:#5e6d85; letter-spacing:1.8px;";

const QString kRowLabelStyle =
    "font-size:15px; font-weight:700; color:#173c2c; border:none;";

const QString kDescStyle =
    "font-size:13px; color:#5a6f62; border:none;";

QString radioStyle(const QString &accent) {
    return QString(
        "QRadioButton {"
        "  font-size:14px; font-weight:600; color:#2d4833; padding:6px 0;"
        "}"
        "QRadioButton::indicator {"
        "  width:18px; height:18px; border-radius:9px;"
        "  border:2px solid #b5cfb0;"
        "}"
        "QRadioButton::indicator:checked {"
        "  background:%1; border:2px solid %1;"
        "}").arg(accent);
}

} // namespace

// ─────────────────────────────────────────────────────────────────────────────
// Constructor — builds the settings UI
// ─────────────────────────────────────────────────────────────────────────────
Settings::Settings(QWidget *parent)
    : Screen("Settings", parent) {

    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    QScrollArea *scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setObjectName("screenSurface");
    scroll->viewport()->setObjectName("screenViewport");

    QWidget *page = new QWidget();
    page->setObjectName("screenSurface");
    QVBoxLayout *pageLayout = new QVBoxLayout(page);
    pageLayout->setContentsMargins(72, 32, 72, 56);
    pageLayout->setSpacing(0);

    QWidget *pageInner = new QWidget();
    pageInner->setStyleSheet("background:transparent; border:none;");
    pageInner->setMaximumWidth(820);
    pageInner->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QVBoxLayout *inner = new QVBoxLayout(pageInner);
    inner->setContentsMargins(0, 0, 0, 0);
    inner->setSpacing(0);

    // ── Page header ───────────────────────────────────────────────────────────
    m_eyebrowLbl = new QLabel("PREFERENCES");
    m_eyebrowLbl->setStyleSheet(kSectionLabelStyle);

    m_titleLbl = new QLabel("Settings");
    m_titleLbl->setStyleSheet(
        "font-size:40px; font-weight:800; color:#111111; letter-spacing:-0.8px;");

    m_subLbl = new QLabel("Choose the MindEase visual theme.");
    m_subLbl->setStyleSheet("font-size:15px; color:#4c5e78; border:none;");

    inner->addWidget(m_eyebrowLbl, 0, Qt::AlignLeft);
    inner->addSpacing(10);
    inner->addWidget(m_titleLbl, 0, Qt::AlignLeft);
    inner->addSpacing(10);
    inner->addWidget(m_subLbl, 0, Qt::AlignLeft);
    inner->addSpacing(36);

    // ─────────────────────────────────────────────────────────────────────────
    // Card 1: Appearance
    // ─────────────────────────────────────────────────────────────────────────
    m_appearCard = new QFrame();
    m_appearCard->setStyleSheet(kCardStyle);
    QVBoxLayout *appearLayout = new QVBoxLayout(m_appearCard);
    appearLayout->setContentsMargins(36, 30, 36, 30);
    appearLayout->setSpacing(22);

    m_appearLabel = new QLabel("APPEARANCE");
    m_appearLabel->setStyleSheet(kSectionLabelStyle);
    appearLayout->addWidget(m_appearLabel);

    // ── Theme ─────────────────────────────────────────────────────────────────
    QFrame *themeDiv = new QFrame();
    themeDiv->setFrameShape(QFrame::HLine);
    themeDiv->setStyleSheet("background:#eeefeb; border:none;");
    themeDiv->setFixedHeight(1);
    appearLayout->addWidget(themeDiv);

    m_themeLabel = new QLabel("Theme");
    m_themeLabel->setStyleSheet(kRowLabelStyle);
    m_themeDesc = new QLabel(
        "Switch between a warm natural palette and a dark Zen Night mode.");
    m_themeDesc->setWordWrap(true);
    m_themeDesc->setStyleSheet(kDescStyle);
    appearLayout->addWidget(m_themeLabel);
    appearLayout->addWidget(m_themeDesc);

    QWidget *themeRow = new QWidget();
    themeRow->setStyleSheet("background:transparent; border:none;");
    QHBoxLayout *themeLayout = new QHBoxLayout(themeRow);
    themeLayout->setContentsMargins(0, 0, 0, 0);
    themeLayout->setSpacing(24);

    auto *themeGroup = new QButtonGroup(this);

    m_lightRadio = new QRadioButton("Light (Natural)");
    m_lightRadio->setStyleSheet(radioStyle("#4a7a43"));
    m_lightRadio->setChecked(true);

    m_darkRadio = new QRadioButton("Zen Night");
    m_darkRadio->setStyleSheet(radioStyle("#6DBF5E"));

    themeGroup->addButton(m_lightRadio, 0);
    themeGroup->addButton(m_darkRadio,  1);

    themeLayout->addWidget(m_lightRadio);
    themeLayout->addWidget(m_darkRadio);
    themeLayout->addStretch();
    appearLayout->addWidget(themeRow);

    inner->addWidget(m_appearCard);
    inner->addSpacing(32);

    // ─────────────────────────────────────────────────────────────────────────
    // Apply button + status
    // ─────────────────────────────────────────────────────────────────────────
    QWidget *applyRow = new QWidget();
    applyRow->setStyleSheet("background:transparent; border:none;");
    QHBoxLayout *applyLayout = new QHBoxLayout(applyRow);
    applyLayout->setContentsMargins(0, 0, 0, 0);
    applyLayout->setSpacing(20);

    m_statusLbl = new QLabel();
    m_statusLbl->setStyleSheet("font-size:13px; color:#2E7D32; border:none;");

    QPushButton *applyBtn = new QPushButton("Apply Changes");
    applyBtn->setCursor(Qt::PointingHandCursor);
    applyBtn->setMinimumHeight(48);
    applyBtn->setMinimumWidth(180);
    applyBtn->setStyleSheet(
        "QPushButton {"
        "  background:qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "    stop:0 #234030, stop:1 #3d6046);"
        "  color:#fffdf5; border:1px solid #2f523b; border-radius:12px;"
        "  font-size:15px; font-weight:700; padding:12px 28px;"
        "}"
        "QPushButton:hover { background:#2d4d39; }");
    connect(applyBtn, &QPushButton::clicked, this, &Settings::onApply);

    applyLayout->addWidget(m_statusLbl, 1);
    applyLayout->addWidget(applyBtn, 0, Qt::AlignRight);

    inner->addWidget(applyRow);
    inner->addStretch();

    pageLayout->addWidget(pageInner);
    pageLayout->addStretch();

    scroll->setWidget(page);
    root->addWidget(scroll);

    loadFromSettings();
}

// ─────────────────────────────────────────────────────────────────────────────
// Screen interface
// ─────────────────────────────────────────────────────────────────────────────
void Settings::onActivated() {
    loadFromSettings();
}

// ─────────────────────────────────────────────────────────────────────────────
// Feature 5: Theme — re-apply inline styles so Settings content stays readable
// ─────────────────────────────────────────────────────────────────────────────
void Settings::onThemeChanged(bool dark) {
    m_dark = dark;

    // ── Dark palette helpers ──────────────────────────────────────────────────
    const QString cardSS = dark
        ? "QFrame {"
          "  background:qlineargradient(x1:0, y1:0, x2:1, y2:1,"
          "    stop:0 #111E12, stop:1 #0F1A10);"
          "  border:1px solid rgba(74,122,67,0.32);"
          "  border-radius:16px;"
          "}"
        : "QFrame {"
          "  background:#ffffff;"
          "  border:1px solid #dce8d9;"
          "  border-radius:16px;"
          "}";

    // ── Cards ─────────────────────────────────────────────────────────────────
    m_appearCard->setStyleSheet(cardSS);

    // ── Page header ───────────────────────────────────────────────────────────
    m_eyebrowLbl->setStyleSheet(dark
        ? "font-size:11px; font-weight:700; color:#4a7a43; letter-spacing:1.8px;"
        : "font-size:11px; font-weight:700; color:#5e6d85; letter-spacing:1.8px;");
    m_titleLbl->setStyleSheet(dark
        ? "font-size:40px; font-weight:800; color:#C8ECC2; letter-spacing:-0.8px;"
        : "font-size:40px; font-weight:800; color:#111111; letter-spacing:-0.8px;");
    m_subLbl->setStyleSheet(dark
        ? "font-size:15px; color:#6a9c6a; border:none;"
        : "font-size:15px; color:#4c5e78; border:none;");

    // ── Appearance card labels ────────────────────────────────────────────────
    const QString sectionSS = dark
        ? "font-size:11px; font-weight:700; color:#4a7a43; letter-spacing:1.8px;"
        : "font-size:11px; font-weight:700; color:#5e6d85; letter-spacing:1.8px;";
    const QString rowSS = dark
        ? "font-size:15px; font-weight:700; color:#C8ECC2; border:none;"
        : "font-size:15px; font-weight:700; color:#173c2c; border:none;";
    const QString descSS = dark
        ? "font-size:13px; color:#6a9c6a; border:none;"
        : "font-size:13px; color:#5a6f62; border:none;";

    m_appearLabel->setStyleSheet(sectionSS);
    m_themeLabel->setStyleSheet(rowSS);
    m_themeDesc->setStyleSheet(descSS);

    m_lightRadio->setStyleSheet(dark
        ? "QRadioButton { font-size:14px; font-weight:600; color:#8AAD85; padding:6px 0; }"
          "QRadioButton::indicator { width:18px; height:18px; border-radius:9px;"
          "  border:2px solid rgba(74,122,67,0.5); }"
          "QRadioButton::indicator:checked { background:#4a7a43; border:2px solid #4a7a43; }"
        : "QRadioButton { font-size:14px; font-weight:600; color:#2d4833; padding:6px 0; }"
          "QRadioButton::indicator { width:18px; height:18px; border-radius:9px;"
          "  border:2px solid #b5cfb0; }"
          "QRadioButton::indicator:checked { background:#4a7a43; border:2px solid #4a7a43; }");
    m_darkRadio->setStyleSheet(dark
        ? "QRadioButton { font-size:14px; font-weight:600; color:#8AAD85; padding:6px 0; }"
          "QRadioButton::indicator { width:18px; height:18px; border-radius:9px;"
          "  border:2px solid rgba(74,122,67,0.5); }"
          "QRadioButton::indicator:checked { background:#6DBF5E; border:2px solid #6DBF5E; }"
        : "QRadioButton { font-size:14px; font-weight:600; color:#2d4833; padding:6px 0; }"
          "QRadioButton::indicator { width:18px; height:18px; border-radius:9px;"
          "  border:2px solid #b5cfb0; }"
          "QRadioButton::indicator:checked { background:#6DBF5E; border:2px solid #6DBF5E; }");

    m_statusLbl->setStyleSheet(dark
        ? "font-size:13px; color:#6DBF5E; border:none;"
        : "font-size:13px; color:#2E7D32; border:none;");
}

// ─────────────────────────────────────────────────────────────────────────────
// Persistence
// ─────────────────────────────────────────────────────────────────────────────
void Settings::loadFromSettings() {
    QSettings cfg("YangonDevs", "MindEase");

    const bool dark = cfg.value("darkTheme", false).toBool();
    m_darkRadio->setChecked(dark);
    m_lightRadio->setChecked(!dark);
}

// ─────────────────────────────────────────────────────────────────────────────
// Slots
// ─────────────────────────────────────────────────────────────────────────────
void Settings::onApply() {
    QSettings cfg("YangonDevs", "MindEase");

    // ── Theme ─────────────────────────────────────────────────────────────────
    const bool dark = m_darkRadio->isChecked();
    cfg.setValue("darkTheme", dark);
    cfg.remove("fontSize");
    cfg.remove("journalDir");

    cfg.sync();
    emit themeChanged(dark);
    showStatus("Theme applied");
}

void Settings::showStatus(const QString &msg, bool ok) {
    m_statusLbl->setText(msg);
    m_statusLbl->setStyleSheet(
        ok ? "font-size:13px; color:#2E7D32; border:none;"
           : "font-size:13px; color:#C62828; border:none;");
    QTimer::singleShot(4000, m_statusLbl, [this, msg]() {
        if (m_statusLbl->text() == msg) m_statusLbl->clear();
    });
}
