#pragma once

#include <unordered_map>
#include <algorithm>

template <typename D>
class AddressLookupTable
{
public:
	explicit AddressLookupTable(D *pDevice) : pDevice(pDevice) {}
	~AddressLookupTable()
	{
		while (g_map.size())
		{
			auto it = g_map.begin();

			it->second->DeleteMe();

			it = g_map.erase(it);
		}
	}

	template <typename T>
	T *FindAddress(void *Proxy)
	{
		if (Proxy == nullptr)
		{
			return nullptr;
		}

		auto it = g_map.find(Proxy);

		if (it != std::end(g_map))
		{
			return static_cast<T *>(it->second);
		}

		return new T(static_cast<T *>(Proxy), pDevice);
	}

	template <typename T>
	void SaveAddress(T *Wrapper, void *Proxy)
	{
		if (Wrapper != nullptr && Proxy != nullptr)
		{
			g_map[Proxy] = Wrapper;
		}
	}

	template <typename T>
	void DeleteAddress(T *Wrapper)
	{
		if (Wrapper != nullptr)
		{
			auto it = std::find_if(g_map.begin(), g_map.end(),
				[Wrapper](std::pair<void*, class AddressLookupTableObject*> Map) -> bool { return Map.second == Wrapper; });

			if (it != std::end(g_map))
			{
					it = g_map.erase(it);
			}
		}
	}

private:
	D *const pDevice;
	std::unordered_map<void*, class AddressLookupTableObject*> g_map;
};

class AddressLookupTableObject
{
public:
	virtual ~AddressLookupTableObject() { }

	void DeleteMe()
	{
		delete this;
	}
};
