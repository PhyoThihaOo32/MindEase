#ifndef HOME_H
#define HOME_H

#include "core/screen.h"

class Home : public Screen {
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);

    QString screenId() const override { return "home"; }
    void onThemeChanged(bool dark) override;   // Feature 5: Zen Night readability

signals:
    void requestScreen(const QString &screenId);
};

#endif // HOME_H
