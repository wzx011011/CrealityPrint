#ifndef LOG_H
#define LOG_H

void logInit();

#endif // LOG_H


/* log 使用说明
 * log类型：
 * qDebug：调试信息
 * qWarning：警告信息
 * qCritical：严重错误
 * qFatal：致命错误
 *
 *
 * 1. main函数中初始化：logInit()；

   2.使用的地方打印对应级别的log:
    qDebug("This is a debug message");
    qWarning("This is a warning message");
    qCritical("This is a critical message");
    qFatal("This is a fatal message");

    输出内容：
    将Type的类型转换成Qstring 类型输出
    qDebug()<<Type;

    3.log.txt 对应的内容：
 [log类型][具体时间][log的具体位置][log所在的函数] Message: 自定义输出的内容
[Debug] [2020-04-16 10:26:25]  [..\Creative3D\src\main.cpp: Line: 21]  [Function: int __cdecl main(int,char *[])]
Message: This is a debug message
[Warning] [2020-04-16 10:26:25]  [..\Creative3D\src\main.cpp: Line: 22]  [Function: int __cdecl main(int,char *[])]
Message: This is a warning message
[Critical] [2020-04-16 10:26:25]  [..\Creative3D\src\main.cpp: Line: 23]  [Function: int __cdecl main(int,char *[])]
Message: This is a critical message
[Warning] [2020-04-16 10:26:26]  [aspects\qaspectengine.cpp: Line: 116]  [Function: void __cdecl Qt3DCore::QAspectEnginePrivate::initEntity(class Qt3DCore::QEntity *)]
Message: Trying to assign a non shareable component to more than one Entity
[Debug] [2020-04-16 10:27:00]  [..\Creative3D\src\main.cpp: Line: 21]  [Function: int __cdecl main(int,char *[])]
Message: This is a debug message
[Warning] [2020-04-16 10:27:00]  [..\Creative3D\src\main.cpp: Line: 22]  [Function: int __cdecl main(int,char *[])]
Message: This is a warning message
[Critical] [2020-04-16 10:27:01]  [..\Creative3D\src\main.cpp: Line: 23]  [Function: int __cdecl main(int,char *[])]
Message: This is a critical message
[Warning] [2020-04-16 10:27:01]  [aspects\qaspectengine.cpp: Line: 116]  [Function: void __cdecl Qt3DCore::QAspectEnginePrivate::initEntity(class Qt3DCore::QEntity *)]
Message: Trying to assign a non shareable component to more than one Entity
[Debug] [2020-04-16 10:29:34]  [..\Creative3D\src\main.cpp: Line: 21]  [Function: int __cdecl main(int,char *[])]
Message: This is a debug message
[Warning] [2020-04-16 10:29:34]  [..\Creative3D\src\main.cpp: Line: 22]  [Function: int __cdecl main(int,char *[])]
Message: This is a warning message
[Critical] [2020-04-16 10:29:35]  [..\Creative3D\src\main.cpp: Line: 23]  [Function: int __cdecl main(int,char *[])]
Message: This is a critical message
[Fatal] [2020-04-16 10:29:35]  [..\Creative3D\src\main.cpp: Line: 24]  [Function: int __cdecl main(int,char *[])]
Message: This is a fatal message
[Warning] [2020-04-16 10:29:36]  [aspects\qaspectengine.cpp: Line: 116]  [Function: void __cdecl Qt3DCore::QAspectEnginePrivate::initEntity(class Qt3DCore::QEntity *)]
Message: Trying to assign a non shareable component to more than one Entity
*/



