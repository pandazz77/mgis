#pragma once

#include <QString>
#include <QColor>
#include <QFont>

class TextPixmap{
    public:
        TextPixmap(const QString &str="", const QFont &font=QFont(),
            const QColor& textColor = Qt::black, const QColor &bgColor=Qt::transparent);
        
        QPixmap pixmap();

        QFont font;
        QString str;
        QColor textColor;
        QColor bgColor;
};