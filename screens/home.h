#ifndef HOME_H
#define HOME_H

#include "core/screen.h"

class Home : public Screen {
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);

    QString screenId() const override { return "home"; }

signals:
    void requestScreen(const QString &screenId);
};

#endif // HOME_H
