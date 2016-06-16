
#include "path.h"


Path::Path(FrameEditorState* state, QModelIndex& i) : QGraphicsObject(0),
                                                      index(i)
{
    setEnabled(false); //never handle user events

    //convert laser path into actual point items
    lzr::Frame path = index.data().value<lzr::Frame>();
    for(lzr::Point lzr_point : path)
    {
        own_point(new Point(state, lzr_point), points.size());
    }
}

Point* Path::first()
{
    return points.front();
}

Point* Path::last()
{
    return points.back();
}

void Path::setEnabled(bool enabled)
{
    foreach(Point* point, points)
    {
        point->setEnabled(enabled);
    }

    //always show the enabled paths on top of the disabled ones
    setZValue(enabled ? 1 : 0);
}

void Path::add_point(Point* point, bool add_at_front)
{
    if(add_at_front)
    {
        own_point(point, 0);
        //since this point was added at the back, and colors are stored on
        //the second point, we need to copy this point's color to the next point
        if(points.size() > 1)
            points[1]->set_color(points[0]->get_color());
    }
    else
    {
        own_point(point, points.size());
    }

    emit changed(this);
}

lzr::Frame Path::to_LZR() const
{
    lzr::Frame frame;

    foreach(Point* point, points)
    {
        frame.add(point->to_LZR());
    }

    return frame;
}

QModelIndex Path::get_index()
{
    return index;
}

size_t Path::size()
{
    return points.size();
}

void Path::own_point(Point* point, int where)
{
    points.insert(where, point);
    point->setParentItem(this);
    connect(point, SIGNAL(changed()),
            this, SLOT(point_changed()));
    connect(point, SIGNAL(remove_point(Point*)),
            this, SLOT(remove_point(Point*)));
}

QRectF Path::boundingRect() const
{
    return QRectF(-1,-1,2,2); //the whole screen/grid
}

void Path::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    for(int i = 0; i < points.size() - 1; i++)
    {
        const Point* p1 = points[i];
        const Point* p2 = points[i+1];
        QLineF line(p1->x(), p1->y(), p2->x(), p2->y());
        painter->setPen(QPen(p2->get_color(), 0));
        painter->drawLine(line);
    }
}

void Path::point_changed()
{
    emit changed(this);
}

void Path::remove_point(Point* point)
{
    points.removeAll(point);
    delete point;

    emit changed(this);
}