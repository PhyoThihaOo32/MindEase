#include "screens/journal.h"
#include "core/lucideicons.h"
#include <QColor>
#include <QDateTime>
#include <QFileDialog>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QMenu>
#include <QMessageBox>
#include <QShortcut>
#include <QPrinter>
#include <QPrintDialog>
#include <QPushButton>
#include <QRegularExpression>
#include <QScrollArea>
#include <QSizePolicy>
#include <QStandardPaths>
#include <QTextDocument>
#include <QTimer>

// ─────────────────────────────────────────────────────────────────────────────
// Anonymous-namespace helpers
// ─────────────────────────────────────────────────────────────────────────────
namespace {

struct EntryDisplayParts {
    QString title;
    QString body;
    QString excerpt;
};

void applyJournalShadow(QWidget *widget,
                        int blur = 18,
                        int yOffset = 4,
                        const QColor &color = QColor(121, 154, 112, 14)) {
    auto *shadow = new QGraphicsDropShadowEffect(widget);
    shadow->setBlurRadius(blur);
    shadow->setOffset(0, yOffset);
    shadow->setColor(color);
    widget->setGraphicsEffect(shadow);
}

void clearLayout(QLayout *layout) {
    if (!layout) return;
    while (QLayoutItem *item = layout->takeAt(0)) {
        if (QWidget *w = item->widget()) delete w;
        if (QLayout *l = item->layout()) clearLayout(l);
        delete item;
    }
}

EntryDisplayParts buildEntryDisplay(const JournalEntry &entry) {
    EntryDisplayParts parts;
    const QString raw = entry.body().trimmed();
    if (raw.isEmpty()) { parts.title = "Untitled reflection"; return parts; }

    QString fullBody = raw;
    const QRegularExpression paragraphBreak("\n\\s*\n");
    QStringList paragraphs = raw.split(paragraphBreak, Qt::SkipEmptyParts);

    if (paragraphs.size() >= 2) {
        const QString candidateTitle = paragraphs.first().trimmed();
        if (!candidateTitle.isEmpty() && candidateTitle.size() <= 90) {
            parts.title = candidateTitle;
            paragraphs.takeFirst();
            fullBody = paragraphs.join("\n\n").trimmed();
        }
    }

    if (parts.title.isEmpty()) {
        const QString firstLine = raw.section('\n', 0, 0).trimmed();
        if (!firstLine.isEmpty() && firstLine.size() <= 90) {
            parts.title = firstLine;
        } else {
            parts.title = raw.simplified();
            if (parts.title.size() > 56)
                parts.title = parts.title.left(56).trimmed() + "...";
        }
    }

    if (fullBody.isEmpty()) fullBody = raw;
    parts.body    = fullBody;
    parts.excerpt = fullBody.simplified();
    if (parts.excerpt.size() > 180)
        parts.excerpt = parts.excerpt.left(180).trimmed() + "...";

    return parts;
}

} // namespace

// ─────────────────────────────────────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────────────────────────────────────
Journal::Journal(QWidget *parent)
    : Screen("My Journal", parent),
      m_storage(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                + "/MindEase_Journal") {

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
    pageInner->setMaximumWidth(1680);
    pageInner->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QVBoxLayout *inner = new QVBoxLayout(pageInner);
    inner->setContentsMargins(0, 0, 0, 0);
    inner->setSpacing(0);

    // ── Header ───────────────────────────────────────────────────────────────
    m_eyebrowLbl = new QLabel("ZEN SPACE");
    m_eyebrowLbl->setStyleSheet(
        "font-size:12px; font-weight:700; color:#5e6d85; letter-spacing:1.8px;");

    m_titleLbl = new QLabel("My Journal");
    m_titleLbl->setStyleSheet(
        "font-size:40px; font-weight:800; color:#111111; letter-spacing:-0.8px;");

    m_subLbl = new QLabel(
        "Create entries to reflect on events, and return to past reflections.");
    m_subLbl->setStyleSheet("font-size:15px; color:#4c5e78; border:none;");

    inner->addWidget(m_eyebrowLbl, 0, Qt::AlignLeft);
    inner->addSpacing(10);
    inner->addWidget(m_titleLbl, 0, Qt::AlignLeft);
    inner->addSpacing(12);
    inner->addWidget(m_subLbl, 0, Qt::AlignLeft);
    inner->addSpacing(34);

    // ── Composer card ────────────────────────────────────────────────────────
    m_composerCard = new QFrame();
    m_composerCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_composerCard->setMinimumHeight(640);
    m_composerCard->setStyleSheet(
        "QFrame {"
        "  background:qradialgradient(cx:0.18, cy:0.08, radius:1.2,"
        "                              fx:0.18, fy:0.08,"
        "                              stop:0 #ffffff, stop:0.58 #fbfdf8, stop:1 #edf5eb);"
        "  border:1px solid #c7d8c1;"
        "  border-radius:20px;"
        "}");
    applyJournalShadow(m_composerCard, 26, 8, QColor(121, 154, 112, 20));
    QVBoxLayout *composerLayout = new QVBoxLayout(m_composerCard);
    composerLayout->setContentsMargins(44, 38, 44, 30);
    composerLayout->setSpacing(0);

    m_titleEdit = new QLineEdit();
    m_titleEdit->setPlaceholderText("Entry title...");
    m_titleEdit->setStyleSheet(
        "QLineEdit {"
        "  border:none; background:transparent;"
        "  font-size:27px; font-weight:700; color:#173c2c; padding:0 0 8px 0;"
        "}"
        "QLineEdit::placeholder { color:#9eaa9d; }");
    composerLayout->addWidget(m_titleEdit);
    composerLayout->addSpacing(10);

    m_editor = new QTextEdit();
    m_editor->setAcceptRichText(false);
    m_editor->setPlaceholderText("How are you feeling today? What's on your mind?");
    m_editor->setMinimumHeight(430);
    m_editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_editor->setStyleSheet(
        "QTextEdit {"
        "  border:none; background:transparent;"
        "  font-size:17px; color:#365143; padding:12px 0 0 0; line-height:170%;"
        "}"
        "QTextEdit::placeholder { color:#8fa09a; }");
    composerLayout->addWidget(m_editor);
    composerLayout->addSpacing(20);

    m_composerDivider = new QFrame();
    m_composerDivider->setFrameShape(QFrame::HLine);
    m_composerDivider->setStyleSheet("background:#dce8d8; border:none;");
    m_composerDivider->setFixedHeight(1);
    composerLayout->addWidget(m_composerDivider);
    composerLayout->addSpacing(18);

    QWidget *footerRow = new QWidget();
    footerRow->setStyleSheet("background:transparent; border:none;");
    QHBoxLayout *footerLayout = new QHBoxLayout(footerRow);
    footerLayout->setContentsMargins(0, 0, 0, 0);
    footerLayout->setSpacing(16);

    m_dateLbl = new QLabel();
    m_dateLbl->setStyleSheet("font-size:14px; font-weight:600; color:#5a6f62; border:none;");

    m_statusLbl = new QLabel();
    m_statusLbl->setStyleSheet("font-size:12px; color:#2E7D32; border:none;");

    // ── Keyboard shortcut hint ───────────────────────────────────────────────
    m_shortcutHint = new QLabel("Ctrl+S to save");
    m_shortcutHint->setStyleSheet("font-size:11px; color:#9eaa9d; border:none;");

    QPushButton *saveBtn = new QPushButton("Save Entry");
    saveBtn->setCursor(Qt::PointingHandCursor);
    saveBtn->setMinimumHeight(44);
    saveBtn->setMinimumWidth(150);
    saveBtn->setStyleSheet(
        "QPushButton {"
        "  background:qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "                              stop:0 #234030, stop:1 #3d6046);"
        "  color:#fffdf5; border:1px solid #2f523b; border-radius:10px;"
        "  font-size:14px; font-weight:700; padding:10px 24px;"
        "}"
        "QPushButton:hover { background:#2d4d39; border-color:#53725a; }");
    connect(saveBtn, &QPushButton::clicked, this, &Journal::saveEntry);

    // Ctrl+S shortcut
    auto *saveShortcut = new QShortcut(QKeySequence::Save, this);
    connect(saveShortcut, &QShortcut::activated, this, &Journal::saveEntry);

    footerLayout->addWidget(m_dateLbl);
    footerLayout->addWidget(m_statusLbl, 1);
    footerLayout->addWidget(m_shortcutHint);
    footerLayout->addWidget(saveBtn, 0, Qt::AlignRight);
    composerLayout->addWidget(footerRow);
    inner->addWidget(m_composerCard);
    inner->addSpacing(42);

    // ── Past Reflections header ──────────────────────────────────────────────
    m_pastLbl = new QLabel("PAST REFLECTIONS");
    m_pastLbl->setStyleSheet(
        "font-size:12px; font-weight:700; color:#5e6d85; letter-spacing:1.8px;");
    inner->addWidget(m_pastLbl, 0, Qt::AlignLeft);
    inner->addSpacing(14);

    // ── Feature 4: Search + Date-filter row ──────────────────────────────────
    QWidget *filterRow = new QWidget();
    filterRow->setStyleSheet("background:transparent; border:none;");
    QHBoxLayout *filterLayout = new QHBoxLayout(filterRow);
    filterLayout->setContentsMargins(0, 0, 0, 0);
    filterLayout->setSpacing(12);

    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("Search reflections...");
    m_searchEdit->setClearButtonEnabled(true);
    m_searchEdit->setStyleSheet(
        "QLineEdit {"
        "  border:1px solid #c7d8c1; border-radius:10px;"
        "  background:#ffffff; padding:10px 16px;"
        "  font-size:14px; color:#365143;"
        "}"
        "QLineEdit:focus { border-color:#7aac6e; }"
        "QLineEdit::placeholder { color:#9eaa9d; }");

    m_periodFilter = new QComboBox();
    m_periodFilter->addItems({ "All Time", "Today", "This Week", "This Month" });
    m_periodFilter->setMinimumHeight(42);
    m_periodFilter->setMinimumWidth(140);
    m_periodFilter->setCursor(Qt::PointingHandCursor);
    m_periodFilter->setStyleSheet(
        "QComboBox {"
        "  border:1px solid #c7d8c1; border-radius:10px;"
        "  background:#ffffff; padding:8px 14px;"
        "  font-size:14px; color:#365143; font-weight:600;"
        "}"
        "QComboBox:focus { border-color:#7aac6e; }"
        "QComboBox::drop-down { border:none; width:28px; }"
        "QComboBox QAbstractItemView {"
        "  background:#ffffff; border:1px solid #c7d8c1;"
        "  selection-background-color:#edf5eb; selection-color:#173c2c;"
        "}");

    filterLayout->addWidget(m_searchEdit, 1);
    filterLayout->addWidget(m_periodFilter);

    connect(m_searchEdit,   &QLineEdit::textChanged,
            this,           &Journal::applyFilter);
    connect(m_periodFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,           &Journal::applyFilter);

    inner->addWidget(filterRow);
    inner->addSpacing(18);

    // ── Entry cards host ─────────────────────────────────────────────────────
    m_entriesHost = new QWidget();
    m_entriesHost->setStyleSheet("background:transparent; border:none;");
    m_entriesHost->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_entriesLayout = new QVBoxLayout(m_entriesHost);
    m_entriesLayout->setContentsMargins(0, 0, 0, 0);
    m_entriesLayout->setSpacing(18);
    inner->addWidget(m_entriesHost);
    inner->addStretch();

    pageLayout->addWidget(pageInner);
    pageLayout->addStretch();

    scroll->setWidget(page);
    root->addWidget(scroll);

    refreshDateLabel();
    refreshEntryList();
}

// ─────────────────────────────────────────────────────────────────────────────
// Screen interface
// ─────────────────────────────────────────────────────────────────────────────
void Journal::onActivated() {
    refreshDateLabel();
    refreshEntryList();
}

// ─────────────────────────────────────────────────────────────────────────────
// Status / date helpers
// ─────────────────────────────────────────────────────────────────────────────
void Journal::showStatus(const QString &msg, bool ok) {
    m_statusLbl->setText(msg);
    m_statusLbl->setStyleSheet(
        ok ? "font-size:12px; color:#2E7D32; border:none;"
           : "font-size:12px; color:#C62828; border:none;");
    QTimer::singleShot(4000, m_statusLbl, [this, msg]() {
        if (m_statusLbl->text() == msg) m_statusLbl->clear();
    });
}

void Journal::refreshDateLabel() {
    const QDate today = QDate::currentDate();
    m_dateLbl->setText(QString("Today, %1").arg(today.toString("MMMM d, yyyy")));
}

// ─────────────────────────────────────────────────────────────────────────────
// Save / clear
// ─────────────────────────────────────────────────────────────────────────────
void Journal::saveEntry() {
    const QString titleText = m_titleEdit->text().trimmed();
    const QString bodyText  = m_editor->toPlainText().trimmed();

    if (bodyText.isEmpty()) {
        showStatus("Write something before saving.", false);
        return;
    }

    QString combined = bodyText;
    if (!titleText.isEmpty())
        combined = titleText + "\n\n" + bodyText;

    JournalEntry entry(QDateTime::currentDateTime(), combined);
    if (!m_storage.saveEntry(entry)) {
        showStatus("Could not save this entry.", false);
        return;
    }

    showStatus("Entry saved");
    clearEditor();
    refreshDateLabel();
    refreshEntryList();
}

void Journal::clearEditor() {
    m_titleEdit->clear();
    m_editor->clear();
    m_titleEdit->setFocus();
}

// ─────────────────────────────────────────────────────────────────────────────
// Feature 4: Search + Date Filter
// ─────────────────────────────────────────────────────────────────────────────
void Journal::refreshEntryList() {
    // Load all entries from disk into cache, then let applyFilter render them.
    m_allEntries = m_storage.loadEntries();
    applyFilter();
}

void Journal::applyFilter() {
    const QString query = m_searchEdit ? m_searchEdit->text().trimmed().toLower()
                                       : QString();
    const int period = m_periodFilter ? m_periodFilter->currentIndex() : 0;
    const QDate today = QDate::currentDate();

    QVector<JournalEntry> filtered;
    filtered.reserve(m_allEntries.size());

    for (const JournalEntry &e : m_allEntries) {
        // ── Period filter ────────────────────────────────────────────────────
        bool periodOk = true;
        switch (period) {
            case 1: periodOk = (e.dateTime().date() == today);               break;
            case 2: periodOk = (e.dateTime().date() >= today.addDays(-6));   break;
            case 3: periodOk = (e.dateTime().date() >= today.addDays(-29));  break;
            default: break;
        }
        if (!periodOk) continue;

        // ── Text search ──────────────────────────────────────────────────────
        if (!query.isEmpty() && !e.body().toLower().contains(query))
            continue;

        filtered.append(e);
    }

    buildEntryCards(filtered);
}

// ─────────────────────────────────────────────────────────────────────────────
// Build entry cards (called by applyFilter with the filtered subset)
// ─────────────────────────────────────────────────────────────────────────────
void Journal::buildEntryCards(const QVector<JournalEntry> &entries) {
    clearLayout(m_entriesLayout);

    if (entries.isEmpty()) {
        QFrame *emptyCard = new QFrame();
        emptyCard->setStyleSheet(m_dark
            ? "QFrame {"
              "  background:qlineargradient(x1:0, y1:0, x2:1, y2:1,"
              "    stop:0 #111E12, stop:1 #0F1A10);"
              "  border:1px solid rgba(74,122,67,0.28); border-radius:16px;"
              "}"
            : "QFrame {"
              "  background:qlineargradient(x1:0, y1:0, x2:1, y2:1,"
              "    stop:0 #ffffff, stop:1 #eff6ec);"
              "  border:1px solid #c7d8c1; border-radius:16px;"
              "}");
        applyJournalShadow(emptyCard, 14, 4, QColor(121, 154, 112, m_dark ? 6 : 12));
        QVBoxLayout *emptyLayout = new QVBoxLayout(emptyCard);
        emptyLayout->setContentsMargins(30, 26, 30, 26);

        // Different message when search/filter is active vs. truly empty
        const bool filtering = (m_searchEdit && !m_searchEdit->text().isEmpty())
                             || (m_periodFilter && m_periodFilter->currentIndex() != 0);
        QLabel *emptyTitle = new QLabel(filtering ? "No matching reflections"
                                                  : "No reflections yet");
        emptyTitle->setStyleSheet(m_dark
            ? "font-size:18px; font-weight:700; color:#C8ECC2; border:none;"
            : "font-size:18px; font-weight:700; color:#173c2c; border:none;");
        QLabel *emptyBody = new QLabel(
            filtering ? "Try a different search term or time period."
                      : "Your saved entries will appear here after you write and save "
                        "your first journal reflection.");
        emptyBody->setWordWrap(true);
        emptyBody->setStyleSheet(m_dark
            ? "font-size:14px; color:#6a9c6a; border:none; line-height:160%;"
            : "font-size:14px; color:#5a6f62; border:none; line-height:160%;");
        emptyLayout->addWidget(emptyTitle);
        emptyLayout->addSpacing(6);
        emptyLayout->addWidget(emptyBody);

        m_entriesLayout->addWidget(emptyCard);
        m_entriesLayout->addStretch();
        return;
    }

    for (const JournalEntry &entry : entries) {
        const EntryDisplayParts parts = buildEntryDisplay(entry);

        QFrame *card = new QFrame();
        card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        card->setStyleSheet(m_dark
            ? "QFrame {"
              "  background:qlineargradient(x1:0, y1:0, x2:1, y2:1,"
              "    stop:0 #111E12, stop:0.72 #142016, stop:1 #0F1A10);"
              "  border:1px solid rgba(74,122,67,0.28);"
              "  border-left:4px solid rgba(109,191,94,0.65);"
              "  border-radius:16px;"
              "}"
            : "QFrame {"
              "  background:qlineargradient(x1:0, y1:0, x2:1, y2:1,"
              "    stop:0 #ffffff, stop:0.72 #fbfdf8, stop:1 #edf5eb);"
              "  border:1px solid #c7d8c1; border-left:4px solid #93ad89;"
              "  border-radius:16px;"
              "}");
        applyJournalShadow(card, 16, 4, QColor(121, 154, 112, m_dark ? 6 : 14));
        card->setToolTip(parts.body);

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(30, 22, 30, 24);
        cardLayout->setSpacing(12);

        // ── Top row: title + action buttons ──────────────────────────────────
        QWidget *topRow = new QWidget();
        topRow->setStyleSheet("background:transparent; border:none;");
        QHBoxLayout *topLayout = new QHBoxLayout(topRow);
        topLayout->setContentsMargins(0, 0, 0, 0);
        topLayout->setSpacing(10);

        QLabel *titleLabel = new QLabel(parts.title);
        titleLabel->setWordWrap(true);
        titleLabel->setStyleSheet(m_dark
            ? "font-size:18px; font-weight:800; color:#C8ECC2; border:none;"
            : "font-size:18px; font-weight:800; color:#173c2c; border:none;");

        // ── Feature 6: Export button with dropdown menu ───────────────────────
        QPushButton *exportBtn = new QPushButton("Export ▾");
        exportBtn->setCursor(Qt::PointingHandCursor);
        exportBtn->setStyleSheet(m_dark
            ? "QPushButton {"
              "  background:#1A3018; border:1px solid rgba(74,122,67,0.45);"
              "  border-radius:9px; color:#7aac6e;"
              "  font-size:12px; font-weight:700; padding:6px 12px;"
              "}"
              "QPushButton:hover { background:#213d20; border-color:#5a8f54; }"
              "QPushButton::menu-indicator { image:none; }"
            : "QPushButton {"
              "  background:#f0f8ed; border:1px solid #b8d4b0;"
              "  border-radius:9px; color:#2d5c39;"
              "  font-size:12px; font-weight:700; padding:6px 12px;"
              "}"
              "QPushButton:hover { background:#e4f0e0; border-color:#9ac094; }"
              "QPushButton::menu-indicator { image:none; }");

        QMenu *exportMenu = new QMenu(exportBtn);
        exportMenu->setStyleSheet(m_dark
            ? "QMenu {"
              "  background:#0D1A0E; border:1px solid rgba(74,122,67,0.4);"
              "  border-radius:8px; padding:4px;"
              "}"
              "QMenu::item { padding:8px 20px; font-size:13px; color:#8AAD85; border-radius:6px; }"
              "QMenu::item:selected { background:#1A3018; color:#C8ECC2; }"
            : "QMenu {"
              "  background:#ffffff; border:1px solid #c7d8c1; border-radius:8px; padding:4px;"
              "}"
              "QMenu::item { padding:8px 20px; font-size:13px; color:#173c2c; border-radius:6px; }"
              "QMenu::item:selected { background:#edf5eb; }");
        exportMenu->addAction(lucideIcon("file-text", QColor("#173c2c"), 18),
                              "Save as PDF", this, [this, entry]() { exportAsPdf(entry); });
        exportMenu->addAction(lucideIcon("pencil", QColor("#173c2c"), 18),
                              "Save as Plain Text", this, [this, entry]() { exportAsText(entry); });
        exportBtn->setMenu(exportMenu);

        // Delete button
        QPushButton *deleteBtn = new QPushButton("Delete");
        deleteBtn->setCursor(Qt::PointingHandCursor);
        deleteBtn->setStyleSheet(m_dark
            ? "QPushButton {"
              "  background:#2a1212; border:1px solid rgba(163,58,46,0.4); border-radius:9px;"
              "  color:#e07060; font-size:12px; font-weight:700; padding:6px 12px;"
              "}"
              "QPushButton:hover { background:#351818; border-color:rgba(163,58,46,0.7); }"
            : "QPushButton {"
              "  background:#fff9f6; border:1px solid #efd3cc; border-radius:9px;"
              "  color:#a33a2e; font-size:12px; font-weight:700; padding:6px 12px;"
              "}"
              "QPushButton:hover { background:#fff0ec; border-color:#e1b9b0; }");
        const QString path = entry.filePath();
        connect(deleteBtn, &QPushButton::clicked, this, [this, path]() {
            deleteEntryAtPath(path);
        });

        topLayout->addWidget(titleLabel, 1);
        topLayout->addWidget(exportBtn, 0, Qt::AlignTop);
        topLayout->addWidget(deleteBtn, 0, Qt::AlignTop);
        cardLayout->addWidget(topRow);

        QLabel *bodyLbl = new QLabel(parts.excerpt);
        bodyLbl->setWordWrap(true);
        bodyLbl->setStyleSheet(m_dark
            ? "font-size:14px; color:#6a9c6a; border:none; line-height:165%;"
            : "font-size:14px; color:#4f6255; border:none; line-height:165%;");
        cardLayout->addWidget(bodyLbl);

        QLabel *dateLbl = new QLabel(entry.dateTime().toString("MMM d, yyyy · h:mm AP"));
        dateLbl->setStyleSheet(m_dark
            ? "font-size:13px; font-weight:600; color:#4a7a43; border:none;"
            : "font-size:13px; font-weight:600; color:#71856f; border:none;");
        cardLayout->addWidget(dateLbl);

        m_entriesLayout->addWidget(card);
    }

    m_entriesLayout->addStretch();
}

// ─────────────────────────────────────────────────────────────────────────────
// Delete
// ─────────────────────────────────────────────────────────────────────────────
void Journal::deleteEntryAtPath(const QString &path) {
    const JournalEntry entry = m_storage.loadEntry(path);
    const EntryDisplayParts parts = buildEntryDisplay(entry);

    QMessageBox dialog(this);
    dialog.setWindowTitle("MindEase");
    dialog.setIcon(QMessageBox::NoIcon);
    dialog.setText("Delete this reflection?");
    dialog.setInformativeText(
        QString("This will remove \"%1\" from your saved journal entries.").arg(parts.title));
    dialog.setTextFormat(Qt::PlainText);
    dialog.setStyleSheet(
        "QMessageBox { background:#f4f5ec; border:1px solid #c8d5c4; }"
        "QLabel { color:#264437; }"
        "QLabel#qt_msgbox_label { font-size:20px; font-weight:700; min-width:320px; }"
        "QLabel#qt_msgbox_informativelabel { font-size:14px; color:#5a6f62; min-width:320px; }"
        "QPushButton { background:#edf3e6; color:#264437; border:1px solid #c3d2bf;"
        "  border-radius:10px; min-width:100px; min-height:38px;"
        "  padding:8px 16px; font-size:13px; font-weight:700; }"
        "QPushButton:hover { background:#e4ecde; border-color:#aebfa8; }");

    QPushButton *cancelBtn = dialog.addButton("Cancel", QMessageBox::RejectRole);
    QPushButton *deleteBtn = dialog.addButton("Delete", QMessageBox::DestructiveRole);
    deleteBtn->setStyleSheet(
        "QPushButton { background:qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "  stop:0 #dcebd4, stop:1 #cfe2c9); color:#234030;"
        "  border:1px solid #aac19f; border-radius:10px;"
        "  min-width:100px; min-height:38px; padding:8px 16px;"
        "  font-size:13px; font-weight:700; }"
        "QPushButton:hover { background:#d3e5cc; border-color:#96af8b; }");
    dialog.setDefaultButton(cancelBtn);
    dialog.setEscapeButton(cancelBtn);
    dialog.exec();

    if (dialog.clickedButton() != deleteBtn) return;

    if (!m_storage.deleteEntry(path)) {
        showStatus("Could not delete that entry.", false);
        return;
    }

    showStatus("Entry deleted.");
    refreshEntryList();
}

// ─────────────────────────────────────────────────────────────────────────────
// Feature 6: Export
// ─────────────────────────────────────────────────────────────────────────────
QString Journal::buildHtmlExport(const JournalEntry &entry) const {
    const EntryDisplayParts parts = buildEntryDisplay(entry);
    const QString dateStr = entry.dateTime().toString("dddd, MMMM d, yyyy · h:mm AP");

    return QString(R"(
<html><head><style>
  body { font-family: Georgia, serif; color: #1a2e1b; max-width:720px; margin:60px auto; padding:0 40px; }
  h1   { font-size:28px; font-weight:bold; color:#173c2c; margin-bottom:6px; }
  .meta{ font-size:13px; color:#71856f; margin-bottom:28px; }
  hr   { border:none; border-top:1px solid #c7d8c1; margin:24px 0; }
  p    { font-size:16px; line-height:1.75; white-space:pre-wrap; color:#365143; }
  .footer { font-size:11px; color:#9eaa9d; margin-top:60px; }
</style></head><body>
  <h1>%1</h1>
  <div class="meta">%2</div>
  <hr>
  <p>%3</p>
  <div class="footer">Exported from MindEase · YangonDevs · BMCC</div>
</body></html>
)").arg(parts.title.toHtmlEscaped(), dateStr.toHtmlEscaped(), parts.body.toHtmlEscaped());
}

void Journal::exportAsPdf(const JournalEntry &entry) {
    const QString dateTag = entry.dateTime().toString("yyyy-MM-dd");
    const QString defaultPath =
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
        + "/" + dateTag + "_journal.pdf";

    const QString path = QFileDialog::getSaveFileName(
        this, "Export Journal Entry as PDF", defaultPath, "PDF Files (*.pdf)");
    if (path.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(path);
    printer.setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);

    QTextDocument doc;
    doc.setHtml(buildHtmlExport(entry));
    doc.print(&printer);

    showStatus("Exported PDF to Desktop");
}

void Journal::exportAsText(const JournalEntry &entry) {
    const QString dateTag = entry.dateTime().toString("yyyy-MM-dd");
    const QString defaultPath =
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
        + "/" + dateTag + "_journal.txt";

    const QString path = QFileDialog::getSaveFileName(
        this, "Export Journal Entry as Text", defaultPath, "Text Files (*.txt)");
    if (path.isEmpty()) return;

    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        showStatus("Could not write file.", false);
        return;
    }
    QTextStream out(&f);
    out << "MindEase Journal Entry\n";
    out << "======================\n\n";
    out << entry.dateTime().toString("dddd, MMMM d, yyyy  h:mm AP") << "\n\n";
    out << QString(30, QChar(0x2500)) << "\n\n";
    out << entry.body() << "\n\n";
    out << QString(30, QChar(0x2500)) << "\n";
    out << "Exported from MindEase · YangonDevs · BMCC\n";
    f.close();

    showStatus("Exported plain text to Desktop");
}
