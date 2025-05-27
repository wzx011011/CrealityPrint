#ifndef AM_TEMPLATE_H
#define AM_TEMPLATE_H

#include <QStringList>

class am_template
{

    QString name;                // 模板宏观名称
    QStringList all_data_blocks; // 描述宏观样板的块日期字符串组

public:

    am_template(const QString name_of_macro, const QStringList data_blocks);
    QString get_name();
    QStringList get_data_blocks();

};

#endif // AM_TEMPLATE_H
