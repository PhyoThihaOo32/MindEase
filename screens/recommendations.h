#ifndef RECOMMENDATIONS_H
#define RECOMMENDATIONS_H

#include "core/screen.h"
#include <QPushButton>
#include <QStackedWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVector>

// ─────────────────────────────────────────────────────────────────────────────
// Recommendations — BMCC Resources screen
//
// Inherits Screen and provides a concrete implementation of the abstract
// interface. Demonstrates:
//   • Inheritance   — extends the custom Screen base class (not QWidget directly)
//   • Polymorphism  — overrides screenId() and onActivated() from Screen
// ─────────────────────────────────────────────────────────────────────────────

class Recommendations : public Screen {
    Q_OBJECT

public:
    explicit Recommendations(QWidget *parent = nullptr);

    // ── Implement pure virtual (required by Screen) ───────────────────────────
    QString screenId() const override { return "resources"; }

    // ── Override activation hook ──────────────────────────────────────────────
    // Called by MainWindow each time the user navigates to this screen.
    void onActivated() override;
    void onThemeChanged(bool dark) override;   // Feature 5: Zen Night readability

private:
    void applyTopicSearch(const QString &query);
    void openBestTopicMatch();
    void showTopic(int index);
    void showHome();

    QStackedWidget *m_stack;
    QLineEdit      *m_searchInput;
    QLabel         *m_searchStatus;
    QVector<QWidget*> m_topicCards;
    QVector<QLabel*>  m_topicSubtitleLabels;

    // Feature 5: landing-page header + search widgets that need re-theming
    bool          m_dark       = false;
    QLabel       *m_greeting   = nullptr;   // "WELCOME" eyebrow
    QLabel       *m_heading    = nullptr;   // big question heading
    QLabel       *m_sub        = nullptr;   // subtitle text
    QLabel       *m_searchIcon = nullptr;   // ⌕ icon label
    QPushButton  *m_searchBtn  = nullptr;   // "Search" button
};

#endif // RECOMMENDATIONS_H
