#ifndef _ADDTABLEDATA_H
#define _ADDTABLEDATA_H
#include <QtCore/QObject>
#include <QModelIndex>
#include "basickernelexport.h"
#include "utils/printernetmng.h"
#include "data/printerdata.h"
namespace creative_kernel
{
    class BASIC_KERNEL_API AddTableData : public QObject
    {
        Q_OBJECT
    public:
        virtual ~AddTableData();
        static AddTableData* instance()
        {
            static AddTableData tab;
            return &tab;
        };
        

        Q_INVOKABLE void add();
        Q_INVOKABLE void searchPrinter();

        Q_INVOKABLE void remove(QModelIndexList indexs);
        Q_INVOKABLE void selects(QModelIndexList indexs);

        void sendLocalSliceFile(QString filePath);
        QString ZipGCodeFile(QString filePath);
    protected slots:
        void slotPrinterMsg(const PrinterData& pd);

    protected:
        void remove(QObject* obj);

    private:
        AddTableData(QObject* parent = nullptr);


    };
}
#endif // _ADDTABLEDATA_H
