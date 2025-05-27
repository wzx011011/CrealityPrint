#include "appsetting.h"
#include <QtCore/QSettings>
#include <QtCore/QtCore>

namespace creative_kernel
{
	AppSetting::AppSetting()
	{
	}
	
	AppSetting::~AppSetting()
	{
	}


    QColor AppSetting::getColor(QString tag)
    {
        tag = tag.toLower();
        if (tag == "modelui_basecolor")
        {
            return QColor("transparent");
        }
        return QColor("white");
    }

    QVector4D AppSetting::getVector4D(QString tag)
    {
        if (tag == "printer_basecolor")
        {
            return QVector4D(0.180f, 0.541f, 0.968f, 1.0f);
        }
        else if (tag == "modeleffect_ambient")
        {
            //return QVector4D(0.6, 0.6, 0.6, 1.0);
            return QVector4D(0.12f, 0.12f, 0.12f, 1.0f);
        }
        else if (tag == "modeleffect_diffuse")
        {
            //return QVector4D(0.9, 0.9, 0.9, 1.0);
            return QVector4D(0.8f, 0.8f, 0.8f, 1.0);
        }
        else if (tag == "modeleffect_specular")
        {
            //return QVector4D(0.125, 0.125, 0.125, 1.0);
            return QVector4D(0.4f, 0.4f, 0.4f, 1.0);
        }
        return QVector4D(1.0f, 1.0f, 1.0f, 1.0f);
    }

    QVariantList AppSetting::getShaderSetting(QString tag)
    {
        QVariantList values = QVariantList();
        tag = tag.toLower();
        if (tag == "modeleffect_statecolors")
        {
            //values << QVector4D(0.621f, 0.621f, 0.721f, 1.0f)//未选中
            //    << QVector4D(0.462f, 0.663f, 0.976f, 1.0f)//鼠标经过
            //    << QVector4D(0.306f, 0.702f, 0.769f, 1.0f)//选中
            //    << QVector4D(0.588f, 0.03f, 0.03f, 1.0f)//超出平台
            //    << QVector4D(1.0f, 1.0f, 0.00f, 1.0f);
            values << QVector4D(1.0f, 1.0f, 1.0f, 1.0f)//未选中
                << QVector4D(0.173f, 0.816f, 1.0f, 1.0f)//鼠标经过
                << QVector4D(0.055f, 0.638f, 0.882f, 1.0f) + QVector4D(0.24f, 0.24, 0.24, 0.0f)//选中
                << QVector4D(0.588f, 0.03f, 0.03f, 1.0f)//超出平台
                << QVector4D(1.0f, 1.0f, 0.00f, 1.0f)
                << QVector4D(1.0f, 0.54f, 0.0f, 1.0f);//模型内部
        }

        return values;
    }

}