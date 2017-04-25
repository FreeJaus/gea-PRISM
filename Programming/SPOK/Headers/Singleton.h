#pragma once

template<class T> class Singleton{

protected:

	static T *myInstance;

	Singleton<T>() {};

public:

	static T* GetInstance() { return myInstance; }

};

template <class T> T* Singleton<T>::myInstance = new T();