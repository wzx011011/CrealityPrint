#ifndef APERTURE_H
#define APERTURE_H

#include "am_template.h"
#include <QString>
#include <QPainter>
#include <QTextStream>

class Aperture
{

    am_template* my_am_template;    //  AM定义的宏模式指示器

    //  光圈参数:
    int d_code = -1;                //  号码孔径
    QString name_of_template = "";  //  曲率模板
    QString type_of_template = "";  //  STANDART_C, R, O, P or MACRO
    QString modifiers = "";         //  画的参数(尺寸和大小)的行。
    QStringList mod_list;           //  分开x的行参数数组

    //  坐标格式:
    int x_int;
    int x_dec;
    int y_int;
    int y_dec;

    struct primitive_struct{
        QPainterPath path;                  //  原始图像
        float rotation = 0;                 //  旋转角
        bool std_aperture = 1;              //  标准顶点 (如果是1，你不需要转弯。)
    };

    QList <primitive_struct> primitives;    //宏观样板原始列表

    struct variable{
        int index=0;
        float value=0;
    };

    //  宏观模板原始描述修改器中的表达式计算
    float calculate_expression(const QString expression, QList<variable>* dict);

public:
    Aperture(const int d_code_number_of_aperture = 0, const QString name_of_temp = "", const QString type_of_temp = "", const QString modifs = "", am_template* am_temp = nullptr);

    int get_d_code();
    QString get_name();
    QString get_type();

    void create(const int dpi);                                         //  建立孔径
    int draw_me(const int x_pos, const int y_pos, QPainter* painter);   //  曲柄图象修饰
    int get_std_circ_dia_in_px(const int dpi);                          //  得到标准孔径-圆的直径来画线

};

#endif // APERTURE_H
