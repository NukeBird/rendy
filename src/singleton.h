#pragma once
#include <memory>

template<typename T>
class Singleton
{
public:
	static std::shared_ptr<T> get_instance()
	{
		static auto obj = std::make_shared<T>();
		return obj;
	}

	Singleton(const Singleton&) = delete;
	Singleton& operator= (const Singleton) = delete;
protected:
	Singleton() = default;
	~Singleton() = default;
};