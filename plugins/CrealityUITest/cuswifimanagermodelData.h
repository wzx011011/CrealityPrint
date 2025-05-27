#ifndef CUSWIFILISTMODELDATA_H
#define CUSWIFILISTMODELDATA_H
#include <QString>
enum TransAndPrintState
{
    //初始状态
    INITIALICE_STATE,
    //传输过程
    TRANS_PREPARED_STATE, //等待传输
    TRANS_PAUSE_STATE, //传输暂停
    TRANS_STOP_STATE, //传输停止
    TRANSING_STATE, //传输中
    TRANS_FINISHED_STATE, //传输完成
    //打印过程
    PRINTING_STATE, //打印中
    PRINT_PAUSE_STATE, //打印暂停
    PRINT_PAUSEING_STATE,//打印暂停中
    PRINT_STOP_STATE, //打印停止
    PRINT_STOPING_STATE, //打印停止中
    PRINT_FINISHED_STATE, //打印完成
    TOKEN_ERROR,         //token 错误

    STARTING_STATE = 100,       //记录一下开始打印/传输---响应之间的状态。10秒；避免在未响应的时间类，能够两次点击开始打印
    UNCONNECT_STATE         //网络未连接状态
};

struct PrintingPara
{
    QString initExposure = "";              // 初始曝光   单位（秒）
    QString delayLight = ""; 			    //灭灯延时   单位（秒）
    QString printExposure = "";			    //打印曝光   单位（秒）
    QString printHeight = "";			    //打印上升高度   单位（毫米）
    QString eleSpeed = "";					//电机速度 （毫米/秒）
    QString bottomExposureNum = "";		    //底层曝光层数 （层）
    QString printRemainTime = "";           //剩余时间

    QString filename = "";
    QString sliceLayerCount = "";           //切片总层数
    QString layerThickness = "";            //层厚
    QString resin = "";                     //树脂（待定）
};


#endif // CUSWIFILISTMODELDATA_H
