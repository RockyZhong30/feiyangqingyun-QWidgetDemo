﻿#ifndef DBPAGE_H
#define DBPAGE_H

/**
 * 数据库通用翻页类 作者:feiyangqingyun(QQ:517216493) 2017-1-15
 * 1:自动按照设定的每页多少行数据分页
 * 2:只需要传入表名/字段集合/每页行数/翻页指示按钮/文字指示标签
 * 3:提供公共静态方法绑定字段数据到下拉框
 * 4:建议条件字段用数字类型的主键,速度极快
 * 5:增加线程查询符合条件的记录总数,数据量巨大时候不会卡主界面
 * 6:提供查询结果返回信号,包括当前页/总页数/总记录数/查询用时
 * 7:可设置所有列或者某一列对齐样式例如居中或者右对齐
 * 8:可设置增加一列,列的位置,标题,宽度
 * 9:可设置要查询的字段集合
 */

#include <QtGui>
#include <QtSql>
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtWidgets>
#endif

//自定义模型设置列居中和右对齐
class SqlQueryModel: public QSqlQueryModel
{
public:
    explicit SqlQueryModel(QObject *parent = 0);

protected:
    QVariant data(const QModelIndex &index, int role) const;

private:
    bool allCenter;                 //所有居中
    QList<int> alignCenterColumn;   //居中对齐列
    QList<int> alignRightColumn;    //右对齐列

public:
    //设置所有列居中
    void setAllCenter(bool allCenter);

    //设置居中对齐列索引集合
    void setAlignCenterColumn(const QList<int> &alignCenterColumn);

    //设置右对齐列索引集合
    void setAlignRightColumn(const QList<int> &alignRightColumn);

};

//计算复合条件的记录总行数,以便分页
class DbCountThread : public QThread
{
    Q_OBJECT
public:
    explicit DbCountThread(QObject *parent = 0);

private:
    QString connName;   //数据库连接名称
    QString sql;        //要执行的查询语句

protected:
    void run();

signals:
    void receiveCount(quint32 count, double msec);

public slots:
    //设置数据库连接名称
    void setConnName(const QString &connName);

    //设置要执行的查询语句
    void setSql(const QString &sql);

};

class DbPage : public QObject
{
    Q_OBJECT
public:
    enum DbType {
        DbType_Sqlite = 0,      //sqlite数据库
        DbType_MySql = 1,       //mysql数据库
        DbType_SqlServer = 3,   //sqlserver数据库
        DbType_Access = 4,      //access数据库
        DbType_PostgreSQL = 5   //postgresql数据库
    };

    static DbPage *Instance();
    explicit DbPage(QObject *parent = 0);

    //绑定数据到下拉框
    static void bindData(const QString &columnName, const QString &orderColumn, const QString &tableName,
                         QComboBox *cbox, const QString &connName = "qt_sql_default_connection");
    static void bindData(const QString &columnName, const QString &orderColumn, const QString &tableName,
                         QList<QComboBox *> cboxs, const QString &connName = "qt_sql_default_connection");

private:
    static QScopedPointer<DbPage> self;

    int startIndex;             //分页开始索引,每次翻页都变动
    QString tempSql;            //临时SQL语句
    QString sql;                //sql语句
    SqlQueryModel *queryModel;  //查询模型

    QLabel *labPageCount;       //总页数标签
    QLabel *labPageCurrent;     //当前页标签
    QLabel *labResultCount;     //总记录数标签
    QLabel *labResultCurrent;   //每页记录数标签
    QLabel *labResult;          //显示查询用时标签
    QLabel *labInfo;            //总页数当前页总记录数每页记录数

    QTableView *tableView;      //显示数据的表格对象
    QAbstractButton *btnFirst;  //第一页按钮对象
    QAbstractButton *btnPre;    //上一页按钮对象
    QAbstractButton *btnNext;   //下一页按钮对象
    QAbstractButton *btnLast;   //末一页按钮对象

    QString countName;          //统计表行数用字段
    QString connName;           //所使用的数据库连接名
    DbType dbType;              //数据库类型

    quint32 pageCurrent;        //当前第几页
    quint32 pageCount;          //总页数
    quint32 resultCount;        //总记录数
    quint32 resultCurrent;      //每页显示记录数

    QString tableName;          //表名
    QString selectColumn;       //要查询的字段集合
    QString orderSql;           //排序语句
    QString whereSql;           //条件语句
    QList<QString> columnNames; //列名集合
    QList<int> columnWidths;    //列宽集合

    int insertColumnIndex;      //插入的列的索引位置
    QString insertColumnName;   //插入的列的标题
    int insertColumnWidth;      //插入的列的宽度

private slots:
    void first();               //第一页
    void previous();            //上一页
    void next();                //下一页
    void last();                //末一页

private slots:
    //绑定sql语句到表格
    void bindData(const QString &sql);

    //收到记录行数
    void slot_receiveCount(quint32 count, double msec);

signals:
    void receivePage(quint32 pageCurrent, quint32 pageCount, quint32 resultCount, quint32 resultCurrent);
    void receiveCount(quint32 count, double msec);

public slots:
    //设置需要显示数据的表格,数据翻页对应的按钮
    void setControl(QTableView *tableView,
                    QLabel *labPageCount, QLabel *labPageCurrent,
                    QLabel *labResultCount, QLabel *labResultCurrent,
                    QLabel *labResult, QLabel *labInfo,
                    QAbstractButton *btnFirst, QAbstractButton *btnPre,
                    QAbstractButton *btnNext, QAbstractButton *btnLast,
                    const QString &countName, const QString &connName = "qt_sql_default_connection");

    //设置数据库连接名称
    void setConnName(const QString &connName);

    //设置数据库类型
    void setDbType(const DbType &dbType);

    //设置要查询的表名
    void setTableName(const QString &tableName);

    //设置要查询的字段列名集合
    void setSelectColumn(const QString &selectColumn);

    //设置排序sql
    void setOrderSql(const QString &orderSql);

    //设置条件sql
    void setWhereSql(const QString &whereSql);

    //设置每页显示多少行数据
    void setResultCurrent(int resultCurrent);

    //设置列名称集合
    void setColumnNames(const QList<QString> &columnNames);

    //设置列宽度集合
    void setColumnWidths(const QList<int> &columnWidths);

    //设置所有列居中
    void setAllCenter(bool allCenter);

    //设置居中对齐列索引集合
    void setAlignCenterColumn(const QList<int> &alignCenterColumn);

    //设置右对齐列索引集合
    void setAlignRightColumn(const QList<int> &alignRightColumn);

    //设置插入的列的索引
    void setInsertColumnIndex(int insertColumnIndex);

    //设置插入的列的标题
    void setInsertColumnName(const QString &insertColumnName);

    //设置插入的列的宽度
    void setInsertColumnWidth(int insertColumnWidth);

    //执行查询
    void select();
};

#endif // DBPAGE_H
