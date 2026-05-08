#ifndef LUCIDEICONS_H
#define LUCIDEICONS_H

#include <QColor>
#include <QIcon>
#include <QLabel>
#include <QPixmap>
#include <QString>

QString lucideIconNameForToken(const QString &token);
QPixmap lucidePixmap(const QString &tokenOrName,
                     const QColor &stroke = QColor("#173c2c"),
                     int size = 24);
QIcon lucideIcon(const QString &tokenOrName,
                 const QColor &stroke = QColor("#173c2c"),
                 int size = 24);
void setLucideIcon(QLabel *label,
                   const QString &tokenOrName,
                   const QColor &stroke = QColor("#173c2c"),
                   int size = 24);
QString lucideCleanText(QString text);

#endif // LUCIDEICONS_H
