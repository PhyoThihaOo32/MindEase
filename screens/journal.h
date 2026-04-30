#ifndef JOURNAL_H
#define JOURNAL_H

#include "core/screen.h"
#include "models/journalentry.h"
#include "storage/journalstorage.h"
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QVector>

class Journal : public Screen {
    Q_OBJECT

public:
    explicit Journal(QWidget *parent = nullptr);
    QString screenId() const override { return "journal"; }
    void onActivated() override;

    // Called by MainWindow when Settings changes the journal directory.
    void setStoragePath(const QString &path);

private slots:
    void saveEntry();
    void clearEditor();
    void applyFilter();        // Feature 4: live search + date filter

private:
    // Refresh: loads from disk → m_allEntries → applyFilter()
    void refreshEntryList();
    void refreshDateLabel();
    void deleteEntryAtPath(const QString &path);
    void showStatus(const QString &msg, bool ok = true);

    // Feature 4: card builder (called by applyFilter with filtered subset)
    void buildEntryCards(const QVector<JournalEntry> &entries);

    // Feature 6: export
    void exportEntry(const JournalEntry &entry);
    void exportAsPdf(const JournalEntry &entry);
    void exportAsText(const JournalEntry &entry);
    QString buildHtmlExport(const JournalEntry &entry) const;

    // Composer
    QLineEdit    *m_titleEdit     = nullptr;
    QTextEdit    *m_editor        = nullptr;
    QLabel       *m_dateLbl       = nullptr;
    QLabel       *m_statusLbl     = nullptr;

    // Past-reflections panel
    QWidget      *m_entriesHost   = nullptr;
    QVBoxLayout  *m_entriesLayout = nullptr;

    // Feature 4: search / filter widgets + cached list
    QLineEdit    *m_searchEdit    = nullptr;
    QComboBox    *m_periodFilter  = nullptr;
    QVector<JournalEntry> m_allEntries;

    JournalStorage m_storage;
};

#endif // JOURNAL_H
