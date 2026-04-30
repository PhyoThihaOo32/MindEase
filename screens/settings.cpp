#include "screens/settings.h"
#include <QButtonGroup>
#include <QFileDialog>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QSettings>
#include <QSizePolicy>
#include <QStandardPaths>
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

    m_subLbl = new QLabel("Personalise MindEase to suit your environment and workflow.");
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

    m_lightRadio = new QRadioButton("☀️   Light (Natural)");
    m_lightRadio->setStyleSheet(radioStyle("#4a7a43"));
    m_lightRadio->setChecked(true);

    m_darkRadio = new QRadioButton("🌙   Zen Night");
    m_darkRadio->setStyleSheet(radioStyle("#6DBF5E"));

    themeGroup->addButton(m_lightRadio, 0);
    themeGroup->addButton(m_darkRadio,  1);

    themeLayout->addWidget(m_lightRadio);
    themeLayout->addWidget(m_darkRadio);
    themeLayout->addStretch();
    appearLayout->addWidget(themeRow);

    // ── Font size ─────────────────────────────────────────────────────────────
    QFrame *fontDiv = new QFrame();
    fontDiv->setFrameShape(QFrame::HLine);
    fontDiv->setStyleSheet("background:#eeefeb; border:none;");
    fontDiv->setFixedHeight(1);
    appearLayout->addWidget(fontDiv);

    m_fontLabel = new QLabel("Base Font Size");
    m_fontLabel->setStyleSheet(kRowLabelStyle);
    m_fontDesc = new QLabel(
        "Adjusts the global font scale across the application. Default: 14 px.");
    m_fontDesc->setWordWrap(true);
    m_fontDesc->setStyleSheet(kDescStyle);
    appearLayout->addWidget(m_fontLabel);
    appearLayout->addWidget(m_fontDesc);

    QWidget *sliderRow = new QWidget();
    sliderRow->setStyleSheet("background:transparent; border:none;");
    QHBoxLayout *sliderLayout = new QHBoxLayout(sliderRow);
    sliderLayout->setContentsMargins(0, 0, 0, 0);
    sliderLayout->setSpacing(16);

    m_minLbl = new QLabel("12px");
    m_minLbl->setStyleSheet(kDescStyle);

    m_fontSlider = new QSlider(Qt::Horizontal);
    m_fontSlider->setRange(12, 20);
    m_fontSlider->setValue(14);
    m_fontSlider->setTickPosition(QSlider::TicksBelow);
    m_fontSlider->setTickInterval(2);
    m_fontSlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "  height:6px; background:#dce8d9; border-radius:3px;"
        "}"
        "QSlider::handle:horizontal {"
        "  width:20px; height:20px; margin:-7px 0;"
        "  background:#4a7a43; border-radius:10px;"
        "}"
        "QSlider::sub-page:horizontal {"
        "  background:#7aac6e; border-radius:3px;"
        "}");

    m_maxLbl = new QLabel("20px");
    m_maxLbl->setStyleSheet(kDescStyle);

    m_fontSizeLbl = new QLabel("14 px");
    m_fontSizeLbl->setStyleSheet(
        "font-size:16px; font-weight:700; color:#173c2c; min-width:50px; border:none;");
    m_fontSizeLbl->setAlignment(Qt::AlignCenter);

    connect(m_fontSlider, &QSlider::valueChanged, this, [this](int v) {
        m_fontSizeLbl->setText(QString("%1 px").arg(v));
    });

    sliderLayout->addWidget(m_minLbl);
    sliderLayout->addWidget(m_fontSlider, 1);
    sliderLayout->addWidget(m_maxLbl);
    sliderLayout->addSpacing(16);
    sliderLayout->addWidget(m_fontSizeLbl);
    appearLayout->addWidget(sliderRow);

    inner->addWidget(m_appearCard);
    inner->addSpacing(24);

    // ─────────────────────────────────────────────────────────────────────────
    // Card 2: Journal Storage
    // ─────────────────────────────────────────────────────────────────────────
    m_journalCard = new QFrame();
    m_journalCard->setStyleSheet(kCardStyle);
    QVBoxLayout *journalLayout = new QVBoxLayout(m_journalCard);
    journalLayout->setContentsMargins(36, 30, 36, 30);
    journalLayout->setSpacing(16);

    m_journalSec = new QLabel("JOURNAL STORAGE");
    m_journalSec->setStyleSheet(kSectionLabelStyle);
    journalLayout->addWidget(m_journalSec);

    QFrame *jDiv = new QFrame();
    jDiv->setFrameShape(QFrame::HLine);
    jDiv->setStyleSheet("background:#eeefeb; border:none;");
    jDiv->setFixedHeight(1);
    journalLayout->addWidget(jDiv);

    m_jLabel = new QLabel("Journal Directory");
    m_jLabel->setStyleSheet(kRowLabelStyle);
    m_jDesc = new QLabel(
        "Where MindEase saves your journal .txt files. "
        "Leave blank to use the default Documents/MindEase_Journal folder.");
    m_jDesc->setWordWrap(true);
    m_jDesc->setStyleSheet(kDescStyle);
    journalLayout->addWidget(m_jLabel);
    journalLayout->addWidget(m_jDesc);

    QWidget *dirRow = new QWidget();
    dirRow->setStyleSheet("background:transparent; border:none;");
    QHBoxLayout *dirLayout = new QHBoxLayout(dirRow);
    dirLayout->setContentsMargins(0, 0, 0, 0);
    dirLayout->setSpacing(10);

    m_dirEdit = new QLineEdit();
    m_dirEdit->setPlaceholderText("Default: Documents/MindEase_Journal");
    m_dirEdit->setStyleSheet(
        "QLineEdit {"
        "  border:1px solid #c7d8c1; border-radius:10px;"
        "  background:#f8fcf7; padding:10px 16px;"
        "  font-size:14px; color:#365143;"
        "}"
        "QLineEdit:focus { border-color:#7aac6e; }");

    QPushButton *browseBtn = new QPushButton("Browse...");
    browseBtn->setCursor(Qt::PointingHandCursor);
    browseBtn->setMinimumHeight(44);
    browseBtn->setStyleSheet(
        "QPushButton {"
        "  background:#edf3e6; border:1px solid #c3d2bf; border-radius:10px;"
        "  color:#264437; font-size:14px; font-weight:700; padding:8px 20px;"
        "}"
        "QPushButton:hover { background:#e0ecda; }");
    connect(browseBtn, &QPushButton::clicked, this, &Settings::onBrowseDir);

    QPushButton *resetBtn = new QPushButton("Reset to Default");
    resetBtn->setCursor(Qt::PointingHandCursor);
    resetBtn->setMinimumHeight(44);
    resetBtn->setStyleSheet(
        "QPushButton {"
        "  background:transparent; border:1px solid #dce8d9; border-radius:10px;"
        "  color:#71856f; font-size:13px; font-weight:600; padding:8px 16px;"
        "}"
        "QPushButton:hover { background:#f0f7ee; }");
    connect(resetBtn, &QPushButton::clicked, this, &Settings::onResetDir);

    dirLayout->addWidget(m_dirEdit, 1);
    dirLayout->addWidget(browseBtn);
    dirLayout->addWidget(resetBtn);
    journalLayout->addWidget(dirRow);

    inner->addWidget(m_journalCard);
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
    m_journalCard->setStyleSheet(cardSS);

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
    m_fontLabel->setStyleSheet(rowSS);
    m_fontDesc->setStyleSheet(descSS);
    m_minLbl->setStyleSheet(descSS);
    m_maxLbl->setStyleSheet(descSS);

    m_fontSizeLbl->setStyleSheet(dark
        ? "font-size:16px; font-weight:700; color:#C8ECC2; min-width:50px; border:none;"
        : "font-size:16px; font-weight:700; color:#173c2c; min-width:50px; border:none;");

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

    m_fontSlider->setStyleSheet(dark
        ? "QSlider::groove:horizontal { height:6px; background:#1A3018; border-radius:3px; }"
          "QSlider::handle:horizontal { width:20px; height:20px; margin:-7px 0;"
          "  background:#6DBF5E; border-radius:10px; }"
          "QSlider::sub-page:horizontal { background:#4a7a43; border-radius:3px; }"
        : "QSlider::groove:horizontal { height:6px; background:#dce8d9; border-radius:3px; }"
          "QSlider::handle:horizontal { width:20px; height:20px; margin:-7px 0;"
          "  background:#4a7a43; border-radius:10px; }"
          "QSlider::sub-page:horizontal { background:#7aac6e; border-radius:3px; }");

    // ── Journal storage card labels ───────────────────────────────────────────
    m_journalSec->setStyleSheet(sectionSS);
    m_jLabel->setStyleSheet(rowSS);
    m_jDesc->setStyleSheet(descSS);

    m_dirEdit->setStyleSheet(dark
        ? "QLineEdit {"
          "  border:1px solid rgba(74,122,67,0.4); border-radius:10px;"
          "  background:#0D1A0E; padding:10px 16px;"
          "  font-size:14px; color:#8AAD85;"
          "}"
          "QLineEdit:focus { border-color:#6DBF5E; }"
        : "QLineEdit {"
          "  border:1px solid #c7d8c1; border-radius:10px;"
          "  background:#f8fcf7; padding:10px 16px;"
          "  font-size:14px; color:#365143;"
          "}"
          "QLineEdit:focus { border-color:#7aac6e; }");

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

    const int fontSize = cfg.value("fontSize", 14).toInt();
    m_fontSlider->setValue(fontSize);
    m_fontSizeLbl->setText(QString("%1 px").arg(fontSize));

    m_dirEdit->setText(cfg.value("journalDir").toString());
}

// ─────────────────────────────────────────────────────────────────────────────
// Slots
// ─────────────────────────────────────────────────────────────────────────────
void Settings::onApply() {
    QSettings cfg("YangonDevs", "MindEase");

    // ── Theme ─────────────────────────────────────────────────────────────────
    const bool dark = m_darkRadio->isChecked();
    cfg.setValue("darkTheme", dark);
    emit themeChanged(dark);

    // ── Font size ─────────────────────────────────────────────────────────────
    const int fontSize = m_fontSlider->value();
    cfg.setValue("fontSize", fontSize);
    emit fontSizeChanged(fontSize);

    // ── Journal dir ───────────────────────────────────────────────────────────
    const QString dir = m_dirEdit->text().trimmed();
    cfg.setValue("journalDir", dir);
    if (!dir.isEmpty())
        emit journalDirChanged(dir);

    cfg.sync();
    showStatus("Settings applied ✓");
}

void Settings::onBrowseDir() {
    const QString chosen = QFileDialog::getExistingDirectory(
        this, "Choose Journal Folder",
        m_dirEdit->text().isEmpty()
            ? QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            : m_dirEdit->text());
    if (!chosen.isEmpty())
        m_dirEdit->setText(chosen);
}

void Settings::onResetDir() {
    m_dirEdit->clear();
    QSettings("YangonDevs", "MindEase").remove("journalDir");
    emit journalDirChanged(
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
        + "/MindEase_Journal");
    showStatus("Journal directory reset to default ✓");
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
