#ifndef FPSTEXT_H_7FE54CFA_E610_4C32_A4D7_A7AF0E56FF04
#define FPSTEXT_H_7FE54CFA_E610_4C32_A4D7_A7AF0E56FF04

#include <QQuickPaintedItem>

class FPSText : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
public:
    FPSText(QQuickItem *parent = nullptr);
    ~FPSText();

    void paint(QPainter *painter);

    int fps() const {
        return _currentFPS;
    }

signals:
    void fpsChanged(int);

private:
    void recalculateFPS();
    int _currentFPS;
    int _cacheCount;
    QVector<qint64> _times;
};

#endif // FPSTEXT_H_7FE54CFA_E610_4C32_A4D7_A7AF0E56FF04