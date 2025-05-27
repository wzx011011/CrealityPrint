#include "aperture.h"
#include <QtMath>
#include <QFile>
#include <QTime>
#include <QStack>
#include <QDebug>

Aperture::Aperture(const int d_code_number_of_aperture, const QString name_of_temp, const QString type_of_temp, const QString modifs, am_template* am_temp){

    d_code = d_code_number_of_aperture;
    name_of_template = name_of_temp;
    type_of_template = type_of_temp;
    modifiers = modifs;
    mod_list = modifiers.split('X');
    my_am_template = am_temp;

}

void Aperture::create(const int dpi){

    primitive_struct local_primitive;
    local_primitive.path.setFillRule(Qt::OddEvenFill);

    //---------------------------------------------------------------
    // STANDART_CIRCLE
    //---------------------------------------------------------------
    if (type_of_template == "C"){
        int diameter=0;
        int hole_diameter=-1;

        diameter = qRound(mod_list.at(0).toDouble()*dpi);
        QRectF rect_diameter(-diameter/2,-diameter/2,diameter,diameter);
        local_primitive.path.arcTo(rect_diameter,0,360);

        if (mod_list.size()>1){
            hole_diameter = qRound(mod_list.at(1).toDouble()*dpi);
            QRectF rect_hole(-hole_diameter/2,-hole_diameter/2,hole_diameter,hole_diameter);
            local_primitive.path.moveTo(-(diameter-hole_diameter)/2,0);
            local_primitive.path.arcTo(rect_hole,0,360);
        }
        primitives.append(local_primitive);
    }
    //---------------------------------------------------------------
    // STANDART_RECTANGLE
    //---------------------------------------------------------------
    else if (type_of_template == "R"){
        int x_size;
        int y_size;
        int hole_diameter=-1;

        x_size = qRound(mod_list.at(0).toDouble()*dpi);
        y_size = qRound(mod_list.at(1).toDouble()*dpi);
        QRectF main_rect(-x_size/2,-y_size/2,x_size,y_size);
        local_primitive.path.addRect(main_rect);

        if (mod_list.size()>2){
            hole_diameter = qRound(mod_list.at(2).toDouble()*dpi);
            QRectF rect_hole(-hole_diameter/2,-hole_diameter/2,hole_diameter,hole_diameter);
            local_primitive.path.moveTo(x_size/2+hole_diameter/2, y_size/2);
            local_primitive.path.arcTo(rect_hole,0,360);
        }
        primitives.append(local_primitive);
    }
    //---------------------------------------------------------------
    // STANDART_OBROUND
    //---------------------------------------------------------------
    else if (type_of_template == "O"){
        int x_size;
        int y_size;
        int hole_diameter=-1;

        x_size = qRound(mod_list.at(0).toDouble()*dpi);
        y_size = qRound(mod_list.at(1).toDouble()*dpi);

        QRectF main_rect(-x_size/2,-y_size/2,x_size,y_size);
        int R_of_obround;
        if (x_size>=y_size){
            R_of_obround = y_size/2;
        }
        else {
            R_of_obround = x_size/2;
        }
        local_primitive.path.addRoundedRect(main_rect, R_of_obround, R_of_obround);

        if (mod_list.size()>2){
            hole_diameter = qRound(mod_list.at(2).toDouble()*dpi);
            QRectF rect_hole(-hole_diameter/2,-hole_diameter/2,hole_diameter,hole_diameter);
            local_primitive.path.moveTo(x_size/2+hole_diameter/2, y_size/2);
            local_primitive.path.arcTo(rect_hole,0,360);
        }
        primitives.append(local_primitive);
    }
    //---------------------------------------------------------------
    // STANDART_POLYGON
    //---------------------------------------------------------------
    else if (type_of_template == "P"){
        const double pi = 3.1415926535897;

        int outer_diameter;
        int vertices;
        double rotation = 0;
        int hole_diameter = -1;

        outer_diameter = qRound(mod_list.at(0).toDouble()*dpi);
        vertices = (mod_list.at(1).toInt());
        if (mod_list.size()>2){
            rotation = mod_list.at(2).toDouble();
        }

        double d_angle = 360/vertices;
        QVector <QPointF> points;
        QPointF point;
        double i_angle = rotation;
        for (int i=0;i<vertices;i++) {
            point.setX((outer_diameter/2)*qCos(i_angle*pi/180));
            point.setY((outer_diameter/2)*qSin(i_angle*pi/180));
            points.append(point);
            i_angle = i_angle + d_angle;
        }
        QPolygonF polygon(points);
        local_primitive.path.addPolygon(polygon);

        if (mod_list.size()>3){
            hole_diameter = qRound(mod_list.at(3).toDouble()*dpi);
            QRectF rect_hole(-hole_diameter/2,-hole_diameter/2,hole_diameter,hole_diameter);
            local_primitive.path.moveTo(0,0);
            local_primitive.path.moveTo(hole_diameter/2,0);
            local_primitive.path.arcTo(rect_hole,0,360);

        }
        primitives.append(local_primitive);
    }

    //---------------------------------------------------------------
    // MACRO TEMPLATE
    //---------------------------------------------------------------
    else if (type_of_template == "MACRO"){  
        QList<variable> var_dictionary;
        variable current_var;
        QStringList curr_AM_data_list = my_am_template->get_data_blocks();
        QString arithmetic_expression, macro_content, var_name;

        for (int j=0;j<mod_list.size();j++) {
            current_var.index = j+1;
            current_var.value = mod_list.at(j).toFloat();
            var_dictionary.append(current_var);
        }
       
        for (int i=0; i<curr_AM_data_list.size();i++) {
            primitive_struct* new_local_primitive = new primitive_struct;
            new_local_primitive->rotation = 0;
            new_local_primitive->path.setFillRule(Qt::OddEvenFill);
            var_name="";
            //---------------------------------------------------------------
            // 如果有“=”，那么这就是变量的定义
            //---------------------------------------------------------------
            if (curr_AM_data_list.at(i).contains('=')){
                macro_content = curr_AM_data_list.at(i);                                    
                arithmetic_expression = macro_content.mid(macro_content.indexOf('=')+1);    
                arithmetic_expression.remove('*');
                
                for (int j = macro_content.indexOf('$')+1; macro_content.at(j).isDigit(); j++) {
                    var_name.append(macro_content.at(j));                    
                }
                current_var.index = var_name.toInt();              
                current_var.value = calculate_expression(arithmetic_expression, &var_dictionary);
                int var_list_index = -1;

                for (int j=0; j<var_dictionary.size(); j++) {
                    if (var_dictionary.at(j).index == current_var.index){
                        var_list_index = j;
                        break;
                    }
                }
                  
                if (var_list_index>-1){
                    var_dictionary.replace(var_list_index, current_var);
                }
                
                else if (var_list_index==-1) {
                    var_dictionary.append(current_var);
                }
            }
            //---------------------------------------------------------------
            // 没有“=”，所以这个描述是原始的。
            //---------------------------------------------------------------
            else {               
                QStringList am_mods = curr_AM_data_list.at(i).split(',');
              
                int prim_code = am_mods.at(0).toInt();
                switch (prim_code) {
                    //Circle
                    case 1:{
                        //bool exposure = 1;
                        float diameter = qRound(double(calculate_expression(am_mods.at(2), &var_dictionary))*dpi);       
                        float center_x = qRound(double(calculate_expression(am_mods.at(3), &var_dictionary))*dpi);      
                        float center_y = qRound(double(calculate_expression(am_mods.at(4), &var_dictionary))*dpi);

                        QRectF rect_diameter(qreal(center_x-diameter/2), qreal(center_y-diameter/2), qreal(diameter), qreal(diameter));
                        new_local_primitive->path.arcTo(rect_diameter,0,360);
                        new_local_primitive->path.closeSubpath();
                        if (am_mods.size()>5){
                            new_local_primitive->rotation = calculate_expression(am_mods.at(5), &var_dictionary);
                        }
                        new_local_primitive->path.setFillRule(Qt::WindingFill);
                    }break;
                    //Vector Line
                    case 20:{
                        //bool exposure = 1;
//                        float width = 0;
//                        float start_x = 0, start_y = 0;
//                        float end_x = 0, end_y = 0;
//                        float rotation;

                    }break;
                    //Center Line
                    case 21:{
                        //bool exposure = am_mods.at(1).toInt();
                        float width = qRound(double(calculate_expression(am_mods.at(2), &var_dictionary))*dpi); 
                        float height = qRound(double(calculate_expression(am_mods.at(3), &var_dictionary))*dpi);
                        float center_x = qRound(double(calculate_expression(am_mods.at(4), &var_dictionary))*dpi);
                        float center_y = qRound(double(calculate_expression(am_mods.at(5), &var_dictionary))*dpi);
                        new_local_primitive->rotation = calculate_expression(am_mods.at(6), &var_dictionary);

                        QRectF rect_primitive(qreal(center_x-width/2), qreal(center_y-height/2), qreal(width), qreal(height));
                        new_local_primitive->path.addRect(rect_primitive);
                        new_local_primitive->rotation = calculate_expression(am_mods.last(), &var_dictionary);
                        new_local_primitive->path.closeSubpath();
                    }break;
                    //Outline
                    case 4:{
                        //bool exposure = 1;
                        QPointF pnt;
                        QList<QPointF> points;

                        for (int i = 3;i<am_mods.size()-2;i=i+2) {
                            pnt.setX(qRound(double(calculate_expression(am_mods.at(i), &var_dictionary))*dpi));
                            pnt.setY(qRound(double(calculate_expression(am_mods.at(i+1), &var_dictionary))*dpi));

                            points.append(pnt);
                        }
                        new_local_primitive->path.moveTo(points.at(0));
                        for (int i = 1; i<points.size(); i++) {
                            new_local_primitive->path.lineTo(points.at(i));
                        }
                        new_local_primitive->path.closeSubpath();
                        new_local_primitive->path.setFillRule(Qt::WindingFill);
                        new_local_primitive->rotation = calculate_expression(am_mods.last(), &var_dictionary);

                    }break;
                    //Polygon

                    case 5:{
                        //bool exposure = 1;
//                        int number_of_vertices = 3;
//                        float center_x = 0, center_y = 0;
//                        float diameter = 0;
                        //float rotation;

                    }break;
                    //Moire
                    case 6:{
                        //bool exposure = 1;
//                        float center_x = 0, center_y = 0;
//                        float outer_ring_diameter = 0;
//                        float ring_thickness = 0;
//                        float gap = 0;
//                        int max_number_of_rings = 0;
//                        float crosshair_thickness = 0;
//                        float crosshair_length = 0;//if 0, there is no crosshair
                        //float rotation = 0;

                    }break;
                    //Thermal
                    case 7:{
                        //bool exposure = 1;
//                        float center_x = 0, center_y = 0;
//                        float outer_diameter = 0;
//                        float inner_diameter = 0;
//                        float gap_thickness = 0;
                        //float rotation = 0;

                    }break;
                }  //end of switch
            } //end of

            new_local_primitive->std_aperture = false;  
            primitives.append(*new_local_primitive); 
            delete new_local_primitive; 
        } //end of "for (int i=0; i<curr_AM_data_list.size();i++)"
    } //end of else if (type_of_template == "MACRO"
    //---------------------------------------------------------------
    // ERROR TYPE
    //---------------------------------------------------------------
    else {
        //错误模式(非标准或宏观错误!)
    }
}//end of void Aperture::create(int x_i, int x_d, int y_i, int y_d)

int Aperture::get_d_code(){
    return d_code;
}

QString Aperture::get_name(){
    return name_of_template;
}

QString Aperture::get_type(){
    return type_of_template;
}

int Aperture::draw_me(const int x_pos, const int y_pos, QPainter *painter){
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->translate(x_pos,y_pos);
    for (int i = 0; i<primitives.size();i++) {
        if (!primitives.at(i).std_aperture){
            painter->rotate(qreal(primitives.at(i).rotation));
        }
        painter->fillPath(primitives.at(i).path, Qt::black);
        painter->drawPath(primitives.at(i).path);
    }
    painter->translate(-x_pos,-y_pos);
    painter->restore();

    return 0;
}

int Aperture::get_std_circ_dia_in_px(const int dpi){
    return qRound(mod_list.at(0).toDouble()*dpi);
}

float Aperture::calculate_expression(const QString expression, QList<variable>* dict){

    QString in_str, opz_str, out_str;
    in_str = expression;

    enum type_of_symbol{num,var,oper};  
    struct symbol_struct{
        type_of_symbol symbol_type;     
        QString val;                    
        float num_val=0;               
        int priority = -1;              //优先级(如果是操作的话)为0.1.2
    };

    QList<symbol_struct> main_outqlist;
    symbol_struct item;
    QStack<symbol_struct> operation_stack;

    //-------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------

    int i=0;

    while (i<in_str.size()) {
        QString argument;
        //
        //  如果操作数 数字
        //
        if (in_str.at(i).isDigit()){
            while ((in_str.at(i).isDigit())||(in_str.at(i)=='.')) {
                argument.append(in_str.at(i));
                i++;
                if (i>=in_str.size())
                    break;
            }
            
            item.symbol_type = num;
            item.val = argument;
            item.num_val = argument.toFloat();
            main_outqlist.append(item);
        }
        //
        //  如果操作数变量
        //
        else if (in_str.at(i)=='$') {
            //argument.append(in_str.at(i));
            argument = "";
            while (in_str.at(i+1).isDigit()) {
                argument.append(in_str.at(i+1));
                i++;
                if ((i+1)>=in_str.size())
                    break;
            }
            //
            //  在变量字典中寻找数字(名称)变量。
            //
            bool dict_contains_var = false;
            for (int j=0; j<dict->size(); j++) {
                if (dict->at(j).index == argument.toInt()){
                    item.symbol_type = var;
                    item.val = "$" + QString::number(dict->at(j).index);
                    item.num_val = dict->at(j).value;
                    dict_contains_var = true;
                    break;
                }
            }
            
            if (!dict_contains_var) {
                variable current_var;

                current_var.index = argument.toInt();
                current_var.value = 0;
                dict->append(current_var);

                item.symbol_type = var;
                item.val = "$" + argument;
                item.num_val = 0;
            }
            //
            main_outqlist.append(item);
            i++; 

        }
        //
        //  正交负函数
        //
        else {
            item.symbol_type = oper;
            
            if ((in_str.at(i)=='-')&&((i==0)||(!(in_str.at(i-1).isDigit())))) {
                item.val = "un_m";
                item.priority = 1;
                operation_stack.push(item);
            }         
            else if (in_str.at(i)=='('){
                item.val = "(";
                item.priority = 2;
                operation_stack.push(item);                
            }          
            else if (in_str.at(i)==')') {
                while (operation_stack.top().val != "(") {
                    main_outqlist.append(operation_stack.top());

                    operation_stack.pop();
                    if (operation_stack.isEmpty()){
                        break;
                    }
                }
                if (!operation_stack.isEmpty()){
                    operation_stack.pop(); 
                }
            }           
            else {
                if (in_str.at(i)=='+') {
                    item.val = "+";
                    item.priority = 0;
                }
                else if ((in_str.at(i)=='x')||(in_str.at(i)=='X')) {
                    item.val = "x";
                    item.priority = 1;
                }
                else if (in_str.at(i)=='/') {
                    item.val = "/";
                    item.priority = 1;
                }
                else if (in_str.at(i)=='-') {
                    if ((i==0)||(!(in_str.at(i-1).isDigit()))) {
                        item.val = "un_m";
                        item.priority = 1;
                        operation_stack.push(item);                        
                    }
                    else if (in_str.at(i-1).isDigit()){                        
                        item.val = "-";
                        item.priority = 0;
                    }
                }
                if (!operation_stack.isEmpty()){
                    while ((item.priority < operation_stack.top().priority)&&(operation_stack.top().val != "(")) {
                        main_outqlist.append(operation_stack.top());

                        operation_stack.pop();
                        if (operation_stack.isEmpty()) {
                            break;
                        }
                    }
                }
                if (item.val!=""){
                    operation_stack.push(item);
                }                
            }

            i++;

        }
    }//end of while (!end_of_str)

    while (!operation_stack.isEmpty()) {
        main_outqlist.append(operation_stack.top());
        operation_stack.pop();    }

    //--------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------
    // 计算
    //--------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------

    QStack<float> symbol_stack;          
    for (int i=0;i<main_outqlist.size();i++) {      
        if ((main_outqlist.at(i).symbol_type == num)||(main_outqlist.at(i).symbol_type == var)){
            symbol_stack.push(main_outqlist.at(i).num_val);            
        }
        else if (main_outqlist.at(i).symbol_type == oper) {         
            float val1, val2, result;
            
            if (main_outqlist.at(i).val!="un_m"){
                if (!symbol_stack.isEmpty()){                    
                    val1 = symbol_stack.pop();                    
                    if (!symbol_stack.isEmpty()){
                        val2 = symbol_stack.pop();                        
                    }
                    else {
                        break;  
                    }
                }
                else {
                    break;
                }
                if (main_outqlist.at(i).val=="+"){
                    result = val2 + val1;
                    symbol_stack.push(result);
                }
                else if (main_outqlist.at(i).val=="-") {
                    result = val2 - val1;                    
                    symbol_stack.push(result);                    
                }
                else if (main_outqlist.at(i).val=="/") {
                    result = val2 / val1;
                    symbol_stack.push(result);
                }
                else if ((main_outqlist.at(i).val=="x")||(main_outqlist.at(i).val=="X")) {
                    result = val2 * val1;
                    symbol_stack.push(result);
                }
            }         
            else {                
                if (!symbol_stack.isEmpty()){                    
                    result = (symbol_stack.top()) * (-1);
                    symbol_stack.pop();
                    symbol_stack.push(result);
                }
            }
        }
    }
  
    if (!symbol_stack.isEmpty()){
        return symbol_stack.top();
    }
    else {
        qDebug()<<"在其中一个宏观模式中找不到表达式!";
        return -666;
    }
}
