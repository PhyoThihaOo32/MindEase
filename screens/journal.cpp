#include "screens/journal.h"
#include "models/journalentry.h"
#include <QColor>
#include <QDateTime>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QScrollArea>
#include <QSizePolicy>
#include <QStandardPaths>
#include <QTimer>

namespace {

struct EntryDisplayParts {
    QString title;
    QString body;
    QString excerpt;
};

static void applyJournalShadow(QWidget *widget,
                               int blur = 18,
                               int yOffset = 4,
                               const QColor &color = QColor(121, 154, 112, 14)) {
    auto *shadow = new QGraphicsDropShadowEffect(widget);
    shadow->setBlurRadius(blur);
    shadow->setOffset(0, yOffset);
    shadow->setColor(color);
    widget->setGraphicsEffect(shadow);
}

static void clearLayout(QLayout *layout) {
    if (!layout)
        return;

    while (QLayoutItem *item = layout->takeAt(0)) {
        if (QWidget *widget = item->widget())
            delete widget;
        if (QLayout *childLayout = item->layout())
            clearLayout(childLayout);
        delete item;
    }
}

static EntryDisplayParts buildEntryDisplay(const JournalEntry &entry) {
    EntryDisplayParts parts;
    const QString raw = entry.body().trimmed();
    if (raw.isEmpty()) {
        parts.title = "Untitled reflection";
        return parts;
    }

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

    if (fullBody.isEmpty())
        fullBody = raw;

    parts.body = fullBody;
    parts.excerpt = fullBody.simplified();
    if (parts.excerpt.size() > 180)
        parts.excerpt = parts.excerpt.left(180).trimmed() + "...";

    return parts;
}

} // namespace

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

    QLabel *eyebrow = new QLabel("ZEN SPACE");
    eyebrow->setStyleSheet(
        "font-size:12px; font-weight:700; color:#5e6d85; letter-spacing:1.8px;");

    QLabel *title = new QLabel("My Journal");
    title->setStyleSheet(
        "font-size:40px; font-weight:800; color:#111111; letter-spacing:-0.8px;");

    QLabel *sub = new QLabel(
        "Create entries to reflect on events, and return to past reflections.");
    sub->setStyleSheet("font-size:15px; color:#4c5e78; border:none;");

    inner->addWidget(eyebrow, 0, Qt::AlignLeft);
    inner->addSpacing(10);
    inner->addWidget(title, 0, Qt::AlignLeft);
    inner->addSpacing(12);
    inner->addWidget(sub, 0, Qt::AlignLeft);
    inner->addSpacing(34);

    QFrame *composerCard = new QFrame();
    composerCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    composerCard->setMinimumHeight(640);
    composerCard->setStyleSheet(
        "QFrame {"
        "  background:qradialgradient(cx:0.18, cy:0.08, radius:1.2,"
        "                              fx:0.18, fy:0.08,"
        "                              stop:0 #ffffff, stop:0.58 #fbfdf8, stop:1 #edf5eb);"
        "  border:1px solid #c7d8c1;"
        "  border-radius:20px;"
        "}");
    applyJournalShadow(composerCard, 26, 8, QColor(121, 154, 112, 20));
    QVBoxLayout *composerLayout = new QVBoxLayout(composerCard);
    composerLayout->setContentsMargins(44, 38, 44, 30);
    composerLayout->setSpacing(0);

    m_titleEdit = new QLineEdit();
    m_titleEdit->setPlaceholderText("Entry title...");
    m_titleEdit->setStyleSheet(
        "QLineEdit {"
        "  border:none;"
        "  background:transparent;"
        "  font-size:27px;"
        "  font-weight:700;"
        "  color:#173c2c;"
        "  padding:0 0 8px 0;"
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
        "  border:none;"
        "  background:transparent;"
        "  font-size:17px;"
        "  color:#365143;"
        "  padding:12px 0 0 0;"
        "  line-height:170%;"
        "}"
        "QTextEdit::placeholder { color:#8fa09a; }");
    composerLayout->addWidget(m_editor);

    composerLayout->addSpacing(20);

    QFrame *divider = new QFrame();
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("background:#dce8d8; border:none;");
    divider->setFixedHeight(1);
    composerLayout->addWidget(divider);

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

    QPushButton *saveBtn = new QPushButton("Save Entry");
    saveBtn->setCursor(Qt::PointingHandCursor);
    saveBtn->setMinimumHeight(44);
    saveBtn->setMinimumWidth(150);
    saveBtn->setStyleSheet(
        "QPushButton {"
        "  background:qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "                              stop:0 #234030, stop:1 #3d6046);"
        "  color:#fffdf5;"
        "  border:1px solid #2f523b;"
        "  border-radius:10px;"
        "  font-size:14px;"
        "  font-weight:700;"
        "  padding:10px 24px;"
        "}"
        "QPushButton:hover { background:#2d4d39; border-color:#53725a; }");
    connect(saveBtn, &QPushButton::clicked, this, &Journal::saveEntry);

    footerLayout->addWidget(m_dateLbl);
    footerLayout->addWidget(m_statusLbl, 1);
    footerLayout->addWidget(saveBtn, 0, Qt::AlignRight);
    composerLayout->addWidget(footerRow);

    inner->addWidget(composerCard);
    inner->addSpacing(42);

    QLabel *pastLabel = new QLabel("PAST REFLECTIONS");
    pastLabel->setStyleSheet(
        "font-size:12px; font-weight:700; color:#5e6d85; letter-spacing:1.8px;");
    inner->addWidget(pastLabel, 0, Qt::AlignLeft);
    inner->addSpacing(18);

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

void Journal::onActivated() {
    refreshDateLabel();
    refreshEntryList();
}

void Journal::showStatus(const QString &msg, bool ok) {
    m_statusLbl->setText(msg);
    m_statusLbl->setStyleSheet(
        ok ? "font-size:12px; color:#2E7D32; border:none;"
           : "font-size:12px; color:#C62828; border:none;");
    QTimer::singleShot(4000, m_statusLbl, [this, msg]() {
        if (m_statusLbl->text() == msg)
            m_statusLbl->clear();
    });
}

void Journal::refreshDateLabel() {
    const QDate today = QDate::currentDate();
    m_dateLbl->setText(QString("Today, %1").arg(today.toString("MMMM d, yyyy")));
}

void Journal::saveEntry() {
    const QString title = m_titleEdit->text().trimmed();
    const QString body = m_editor->toPlainText().trimmed();

    if (body.isEmpty()) {
        showStatus("Write something before saving.", false);
        return;
    }

    QString combined = body;
    if (!title.isEmpty())
        combined = title + "\n\n" + body;

    const QDateTime now = QDateTime::currentDateTime();
    JournalEntry entry(now, combined);

    if (!m_storage.saveEntry(entry)) {
        showStatus("Could not save this entry.", false);
        return;
    }

    showStatus("Entry saved.");
    clearEditor();
    refreshDateLabel();
    refreshEntryList();
}

void Journal::clearEditor() {
    m_titleEdit->clear();
    m_editor->clear();
    m_titleEdit->setFocus();
}

void Journal::deleteEntryAtPath(const QString &path) {
    const JournalEntry entry = m_storage.loadEntry(path);
    const EntryDisplayParts parts = buildEntryDisplay(entry);

    QMessageBox dialog(this);
    dialog.setWindowTitle("MindEase");
    dialog.setIcon(QMessageBox::NoIcon);
    dialog.setText("Delete this reflection?");
    dialog.setInformativeText(
        QString("This will remove \"%1\" from your saved journal entries.")
            .arg(parts.title));
    dialog.setTextFormat(Qt::PlainText);
    dialog.setStyleSheet(
        "QMessageBox {"
        "  background:#f4f5ec;"
        "  border:1px solid #c8d5c4;"
        "}"
        "QLabel {"
        "  color:#264437;"
        "}"
        "QLabel#qt_msgbox_label {"
        "  font-size:20px;"
        "  font-weight:700;"
        "  min-width:320px;"
        "}"
        "QLabel#qt_msgbox_informativelabel {"
        "  font-size:14px;"
        "  color:#5a6f62;"
        "  min-width:320px;"
        "}"
        "QPushButton {"
        "  background:#edf3e6;"
        "  color:#264437;"
        "  border:1px solid #c3d2bf;"
        "  border-radius:10px;"
        "  min-width:100px;"
        "  min-height:38px;"
        "  padding:8px 16px;"
        "  font-size:13px;"
        "  font-weight:700;"
        "}"
        "QPushButton:hover {"
        "  background:#e4ecde;"
        "  border-color:#aebfa8;"
        "}"
        "QPushButton:pressed {"
        "  background:#dbe6d5;"
        "}"
    );

    QPushButton *cancelBtn = dialog.addButton("Cancel", QMessageBox::RejectRole);
    QPushButton *deleteBtn = dialog.addButton("Delete", QMessageBox::DestructiveRole);
    deleteBtn->setStyleSheet(
        "QPushButton {"
        "  background:qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "                            stop:0 #dcebd4, stop:1 #cfe2c9);"
        "  color:#234030;"
        "  border:1px solid #aac19f;"
        "  border-radius:10px;"
        "  min-width:100px;"
        "  min-height:38px;"
        "  padding:8px 16px;"
        "  font-size:13px;"
        "  font-weight:700;"
        "}"
        "QPushButton:hover {"
        "  background:#d3e5cc;"
        "  border-color:#96af8b;"
        "}"
        "QPushButton:pressed {"
        "  background:#c8ddc0;"
        "}"
    );
    dialog.setDefaultButton(cancelBtn);
    dialog.setEscapeButton(cancelBtn);
    dialog.exec();

    if (dialog.clickedButton() != deleteBtn)
        return;

    if (!m_storage.deleteEntry(path)) {
        showStatus("Could not delete that entry.", false);
        return;
    }

    showStatus("Entry deleted.");
    refreshEntryList();
}

void Journal::refreshEntryList() {
    clearLayout(m_entriesLayout);

    const QVector<JournalEntry> entries = m_storage.loadEntries();
    if (entries.isEmpty()) {
        QFrame *emptyCard = new QFrame();
        emptyCard->setStyleSheet(
            "QFrame {"
            "  background:qlineargradient(x1:0, y1:0, x2:1, y2:1,"
            "                              stop:0 #ffffff, stop:1 #eff6ec);"
            "  border:1px solid #c7d8c1;"
            "  border-radius:16px;"
            "}");
        applyJournalShadow(emptyCard, 14, 4, QColor(121, 154, 112, 12));
        QVBoxLayout *emptyLayout = new QVBoxLayout(emptyCard);
        emptyLayout->setContentsMargins(30, 26, 30, 26);

        QLabel *emptyTitle = new QLabel("No reflections yet");
        emptyTitle->setStyleSheet("font-size:18px; font-weight:700; color:#173c2c; border:none;");
        QLabel *emptyBody = new QLabel("Your saved entries will appear here after you write and save your first journal reflection.");
        emptyBody->setWordWrap(true);
        emptyBody->setStyleSheet("font-size:14px; color:#5a6f62; border:none; line-height:160%;");
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
        card->setStyleSheet(
            "QFrame {"
            "  background:qlineargradient(x1:0, y1:0, x2:1, y2:1,"
            "                              stop:0 #ffffff, stop:0.72 #fbfdf8, stop:1 #edf5eb);"
            "  border:1px solid #c7d8c1;"
            "  border-left:4px solid #93ad89;"
            "  border-radius:16px;"
            "}");
        applyJournalShadow(card, 16, 4, QColor(121, 154, 112, 14));
        card->setToolTip(parts.body);

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(30, 22, 30, 24);
        cardLayout->setSpacing(12);

        QWidget *topRow = new QWidget();
        topRow->setStyleSheet("background:transparent; border:none;");
        QHBoxLayout *topLayout = new QHBoxLayout(topRow);
        topLayout->setContentsMargins(0, 0, 0, 0);
        topLayout->setSpacing(12);

        QLabel *titleLbl = new QLabel(parts.title);
        titleLbl->setWordWrap(true);
        titleLbl->setStyleSheet(
            "font-size:18px; font-weight:800; color:#173c2c; border:none;");

        QPushButton *deleteBtn = new QPushButton("Delete");
        deleteBtn->setCursor(Qt::PointingHandCursor);
        deleteBtn->setStyleSheet(
            "QPushButton {"
            "  background:#fff9f6;"
            "  border:1px solid #efd3cc;"
            "  border-radius:9px;"
            "  color:#a33a2e;"
            "  font-size:12px;"
            "  font-weight:700;"
            "  padding:6px 12px;"
            "}"
            "QPushButton:hover { background:#fff0ec; border-color:#e1b9b0; }");
        const QString path = entry.filePath();
        connect(deleteBtn, &QPushButton::clicked, this, [this, path]() {
            deleteEntryAtPath(path);
        });

        topLayout->addWidget(titleLbl, 1);
        topLayout->addWidget(deleteBtn, 0, Qt::AlignTop);
        cardLayout->addWidget(topRow);

        QLabel *bodyLbl = new QLabel(parts.excerpt);
        bodyLbl->setWordWrap(true);
        bodyLbl->setStyleSheet(
            "font-size:14px; color:#4f6255; border:none; line-height:165%;");
        cardLayout->addWidget(bodyLbl);

        QLabel *dateLbl = new QLabel(entry.dateTime().date().toString("MMM d, yyyy"));
        dateLbl->setStyleSheet("font-size:13px; font-weight:600; color:#71856f; border:none;");
        cardLayout->addWidget(dateLbl);

        m_entriesLayout->addWidget(card);
    }

    m_entriesLayout->addStretch();
}
