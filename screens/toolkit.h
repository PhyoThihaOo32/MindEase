#ifndef TOOLKIT_H
#define TOOLKIT_H

#include "core/screen.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QVector>

// ─────────────────────────────────────────────────────────────────────────────
// Toolkit — Mental Health Toolkit screen
//
// Inherits Screen and provides a concrete implementation of the abstract
// interface. Demonstrates:
//   • Inheritance   — extends the custom Screen base class
//   • Polymorphism  — overrides screenId() and onActivated() from Screen
// ─────────────────────────────────────────────────────────────────────────────

class Toolkit : public Screen {
    Q_OBJECT

public:
    explicit Toolkit(QWidget *parent = nullptr);

    // ── Implement pure virtual (required by Screen) ───────────────────────────
    QString screenId() const override { return "toolkit"; }

    // ── Override activation hook ──────────────────────────────────────────────
    void onActivated() override;
    void onThemeChanged(bool dark) override;   // Feature 5: Zen Night readability

private:
    void applyFolderSearch(const QString &query);
    void openBestFolderMatch();
    void showFolder(int index);
    void showHome();

    QStackedWidget *m_stack = nullptr;
    QLineEdit      *m_searchInput = nullptr;
    QLabel         *m_searchStatus = nullptr;
    QVector<QWidget*> m_folderCards;
    QVector<QLabel*>  m_folderSubtitleLabels;

    // Feature 5: landing-page header + search widgets that need re-theming
    bool          m_dark       = false;
    QLabel       *m_greeting   = nullptr;   // "WELCOME" eyebrow
    QLabel       *m_heading    = nullptr;   // big heading
    QLabel       *m_sub        = nullptr;   // subtitle text
    QLabel       *m_searchIcon = nullptr;   // ⌕ icon label
    QPushButton  *m_searchBtn  = nullptr;   // "Search" button
};

#endif // TOOLKIT_H
