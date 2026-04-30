#ifndef SETTINGS_H
#define SETTINGS_H

#include "core/screen.h"
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QSlider>

// ─────────────────────────────────────────────────────────────────────────────
// Settings — QSettings-backed preferences screen.
//
// Demonstrates: Screen subclass, signals/slots, QSettings persistence,
// Q_PROPERTY-style preference propagation.
//
// Emits three signals after the user clicks "Apply":
//   themeChanged(bool dark)          → MainWindow swaps full stylesheet
//   fontSizeChanged(int px)          → MainWindow updates app base font
//   journalDirChanged(QString path)  → Journal reinitialises its storage
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
    void fontSizeChanged(int px);
    void journalDirChanged(const QString &path);

private slots:
    void onApply();
    void onBrowseDir();
    void onResetDir();

private:
    void loadFromSettings();
    void showStatus(const QString &msg, bool ok = true);

    QRadioButton *m_lightRadio  = nullptr;
    QRadioButton *m_darkRadio   = nullptr;
    QSlider      *m_fontSlider  = nullptr;
    QLabel       *m_fontSizeLbl = nullptr;
    QLineEdit    *m_dirEdit     = nullptr;
    QLabel       *m_statusLbl   = nullptr;

    // Feature 5: stored pointers for inline-styled widgets that need re-theming
    bool          m_dark         = false;
    QFrame       *m_appearCard   = nullptr;
    QFrame       *m_journalCard  = nullptr;
    // Page-level header
    QLabel       *m_eyebrowLbl   = nullptr;
    QLabel       *m_titleLbl     = nullptr;
    QLabel       *m_subLbl       = nullptr;
    // Appearance card interior
    QLabel       *m_appearLabel  = nullptr;
    QLabel       *m_themeLabel   = nullptr;
    QLabel       *m_themeDesc    = nullptr;
    QLabel       *m_fontLabel    = nullptr;
    QLabel       *m_fontDesc     = nullptr;
    QLabel       *m_minLbl       = nullptr;
    QLabel       *m_maxLbl       = nullptr;
    // Journal storage card interior
    QLabel       *m_journalSec   = nullptr;
    QLabel       *m_jLabel       = nullptr;
    QLabel       *m_jDesc        = nullptr;
};

#endif // SETTINGS_H
