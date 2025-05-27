#ifndef __MYTOOLS_H__
#define __MYTOOLS_H__
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

class tools
{
public:
    tools();
    ~tools();
    //���ַ���ת��Ϊ������
    static float StrToFloat(const std::string& value);

    //���ַ���ת��Ϊ������
    static int StrToInt(const std::string& strSrc);

    //���ַ���ת��Ϊbool
    static bool StrToBool(const std::string& strSrc);



    //������ת��Ϊ�ַ���
    static string float2Str(float floateger);

    //intת��Ϊ�ַ���
    static string int2Str(int num);

    //boolת��Ϊ�ַ���
    static string bool2Str(bool bValue);

    //�����ļ��� SourceFile Դ�ļ�  NewFile Ŀ���ļ�
    static bool ForCopyFile(const char* SourceFile, const char* NewFile);//

    //�ָ��ַ��� Դ��Src��Ŀ�괮Vctdest  �ָ��c
    static void SplitString(const std::string& Src, std::vector<std::string>& Vctdest, const std::string& c);

    /* C++��ȥ���ַ�����β��˫���ź��� */
    static int trim_z(std::string& s);
    /* C��ȥ���ַ�����β��˫���ź���  */
    static int trim_z(char* s);

    /* C��ȥ���ַ�����(��)�ո���  */
    static int ltrim_z(char* s);
    /* C��ȥ���ַ���β(��)�ո���  */
    static int rtrim_z(char* s);

    /* ȥ���Ҳ���(һ��)�ַ�(\n)*/
    static int rtrim_n_z(char* s, char ch);
    /* ȥ�������(һ��)�ַ�(\n)*/
    static int ltrim_n_z(char* s, char ch);
    /* ȥ������ס��Ҳ���(һ��)�ַ�(\n)*/
    static int trim_n_z(char* s, char ch);

    /*  �ָ��ַ���
    @pch ��Ҫ�ָ���ַ���
    @sp �ָ���ţ��磺", : _ -"��
    @return vector<char*>
    ���ӣ�
    pch="����������"; sp=":"
    vector<char*> vec;
    vec[0] = "����";
    vec[1] = "����";*/
    static std::vector<char*> split_z(char* pch, char* sp);
};


#endif
