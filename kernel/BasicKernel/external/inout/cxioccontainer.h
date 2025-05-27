#pragma once
#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <string>

enum HANDLETYPE
{
	EOPEN,
	ESAVE
};

////�ļ������Ϳ���IOCContainer,
template <class T>
class CXIocContainer
{
public:
	CXIocContainer(void) {};
	virtual ~CXIocContainer() {};
public:
	//////ģʽ���Ʒ�ת����
	////��pType Ϊ��ʱ��Ϊע������
	template<class Drived>
	void RegisterDealType(std::string typeKey, Drived* pType = nullptr)
	{
		Drived* p = pType;
		if (!pType)
		{
			p = new Drived();
		}
		else
		{
			p = (Drived*)pType;
		}
		std::function<T* ()> function = [=] {return p; };

		RegisterType(typeKey, function);////ע������
	}

	/////unregister and free object
	template<class Drived>
	void UnRegisterDealType(std::string typeKey)
	{
		auto it = m_openTypeMap.find(typeKey);
		if (it != m_openTypeMap.end())
		{
			Drived* p = (it->second)();
			m_openTypeMap.erase(it);
			delete p;
			p = nullptr;
		}
	}

	///////��ȡ�����еĶ���ָ��
	T* Resolve(std::string strKey)
	{
		if (m_openTypeMap.find(strKey) == m_openTypeMap.end())
		{
			return nullptr;
		}
		std::function<T* ()> function = m_openTypeMap[strKey];
		return function();
	}

	////////����ָ��
	//std::shared_ptr<T> ResolveShared(std::string strKey)
	//{
	//	T* ptr = Resolve(strKey);
	//	return std::shared_ptr<T>(ptr);
	//}
private:
	void RegisterType(std::string typeKey, std::function<T* ()> creator)
	{
		if (m_openTypeMap.find(typeKey) != m_openTypeMap.end())
		{
			printf( "exist this key type");
			throw std::invalid_argument("exist this key type");
			return;
		}
		m_openTypeMap.emplace(typeKey, creator);
	}
private:
	std::map<std::string, std::function<T* ()>> m_openTypeMap;
};

