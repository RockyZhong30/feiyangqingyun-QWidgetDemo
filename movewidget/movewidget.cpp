#include "movewidget.h"
#include "qevent.h"
#include "qdebug.h"

MoveWidget::MoveWidget(QObject *parent) : QObject(parent)
{
    lastPoint = QPoint(0, 0);
    pressed = false;
    leftButton = true;
    widget = 0;
}

bool MoveWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (widget != 0 && watched == widget) {
        QMouseEvent *mouseEvent = (QMouseEvent *)event;
        if (mouseEvent->type() == QEvent::MouseButtonPress) {
            //如果限定了只能鼠标左键拖动则判断当前是否是鼠标左键
            if (leftButton && mouseEvent->button() != Qt::LeftButton) {
                return false;
            }

            //判断控件的区域是否包含了当前鼠标的坐标
            if (widget->rect().contains(mouseEvent->pos())) {
                lastPoint = mouseEvent->pos();
                pressed = true;
            }
        } else if (mouseEvent->type() == QEvent::MouseMove && pressed) {
            //计算坐标偏移值,调用move函数移动过去
            int offsetX = mouseEvent->pos().x() - lastPoint.x();
            int offsetY = mouseEvent->pos().y() - lastPoint.y();
            widget->move(widget->x() + offsetX, widget->y() + offsetY);
        } else if (mouseEvent->type() == QEvent::MouseButtonRelease && pressed) {
            pressed = false;
        }
    }

    return QObject::eventFilter(watched, event);
}

void MoveWidget::setWidget(QWidget *widget)
{
    if (this->widget == 0) {
        this->widget = widget;
        this->widget->installEventFilter(this);
    }
}

void MoveWidget::setLeftButton(bool leftButton)
{
    this->leftButton = leftButton;
}
