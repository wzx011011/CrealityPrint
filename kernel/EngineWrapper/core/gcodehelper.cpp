#include "gcodehelper.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>

#include "qtusercore/string/stringtool.h"
#include <QFileInfo>

#include "mmesh/trimesh/trimeshutil.h"
#include "data/modeln.h"
#include "trimesh2/TriMesh.h"
#include "utils/matrixutil.h"
#include "interface/selectorinterface.h"
#include "interface/modelinterface.h"
using namespace creative_kernel;
#include <QCoreApplication>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <QDebug>

#include <QBuffer>
namespace cxsw
{
    bool cxSaveGCode(const QString& fileName, const std::string& previewImageString,
        const std::vector<std::string>& layerString, const std::string& prefixString,const std::string& tailCode, const QImage* qimg)
    {
        QFileInfo fileinfo(fileName);
        QString suffix = fileinfo.suffix();
        suffix = suffix.toLower();
        
        if ("gcode" == suffix )
        {
            /*QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
            {
                qDebug() << "can't open file " << fileName;
                return false;
            }
            //输出到文件
            QTextStream in(&file);*/

            std::ofstream out(fileName.toLocal8Bit().data(), std::ios::out | std::ios::binary | std::ios::app);
            if (!out)
            {
                return false;
            }

            
               
            ///添加预览图信息
            if (!previewImageString.empty())
            {
                //in << previewImageString.data() << endl;
                out.write(previewImageString.data(), strlen(previewImageString.data()));
            }
            if (qimg != nullptr)
            {
                out.write("\r\n", 2); //in << endl;
            }
            /*if (qimg != nullptr)
            {
                out.write(imgHeadStr.c_str(), strlen(imgHeadStr.c_str()));
                char *p = (char*)&pngSize;
                out.write(p, 4);
                out.write(pngBuf, pngSize);
                out.write(stdEnd.c_str(), 1);

                p = nullptr;
                delete[]pngBuf;
                pngBuf = nullptr;
                if (QFileInfo::exists(tmpPreviewImg))
                    QFile::remove(tmpPreviewImg);
            }*/
            //添加打印时间等信息
            if (!prefixString.empty())
            {
                std::vector<std::string> vctTemp;
                qtuser_core::SplitString((char*)prefixString.data(), vctTemp, "\\n");
                for (size_t i = 0; i < vctTemp.size(); i++)
                {
                    out.write(vctTemp[i].data(), strlen(vctTemp[i].data()));//in << vctTemp[i].data() << endl;//
                    out.write("\n", 1);
                }
            }
            //load();
            for (const std::string& val : layerString)
            {
                if (val.find("\\n"))
                {
                    std::vector<std::string> vctTemp;
                    qtuser_core::SplitString((char*)val.data(), vctTemp, "\\n");
                    for (size_t i = 0; i < vctTemp.size(); i++)
                    {
                        out.write(vctTemp[i].data(), strlen(vctTemp[i].data())); //in << vctTemp[i].data() << endl;//
                        out.write("\n", 1);
                    }
                }
                else
                {

                    out.write(val.data(), strlen(val.data())); //in << val.data();//
                }
            }

            if (!tailCode.empty())
            {
                std::vector<std::string> vctTemp;
                qtuser_core::SplitString((char*)tailCode.data(), vctTemp, "\\n");
                for (size_t i = 0; i < vctTemp.size(); i++)
                {
                    out.write(vctTemp[i].data(), strlen(vctTemp[i].data()));//in << vctTemp[i].data() << endl;//
                    out.write("\n", 1);
                }
            }
            out.close();//file.close();//
            return true;
        }
        else //"cxcode" by TCJ
        {
            //save all stl
            QString tmpSTL = QCoreApplication::applicationDirPath() + "/tmpSTL.stl";
            saveAllSTL(tmpSTL);
            //read stl
            unsigned int stlsize = 0;
            char* stlbuf = nullptr;                     
            readFile(tmpSTL.toLocal8Bit().data(), stlsize, stlbuf);
            //get gcode
            std::ofstream out(fileName.toLocal8Bit().data(), std::ios::out | std::ios::binary);
            if (!out)
            {
                return 0;
            }
            unsigned int gcodelen = 0;
            std::string gcodelines;

            /*if (!previewImageString.empty())
            {
                gcodelen += strlen(previewImageString.data());
                gcodelines += previewImageString.data();
            }*/
            //previewImg
            QString tmpPreviewImg = "";
            std::string imgHeadStr = "";
            unsigned int pngSize = 0;//图片的大小数据（4字节）
            char* pngBuf = nullptr;//PNG图原始二进制数据
            std::string stdEnd = "\n";//标准换行符
            unsigned int imgTotalSize = 0;
            if (qimg != nullptr)
            {
                //qimg->save(QCoreApplication::applicationDirPath()+"/tSource.png");
                imgHeadStr = "\n";
                imgHeadStr += ";image600:";
                QImage newImg = *qimg;
                changeImageBackcolor(newImg, 600, 600);
                //write
                tmpPreviewImg = QCoreApplication::applicationDirPath() + "/tPreviewImg.png";
                newImg.save(tmpPreviewImg);
                //read
                readFile(tmpPreviewImg.toLocal8Bit().data(), pngSize, pngBuf);
                imgTotalSize = strlen(imgHeadStr.c_str()) + 4 + pngSize + 1;
            }

            gcodelen += 1;
            gcodelines += "\n";

            if (!prefixString.empty())
            {
                std::vector<std::string> vctTemp;
                qtuser_core::SplitString((char*)prefixString.data(), vctTemp, "\\n");
                for (size_t i = 0; i < vctTemp.size(); i++)
                {
                    gcodelen += (strlen(vctTemp[i].data()) + 1);
                    gcodelines += vctTemp[i].data();
                    gcodelines += "\n";
                }
            }
            for (const std::string& val : layerString)
            {
                if (val.find("\\n"))
                {
                    std::vector<std::string> vctTemp;
                    qtuser_core::SplitString((char*)val.data(), vctTemp, "\\n");
                    for (size_t i = 0; i < vctTemp.size(); i++)
                    {
                        gcodelen += (strlen(vctTemp[i].data()) + 1);
                        gcodelines += vctTemp[i].data();
                        gcodelines += "\n";
                    }
                }
                else
                {
                    gcodelen += strlen(val.data());
                    gcodelines += val.data();
                }
            }
            if (!tailCode.empty())
            {
                std::vector<std::string> vctTemp;
                qtuser_core::SplitString((char*)tailCode.data(), vctTemp, "\\n");
                for (size_t i = 0; i < vctTemp.size(); i++)
                {
                    gcodelen += (strlen(vctTemp[i].data()) + 1);
                    gcodelines += vctTemp[i].data();
                    gcodelines += "\n";
                }
            }        
            //-----------------------------------------write cxcode
            //cxcode head
            char cxHead[7] = {';','c','x','c','o','d','e'};
            out.write(cxHead, sizeof(cxHead));

            //total size
            gcodelen += imgTotalSize;
            unsigned int totalSize = stlsize + gcodelen;

            char* strUInt =(char*)&totalSize;
            out.write(strUInt, 4);

            //stl(size + buff)
            strUInt = (char*)&stlsize;
            out.write(strUInt, 4);
            
            out.write(reinterpret_cast<char*>(stlbuf), stlsize);

            //gcode(size + buff(image+gcode))
            strUInt = (char*)&gcodelen;
            out.write(strUInt, 4);

            if (qimg != nullptr)//previewImage
            {
                out.write(imgHeadStr.c_str(), strlen(imgHeadStr.c_str()));
                strUInt = (char*)&pngSize;
                out.write(strUInt, 4);
                out.write(pngBuf, pngSize);
                out.write(stdEnd.c_str(), 1);
            }
            out.write(gcodelines.c_str(), gcodelen - imgTotalSize);//main gcode
            //-----------------------------------------write end
            out.close();
            delete[]stlbuf;
            stlbuf = nullptr;
            strUInt = nullptr;
            if (qimg != nullptr)
            {
                delete[]pngBuf;
                pngBuf = nullptr;
                if (QFileInfo::exists(tmpPreviewImg))
                    QFile::remove(tmpPreviewImg);
            }
            if (QFileInfo::exists(tmpSTL))
                QFile::remove(tmpSTL);

            return true;
        }
    }

	bool cxSaveGCode(const QString& fileName, const std::string& previewImageString,
        const std::vector<std::string>& layerString, const std::vector<std::string>& prefixString, std::vector<std::string>& tailCode)
	{
        if (prefixString.size() > 0)
            return cxSaveGCode(fileName, previewImageString, layerString, prefixString.at(0), tailCode.at(0));
        return cxSaveGCode(fileName, previewImageString, layerString, std::string(), tailCode.at(0));
	}

    bool cxLoadGCode(const QString& fileName,
        std::vector<std::string>& layerString, std::vector<std::string>& prefixString, std::vector<std::string>& tailCode)
    {
        layerString.clear();
        prefixString.clear();

        qDebug() << "cxLoadGCode : filename " << fileName;
        QFile file;
        file.setFileName(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            qDebug() << "can't open file " << fileName;
            return false;
        }
#if 0
        //QByteArray array;
        QList<QByteArray> vctarry;
        std::string value = "";
        while (file.atEnd() == false)
        {
            QByteArray arryall = file.readAll();
            vctarry = arryall.split('\n');
        }

        std::vector<std::string>* use = &prefixString;

        for (auto array : vctarry)
        {
            std::string temp = array.toStdString() + "\n";

            if (temp.find(";LAYER:0") != std::string::npos || temp.find(";LAYER:-") != std::string::npos || temp.find(";LAYER_COUNT:") != std::string::npos)
            {
                //value += temp;
                use->push_back(value);
                use = &layerString;
                value.clear();
                //continue;
            }
            else if (temp.find(";;----------------------tail----------------------") != std::string::npos)
            {
                value += temp;
                use->push_back(value);
                use = &tailCode;
                value.clear();
                continue;
            }
            else if (temp.find(";LAYER:") == 0)
            {
                use->push_back(value);
                value.clear();
            }

            value += temp;
        }
        use->push_back(value);

        file.close();
#else
        //QByteArray array;
        QList<QByteArray> vctarry;
        std::string value = "";
        bool headend = false;
        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            std::string temp = line.toStdString();
            if (temp.find(";LAYER_COUNT") != std::string::npos)
            {
                prefixString.push_back(value);
                layerString.push_back(temp);
                headend = true;
                value.clear();
                continue;
            }
            value += temp;
            if (!headend) { continue; }
            if (temp.find(";TIME_ELAPSED:") != std::string::npos)
            {
                layerString.push_back(value);
                value.clear();
                continue;
            }
        }
        tailCode.push_back(value);
        file.close();
#endif
        qDebug() << "cxLoadGCode : load finished! " ;
        return true;
	}

    void image2String(const QImage& image, int eWidth, int eHeight, bool clear, std::string& outString)
    {
        quint16 tmpInt16;
        uchar tmpChar[2] = { 0 };
        QString tmpNum = QString::number(eWidth);
        if(clear)
            outString.clear();
        outString += ";image" + tmpNum.toStdString() + ":" ;

        int r, g, b, rgb, width, height;
        QImage hendimg = image.scaled(eWidth, eHeight, Qt::KeepAspectRatio);
        width = hendimg.width();
        height = hendimg.height();

        int rgbtest = 0;
        int lineCount = 0;
        for (int h = 0; h < height; h++)
        {
            for (int w = 0; w < width; w++)
            {
                QColor pcolor(hendimg.pixel(w, h));
                r = pcolor.red() >> 3;
                g = pcolor.green() >> 2;
                b = pcolor.blue() >> 3;
                rgb = (r << 11) | (g << 5) | b;
                *(qint16*)tmpChar = (quint16)rgb;
                for (int i = 0; i < 2; i++)
                {
                    quint8 hbit = ((quint8)tmpChar[i]) >> 4;
                    quint8 lbit = ((quint8)tmpChar[i]) & 15;
                    outString += "0123456789ABCDEF"[hbit % 16];
                    outString += "0123456789ABCDEF"[lbit % 16];
                }
            }
        }
        outString += "\r\n";
    }
    void changeImageBackcolor(QImage& image, int eWidth, int eHeight)
    {
        image = image.scaled(eWidth, eHeight, Qt::KeepAspectRatio);
        image = image.convertToFormat(QImage::Format_RGBA8888_Premultiplied, Qt::NoFormatConversion);
        int bmpWidth = image.width();
        int bmpHeight = image.height();
        QColor bmpBack(0, 0, 0);
        QColor bmpBackA(254, 254, 254, 0);
        for (int i = 0; i < bmpWidth; ++i)
        {
            for (int j = 0; j < bmpHeight; ++j)
            {
                if (image.pixelColor(i, j) == bmpBack)
                {
                    image.setPixelColor(i, j, bmpBackA);
                }
            }
        }
    }
    void saveAllSTL(QString path)
    {
        selectAll();
        std::vector<trimesh::TriMesh*> meshs;
        QList<ModelN*> models = selectionms();

        if (models.size() > 0)
        {
            for (size_t i = 0; i < models.size(); i++)
            {
                bool isfanzhuan = models.at(i)->isFanZhuan();
                QMatrix4x4 matrix = models.at(i)->globalMatrix();
                trimesh::TriMesh* meshTemp = models.at(i)->mesh();
                trimesh::TriMesh* newMesh = new trimesh::TriMesh();
                *newMesh = *meshTemp;

                trimesh::fxform xf = qMatrix2Xform(matrix);
                int size = meshTemp->vertices.size();
                for (int n = 0; n < size; ++n)
                {
                    trimesh::vec3 v = meshTemp->vertices.at(n);
                    newMesh->vertices.at(n) = xf * v;
                }
                if (isfanzhuan)
                {
                    size = newMesh->faces.size();
                    for (int n = 0; n < size; ++n)
                    {
                        newMesh->faces.at(n)[1] = meshTemp->faces.at(n)[2];
                        newMesh->faces.at(n)[2] = meshTemp->faces.at(n)[1];
                    }
                }
                meshs.push_back(newMesh);
            }

            trimesh::TriMesh* newmodel = new trimesh::TriMesh();
            mmesh::mergeTriMesh(newmodel, meshs);
            char buff[256] = { 0 };
            QByteArray qbyteTemp = path.toLocal8Bit();
            qDebug() <<"saveAllSTL path length:"<<qbyteTemp.length();
            qDebug() <<"saveAllSTL path:" << qbyteTemp;
            strcpy(buff, qbyteTemp.data());
            newmodel->write(buff);
            newmodel->clear();
        }
        qDeleteAll(meshs);
    }
    bool readFile(const char* path, unsigned int& fileSize, char* & fileBuf)
    {
        std::ifstream in;
        in.open(path, std::ios::binary | std::ios::in);
        if (!in.is_open())
            return false;
        in.seekg(0, std::ios::end);
        fileSize = in.tellg();
        fileBuf = new char[fileSize];
        in.seekg(0, std::ios::beg);
        in.read(fileBuf, fileSize);
        in.close();
        return true;
    }
    QImage Hex565Str2Image888(const std::string& imgString, int width, int height)
    {
        QImage retImg(width, height, QImage::Format_RGB888);
        std::map<char, unsigned char> hexMap;
        hexMap.insert(std::pair<char, unsigned char>('0', 0));
        hexMap.insert(std::pair<char, unsigned char>('1', 0));
        hexMap.insert(std::pair<char, unsigned char>('2', 0));
        hexMap.insert(std::pair<char, unsigned char>('3', 0));
        hexMap.insert(std::pair<char, unsigned char>('4', 0));
        hexMap.insert(std::pair<char, unsigned char>('5', 0));
        hexMap.insert(std::pair<char, unsigned char>('6', 0));
        hexMap.insert(std::pair<char, unsigned char>('7', 0));
        hexMap.insert(std::pair<char, unsigned char>('8', 0));
        hexMap.insert(std::pair<char, unsigned char>('9', 0));
        hexMap.insert(std::pair<char, unsigned char>('A', 10));
        hexMap.insert(std::pair<char, unsigned char>('B', 11));
        hexMap.insert(std::pair<char, unsigned char>('C', 12));
        hexMap.insert(std::pair<char, unsigned char>('D', 13));
        hexMap.insert(std::pair<char, unsigned char>('E', 14));
        hexMap.insert(std::pair<char, unsigned char>('F', 15));
        int strSize = imgString.length();
        int pixIndex = 0;
        for (int i = 0; i < strSize; i += 4) ///取两个字节字符
        {
            //第一个字节
            qint8 h1 = hexMap[imgString[i]];
            qint8 l1 = hexMap[imgString[i + 1]];
            //第二个字节
            qint8 h2 = hexMap[imgString[i + 2]];
            qint8 l2 = hexMap[imgString[i + 3]];
            //需要交换字节顺序
            qint16 v1 = (h1 << 4) | l1;
            qint16 v2 = (h2 << 4) | l2;
            qint16 pix565 = (v2 << 8) | v1; ///RGB565

            ///RGB888
            qint8 R = pix565 >> 11;
            qint8 G = (pix565 << 5) >> 10;
            qint8 B = pix565 & 0x001F;
            retImg.pixel((i / 4) / 200, (i / 4) % 200);
        }
        return std::move(retImg);
    }

    void writeImageInfo(const QString& fileName, const QImage& image, int eWidth, int eHeight, const QString& sType, int layers)
    {
        QFileInfo fileinfo(fileName);
        QString suffix = fileinfo.suffix();
        suffix = suffix.toLower();

        if ("gcode" != suffix)
        {
            return;
        }

        ImageInfo imgNode;
        imgNode.comment = ';';
        imgNode.identify[0] = 0XAA;
        imgNode.identify[1] = 0X55;
        imgNode.identify[2] = 0XA5;
        imgNode.identify[3] = 0X5A;
        imgNode.len[0] = 0x00;
        imgNode.len[1] = 0x00;
        imgNode.len[2] = 0x00;
        imgNode.len[3] = 0x00;//1 + 1 + 2 + 2 + 2;//8+n
        imgNode.imgType = 0b0;
        imgNode.resolution = 0b0;
        imgNode.modelLineS[0] = 0x00;
        imgNode.modelLineS[1] = 0x00;
        imgNode.modelLineE[0] = 0x00;
        imgNode.modelLineE[1] = 0x00;
        imgNode.modelHeight[0] = 0x00;
        imgNode.modelHeight[1] = 0x00;
        char* pBuff = NULL;//n

        QImage previewImg = image.scaled(eWidth, eHeight, Qt::KeepAspectRatio);

        //图片类型
        //QString sType = "png";//从界面获取
        if ("jpg" == sType)
        {
            imgNode.imgType = 0x00;
        }
        else if ("png" == sType)
        {
            imgNode.imgType = 0x01;
        }

        //分辨率
        if ((36 == eWidth)&&(36 == eHeight))
        {
            imgNode.resolution = 0b0;
        }
        else if ((48 == eWidth) && (48 == eHeight))
        {
            imgNode.resolution = 0b1;
        }
        else if ((64 == eWidth) && (64 == eHeight))
        {
            imgNode.resolution = 0b10;
        }
        else if ((96 == eWidth) && (96 == eHeight))
        {
            imgNode.resolution = 0b11;
        }
        else if ((144 == eWidth) && (144 == eHeight))
        {
            imgNode.resolution = 0b100;
        }
        else if ((200 == eWidth) && (200 == eHeight))
        {
            imgNode.resolution = 0b101;
        }
        else if ((300 == eWidth) && (300 == eHeight))
        {
            imgNode.resolution = 0b110;
        }
        else if ((400 == eWidth) && (400 == eHeight))
        {
            imgNode.resolution = 0b111;
        }
        else if ((600 == eWidth) && (600 == eHeight))
        {
            imgNode.resolution = 0b1000;
        }

        //图片内的有效像素的起点、结束点行值
        int lineStart = 0, lineEnd = 0;
        QColor bmpBack(0, 0, 0);
        bool isValid = false;
        for (int w = 0; w < eHeight; w++)
        {
            isValid = false;
            for (int h = 0; h < eWidth; h++)
            {
                QColor pcolor(previewImg.pixel(w, h));
                if (pcolor != bmpBack)
                {
                    isValid = true;
                    lineStart = w;
                    break;
                }
            }
            if (isValid)
                break;
        }
        for (int w = eHeight - 1; w >= 0; w--)
        {
            isValid = false;
            for (int h = 0; h < eWidth; h++)
            {
                QColor pcolor(previewImg.pixel(w, h));
                if (pcolor != bmpBack)
                {
                    isValid = true;
                    lineEnd = w;
                    break;
                }
            }
            if (isValid)
                break;
        }

        imgNode.modelLineS[0] = lineStart & 0xFF;
        imgNode.modelLineS[1] = lineStart >> 8;
        
        imgNode.modelLineE[0] = lineEnd & 0xFF;
        imgNode.modelLineE[1] = lineEnd >> 8;

        //高度（层数）
        imgNode.modelHeight[0] = layers & 0xFF;
        imgNode.modelHeight[1] = layers >> 8;;

        //图片buff  
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);

        if ("jpg" == sType)
        {
            previewImg.save(&buffer, "JPG");
        }
        else if ("png" == sType)
        {
            previewImg.save(&buffer, "PNG");
        }
        
        unsigned int buffSize = ba.size() + 1;
        pBuff = new char[buffSize];
        memcpy(pBuff, ba.data(), buffSize);

        buffSize += 8;
        imgNode.len[0] = buffSize & 0xFF;
        imgNode.len[1] = (buffSize >> 8) & 0xFF;
        imgNode.len[2] = (buffSize >> 16) & 0xFF;
        imgNode.len[3] = (buffSize >> 24) & 0xFF;
      
        //write
        std::string gcodeFile = fileName.toLocal8Bit().data();       
        std::ofstream out(gcodeFile, std::ios::out | std::ios::binary | std::ios::app);
        if (!out)
        {
            return;
        }
        out.write((char*)&imgNode, sizeof(imgNode));
        out.write(pBuff, buffSize);

        delete[]pBuff;
        pBuff = NULL;
        out.close();
    }
}
