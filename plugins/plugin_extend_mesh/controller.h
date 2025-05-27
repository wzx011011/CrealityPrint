#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "processor.h"
#include <QDebug>
//#include <QFuture>
//#include <QtConcurrent/QtConcurrent>
//#include <QProcess>

class controller: public QObject
{
    Q_OBJECT

public:
    controller(QString inputpath, QString outpath);
    ~controller();

    void load_ini_file(QString iniFile);
    void initParam();
    //当关上窗口的时候
    void prepare_for_exit();

    QStringList list_of_gerbers;//gerber文件列表
    QStringList list_of_svgfiles;//输出的图片列表

private:
    QString m_open_path_ini="";
    QString m_save_path_ini="";
    QString m_image_format_ini="png";
    QString m_dpi_ini="300";
    QString m_image_size_ini="by_outline";
    QString m_opacity_mode_ini="off";
    QString m_opacity_value_ini="0.8";
    QString m_default_image_width_ini="100";
    QString m_default_image_height_ini="100";
    QString m_default_dx_ini="0";
    QString m_default_dy_ini="0";
    QString m_frame_thickness_ini="1";

private:
    Processor* p;//

    /*struct thread_struct{
        QFuture<int>* future_handle;    // 流指示器
        Processor* processor_handle;    // 对象指示器
        int widget_index;
    };

    QList<thread_struct> threads;       // 处理流列表*/
    int count_of_finished_processes = 0;// 完成流计数器
    int count_of_gerbers = 0;
    int return_code=-1;

    bool everything_was_ok;             // 所有的文件都经过了适当的处理。
    bool at_least_one_done;             // 至少有一个文件被处理过。

    //QMessageBox msgBox;

signals:
    void message(QString);

public slots:
    //当一个启动的过程发出完成信号时，
    void process_finished(QString fileName, int returnCode);
    //弹出消息框
    void show_message(QString msg_text);

public:
    //接受命令从窗口开始处理
    void run_all();

};

#endif // CONTROLLER_H
