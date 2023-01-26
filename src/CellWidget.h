#ifndef NETLIST_CELL_WIDGET_H
#define NETLIST_CELL_WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPoint>
class QPainter;
#include "Box.h"
#include "Line.h"
#include "Shape.h"
#pragma once


namespace Netlist
{

class Cell;
class Box;
class Term;
class CellWidget : public QWidget
{
    Q_OBJECT;
public:
    CellWidget         ( QWidget* parent=NULL );
    virtual        ~CellWidget         ();
    void    setCell            ( Cell* );
    inline  Cell*   getCell            () const
    {
        return cell_;
    }
    inline  int     xToScreenX         ( int x ) const
    {
        return x-viewport_.getX1();
    }
    inline  int     yToScreenY         ( int y ) const
    {
        return viewport_.getY2()-y;
    }
    inline  QRect   boxToScreenRect    ( const Box& box) const
    {
        return QRect(xToScreenX(box.getX1()),yToScreenY(box.getY1()),xToScreenX(box.getX2())-xToScreenX(box.getX1()),yToScreenY(box.getY2())-yToScreenY(box.getY1()));
    }
    inline  QPoint  pointToScreenPoint ( const Point& point) const
    {
        return QPoint(xToScreenX(point.getX()),yToScreenY(point.getY()));
    }
    inline  int     screenXToX         ( int x ) const
    {
        return x+viewport_.getX1();
    }
    inline  int     screenYToY         ( int y ) const
    {
        return viewport_.getY2()+y;
    }
    inline  Box     screenRectToBox    ( const QRect& QR) const
    {
        return Box(screenXToX(QR.x()),screenYToY(QR.y()),screenXToX(QR.x()+QR.width()),screenYToY(QR.y()+QR.height()));
    }
    inline  Point   screenPointToPoint ( const QPoint& QP) const
    {
        return Point(screenXToX(QP.x()),screenYToY(QP.y()));
    }
    virtual QSize   minimumSizeHint    () const;
    virtual void    resizeEvent        ( QResizeEvent* );
    void    query              ( unsigned int flags , QPainter & painter );
protected:
    virtual void    paintEvent         ( QPaintEvent* );
    virtual void    keyPressEvent      ( QKeyEvent* );
public slots:
    void    goLeft             ();
    void    goRight            ();
    void    goUp               ();
    void    goDown             ();
private:
    Cell* cell_;
    Box   viewport_;
};

}
#endif
