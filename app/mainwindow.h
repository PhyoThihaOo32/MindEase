#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QVector>

// Forward declarations
class Screen;
class FallingLeafOverlay;
class Journal;     // kept as concrete type to call setStoragePath()

// ─────────────────────────────────────────────────────────────────────────────
// MainWindow — top-level application window
//
// Owns the sidebar and a QStackedWidget of Screen* objects. Uses runtime
// polymorphism: screens are stored as Screen* and switchScreen() calls the
// virtual onActivated() on whichever concrete subclass is currently active.
// ─────────────────────────────────────────────────────────────────────────────

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void switchScreen(int index);
    void applyTheme(bool dark);        // Feature 5: theme toggle
    void applyFontSize(int px);        // Feature 5: font size

private:
    void buildNavigationBar();
    void applyStyle();
    void applyDarkStyle();
    void applyLightStyle();

    QWidget        *centralWidget;
    QVBoxLayout    *mainLayout;
    QWidget        *navShell;
    QStackedWidget *stack;
    FallingLeafOverlay *leafOverlay;

    QVector<QPushButton*> navButtons;
    QVector<Screen*>      screens;    // polymorphic ownership list
    Journal              *m_journal = nullptr;  // concrete ref for Settings→Journal updates
};

#endif // MAINWINDOW_H
