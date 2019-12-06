﻿#ifndef LUNARCALENDARITEM_H
#define LUNARCALENDARITEM_H

#include <QWidget>
#include <QDate>

#ifdef quc
#if (QT_VERSION < QT_VERSION_CHECK(5,7,0))
#include <QtDesigner/QDesignerExportWidget>
#else
#include <QtUiPlugin/QDesignerExportWidget>
#endif

class QDESIGNER_WIDGET_EXPORT LunarCalendarItem : public QWidget
#else
class LunarCalendarItem : public QWidget
#endif

{
    Q_OBJECT
    Q_ENUMS(DayType)
    Q_ENUMS(SelectType)

    Q_PROPERTY(bool select READ getSelect WRITE setSelect)
    Q_PROPERTY(bool showLunar READ getShowLunar WRITE setShowLunar)
    Q_PROPERTY(QString bgImage READ getBgImage WRITE setBgImage)
    Q_PROPERTY(SelectType selectType READ getSelectType WRITE setSelectType)

    Q_PROPERTY(QDate date READ getDate WRITE setDate)
    Q_PROPERTY(QString lunar READ getLunar WRITE setLunar)
    Q_PROPERTY(DayType dayType READ getDayType WRITE setDayType)

    Q_PROPERTY(QColor borderColor READ getBorderColor WRITE setBorderColor)
    Q_PROPERTY(QColor weekColor READ getWeekColor WRITE setWeekColor)
    Q_PROPERTY(QColor superColor READ getSuperColor WRITE setSuperColor)
    Q_PROPERTY(QColor lunarColor READ getLunarColor WRITE setLunarColor)

    Q_PROPERTY(QColor currentTextColor READ getCurrentTextColor WRITE setCurrentTextColor)
    Q_PROPERTY(QColor otherTextColor READ getOtherTextColor WRITE setOtherTextColor)
    Q_PROPERTY(QColor selectTextColor READ getSelectTextColor WRITE setSelectTextColor)
    Q_PROPERTY(QColor hoverTextColor READ getHoverTextColor WRITE setHoverTextColor)

    Q_PROPERTY(QColor currentLunarColor READ getCurrentLunarColor WRITE setCurrentLunarColor)
    Q_PROPERTY(QColor otherLunarColor READ getOtherLunarColor WRITE setOtherLunarColor)
    Q_PROPERTY(QColor selectLunarColor READ getSelectLunarColor WRITE setSelectLunarColor)
    Q_PROPERTY(QColor hoverLunarColor READ getHoverLunarColor WRITE setHoverLunarColor)

    Q_PROPERTY(QColor currentBgColor READ getCurrentBgColor WRITE setCurrentBgColor)
    Q_PROPERTY(QColor otherBgColor READ getOtherBgColor WRITE setOtherBgColor)
    Q_PROPERTY(QColor selectBgColor READ getSelectBgColor WRITE setSelectBgColor)
    Q_PROPERTY(QColor hoverBgColor READ getHoverBgColor WRITE setHoverBgColor)

public:
    enum DayType {
        DayType_MonthPre = 0,       //上月剩余天数
        DayType_MonthNext = 1,      //下个月的天数
        DayType_MonthCurrent = 2,   //当月天数
        DayType_WeekEnd = 3         //周末
    };

    enum SelectType {
        SelectType_Rect = 0,        //矩形背景
        SelectType_Circle = 1,      //圆形背景
        SelectType_Triangle = 2,    //带三角标
        SelectType_Image = 3        //图片背景
    };

    explicit LunarCalendarItem(QWidget *parent = 0);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void drawBg(QPainter *painter);
    void drawBgCurrent(QPainter *painter, const QColor &color);
    void drawDay(QPainter *painter);
    void drawLunar(QPainter *painter);

private:
    bool hover;                     //鼠标是否悬停
    bool pressed;                   //鼠标是否按下

    bool select;                    //是否选中
    bool showLunar;                 //显示农历
    QString bgImage;                //背景图片
    SelectType selectType;          //选中模式

    QDate date;                     //当前日期
    QString lunar;                  //农历信息
    DayType dayType;                //当前日类型

    QColor borderColor;             //边框颜色
    QColor weekColor;               //周末颜色
    QColor superColor;              //角标颜色
    QColor lunarColor;              //农历节日颜色

    QColor currentTextColor;        //当前月文字颜色
    QColor otherTextColor;          //其他月文字颜色
    QColor selectTextColor;         //选中日期文字颜色
    QColor hoverTextColor;          //悬停日期文字颜色

    QColor currentLunarColor;       //当前月农历文字颜色
    QColor otherLunarColor;         //其他月农历文字颜色
    QColor selectLunarColor;        //选中日期农历文字颜色
    QColor hoverLunarColor;         //悬停日期农历文字颜色

    QColor currentBgColor;          //当前月背景颜色
    QColor otherBgColor;            //其他月背景颜色
    QColor selectBgColor;           //选中日期背景颜色
    QColor hoverBgColor;            //悬停日期背景颜色

public:
    bool getSelect()                const;
    bool getShowLunar()             const;
    QString getBgImage()            const;
    SelectType getSelectType()      const;

    QDate getDate()                 const;
    QString getLunar()              const;
    DayType getDayType()            const;

    QColor getBorderColor()         const;
    QColor getWeekColor()           const;
    QColor getSuperColor()          const;
    QColor getLunarColor()          const;

    QColor getCurrentTextColor()    const;
    QColor getOtherTextColor()      const;
    QColor getSelectTextColor()     const;
    QColor getHoverTextColor()      const;

    QColor getCurrentLunarColor()   const;
    QColor getOtherLunarColor()     const;
    QColor getSelectLunarColor()    const;
    QColor getHoverLunarColor()     const;

    QColor getCurrentBgColor()      const;
    QColor getOtherBgColor()        const;
    QColor getSelectBgColor()       const;
    QColor getHoverBgColor()        const;

    QSize sizeHint()                const;
    QSize minimumSizeHint()         const;

public Q_SLOTS:
    //设置是否选中
    void setSelect(bool select);
    //设置是否显示农历信息
    void setShowLunar(bool showLunar);
    //设置背景图片
    void setBgImage(const QString &bgImage);
    //设置选中背景样式
    void setSelectType(const SelectType &selectType);

    //设置日期
    void setDate(const QDate &date);
    //设置农历
    void setLunar(const QString &lunar);
    //设置类型
    void setDayType(const DayType &dayType);
    //设置日期/农历/类型
    void setDate(const QDate &date, const QString &lunar, const DayType &dayType);

    //设置边框颜色
    void setBorderColor(const QColor &borderColor);
    //设置周末颜色
    void setWeekColor(const QColor &weekColor);
    //设置角标颜色
    void setSuperColor(const QColor &superColor);
    //设置农历节日颜色
    void setLunarColor(const QColor &lunarColor);

    //设置当前月文字颜色
    void setCurrentTextColor(const QColor &currentTextColor);
    //设置其他月文字颜色
    void setOtherTextColor(const QColor &otherTextColor);
    //设置选中日期文字颜色
    void setSelectTextColor(const QColor &selectTextColor);
    //设置悬停日期文字颜色
    void setHoverTextColor(const QColor &hoverTextColor);

    //设置当前月农历文字颜色
    void setCurrentLunarColor(const QColor &currentLunarColor);
    //设置其他月农历文字颜色
    void setOtherLunarColor(const QColor &otherLunarColor);
    //设置选中日期农历文字颜色
    void setSelectLunarColor(const QColor &selectLunarColor);
    //设置悬停日期农历文字颜色
    void setHoverLunarColor(const QColor &hoverLunarColor);

    //设置当前月背景颜色
    void setCurrentBgColor(const QColor &currentBgColor);
    //设置其他月背景颜色
    void setOtherBgColor(const QColor &otherBgColor);
    //设置选中日期背景颜色
    void setSelectBgColor(const QColor &selectBgColor);
    //设置悬停日期背景颜色
    void setHoverBgColor(const QColor &hoverBgColor);

Q_SIGNALS:
    void clicked(const QDate &date, const LunarCalendarItem::DayType &dayType);
};

#endif // LUNARCALENDARITEM_H
