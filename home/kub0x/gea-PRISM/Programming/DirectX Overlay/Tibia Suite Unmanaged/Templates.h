#pragma once

template <class T>
class Singleton 
{
protected:
	Singleton() { }
private:
	static T* myInstance;
public:
	static T* getInstance() {
		if (myInstance == nullptr)
			myInstance = new T();
		return myInstance;
	}
};

template <class T>
T* Singleton<T>::myInstance = NULL;
