#ifndef FALLINGLEAFOVERLAY_H
#define FALLINGLEAFOVERLAY_H

#include <QVector>
#include <QWidget>

class QPaintEvent;
class QResizeEvent;
class QShowEvent;
class QTimer;

class FallingLeafOverlay : public QWidget {
    Q_OBJECT

public:
    explicit FallingLeafOverlay(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void advanceFrame();

private:
    struct Leaf {
        qreal x = 0.0;
        qreal y = 0.0;
        qreal size = 24.0;
        qreal fallSpeed = 0.8;
        qreal swayAmplitude = 12.0;
        qreal swayPhase = 0.0;
        qreal swaySpeed = 0.02;
        qreal drift = 0.0;
        qreal rotation = 0.0;
        qreal rotationSpeed = 0.0;
        qreal opacity = 0.18;
        qreal stretch = 2.8;
    };

    void seedLeaves();
    void resetLeaf(Leaf &leaf, bool randomY);

    QVector<Leaf> m_leaves;
    QTimer *m_timer;
};

#endif // FALLINGLEAFOVERLAY_H
