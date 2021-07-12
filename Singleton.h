#pragma once
#include <exception>

template<typename T>
class Singleton
{
public:
	static T& get()
	{
		return *instance;
	}

	static void create()
	{
		if (instance)
			throw std::exception("Singleton has already been created!");

		instance = new T();
	}

	static void release()
	{
		if (instance == nullptr)
			throw std::exception("Singleton has already been released!");

		delete instance;
		instance = nullptr;
	}

	Singleton(const Singleton&) = delete;
	Singleton operator=(const Singleton&) = delete;

protected:
	Singleton() = default;
	~Singleton() = default;

	static T* instance;
};
