#include "TextPixmap.h"

#include <QFontMetrics>
#include <QPixmap>
#include <QPainter>

TextPixmap::TextPixmap(const QString &str, const QFont &font, const QColor& textColor, const QColor &bgColor):
    str(str),font(font),textColor(textColor),bgColor(bgColor){

}

QPixmap TextPixmap::pixmap(){
    QFontMetrics fm(font);
    QSize textSize = fm.size(Qt::TextSingleLine,str);
    QSize pixSize(textSize*1.5);

    QPixmap pix(pixSize);
    pix.fill(bgColor);
    QPainter painter(&pix);
    QPen pen(textColor);
    pen.setCosmetic(true);
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(10,10,str);
    return pix;
}
