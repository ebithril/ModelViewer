#pragma once
#include "StaticArray.h"


namespace CommonUtilities
{

#define BUFFER_TEMPLATE template< typename Type>
#define BUFFER_OBJECT Buffer<Type>


	BUFFER_TEMPLATE
	class Buffer
	{

	public:

		Buffer();
		~Buffer();

		Type& GetWriteBuffer();
		const Type& GetReadBuffer() const;

		void SwitchBuffers();

	private:
		Type myBuffer1;
		Type myBuffer2;

		Type* myReadBuffer;
		Type* myWriteBuffer;
	};

	BUFFER_TEMPLATE
		Type& BUFFER_OBJECT::GetWriteBuffer()
	{
		return *myWriteBuffer;
	}

	BUFFER_TEMPLATE
		Type& BUFFER_OBJECT::GetReadBuffer()
	{
		return *myReadBuffer;
	}

	BUFFER_TEMPLATE
		BUFFER_OBJECT::Buffer()
	{
		myReadBuffer = myBuffer1;
		myWriteBuffer = myBuffer2;
	}

	BUFFER_TEMPLATE
		void BUFFER_OBJECT::SwitchBuffers()
	{
	}

#undef BUFFER_TEMPLATE
#undef BUFFER_OBJECT

}

namespace CU = CommonUtilities;