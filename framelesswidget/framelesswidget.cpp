﻿#include "framelesswidget.h"
#include "qevent.h"
#include "qdebug.h"

FramelessWidget::FramelessWidget(QObject *parent) : QObject(parent)
{
    padding = 8;
    widget = 0;

    pressed = false;
    pressedLeft = false;
    pressedRight = false;
    pressedTop = false;
    pressedBottom = false;
    pressedLeftTop = false;
    pressedRightTop = false;
    pressedLeftBottom = false;
    pressedRightBottom = false;

    //如果父类是窗体则直接设置
    if (parent->isWidgetType()) {
        setWidget((QWidget *)parent);
    }
}

bool FramelessWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (widget != 0 && watched == widget) {
        if (event->type() == QEvent::Resize) {
            //重新计算八个描点的区域,描点区域的作用还有就是计算鼠标坐标是否在某一个区域内
            int width = widget->width();
            int height = widget->height();

            //左侧描点区域
            rectLeft = QRectF(0, padding, padding, height - padding * 2);
            //上侧描点区域
            rectTop = QRectF(padding, 0, width - padding * 2, padding);
            //右侧描点区域
            rectRight = QRectF(width - padding, padding, padding, height - padding * 2);
            //下侧描点区域
            rectBottom = QRectF(padding, height - padding, width - padding * 2, padding);

            //左上角描点区域
            rectLeftTop = QRectF(0, 0, padding, padding);
            //右上角描点区域
            rectRightTop = QRectF(width - padding, 0, padding, padding);
            //左下角描点区域
            rectLeftBottom = QRectF(0, height - padding, padding, padding);
            //右下角描点区域
            rectRightBottom = QRectF(width - padding, height - padding, padding, padding);
        } else {
            QMouseEvent *mouseEvent = (QMouseEvent *)event;
            if (event->type() == QEvent::MouseButtonPress) {
                //记住当前控件坐标和宽高以及鼠标按下的坐标
                rectX = widget->x();
                rectY = widget->y();
                rectW = widget->width();
                rectH = widget->height();
                lastPos = mouseEvent->pos();

                //判断按下的手柄的区域位置
                if (rectLeft.contains(lastPos)) {
                    pressedLeft = true;
                } else if (rectRight.contains(lastPos)) {
                    pressedRight = true;
                } else if (rectTop.contains(lastPos)) {
                    pressedTop = true;
                } else if (rectBottom.contains(lastPos)) {
                    pressedBottom = true;
                } else if (rectLeftTop.contains(lastPos)) {
                    pressedLeftTop = true;
                } else if (rectRightTop.contains(lastPos)) {
                    pressedRightTop = true;
                } else if (rectLeftBottom.contains(lastPos)) {
                    pressedLeftBottom = true;
                } else if (rectRightBottom.contains(lastPos)) {
                    pressedRightBottom = true;
                } else {
                    pressed = true;
                }
            } else if (event->type() == QEvent::MouseMove) {
                //设置对应鼠标形状
                QPoint point = mouseEvent->pos();
                if (rectLeft.contains(point)) {
                    widget->setCursor(Qt::SizeHorCursor);
                } else if (rectRight.contains(point)) {
                    widget->setCursor(Qt::SizeHorCursor);
                } else if (rectTop.contains(point)) {
                    widget->setCursor(Qt::SizeVerCursor);
                } else if (rectBottom.contains(point)) {
                    widget->setCursor(Qt::SizeVerCursor);
                } else if (rectLeftTop.contains(point)) {
                    widget->setCursor(Qt::SizeFDiagCursor);
                } else if (rectRightTop.contains(point)) {
                    widget->setCursor(Qt::SizeBDiagCursor);
                } else if (rectLeftBottom.contains(point)) {
                    widget->setCursor(Qt::SizeBDiagCursor);
                } else if (rectRightBottom.contains(point)) {
                    widget->setCursor(Qt::SizeFDiagCursor);
                } else {
                    widget->setCursor(Qt::ArrowCursor);
                }

                //根据当前鼠标位置,计算XY轴移动了多少
                QPoint pos = mouseEvent->pos();
                int dx = pos.x() - lastPos.x();
                int dy = pos.y() - lastPos.y();

                //根据按下处的位置判断是否是移动控件还是拉伸控件
                if (pressed) {
                    widget->move(widget->x() + dx, widget->y() + dy);
                } else if (pressedLeft) {
                    int resizeW = widget->width() - dx;
                    if (widget->minimumWidth() <= resizeW) {
                        widget->setGeometry(widget->x() + dx, rectY, resizeW, rectH);
                    }
                } else if (pressedRight) {
                    widget->setGeometry(rectX, rectY, rectW + dx, rectH);
                } else if (pressedTop) {
                    int resizeH = widget->height() - dy;
                    if (widget->minimumHeight() <= resizeH) {
                        widget->setGeometry(rectX, widget->y() + dy, rectW, resizeH);
                    }
                } else if (pressedBottom) {
                    widget->setGeometry(rectX, rectY, rectW, rectH + dy);
                } else if (pressedLeftTop) {
                    int resizeW = widget->width() - dx;
                    int resizeH = widget->height() - dy;
                    if (widget->minimumWidth() <= resizeW) {
                        widget->setGeometry(widget->x() + dx, widget->y(), resizeW, resizeH);
                    }
                    if (widget->minimumHeight() <= resizeH) {
                        widget->setGeometry(widget->x(), widget->y() + dy, resizeW, resizeH);
                    }
                } else if (pressedRightTop) {
                    int resizeW = rectW + dx;
                    int resizeH = widget->height() - dy;
                    if (widget->minimumHeight() <= resizeH) {
                        widget->setGeometry(widget->x(), widget->y() + dy, resizeW, resizeH);
                    }
                } else if (pressedLeftBottom) {
                    int resizeW = widget->width() - dx;
                    int resizeH = rectH + dy;
                    if (widget->minimumWidth() <= resizeW) {
                        widget->setGeometry(widget->x() + dx, widget->y(), resizeW, resizeH);
                    }
                    if (widget->minimumHeight() <= resizeH) {
                        widget->setGeometry(widget->x(), widget->y(), resizeW, resizeH);
                    }
                } else if (pressedRightBottom) {
                    int resizeW = rectW + dx;
                    int resizeH = rectH + dy;
                    widget->setGeometry(widget->x(), widget->y(), resizeW, resizeH);
                }
            } else if (event->type() == QEvent::MouseButtonRelease) {
                //恢复所有
                pressed = false;
                pressedLeft = false;
                pressedRight = false;
                pressedTop = false;
                pressedBottom = false;
                pressedLeftTop = false;
                pressedRightTop = false;
                pressedLeftBottom = false;
                pressedRightBottom = false;
                widget->setCursor(Qt::ArrowCursor);
            }
        }
    }

    return QObject::eventFilter(watched, event);
}

void FramelessWidget::setPadding(int padding)
{
    this->padding = padding;
}

void FramelessWidget::setWidget(QWidget *widget)
{
    if (this->widget == 0) {
        this->widget = widget;
        //设置鼠标追踪为真
        this->widget->setMouseTracking(true);
        //绑定事件过滤器
        this->widget->installEventFilter(this);
        //设置无边框属性
        this->widget->setWindowFlags(Qt::FramelessWindowHint);
        //this->widget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    }
}
