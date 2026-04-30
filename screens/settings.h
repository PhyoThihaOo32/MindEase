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
};

#endif // SETTINGS_H
