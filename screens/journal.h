#ifndef JOURNAL_H
#define JOURNAL_H

#include "core/screen.h"
#include "storage/journalstorage.h"
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>

class Journal : public Screen {
    Q_OBJECT

public:
    explicit Journal(QWidget *parent = nullptr);
    QString screenId() const override { return "journal"; }
    void onActivated() override;

private slots:
    void saveEntry();
    void clearEditor();

private:
    void refreshEntryList();
    void refreshDateLabel();
    void deleteEntryAtPath(const QString &path);
    void showStatus(const QString &msg, bool ok = true);

    QLineEdit    *m_titleEdit = nullptr;
    QTextEdit    *m_editor = nullptr;
    QLabel       *m_dateLbl = nullptr;
    QLabel       *m_statusLbl = nullptr;
    QWidget      *m_entriesHost = nullptr;
    QVBoxLayout  *m_entriesLayout = nullptr;

    JournalStorage m_storage;
};

#endif // JOURNAL_H
