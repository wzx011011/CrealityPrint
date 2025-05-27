#include "controller.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>

controller::controller(QString inputpath, QString outpath)
{
    m_open_path_ini = inputpath;
    m_save_path_ini = outpath;
    list_of_svgfiles.clear();
    initParam();
    QObject::connect(this,SIGNAL(message(QString)),this,SLOT(show_message(QString)));
}

controller::~controller()
{
    prepare_for_exit();
}

void controller::load_ini_file(QString iniFile){  
    QFile file(iniFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit message("can not find "+iniFile);
    }
    else {
        while (!file.atEnd()) {
            QByteArray str = file.readLine().trimmed();

            if (!str.contains('%')) {
                if (str.contains("image_format")) {
                    m_image_format_ini = str.right(str.size()-(str.indexOf('=')+1));
                }
                else if (str.contains("dpi")) {
                    QString pre_dpi;
                    pre_dpi = str.right(str.size()-(str.indexOf('=')+1));
                    bool is_dpi_correct = false;
                    pre_dpi.toInt(&is_dpi_correct);
                    if (is_dpi_correct){
                        m_dpi_ini = pre_dpi;
                    }
                    else {
                        emit message("无法从ini文件获取值，将设置默认值!");
                    }
                }                             
                else if (str.contains("image_size")) {
                    m_image_size_ini = str.right(str.size()-(str.indexOf('=')+1));
                }
                else if (str.contains("default_image_width")) {
                    m_default_image_width_ini = str.right(str.size()-(str.indexOf('=')+1));
                }
                else if (str.contains("default_image_height")) {
                    m_default_image_height_ini = str.right(str.size()-(str.indexOf('=')+1));
                }
                else if (str.contains("default_dx")) {
                    m_default_dx_ini = str.right(str.size()-(str.indexOf('=')+1));
                }
                else if (str.contains("default_dy")) {
                    m_default_dy_ini = str.right(str.size()-(str.indexOf('=')+1));
                }
                else if (str.contains("opacity_mode")) {
                    m_opacity_mode_ini = str.right(str.size()-(str.indexOf('=')+1));
                }
                else if (str.contains("opacity_value")) {
                    m_opacity_value_ini = str.right(str.size()-(str.indexOf('=')+1));
                }
                else if (str.contains("frame_thickness")) {
                    m_frame_thickness_ini = str.right(str.size()-(str.indexOf('=')+1));
                }
            }
        }
        file.close();
    }
}

void controller::initParam()
{
    m_image_format_ini = "png";
    m_dpi_ini = "150";
    m_image_size_ini = "by_ini";
    m_default_image_width_ini = "3.8";
    m_default_image_height_ini = "2.3";
    m_default_dx_ini = "0.9";
    m_default_dy_ini = "0.8";
    m_opacity_mode_ini = "on";
    m_opacity_value_ini = "0.8";
    m_frame_thickness_ini = "1";    
}

void controller::run_all()
{
    if ((QDir(m_save_path_ini).exists())&&(!m_save_path_ini.isEmpty())){      
        /*for (int i=0; i<threads.size(); i++){
            delete threads.at(i).future_handle;
            delete threads.at(i).processor_handle;
        }*/        
        count_of_finished_processes = 0;
        //threads.clear();

        /*thread_struct thread;
        thread.widget_index = 0;
        thread.future_handle = nullptr;
        thread.processor_handle = nullptr;*/
        p = nullptr;
        /*QFuture<int>* future = nullptr;*/

        everything_was_ok = true;
        at_least_one_done = false;
        QString outline_name;
        double w = m_default_image_width_ini.toDouble(), h = m_default_image_height_ini.toDouble(), dx = m_default_dx_ini.toDouble(), dy = m_default_dy_ini.toDouble();   //размеры платы и смещение начала координат по умолчанию...

        //  搜索 ".board"文件,
        //  为所有上传的gerbers绘制图像的大小(板)。
        for (int i=0;i<list_of_gerbers.size();i++) {
            if (list_of_gerbers.at(i).contains(".board")){
                p = new Processor(1);
                p->set_frame_thickness(m_frame_thickness_ini.toDouble());
                p->set_dpi(m_dpi_ini.toInt());     //  утстановка пользовательского разрешения в dpi
                p->set_image_format(m_image_format_ini);
                p->set_paths(list_of_gerbers.at(i),m_save_path_ini);
                p->load_file();      //  загрузка файла в память
                if (m_image_size_ini=="by_outline"){
                    p->get_outline_size(&w, &h, &dx, &dy);           //  计算图的宽度和高度，以及坐标系起点的偏移(例如，在圆板的中心)
                }
                if ((m_opacity_mode_ini=="on")&&(m_image_format_ini=="png")){
                    p->set_opacity_value(m_opacity_value_ini.toFloat());
                }
                outline_name = p->get_outline_filename();
                p->set_w_h_dx_dy(w,h,dx,dy);

                // обработка:
                /*future = new QFuture<int>;
                thread.future_handle = future;
                thread.processor_handle = p;
                thread.widget_index = i;
                threads.append(thread);*/
                connect(p, SIGNAL(gerberFilefinished(QString)), this, SLOT(process_finished(QString)));
                /**future = QtConcurrent::run(p,&Processor::processing);//*/
                return_code = p->processing();
                /*return_code = future->result();*/
                if ((return_code)>-1){
                    at_least_one_done = true;
                }
                else {
                    everything_was_ok = false;
                    break;
                }
                break; // 因为我们找到了一个电路，一个循环
            }
        }

        //  除图像形成外，所有上传文件的循环
        //  如果电路文件处理成功
        if ((at_least_one_done)||(m_image_size_ini=="by_ini")){
            for (int i=0; i<list_of_gerbers.size(); i++) {
                if (!(list_of_gerbers.at(i).contains(".board"))){

                    p = new Processor(0);
                    p->set_frame_thickness(m_frame_thickness_ini.toDouble());
                    p->set_dpi(m_dpi_ini.toInt());
                    p->set_image_format(m_image_format_ini);
                    p->set_paths(list_of_gerbers.at(i),m_save_path_ini);//设置文件路径
                    p->load_file();   // 加载Gerber文件
                    if ((m_opacity_mode_ini=="on")&&(m_image_format_ini=="png")){
                        p->set_opacity_value(m_opacity_value_ini.toFloat());
                    }
                    p->set_outline_file_name(outline_name); //  电路图像路装置
                    p->set_w_h_dx_dy(w,h,dx,dy);

                    /*future = new QFuture<int>;
                    thread.future_handle = future;
                    thread.processor_handle = p;
                    thread.widget_index = i;
                    threads.append(thread);*/
                    connect(p, SIGNAL(gerberFilefinished(QString,int)), this, SLOT(process_finished(QString,int)));
                    /**future = QtConcurrent::run(p,&Processor::processing);//*/
                    p->processing();
                }//end of if
            }//end of for
        }
        else {           
            emit message("文件不会被处理，因为电路文件没有被处理!");
        }
    } // end of if
    else {
        emit message("save file drectory not exists");
    }
}

void controller::process_finished(QString fileName, int returnCode) {
    qDebug() << "receive file:" << fileName;
    if (!fileName.isEmpty())
        list_of_svgfiles << fileName;

    /*int i = 0;  // 向这个插槽发送信号的过程索引。

    for (int j=0; j<threads.size(); j++){
        if (threads.at(j).processor_handle == QObject::sender()){
            i = j;
        }
    }

    if(threads.size()>0)
        if (!threads.at(i).processor_handle->is_outline()) {
            return_code = threads.at(i).future_handle->result();
        }
*/
    if (!p->is_outline())
    {
        return_code = returnCode;
    }

    if ((return_code)>-1){
        at_least_one_done = true;
    }
    else {
        everything_was_ok = false;
        QString err_msg;
        if (return_code == -3){
            err_msg = "错误!\n文件" + list_of_gerbers.at(/*threads.size() > 0?threads.at(i).widget_index:*/0) + "不加工。也许文件太大.";
        }
        else if (return_code == -4) {
            err_msg = "错误!\n文件" + list_of_gerbers.at(/*threads.size() > 0 ? threads.at(i).widget_index : */0) + "不能保存。也许文件太大.";
        }
        else {
            err_msg = "错误!\n文件" + list_of_gerbers.at(/*threads.size() > 0 ? threads.at(i).widget_index : */0) + "不加工。文件不存在或错误类型的文件.";
        }
        emit message(err_msg);
    }
    count_of_finished_processes++;

    if (count_of_finished_processes==list_of_gerbers.size()) {

        if (everything_was_ok) {
            emit message("文件处理完成!");
        }
        else if (at_least_one_done) {
            emit message("文件处理是错误的,至少有一个文件被处理过.");
        }
        else {
            emit message("错误!没有一个文件被处理过!");
        }

        //threads.clear();
    }
}

void controller::prepare_for_exit(){
    /*for (int i=0; i<threads.size(); i++){
        delete threads.at(i).future_handle;
        delete threads.at(i).processor_handle;
    }*/
    if (p)
        delete p;

    for (QString filename : list_of_svgfiles)
    {
        if (QFileInfo::exists(filename))
            QFile::remove(filename);
    }
}

void controller::show_message(QString msg_text)
{
//    msgBox.setWindowTitle("Gerber提示");
//    msgBox.setText(msg_text);
//    msgBox.exec();
    qDebug() << "Gerber tip:" << msg_text;
}
