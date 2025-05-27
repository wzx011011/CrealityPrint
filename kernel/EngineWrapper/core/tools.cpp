#include "tools.h"

#include <stdio.h>
#include <string.h>
#ifndef WIN32
char *_strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}
#endif

tools::tools()
{
}

tools::~tools()
{
}

float tools::StrToFloat(const std::string& strSrc)
{
    return atof(strSrc.data());
}

int tools::StrToInt(const std::string& strSrc)
{
    return atoi(strSrc.data());
}

bool tools::StrToBool(const std::string& strSrc)
{
    if (strSrc == "false" || strSrc == "FALSE")
    {
        return false;
    }
    else
    {
        return true;
    }
}


string tools::float2Str(float floateger)
{
    return std::to_string(floateger);
}

string tools::int2Str(int num)
{
    return std::to_string(num);
}

string tools::bool2Str(bool bValue)
{
    if (bValue)
    {
        return  "true";
    }
    else
    {
        return "false";
    }
}


bool tools::ForCopyFile(const char *SourceFile, const char *NewFile)
{
    std::ifstream in;
    std::ofstream out;
    in.open(SourceFile, ios::binary);//��Դ�ļ�
    if (in.fail())//��Դ�ļ�ʧ��
    {
        cout << "Error 1: Fail to open the source file." << endl;
        in.close();
        out.close();
        return false;
    }
    out.open(NewFile, ios::binary);//����Ŀ���ļ� 
    if (out.fail())//�����ļ�ʧ��
    {
        cout << "Error 2: Fail to create the new file." << endl;
        out.close();
        in.close();
        return false;
    }
    else//�����ļ�
    {
        out << in.rdbuf();
        out.close();
        in.close();
        return true;
    }
}


void tools::SplitString(const std::string& Src, std::vector<std::string>& Vctdest, const std::string& c)
{
    std::string::size_type pos1, pos2;
    pos2 = Src.find(c);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        Vctdest.push_back(Src.substr(pos1, pos2 - pos1));

        pos1 = pos2 + c.size();
        pos2 = Src.find(c, pos1);
    }
    if (pos1 != Src.length())
    {
        Vctdest.push_back(Src.substr(pos1));
    }

}

int tools::trim_z(std::string& s)
{
    if (s.empty())
    {
        return 0;
    }
    s.erase(0, s.find_first_not_of('"'));
    s.erase(s.find_last_not_of('"') + 1);

    return 1;
}

int tools::ltrim_z(char* s)
{
    _strrev(s);
    rtrim_z(s);
    _strrev(s);
    return 1;
}

int tools::rtrim_z(char* s)
{
    int s_len = strlen(s);
    int x = 0;
    for (int i = s_len - 1; i >= 0; i--)
    {
        if (i == s_len - 1 && s[i] == '"')
        {
            s[i] = '\0';
            break;
        }
        x = 1;
        break;
    }
    if (x == 0 && strlen(s) != 0)
        rtrim_z(s);
    return 1;
}

int tools::trim_z(char* s)
{
    rtrim_z(s);
    ltrim_z(s);
    return 1;
}

std::vector<char*> tools::split_z(char* pch, char* sp)
{
    pch = strtok(pch, sp);

    std::vector<char*> vec;
    int i = 0;
    while (pch != NULL)
    {
        ltrim_z(pch);
        vec.push_back(pch);
        pch = strtok(NULL, sp);
        i++;
    }
    return vec;
}

int tools::rtrim_n_z(char* s, char ch)
{
    int s_len = strlen(s);
    int x = 0;
    for (int i = s_len - 1; i >= 0; i--)
    {
        if (i == s_len - 1 && s[i] == ch)
        {
            s[i] = '\0';
            break;
        }
        x = 1;
        break;
    }
    if (x == 0 && strlen(s) != 0)
        rtrim_n_z(s, ch);
    return 1;
}

int tools::ltrim_n_z(char* s, char ch)
{
    _strrev(s);
    int s_len = strlen(s);
    int x = 0;
    for (int i = s_len - 1; i >= 0; i--)
    {
        if (i == s_len - 1 && s[i] == ch)
        {
            s[i] = '\0';
            break;
        }
        x = 1;
        break;
    }
    _strrev(s);
    if (x == 0 && strlen(s) != 0)
        ltrim_n_z(s, ch);
    return 1;
}

int tools::trim_n_z(char* s, char ch)
{
    rtrim_n_z(s, ch);
    ltrim_n_z(s, ch);
    return 1;
}
