#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QString>
#include <QStringList>
#include <QPainter>
#include "aperture.h"

class Processor : public QObject
{    
    Q_OBJECT

    bool is_outline_flag;                   //  表明文件是一个轮廓

    double w, h, dx, dy;                    //  电路板尺寸
    double board_width;                     //
    double board_height;                    //
    double frame_thickness = 0.05;          //  电路周围几英寸的自由场厚度
    int    dpi;                             //  分辨率
    float  opacity_value=1;                 //  透明度,不透明

    QString image_format = "png";           //  图像格式

    QString name_of_gerber_file;            //  输入文件名称(完整路径)

    QString output_folder_path;             //  图片保存路径
    QString name_of_output_file;            //  图片名称（完整路径）

    QString name_of_outline_file="";        //  电路图像文件名称(完整路径)

    QStringList list_of_strings_of_gerber;  //  文件内容

public:

    Processor(const bool is_outline);       //构造

    void set_image_format(const QString format);                                    //  确定图像格式
    void set_paths(const QString gerber_file, const QString output_folder);         //  建立必要进出路线
    int load_file();                                                                //  打开并上传整个gerber文件到oz (StringList)，然后关闭gerber。
    void get_outline_size(double *width, double *height, double *dx, double *dy);   //  (!仅供电路文件使用)计算电路大小和开始位置位移
    QString get_outline_filename();                                                 //  (!仅供轮廓文件使用)获得输出文件的名称
    void set_outline_file_name(const QString);                                      //  为其他文件(不是电路的文件)设置电路名称
    void set_opacity_value(const float val);                                        //  确定透明度程度
    void set_dpi(const int new_scale);                                              //  确定每英寸点数
    void set_frame_thickness(const double);                                         //  图像周围框架厚度装置
    void set_w_h_dx_dy(const double wdt, const double hgt, const double dxx, const double dyy);
    bool is_outline(){
        if (is_outline_flag) return 1;
        else return 0;
    }

    int processing();      //  主要功能是根据原点的大小和位移来处理和塑造图像!

signals:

    void gerberFilefinished(QString,int);    //  加工结束标志

private:    

    const double pi = 3.1415926;            //  圆周率
    const double mm_in_inch = 25.41;        //1英寸等于多少毫米

    //
    //  标准Gerber指令
    //
    enum commands{
        D01 = 1,
        D02 = 2,
        D03 = 3,
        G01 = 4,
        G02 = 5,
        G03 = 6,
        G74 = 7,
        G75 = 8,
        G36 = 9,
        G37 = 10,
        G04 = 11,
        M02 = 12,
        Dnn = 13
    };
    //
    //  扩展gerber指令
    //
    enum extended_commands{
        FS = 1,
        MO = 2,
        AD = 3,
        AM = 4,
        AB = 5,
        LP = 6,
        LM = 7,
        LR = 8,
        LS = 9,
        TF = 10,
        TA = 11,
        TO = 12,
        TD = 13
    };

    //
    //  Graphic state:
    //

    //  坐标 parameters:
    int frmt_x_int = 1;
    int frmt_x_dec = 1;
    int frmt_y_int = 1;
    int frmt_y_dec = 1;

    enum unit{MM,IN};
    unit unit;

    //  Generation parameters:
    int current_x = 0, current_y = 0;
    Aperture* current_aperture;         //  曲柄字典曲柄指示器
    int current_d_code;                 //  目前的d代码支持过时的结构坐标
                                        //  在每一个操作中没有d-code 1-D01，2-D02，3-D03

    enum interpolation_mode{LINEAR, CLOCKWISE_CIRCULAR, COUNTERCLOCKWISE_CIRCULAR};
    interpolation_mode interpolation_mode = LINEAR;
    enum quadrant_mode{SINGLE_QUADRANT, MULTI_QUADRANT};
    quadrant_mode quadrant_mode;

    //  孔径转换参数:
    enum polarity{C,D};
    polarity polarity = D;
    enum mirroring{NO_MIRRORING,X,Y,XY};
    mirroring mirroring = NO_MIRRORING;
    float rotation = -1;
    float scaling = -1;


    int string_to_command(const QString);           //  它将命令行转换为enum命令类型
    int string_to_extended_command(const QString);  //  它将命令行转换为enum enum扩展命令类型。
    int string_to_units(const QString);             //  它将一行(英寸，毫米)转换为enum unit类型。
    int string_to_mirroring(const QString);         //  它将一行与反射模式转换为enum镜像
    int trim_D_argument(QString, const int int_format, const int dec_format, const bool minus);     //  阅读坐标数据行(数字)，并导致小数点。
                                                                                                    //  基于格式 %FSLAX...Y...
    int radius_from_big_I_J(const long long int,const long long int);   //  毕达哥拉斯定理弧中心偏移半径计算
    void norm_angle(int*);                                              //  在QT方法中，角度的变化是必要的
    void check_for_G_in_D(const QString, enum interpolation_mode*);     //  当G- code包含在具有D- code的命令中时，支持过时的结构
    int sqrt_from_big_int(const long long int);                         //  大整数平方根计算(qtmath无法完成)

};

#endif // PROCESSOR_H
