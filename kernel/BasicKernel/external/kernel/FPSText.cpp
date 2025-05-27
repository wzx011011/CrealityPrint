#include "FPSText.h"
#include <QDateTime>
#include <QPainter>

FPSText::FPSText(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , _currentFPS(0)
    , _cacheCount(0)
{
    _times.clear();
    setFlag(QQuickItem::ItemHasContents);
}

FPSText::~FPSText()
{
}

void FPSText::paint(QPainter *painter)
{
    recalculateFPS();

    painter->save();

    QBrush brush(Qt::yellow);

    painter->setBrush(brush);
    painter->setPen(Qt::NoPen);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRoundedRect(0, 0, boundingRect().width(), boundingRect().height(), 0, 0);

    painter->restore();
    update();
}

void FPSText::recalculateFPS()
{
    qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    _times.push_back(currentTime);

    while (currentTime - _times.at(0) > 1000) {
        _times.pop_front();
    }

    int currentCount = _times.length();
    _currentFPS = (currentCount + _cacheCount) / 2;

    if (currentCount != _cacheCount) {
        emit fpsChanged(_currentFPS);
    }
    _cacheCount = currentCount;
}