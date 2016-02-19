#pragma once

template <typename T>
class AutoPointer
{
public:
	AutoPointer();
	AutoPointer(AutoPointer<T>& aAutoPointer);
	AutoPointer(T* aPointer);
	~AutoPointer();

	AutoPointer<T>& operator=(AutoPointer<T>& aAutoPointer);
	AutoPointer<T>& operator=(T* aPointer);
	__forceinline T& operator*();
	__forceinline const T& operator*() const;

	__forceinline T* operator->();

	inline T* get();
	inline const T* get() const;

	T* release();
	void reset(T* aPointer = nullptr);
private:
	T* myPointer;
};

template <typename T>
AutoPointer<T>::AutoPointer()
{
	myPointer = nullptr;
}

template <typename T>
AutoPointer<T>::AutoPointer(AutoPointer<T>& aAutoPointer)
{
	*this = aAutoPointer;
}

template <typename T>
AutoPointer<T>::AutoPointer(T* aPointer)
{
	*this = aPointer;
}

template <typename T>
AutoPointer<T>::~AutoPointer()
{
	reset();
}

template <typename T>
AutoPointer<T>& AutoPointer<T>::operator=(AutoPointer<T>& aAutoPointer)
{
	return *this = aAutoPointer.release();
}

template <typename T>
AutoPointer<T>& AutoPointer<T>::operator=(T* aPointer)
{
	myPointer = aPointer;
	return *this;
}

template <typename T>
T& AutoPointer<T>::operator*()
{
	return *get();
}

template <typename T>
const T& AutoPointer<T>::operator*() const
{
	return *get();
}

template <typename T>
T* AutoPointer<T>::operator->()
{
	return get();
}

template <typename T>
T* AutoPointer<T>::get()
{
	return myPointer;
}

template <typename T>
const T* AutoPointer<T>::get() const
{
	return myPointer;
}

template <typename T>
T* AutoPointer<T>::release()
{
	T* pointer = myPointer;
	myPointer = nullptr;

	return pointer;
}

template <typename T>
void AutoPointer<T>::reset(T* aPointer)
{
	delete[] myPointer;
	myPointer = aPointer;
}