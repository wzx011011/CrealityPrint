#include "gadatainterface.h"
#include "external/kernel/kernel.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkInterface>
#include <QStandardPaths>
#include "sensors_analytics_sdk.h"
#include <string>
#include <cstdlib>
#include <iostream>


using std::string;

namespace creative_kernel
{
    bool startFinish = false;

    void setStartFinish(bool start)
    {
        startFinish = start;
    }

    QString getPCMacAddress()
    {
        QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// ��ȡ��������ӿ��б�
        int nCnt = nets.count();
        QString strMacAddr = "";
        for (int i = 0; i < nCnt; i++)
        {
            if (nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
            {
                strMacAddr = nets[i].hardwareAddress();
                break;
            }
        }
        return strMacAddr;
    }

    bool initSASdk()
    {
        // 暂存文件路径，该文件用于进程退出时将内存中未发送的数据暂存在磁盘，下次发送时加载
        QByteArray cdata = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toLocal8Bit() + "/staging_file";
        const string staging_file_path(cdata);
        //const string staging_file_path = "./staging_file";

        // 服务端数据接收地址
        const string server_url = "http://2-model-admin-dev.crealitygroup.com/api/rest/bicollector/front/sa/data";

        // 随机生成 UUID 作为 distinct_id
        // 注意：这里只是作为 demo 演示，随机生成一个 ID，如果正式使用，可以生成使用其他格式的设备 ID，并且自己保存下来，下次构造 SDK 时使用之前相同的 ID 以标识同一设备。
        const string distinct_id = getPCMacAddress().toStdString();
        //std::cout << "distinct_id: " << distinct_id << std::endl;
        // 神策 ID 分为 “设备 ID” 和 “登录 ID” 两种，随机生成的是 “设备 ID”
        const bool is_login_id = false;

        // 本地最多暂存（未调用 Flush 发送）的数据条数，超过该数值时，将从队首淘汰旧的数据
        const int max_staging_record_size = 200;

        // 初始化 SDK
        bool rst = sensors_analytics::Sdk::Init(staging_file_path, server_url, distinct_id, is_login_id, max_staging_record_size);
        if (rst)
        {
            sensors_analytics::Sdk::EnableLog(true);
        }

        return rst;
    }

    bool sendDataToGA(QString eventType, QString eventAction, QString userID)
    {
        if (!startFinish)
        {
            return false;
        }

        bool rst = initSASdk();
        if (!rst)
        {
            return false;
        }

        // 设置公共属性，这些属性将自动设置在每个行为事件的属性里
        sensors_analytics::PropertiesNode super_properties;
        super_properties.SetString("app_name", "Creality Print");
        std::string platformstr = "Win";
#ifdef __APPLE__
        platformstr = "Mac";
#endif
        super_properties.SetString("platform", platformstr.c_str());
        sensors_analytics::Sdk::RegisterSuperProperties(super_properties);

        // 记录一个行为事件
        sensors_analytics::PropertiesNode event_properties;
        event_properties.SetString(eventType.toStdString(), eventAction.toStdString());

        // 上面所有埋点都没有真正发送到服务端，当有网络的时候，请调用 Flush 手工触发发送
        // 注意：仅当调用 Flush 函数才会触发网络发送
        // 发送是阻塞的，可以考虑使用独立线程调用发送函数
        // 如果因为网络问题发送失败，函数返回值为 false
        bool flush_result = sensors_analytics::Sdk::Flush();
        std::cout << "send result: " << (flush_result ? "true" : "false") << std::endl;

        // 进程结束前没有 Flush 的数据将保存到 staging_file
        sensors_analytics::Sdk::Track("BuyTicket");
        sensors_analytics::Sdk::Shutdown();

        return flush_result;
    }
}