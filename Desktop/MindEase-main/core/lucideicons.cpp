#include "core/lucideicons.h"

#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPolygonF>
#include <QRegularExpression>
#include <QtGlobal>

#include <cmath>

namespace {

bool has(const QString &token, const QString &needle) {
    return token.contains(needle);
}

void drawBook(QPainter &p) {
    p.drawRoundedRect(QRectF(4, 5, 7, 14), 1.5, 1.5);
    p.drawRoundedRect(QRectF(13, 5, 7, 14), 1.5, 1.5);
    p.drawLine(QPointF(11, 6), QPointF(11, 20));
    p.drawLine(QPointF(13, 6), QPointF(13, 20));
}

void drawDollar(QPainter &p) {
    QFont f = p.font();
    f.setPixelSize(17);
    f.setBold(true);
    p.setFont(f);
    p.drawLine(QPointF(12, 3), QPointF(12, 21));
    p.drawText(QRectF(4, 2, 16, 20), Qt::AlignCenter, "S");
}

void drawGlobe(QPainter &p) {
    p.drawEllipse(QRectF(4, 4, 16, 16));
    p.drawLine(QPointF(4, 12), QPointF(20, 12));
    p.drawArc(QRectF(7, 4, 10, 16), 90 * 16, 180 * 16);
    p.drawArc(QRectF(7, 4, 10, 16), -90 * 16, 180 * 16);
}

void drawMessage(QPainter &p) {
    QPainterPath path;
    path.addRoundedRect(QRectF(4, 5, 16, 12), 4, 4);
    p.drawPath(path);
    p.drawLine(QPointF(9, 17), QPointF(6, 21));
    p.drawLine(QPointF(9, 17), QPointF(12, 17));
}

void drawHospital(QPainter &p) {
    p.drawRoundedRect(QRectF(5, 4, 14, 17), 1.5, 1.5);
    p.drawLine(QPointF(12, 8), QPointF(12, 15));
    p.drawLine(QPointF(8.5, 11.5), QPointF(15.5, 11.5));
    p.drawLine(QPointF(8, 21), QPointF(8, 17));
    p.drawLine(QPointF(16, 21), QPointF(16, 17));
}

void drawBriefcase(QPainter &p) {
    p.drawRoundedRect(QRectF(4, 8, 16, 11), 2, 2);
    p.drawRoundedRect(QRectF(9, 5, 6, 3), 1, 1);
    p.drawLine(QPointF(4, 12), QPointF(20, 12));
}

void drawHandshake(QPainter &p) {
    p.drawLine(QPointF(4, 12), QPointF(8, 8));
    p.drawLine(QPointF(20, 12), QPointF(16, 8));
    p.drawLine(QPointF(8, 8), QPointF(11, 11));
    p.drawLine(QPointF(16, 8), QPointF(13, 11));
    p.drawLine(QPointF(9, 13), QPointF(12, 16));
    p.drawLine(QPointF(12, 16), QPointF(15, 13));
    p.drawLine(QPointF(7, 14), QPointF(11, 18));
    p.drawLine(QPointF(17, 14), QPointF(13, 18));
}

void drawPencil(QPainter &p) {
    p.drawLine(QPointF(5, 19), QPointF(16.5, 7.5));
    p.drawLine(QPointF(14, 5), QPointF(19, 10));
    p.drawLine(QPointF(16.5, 7.5), QPointF(14, 5));
    p.drawLine(QPointF(19, 10), QPointF(7.5, 21.5));
    p.drawLine(QPointF(5, 19), QPointF(7.5, 21.5));
}

void drawBrain(QPainter &p) {
    p.drawArc(QRectF(4, 6, 8, 8), 40 * 16, 230 * 16);
    p.drawArc(QRectF(12, 6, 8, 8), -90 * 16, 230 * 16);
    p.drawArc(QRectF(6, 11, 7, 8), 110 * 16, 180 * 16);
    p.drawArc(QRectF(11, 11, 7, 8), -110 * 16, 180 * 16);
    p.drawLine(QPointF(12, 5), QPointF(12, 20));
}

void drawCalendar(QPainter &p) {
    p.drawRoundedRect(QRectF(4, 5, 16, 15), 2, 2);
    p.drawLine(QPointF(4, 9), QPointF(20, 9));
    p.drawLine(QPointF(8, 3), QPointF(8, 7));
    p.drawLine(QPointF(16, 3), QPointF(16, 7));
}

void drawPuzzle(QPainter &p) {
    QPainterPath path;
    path.moveTo(6, 6);
    path.lineTo(10, 6);
    path.cubicTo(10, 3.5, 14, 3.5, 14, 6);
    path.lineTo(18, 6);
    path.lineTo(18, 11);
    path.cubicTo(20.5, 11, 20.5, 15, 18, 15);
    path.lineTo(18, 20);
    path.lineTo(6, 20);
    path.lineTo(6, 15);
    path.cubicTo(8.5, 15, 8.5, 11, 6, 11);
    path.closeSubpath();
    p.drawPath(path);
}

void drawCoffee(QPainter &p) {
    p.drawRoundedRect(QRectF(5, 9, 11, 8), 2, 2);
    p.drawArc(QRectF(14, 10, 6, 5), -80 * 16, 260 * 16);
    p.drawLine(QPointF(5, 20), QPointF(18, 20));
    p.drawLine(QPointF(8, 5), QPointF(8, 7));
    p.drawLine(QPointF(12, 4), QPointF(12, 7));
}

void drawSchool(QPainter &p) {
    p.drawPolyline(QPolygonF() << QPointF(4, 10) << QPointF(12, 5) << QPointF(20, 10));
    p.drawRect(QRectF(6, 10, 12, 10));
    p.drawLine(QPointF(9, 20), QPointF(9, 14));
    p.drawLine(QPointF(15, 20), QPointF(15, 14));
    p.drawLine(QPointF(4, 20), QPointF(20, 20));
}

void drawUsers(QPainter &p) {
    p.drawEllipse(QRectF(7, 5, 6, 6));
    p.drawEllipse(QRectF(14, 7, 5, 5));
    p.drawArc(QRectF(4, 13, 12, 8), 0, 180 * 16);
    p.drawArc(QRectF(12, 14, 10, 7), 0, 180 * 16);
}

void drawPhone(QPainter &p) {
    QPainterPath path;
    path.moveTo(7, 5);
    path.cubicTo(6, 7, 6, 12, 11, 17);
    path.cubicTo(15, 21, 18, 20, 20, 17);
    p.drawPath(path);
    p.drawLine(QPointF(7, 5), QPointF(10, 8));
    p.drawLine(QPointF(17, 15), QPointF(20, 17));
}

void drawRainbow(QPainter &p) {
    p.drawArc(QRectF(4, 8, 16, 16), 0, 180 * 16);
    p.drawArc(QRectF(7, 11, 10, 10), 0, 180 * 16);
    p.drawArc(QRectF(10, 14, 4, 4), 0, 180 * 16);
}

void drawHeart(QPainter &p) {
    QPainterPath path;
    path.moveTo(12, 20);
    path.cubicTo(5, 15, 4, 11, 6, 8);
    path.cubicTo(8, 5, 11, 6, 12, 8);
    path.cubicTo(13, 6, 16, 5, 18, 8);
    path.cubicTo(20, 11, 19, 15, 12, 20);
    p.drawPath(path);
}

void drawLeaf(QPainter &p) {
    QPainterPath path;
    path.moveTo(5, 19);
    path.cubicTo(7, 8, 15, 5, 20, 5);
    path.cubicTo(20, 14, 14, 20, 5, 19);
    p.drawPath(path);
    p.drawLine(QPointF(5, 19), QPointF(16, 8));
}

void drawHeadphones(QPainter &p) {
    p.drawArc(QRectF(5, 5, 14, 14), 20 * 16, 140 * 16);
    p.drawRoundedRect(QRectF(5, 13, 4, 6), 1, 1);
    p.drawRoundedRect(QRectF(15, 13, 4, 6), 1, 1);
}

void drawMusic(QPainter &p) {
    p.drawLine(QPointF(15, 5), QPointF(15, 16));
    p.drawLine(QPointF(15, 5), QPointF(20, 4));
    p.drawEllipse(QRectF(10, 15, 5, 4));
    p.drawLine(QPointF(8, 8), QPointF(8, 18));
    p.drawEllipse(QRectF(3, 17, 5, 4));
}

void drawMoon(QPainter &p) {
    QPainterPath path;
    path.moveTo(17, 19);
    path.cubicTo(9, 20, 4, 14, 6, 7);
    path.cubicTo(8, 11, 13, 15, 19, 14);
    path.cubicTo(18, 16, 18, 18, 17, 19);
    p.drawPath(path);
}

void drawApple(QPainter &p) {
    p.drawArc(QRectF(5, 8, 8, 11), 30 * 16, 250 * 16);
    p.drawArc(QRectF(11, 8, 8, 11), -100 * 16, 250 * 16);
    p.drawLine(QPointF(12, 8), QPointF(12, 5));
    p.drawArc(QRectF(12, 4, 5, 4), 120 * 16, 150 * 16);
}

void drawDumbbell(QPainter &p) {
    p.drawLine(QPointF(8, 12), QPointF(16, 12));
    p.drawRect(QRectF(4, 9, 3, 6));
    p.drawRect(QRectF(17, 9, 3, 6));
    p.drawLine(QPointF(7, 10), QPointF(8, 10));
    p.drawLine(QPointF(16, 10), QPointF(17, 10));
    p.drawLine(QPointF(7, 14), QPointF(8, 14));
    p.drawLine(QPointF(16, 14), QPointF(17, 14));
}

void drawTree(QPainter &p) {
    p.drawLine(QPointF(12, 14), QPointF(12, 21));
    p.drawLine(QPointF(12, 16), QPointF(8, 12));
    p.drawLine(QPointF(12, 16), QPointF(16, 12));
    p.drawArc(QRectF(5, 4, 14, 11), 200 * 16, 300 * 16);
}

void drawLightbulb(QPainter &p) {
    p.drawArc(QRectF(7, 4, 10, 11), 20 * 16, 320 * 16);
    p.drawLine(QPointF(9, 16), QPointF(15, 16));
    p.drawLine(QPointF(10, 20), QPointF(14, 20));
    p.drawLine(QPointF(10, 16), QPointF(10, 20));
    p.drawLine(QPointF(14, 16), QPointF(14, 20));
}

void drawLock(QPainter &p) {
    p.drawRoundedRect(QRectF(5, 10, 14, 10), 2, 2);
    p.drawArc(QRectF(8, 4, 8, 10), 0, 180 * 16);
}

void drawScale(QPainter &p) {
    p.drawLine(QPointF(12, 4), QPointF(12, 20));
    p.drawLine(QPointF(7, 20), QPointF(17, 20));
    p.drawLine(QPointF(5, 8), QPointF(19, 8));
    p.drawLine(QPointF(7, 8), QPointF(4, 14));
    p.drawLine(QPointF(7, 8), QPointF(10, 14));
    p.drawLine(QPointF(17, 8), QPointF(14, 14));
    p.drawLine(QPointF(17, 8), QPointF(20, 14));
    p.drawLine(QPointF(4, 14), QPointF(10, 14));
    p.drawLine(QPointF(14, 14), QPointF(20, 14));
}

void drawFile(QPainter &p) {
    p.drawRect(QRectF(6, 4, 12, 16));
    p.drawLine(QPointF(14, 4), QPointF(18, 8));
    p.drawLine(QPointF(14, 4), QPointF(14, 8));
    p.drawLine(QPointF(14, 8), QPointF(18, 8));
    p.drawLine(QPointF(8, 13), QPointF(16, 13));
    p.drawLine(QPointF(8, 16), QPointF(14, 16));
}

void drawRocket(QPainter &p) {
    QPainterPath path;
    path.moveTo(7, 17);
    path.cubicTo(8, 9, 13, 5, 19, 4);
    path.cubicTo(18, 10, 15, 15, 7, 17);
    path.closeSubpath();
    p.drawPath(path);
    p.drawEllipse(QRectF(13, 8, 3, 3));
    p.drawLine(QPointF(7, 17), QPointF(4, 20));
    p.drawLine(QPointF(9, 15), QPointF(11, 20));
}

void drawTarget(QPainter &p) {
    p.drawEllipse(QRectF(4, 4, 16, 16));
    p.drawEllipse(QRectF(8, 8, 8, 8));
    p.drawEllipse(QRectF(11, 11, 2, 2));
}

void drawMonitor(QPainter &p) {
    p.drawRoundedRect(QRectF(4, 5, 16, 11), 1.5, 1.5);
    p.drawLine(QPointF(10, 20), QPointF(14, 20));
    p.drawLine(QPointF(12, 16), QPointF(12, 20));
}

void drawHand(QPainter &p) {
    p.drawRoundedRect(QRectF(8, 6, 3, 10), 1.3, 1.3);
    p.drawRoundedRect(QRectF(11, 5, 3, 11), 1.3, 1.3);
    p.drawRoundedRect(QRectF(14, 7, 3, 9), 1.3, 1.3);
    QPainterPath palm;
    palm.moveTo(7, 14);
    palm.cubicTo(5, 14, 5, 17, 8, 20);
    palm.lineTo(16, 20);
    palm.cubicTo(19, 17, 19, 12, 17, 10);
    p.drawPath(palm);
}

void drawAccessibility(QPainter &p) {
    p.drawEllipse(QRectF(10, 3, 4, 4));
    p.drawLine(QPointF(12, 8), QPointF(12, 14));
    p.drawLine(QPointF(6, 10), QPointF(18, 10));
    p.drawLine(QPointF(12, 14), QPointF(8, 21));
    p.drawLine(QPointF(12, 14), QPointF(16, 21));
}

void drawCircleAlert(QPainter &p) {
    p.drawEllipse(QRectF(4, 4, 16, 16));
    p.drawLine(QPointF(12, 8), QPointF(12, 13));
    p.drawPoint(QPointF(12, 16));
}

void drawLandmark(QPainter &p) {
    p.drawPolyline(QPolygonF() << QPointF(4, 9) << QPointF(12, 5) << QPointF(20, 9));
    p.drawLine(QPointF(5, 20), QPointF(19, 20));
    p.drawLine(QPointF(6, 10), QPointF(18, 10));
    for (int x : {7, 11, 15}) {
        p.drawLine(QPointF(x, 10), QPointF(x, 20));
    }
}

void drawGraduationCap(QPainter &p) {
    p.drawPolyline(QPolygonF() << QPointF(3, 9) << QPointF(12, 5) << QPointF(21, 9) << QPointF(12, 13) << QPointF(3, 9));
    p.drawLine(QPointF(7, 12), QPointF(7, 16));
    p.drawArc(QRectF(7, 12, 10, 7), 200 * 16, 140 * 16);
    p.drawLine(QPointF(19, 10), QPointF(19, 16));
}

void drawClipboard(QPainter &p) {
    p.drawRoundedRect(QRectF(6, 5, 12, 16), 2, 2);
    p.drawRoundedRect(QRectF(9, 3, 6, 4), 1.5, 1.5);
    p.drawLine(QPointF(9, 12), QPointF(15, 12));
    p.drawLine(QPointF(9, 16), QPointF(14, 16));
}

void drawHome(QPainter &p) {
    p.drawPolyline(QPolygonF() << QPointF(4, 12) << QPointF(12, 5) << QPointF(20, 12));
    p.drawRoundedRect(QRectF(6, 11, 12, 9), 1.5, 1.5);
    p.drawLine(QPointF(11, 20), QPointF(11, 15));
    p.drawLine(QPointF(14, 20), QPointF(14, 15));
}

void drawMedal(QPainter &p) {
    p.drawLine(QPointF(8, 4), QPointF(11, 10));
    p.drawLine(QPointF(16, 4), QPointF(13, 10));
    p.drawEllipse(QRectF(7, 10, 10, 10));
    p.drawLine(QPointF(12, 13), QPointF(12, 17));
}

void drawShield(QPainter &p) {
    QPainterPath path;
    path.moveTo(12, 4);
    path.cubicTo(15, 6, 18, 6, 20, 7);
    path.cubicTo(19, 15, 16, 19, 12, 21);
    path.cubicTo(8, 19, 5, 15, 4, 7);
    path.cubicTo(6, 6, 9, 6, 12, 4);
    p.drawPath(path);
}

void drawStethoscope(QPainter &p) {
    p.drawArc(QRectF(5, 5, 8, 10), 180 * 16, 180 * 16);
    p.drawArc(QRectF(11, 5, 8, 10), 180 * 16, 180 * 16);
    p.drawLine(QPointF(9, 15), QPointF(9, 18));
    p.drawLine(QPointF(15, 15), QPointF(15, 18));
    p.drawArc(QRectF(9, 14, 8, 7), 180 * 16, 180 * 16);
    p.drawEllipse(QRectF(17, 17, 3, 3));
}

void drawToolbox(QPainter &p) {
    p.drawRoundedRect(QRectF(4, 9, 16, 10), 2, 2);
    p.drawRoundedRect(QRectF(9, 5, 6, 4), 1.5, 1.5);
    p.drawLine(QPointF(4, 13), QPointF(20, 13));
    p.drawLine(QPointF(12, 12), QPointF(12, 15));
}

void drawMicroscope(QPainter &p) {
    p.drawLine(QPointF(14, 4), QPointF(9, 12));
    p.drawRoundedRect(QRectF(8, 11, 5, 4), 1.5, 1.5);
    p.drawLine(QPointF(11, 15), QPointF(11, 19));
    p.drawLine(QPointF(6, 20), QPointF(18, 20));
    p.drawArc(QRectF(9, 10, 9, 8), 250 * 16, 220 * 16);
}

void drawSearch(QPainter &p) {
    p.drawEllipse(QRectF(5, 5, 10, 10));
    p.drawLine(QPointF(14, 14), QPointF(20, 20));
}

void drawDefault(QPainter &p) {
    p.drawRoundedRect(QRectF(5, 5, 14, 14), 4, 4);
    p.drawLine(QPointF(9, 12), QPointF(15, 12));
    p.drawLine(QPointF(12, 9), QPointF(12, 15));
}

} // namespace

QString lucideIconNameForToken(const QString &token) {
    const QString t = token.trimmed().toLower();

    if (t == "search" || has(token, "🔍") || has(token, "⌕")) return "search";
    if (has(token, "📚") || has(token, "📖") || has(token, "📘")) return "book-open";
    if (has(token, "💰")) return "dollar-sign";
    if (has(token, "🌐")) return "globe-2";
    if (has(token, "💬")) return "message-circle";
    if (has(token, "🏥")) return "hospital";
    if (has(token, "💼")) return "briefcase";
    if (has(token, "🤝") || has(token, "🫂")) return "handshake";
    if (has(token, "✏") || has(token, "📝")) return "pencil";
    if (has(token, "🎯")) return "target";
    if (has(token, "💻")) return "monitor";
    if (has(token, "🧠")) return "brain";
    if (has(token, "🗓") || has(token, "📅")) return "calendar";
    if (has(token, "🧩")) return "puzzle";
    if (has(token, "☕")) return "coffee";
    if (has(token, "🏫")) return "school";
    if (has(token, "🌈") || has(token, "🏳")) return "rainbow";
    if (has(token, "✊")) return "hand";
    if (has(token, "♿")) return "accessibility";
    if (has(token, "🧘")) return "leaf";
    if (has(token, "💛")) return "heart";
    if (has(token, "🌿")) return "leaf";
    if (has(token, "🎧")) return "headphones";
    if (has(token, "▶")) return "play";
    if (has(token, "🎵")) return "music";
    if (has(token, "😴") || has(token, "🌙")) return "moon";
    if (has(token, "🥗") || has(token, "🍎")) return "apple";
    if (has(token, "🏋") || has(token, "💪")) return "dumbbell";
    if (has(token, "🌳")) return "tree";
    if (has(token, "⏰")) return "alarm-clock";
    if (has(token, "💡")) return "lightbulb";
    if (has(token, "📵")) return "phone-off";
    if (has(token, "🆘")) return "circle-alert";
    if (has(token, "🏛")) return "landmark";
    if (has(token, "⚖")) return "scale";
    if (has(token, "🎓")) return "graduation-cap";
    if (has(token, "🔒")) return "lock";
    if (has(token, "📋")) return "clipboard";
    if (has(token, "🏘")) return "home";
    if (has(token, "🎖")) return "medal";
    if (has(token, "☎") || has(token, "📞")) return "phone";
    if (has(token, "🪖")) return "shield";
    if (has(token, "🩺")) return "stethoscope";
    if (has(token, "🧰")) return "toolbox";
    if (has(token, "📄")) return "file-text";
    if (has(token, "🚀")) return "rocket";
    if (has(token, "🔬")) return "microscope";
    if (has(token, "🕐")) return "clock";
    if (has(token, "📍")) return "map-pin";
    if (has(token, "📧")) return "mail";

    return t.isEmpty() ? "circle" : t;
}

QPixmap lucidePixmap(const QString &tokenOrName, const QColor &stroke, int size) {
    const int safeSize = qMax(12, size);
    QPixmap pixmap(safeSize, safeSize);
    pixmap.fill(Qt::transparent);

    QPainter p(&pixmap);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.scale(safeSize / 24.0, safeSize / 24.0);

    QPen pen(stroke, 1.9, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    p.setPen(pen);
    p.setBrush(Qt::NoBrush);

    const QString icon = lucideIconNameForToken(tokenOrName);
    if (icon == "book-open") drawBook(p);
    else if (icon == "dollar-sign") drawDollar(p);
    else if (icon == "globe-2") drawGlobe(p);
    else if (icon == "message-circle") drawMessage(p);
    else if (icon == "hospital") drawHospital(p);
    else if (icon == "briefcase") drawBriefcase(p);
    else if (icon == "handshake") drawHandshake(p);
    else if (icon == "pencil") drawPencil(p);
    else if (icon == "brain") drawBrain(p);
    else if (icon == "calendar") drawCalendar(p);
    else if (icon == "puzzle") drawPuzzle(p);
    else if (icon == "coffee") drawCoffee(p);
    else if (icon == "school") drawSchool(p);
    else if (icon == "users") drawUsers(p);
    else if (icon == "phone") drawPhone(p);
    else if (icon == "rainbow") drawRainbow(p);
    else if (icon == "heart") drawHeart(p);
    else if (icon == "leaf") drawLeaf(p);
    else if (icon == "headphones") drawHeadphones(p);
    else if (icon == "music") drawMusic(p);
    else if (icon == "moon") drawMoon(p);
    else if (icon == "apple") drawApple(p);
    else if (icon == "dumbbell") drawDumbbell(p);
    else if (icon == "tree") drawTree(p);
    else if (icon == "lightbulb") drawLightbulb(p);
    else if (icon == "lock") drawLock(p);
    else if (icon == "scale") drawScale(p);
    else if (icon == "file-text") drawFile(p);
    else if (icon == "rocket") drawRocket(p);
    else if (icon == "target") drawTarget(p);
    else if (icon == "monitor") drawMonitor(p);
    else if (icon == "hand") drawHand(p);
    else if (icon == "accessibility") drawAccessibility(p);
    else if (icon == "phone-off") {
        drawPhone(p);
        p.drawLine(QPointF(5, 5), QPointF(19, 19));
    }
    else if (icon == "circle-alert") drawCircleAlert(p);
    else if (icon == "landmark") drawLandmark(p);
    else if (icon == "graduation-cap") drawGraduationCap(p);
    else if (icon == "clipboard") drawClipboard(p);
    else if (icon == "home") drawHome(p);
    else if (icon == "medal") drawMedal(p);
    else if (icon == "shield") drawShield(p);
    else if (icon == "stethoscope") drawStethoscope(p);
    else if (icon == "toolbox") drawToolbox(p);
    else if (icon == "microscope") drawMicroscope(p);
    else if (icon == "search") drawSearch(p);
    else if (icon == "play") {
        QPainterPath path;
        path.moveTo(8, 5);
        path.lineTo(19, 12);
        path.lineTo(8, 19);
        path.closeSubpath();
        p.drawPath(path);
    } else if (icon == "clock" || icon == "alarm-clock") {
        p.drawEllipse(QRectF(5, 5, 14, 14));
        p.drawLine(QPointF(12, 8), QPointF(12, 12));
        p.drawLine(QPointF(12, 12), QPointF(15, 14));
    } else if (icon == "map-pin") {
        p.drawEllipse(QRectF(9, 8, 6, 6));
        QPainterPath path;
        path.moveTo(12, 21);
        path.cubicTo(6, 15, 6, 5, 12, 4);
        path.cubicTo(18, 5, 18, 15, 12, 21);
        p.drawPath(path);
    } else if (icon == "mail") {
        p.drawRoundedRect(QRectF(4, 6, 16, 12), 2, 2);
        p.drawLine(QPointF(4, 8), QPointF(12, 13));
        p.drawLine(QPointF(20, 8), QPointF(12, 13));
    } else {
        drawDefault(p);
    }

    return pixmap;
}

QIcon lucideIcon(const QString &tokenOrName, const QColor &stroke, int size) {
    return QIcon(lucidePixmap(tokenOrName, stroke, size));
}

void setLucideIcon(QLabel *label, const QString &tokenOrName, const QColor &stroke, int size) {
    if (!label) return;
    label->setText(QString{});
    label->setPixmap(lucidePixmap(tokenOrName, stroke, size));
    label->setAlignment(Qt::AlignCenter);
}

QString lucideCleanText(QString text) {
    const QList<QPair<QString, QString>> replacements = {
        { "📍", "Location:" },
        { "📞", "Phone:" },
        { "☎️", "Phone:" },
        { "📧", "Email:" },
        { "🕐", "Hours:" },
        { "👤", "Contact:" },
        { "✓", "" },
        { "🔍", "" }
    };

    for (const auto &replacement : replacements) {
        text.replace(replacement.first, replacement.second);
    }
    text.replace(QRegularExpression("[\\t ]+"), " ");
    text.replace(QRegularExpression(" *\\n *"), "\n");
    return text.trimmed().replace(" | ", "  |  ");
}
