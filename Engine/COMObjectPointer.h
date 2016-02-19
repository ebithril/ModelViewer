#pragma once

#include <Unknwn.h>

namespace GraphicsEngine
{
	template<typename TYPE>
	class COMObjectPointer
	{
	public:
		COMObjectPointer();
		COMObjectPointer(TYPE* aPointer);
		COMObjectPointer(const COMObjectPointer& aPointer);
		~COMObjectPointer();

		const COMObjectPointer& operator=(const COMObjectPointer& aPointer);
		const COMObjectPointer& operator=(TYPE* aPointer);

		TYPE* operator->();
		const TYPE* operator->() const;

		TYPE& operator*();
		const TYPE& operator*() const;

		TYPE** operator&();

		TYPE* Get();
		const TYPE* Get() const;

		bool operator==(const COMObjectPointer& aPointer) const;
	private:
		void AddRef();
		void Release();

		IUnknown* myPointer;
	};

	template<typename TYPE>
	COMObjectPointer<TYPE>::COMObjectPointer()
	{
		myPointer = nullptr;
	}

	template<typename TYPE>
	COMObjectPointer<TYPE>::COMObjectPointer(TYPE* aPointer)
	{
		myPointer = nullptr;

		*this = aPointer;
	}

	template<typename TYPE>
	COMObjectPointer<TYPE>::COMObjectPointer(const COMObjectPointer<TYPE>& aPointer)
	{
		myPointer = nullptr;

		*this = aPointer;
	}

	template<typename TYPE>
	COMObjectPointer<TYPE>::~COMObjectPointer()
	{
		Release();
	}


	template<typename TYPE>
	const COMObjectPointer<TYPE>& COMObjectPointer<TYPE>::operator=(const COMObjectPointer<TYPE>& aPointer)
	{
		Release();

		myPointer = aPointer.myPointer;

		AddRef();

		return *this;
	}

	template<typename TYPE>
	const COMObjectPointer<TYPE>& COMObjectPointer<TYPE>::operator=(TYPE* aPointer)
	{
		Release();

		myPointer = aPointer;

		AddRef();

		return *this;
	}
	

	template<typename TYPE>
	TYPE* COMObjectPointer<TYPE>::operator->()
	{
		return Get();
	}
	
	template<typename TYPE>
	const TYPE* COMObjectPointer<TYPE>::operator->() const
	{
		return Get();
	}

	template<typename TYPE>
	TYPE& COMObjectPointer<TYPE>::operator*()
	{
		return *Get();
	}
	
	template<typename TYPE>
	const TYPE& COMObjectPointer<TYPE>::operator*() const
	{
		return *Get();
	}

	template<typename TYPE>
	TYPE** COMObjectPointer<TYPE>::operator&()
	{
		return reinterpret_cast<TYPE**>(&myPointer);
	}

	template<typename TYPE>
	TYPE* COMObjectPointer<TYPE>::Get()
	{
		return reinterpret_cast<TYPE*>(myPointer);
	}

	template<typename TYPE>
	const TYPE* COMObjectPointer<TYPE>::Get() const
	{
		return reinterpret_cast<const TYPE*>(myPointer);
	}

	template<typename TYPE>
	bool COMObjectPointer<TYPE>::operator==(const COMObjectPointer<TYPE>& aPointer) const
	{
		return myPointer == aPointer.myPointer;
	}

	template<typename TYPE>
	void COMObjectPointer<TYPE>::AddRef()
	{
		if (myPointer != nullptr)
		{
			myPointer->AddRef();
		}
	}
	
	template<typename TYPE>
	void COMObjectPointer<TYPE>::Release()
	{
		if (myPointer != nullptr)
		{
			myPointer->Release();
			myPointer = nullptr;
		}
	}
}