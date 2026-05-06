#ifndef SETTINGS_H
#define SETTINGS_H

#include "core/screen.h"
#include <QLabel>
#include <QRadioButton>

// ─────────────────────────────────────────────────────────────────────────────
// Settings — QSettings-backed preferences screen.
//
// Demonstrates: Screen subclass, signals/slots, QSettings persistence,
// Q_PROPERTY-style preference propagation.
//
// Emits after the user clicks "Apply":
//   themeChanged(bool dark)          → MainWindow swaps full stylesheet
// ─────────────────────────────────────────────────────────────────────────────
class Settings : public Screen {
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    QString screenId() const override { return "settings"; }
    void onActivated() override;
    void onThemeChanged(bool dark) override;   // Feature 5: Zen Night readability

signals:
    void themeChanged(bool dark);

private slots:
    void onApply();

private:
    void loadFromSettings();
    void showStatus(const QString &msg, bool ok = true);

    QRadioButton *m_lightRadio  = nullptr;
    QRadioButton *m_darkRadio   = nullptr;
    QLabel       *m_statusLbl   = nullptr;

    // Feature 5: stored pointers for inline-styled widgets that need re-theming
    bool          m_dark         = false;
    QFrame       *m_appearCard   = nullptr;
    // Page-level header
    QLabel       *m_eyebrowLbl   = nullptr;
    QLabel       *m_titleLbl     = nullptr;
    QLabel       *m_subLbl       = nullptr;
    // Appearance card interior
    QLabel       *m_appearLabel  = nullptr;
    QLabel       *m_themeLabel   = nullptr;
    QLabel       *m_themeDesc    = nullptr;
};

#endif // SETTINGS_H
