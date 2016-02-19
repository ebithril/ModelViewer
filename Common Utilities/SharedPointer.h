#pragma once

#include <atomic>

template <typename T>
class SharedPointer
{
public:
	SharedPointer();
	SharedPointer(const SharedPointer<T>& aSharedPointer);
	SharedPointer(T* const aPointer);
	~SharedPointer();

	bool operator==(const SharedPointer<T>& aSharedPointer) const;
	bool operator==(const T* const aPointer) const;

	bool operator!=(const SharedPointer<T>& aSharedPointer) const;
	bool operator!=(const T* const aPointer) const;

	const SharedPointer<T>& operator=(const SharedPointer<T>& aSharedPointer);
	const SharedPointer<T>& operator=(T* const aPointer);

	void reset(T* const aPointer = nullptr);
	inline T* get();
	inline const T* const get() const;

	__forceinline T& operator*();
	__forceinline const T& operator*() const;

	__forceinline T* operator->();
	__forceinline const T* const operator->() const;

	bool unique();
	int use_count();
private:
	std::atomic<int>* myCount;
	T* myPointer;
};

template <typename T>
SharedPointer<T>::SharedPointer()
{
	myCount = nullptr;
	myPointer = nullptr;
}

template <typename T>
SharedPointer<T>::SharedPointer(const SharedPointer<T>& aSharedPointer)
{
	myPointer = nullptr;
	myCount = nullptr;

	*this = aSharedPointer;
}

template <typename T>
SharedPointer<T>::SharedPointer(T* const aPointer)
{
	myPointer = nullptr;
	myCount = nullptr;

	*this = aPointer;
}

template <typename T>
SharedPointer<T>::~SharedPointer()
{
	reset();
}

template <typename T>
bool SharedPointer<T>::operator==(const SharedPointer<T>& aSharedPointer) const
{
	return *this == aSharedPointer.myPointer;
}

template <typename T>
bool SharedPointer<T>::operator==(const T* const aPointer) const
{
	return myPointer == aPointer;
}

template <typename T>
bool SharedPointer<T>::operator!=(const SharedPointer<T>& aSharedPointer) const
{
	return *this != aSharedPointer;
}

template <typename T>
bool SharedPointer<T>::operator!=(const T* const aPointer) const
{
	return myPointer != aPointer;
}

template <typename T>
const SharedPointer<T>& SharedPointer<T>::operator=(const SharedPointer<T>& aSharedPointer)
{
	reset();

	myPointer = aSharedPointer.myPointer;
	myCount = aSharedPointer.myCount;

	if (myCount != nullptr)
	{
		*myCount += 1;
	}

	return *this;
}

template <typename T>
const SharedPointer<T>& SharedPointer<T>::operator=(T* const aPointer)
{
	reset(aPointer);

	return *this;
}

template <typename T>
void SharedPointer<T>::reset(T* const aPointer)
{
	if (myCount != nullptr)
	{
		if (--(*myCount) <= 0)
		{
			delete[] myPointer;
			myPointer = nullptr;

			delete myCount;
			myCount = nullptr;
		}
	}

	myPointer = aPointer;

	if (myPointer == nullptr)
	{
		myCount = nullptr;
	}
	else
	{
		myCount = new std::atomic<int>(1);
	}
}

template <typename T>
T* SharedPointer<T>::get()
{
	return myPointer;
}

template <typename T>
const T* const SharedPointer<T>::get() const
{
	return myPointer;
}

template <typename T>
T& SharedPointer<T>::operator*()
{
	return *get();
}

template <typename T>
const T& SharedPointer<T>::operator*() const
{
	return *get();
}

template <typename T>
T* SharedPointer<T>::operator->()
{
	return get();
}

template <typename T>
const T* const SharedPointer<T>::operator->() const
{
	return get();
}


template <typename T>
bool SharedPointer<T>::unique()
{
	return myCount != nullptr && *myCount == 1;
}

template <typename T>
int SharedPointer<T>::use_count()
{
	return *myCount;
}