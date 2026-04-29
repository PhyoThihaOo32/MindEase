#include "core/fallingleafoverlay.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QRandomGenerator>
#include <QResizeEvent>
#include <QShowEvent>
#include <QTimer>
#include <QtMath>

namespace {
qreal randomBetween(qreal min, qreal max) {
    return min + (QRandomGenerator::global()->generateDouble() * (max - min));
}

QPainterPath makeLeafPath(qreal width, qreal height) {
    QPainterPath silhouette;
    silhouette.moveTo(0.0, -height * 0.52);
    silhouette.cubicTo(width * 1.08, -height * 0.34,
                       width * 0.95, height * 0.10,
                       0.0, height * 0.56);
    silhouette.cubicTo(-width * 0.95, height * 0.10,
                       -width * 1.08, -height * 0.34,
                       0.0, -height * 0.52);
    return silhouette;
}

void drawBambooLeaf(QPainter &painter,
                    const QPointF &center,
                    qreal size,
                    qreal stretch,
                    qreal rotation,
                    qreal opacityScale) {
    const qreal width = size * 0.30;
    const qreal height = size * stretch;
    const QPainterPath silhouette = makeLeafPath(width, height);

    painter.save();
    painter.translate(center);
    painter.rotate(rotation);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(162, 207, 157, qRound(opacityScale * 44.0)));
    painter.drawPath(silhouette.translated(0.0, 4.0));

    QLinearGradient fill(0.0, -height * 0.55, 0.0, height * 0.58);
    fill.setColorAt(0.0, QColor(238, 247, 232, qRound(opacityScale * 72.0)));
    fill.setColorAt(0.34, QColor(142, 187, 133, qRound(opacityScale * 128.0)));
    fill.setColorAt(1.0, QColor(72, 121, 77, qRound(opacityScale * 86.0)));

    painter.setBrush(fill);
    painter.setPen(QPen(QColor(101, 152, 103, qRound(opacityScale * 68.0)), 0.9));
    painter.drawPath(silhouette);

    painter.setPen(QPen(QColor(249, 253, 246, qRound(opacityScale * 82.0)), 0.8));
    painter.drawLine(QPointF(0.0, -height * 0.40), QPointF(0.0, height * 0.40));
    painter.restore();
}

void drawLeafCluster(QPainter &painter,
                     const QPointF &center,
                     qreal size,
                     qreal opacityScale,
                     const QVector<qreal> &angles,
                     const QVector<QPointF> &offsets) {
    const int count = qMin(angles.size(), offsets.size());
    for (int index = 0; index < count; ++index) {
        drawBambooLeaf(painter,
                       center + offsets[index],
                       size * (0.78 + (index % 3) * 0.10),
                       3.9 + (index % 2) * 0.25,
                       angles[index],
                       opacityScale);
    }
}
}

FallingLeafOverlay::FallingLeafOverlay(QWidget *parent)
    : QWidget(parent),
      m_timer(new QTimer(this)) {
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setFocusPolicy(Qt::NoFocus);

    m_timer->setInterval(42);
    connect(m_timer, &QTimer::timeout, this, &FallingLeafOverlay::advanceFrame);
    m_timer->start();
}

void FallingLeafOverlay::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const qreal w = width();
    const qreal h = height();

    QRadialGradient washTop(QPointF(w * 0.86, h * 0.18), qMax(w, h) * 0.22);
    washTop.setColorAt(0.0, QColor(205, 230, 201, 52));
    washTop.setColorAt(0.55, QColor(220, 238, 215, 22));
    washTop.setColorAt(1.0, QColor(220, 238, 215, 0));
    painter.fillRect(QRectF(w * 0.62, 0.0, w * 0.38, h * 0.46), washTop);

    QRadialGradient washBottom(QPointF(w * 0.84, h * 0.70), qMax(w, h) * 0.24);
    washBottom.setColorAt(0.0, QColor(196, 224, 189, 42));
    washBottom.setColorAt(0.62, QColor(213, 234, 207, 14));
    washBottom.setColorAt(1.0, QColor(213, 234, 207, 0));
    painter.fillRect(QRectF(w * 0.56, h * 0.34, w * 0.44, h * 0.66), washBottom);

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(QColor(86, 132, 93, 68), 11.0, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(QPointF(w * 0.90, h * 0.12), QPointF(w * 0.88, h * 0.94));
    painter.drawLine(QPointF(w * 0.95, h * 0.20), QPointF(w * 0.93, h * 0.88));
    painter.setPen(QPen(QColor(65, 103, 74, 42), 8.0, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(QPointF(w * 0.84, h * 0.36), QPointF(w * 0.82, h * 0.96));

    painter.setPen(QPen(QColor(73, 112, 82, 52), 2.0, Qt::SolidLine, Qt::RoundCap));
    const QVector<qreal> stemNodes = {0.22, 0.38, 0.56, 0.74, 0.88};
    for (qreal node : stemNodes) {
        const qreal y1 = h * node;
        painter.drawLine(QPointF(w * 0.872, y1), QPointF(w * 0.906, y1));
        painter.drawLine(QPointF(w * 0.922, y1 - 6.0), QPointF(w * 0.952, y1 - 6.0));
    }

    painter.setPen(QPen(QColor(80, 122, 86, 48), 1.8, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(QPointF(w * 0.89, h * 0.18), QPointF(w * 0.80, h * 0.12));
    painter.drawLine(QPointF(w * 0.90, h * 0.31), QPointF(w * 0.80, h * 0.28));
    painter.drawLine(QPointF(w * 0.88, h * 0.50), QPointF(w * 0.77, h * 0.42));
    painter.drawLine(QPointF(w * 0.92, h * 0.57), QPointF(w * 0.98, h * 0.49));
    painter.drawLine(QPointF(w * 0.90, h * 0.72), QPointF(w * 0.80, h * 0.66));
    painter.restore();

    drawLeafCluster(painter, QPointF(w * 0.82, h * 0.14), 28.0, 1.0,
                    {-54.0, -24.0, 2.0, 28.0, 52.0},
                    {QPointF(-24.0, 6.0), QPointF(-12.0, -8.0), QPointF(0.0, -14.0), QPointF(14.0, -6.0), QPointF(24.0, 8.0)});
    drawLeafCluster(painter, QPointF(w * 0.88, h * 0.28), 24.0, 0.88,
                    {-62.0, -28.0, 12.0, 38.0},
                    {QPointF(-18.0, 4.0), QPointF(-8.0, -8.0), QPointF(10.0, -10.0), QPointF(22.0, 4.0)});
    drawLeafCluster(painter, QPointF(w * 0.78, h * 0.42), 25.0, 0.92,
                    {-58.0, -26.0, 8.0, 34.0, 58.0},
                    {QPointF(-24.0, 10.0), QPointF(-11.0, -3.0), QPointF(0.0, -12.0), QPointF(16.0, -4.0), QPointF(26.0, 10.0)});
    drawLeafCluster(painter, QPointF(w * 0.89, h * 0.60), 30.0, 1.0,
                    {-66.0, -34.0, -4.0, 20.0, 48.0},
                    {QPointF(-28.0, 10.0), QPointF(-14.0, -2.0), QPointF(0.0, -16.0), QPointF(15.0, -6.0), QPointF(30.0, 8.0)});
    drawLeafCluster(painter, QPointF(w * 0.79, h * 0.70), 22.0, 0.82,
                    {-52.0, -18.0, 10.0, 34.0},
                    {QPointF(-18.0, 8.0), QPointF(-8.0, -2.0), QPointF(10.0, -8.0), QPointF(22.0, 6.0)});

    for (const Leaf &leaf : m_leaves) {
        const qreal x = leaf.x + qSin(leaf.swayPhase) * leaf.swayAmplitude;
        drawBambooLeaf(painter, QPointF(x, leaf.y), leaf.size, leaf.stretch, leaf.rotation, leaf.opacity);
    }
}

void FallingLeafOverlay::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    seedLeaves();
}

void FallingLeafOverlay::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    seedLeaves();
}

void FallingLeafOverlay::advanceFrame() {
    if (width() <= 0 || height() <= 0) {
        return;
    }

    for (Leaf &leaf : m_leaves) {
        leaf.y += leaf.fallSpeed;
        leaf.x += leaf.drift;
        leaf.swayPhase += leaf.swaySpeed;
        leaf.rotation += leaf.rotationSpeed;

        if (leaf.y > height() + (leaf.size * leaf.stretch * 1.3) ||
            leaf.x < -(leaf.size * 4.0) ||
            leaf.x > width() + (leaf.size * 4.0)) {
            resetLeaf(leaf, false);
        }
    }

    update();
}

void FallingLeafOverlay::seedLeaves() {
    if (width() <= 0 || height() <= 0) {
        return;
    }

    const int targetCount = qBound(10, width() / 150, 18);

    m_leaves.resize(targetCount);
    for (Leaf &leaf : m_leaves) {
        resetLeaf(leaf, true);
    }

    update();
}

void FallingLeafOverlay::resetLeaf(Leaf &leaf, bool randomY) {
    leaf.x = randomBetween(-width() * 0.06, width() * 1.06);
    leaf.y = randomY
        ? randomBetween(-height() * 0.15, height() * 1.02)
        : randomBetween(-height() * 0.28, -24.0);
    leaf.size = randomBetween(12.0, 22.0);
    leaf.fallSpeed = randomBetween(0.16, 0.42);
    leaf.swayAmplitude = randomBetween(10.0, 24.0);
    leaf.swayPhase = randomBetween(0.0, 6.28318530718);
    leaf.swaySpeed = randomBetween(0.010, 0.022);
    leaf.drift = randomBetween(-0.12, 0.12);
    leaf.rotation = randomBetween(-28.0, 28.0);
    leaf.rotationSpeed = randomBetween(-0.18, 0.18);
    leaf.opacity = randomBetween(0.14, 0.24);
    leaf.stretch = randomBetween(3.6, 4.5);
}
