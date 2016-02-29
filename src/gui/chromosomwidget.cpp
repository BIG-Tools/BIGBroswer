#include "chromosomwidget.h"
#include "app.h"

namespace big {
namespace gui {
ChromosomWidget::ChromosomWidget(QWidget * parent)
    :QWidget(parent)
{

    QColor base = Qt::lightGray;

    mStains["gneg"] = base;
    mStains["gpos25"] = base.darker(150);
    mStains["gpos50"] = base.darker(200);
    mStains["gpos75"] = base.darker(250);
    mStains["gpos100"] =base.darker(300);
    mStains["gvar"] = Qt::red;
    mStains["acen"] = Qt::red;

    mSelectorMin = 200;
    mSelectorMax = 600;





}

void ChromosomWidget::loadCytoBand(const QString &filename)
{
    CytobandReader mReader(filename);
    if (mReader.isValid()) {
        while (mReader.next())
        {
            mChromosoms[mReader.region().chromosom()].append(mReader.region());
        }

    }

}

void ChromosomWidget::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());


    drawChromosom(&painter);


    QBrush areaBrush(QColor(255,0,0,200));
    areaBrush.setStyle(Qt::Dense6Pattern);
    painter.setBrush(areaBrush);

    QRect selector;
    selector.setLeft(mSelectorMin);
    selector.setRight(mSelectorMax);
    selector.setHeight(height());

    painter.drawRect(selector);



}

void ChromosomWidget::drawChromosom(QPainter *painter)
{

    if (mChromosoms.isEmpty())
        return;

        painter->setBrush(Qt::red);
        qint64 max = mChromosoms["chr1"].last().pos();

        int x = 50;
        int up= 50;
        foreach ( Region region, mChromosoms["chr1"])
        {

            QString stain = region.data("stain").toString();
            QRect fragment;
            int w = region.length() * width() / max;
            int h = 50;
            if (stain == "acen" || stain == "gvar"){

                fragment.setTopLeft(QPoint(x,h/2 - 5+up));
                fragment.setWidth(w);
                fragment.setHeight(10);
                x+=1;

            }

            else {
                fragment.setTopLeft(QPoint(x,up));
                fragment.setWidth(w);
                fragment.setHeight(h);
                x+=w;

            }


            painter->setBrush(mStains.value(region.data("stain").toString(),Qt::green));
            painter->setPen(QPen(Qt::transparent));
            painter->drawRect(fragment);

            painter->setPen(QPen(Qt::white));

            painter->drawText(fragment, Qt::AlignCenter,region.name());

        }


}



}}
