// Include Guard
#ifndef MAINWINDOW_H // checks whether MAINWINDOW_H has not been defined yet. This prevents the header file from being included multiple times.
#define MAINWINDOW_H

#include <QMainWindow> // includes Qt's main window class. MainWindow inherits from this.
#include <QStackedWidget> // let the app hold multiple pages but slow only one at a time.
#include <QPushButton> // used for navigation buttons.
#include <QVBoxLayout> // vertical layout support - widgets can be arranged top-to-bottom.
#include <QHBoxLayout> // horizontal layout support - widgets can be arranged left-to-right.
#include <QLabel> // includes text/image label widgets.
#include <QResizeEvent> // includes resize event type - used when the app window changes size.
#include <QVector> // Qt's dynamic array container - similar to std::vector

// Forward declarations - it tells C++ that a Screen and FallingLeafOverlay class exists somewhere, without including its full header here.
class Screen; // - which reduces compile dependencies.
class FallingLeafOverlay;

// ─────────────────────────────────────────────────────────────────────────────
// MainWindow — top-level application window
//
// Owns the sidebar and a QStackedWidget of Screen* objects. Uses runtime
// polymorphism: screens are stored as Screen* and switchScreen() calls the
// virtual onActivated() on whichever concrete subclass is currently active.
// ─────────────────────────────────────────────────────────────────────────────


// declares a class named MainWindow - it inherits from QMainWindow, so it becomes a real Qt application window.
class MainWindow : public QMainWindow {
    Q_OBJECT // Qt macro required for signals, slots and Qt's meta-object system - without this Qt event system breaks - very important in Qt

public:
    MainWindow(QWidget *parent = nullptr); // Constructor - creates a MainWindow
    ~MainWindow(); // Destructor - runs when the MainWindow object is destroyed

protected:
    // this function overrides Qt's resize behavior - and run when the window size changes
    void resizeEvent(QResizeEvent *event) override;

    // Qr slot functions - and are private - can be connected Qt signals like button clicks
private slots:
    // this function changes the visible page in the app - index tells which screen to show
    void switchScreen(int index);
    // this function applies light or dark mode - if dark is true, dark theme is used
    void applyTheme(bool dark);

private:
    // helper function that creates the top/navigation buttons.
    void buildNavigationBar();
    // helpter function that applies the general app style.
    void applyStyle();
    // helper function that applies dark theme styling
    void applyDarkStyle();
    // helper function that applies light theme styling
    void applyLightStyle();

    QWidget        *centralWidget; // pointer to the main widget inside the window
    QVBoxLayout    *mainLayout; // main vertical layout - it arranges the app UI from top to bottom
    QWidget        *navShell; // pointer to QWidget that likely contains the navigation bar/buttons
    QStackedWidget *stack; // to hold all screens/pages and displays one at a time
    FallingLeafOverlay *leafOverlay; // pointer to the animated falling leaf overlay

    QVector<QPushButton*> navButtons; // a list of navigation button pointers
    QVector<Screen*>      screens;    // a list of screen pointers - this is POLYMORPHISM - where different screen classes can be stored as Screen*
};

#endif // end the include guard started at the top - this prevent duplicate inclusion.
