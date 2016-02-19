#pragma once
#include <random>
#include <time.h>

#define MIN(aNumberA, aNumberB)					\
	((aNumberA) < (aNumberB)) ? (aNumberA) : (aNumberB)

#define MAX(aNumberA, aNumberB)	\
	((aNumberA) > (aNumberB)) ? (aNumberA) : (aNumberB)		

#define SAFE_DELETE(aPointer)	\
	delete (aPointer);			\
	(aPointer) = nullptr;

#define SAFE_DELETE_ARRAY(aPointer)	\
	delete[] (aPointer);			\
	(aPointer) = nullptr;									

#define RANDOM(aMinNumber, aMaxNumber)							\
	aMinNumber + (rand() % (aMaxNumber - aMinNumber + 1))	

#define RANDOM_FLOAT(aMax, aMin) ((float(rand()) / float(RAND_MAX)) * (float(aMax) - float(aMin))) + float(aMin)

#define RANDOM_INITIALIZER							\
	srand(static_cast<unsigned int>(time(0)));		

#define SWAP(aDataA, aDataB)	\
	auto swapData = (aDataA);	\
	(aDataA) = (aDataB);		\
	(aDataB) = swapData;		

#define PI 3.1415926535f

#define ABSOLUTE_VALUE(aNumber)	((aNumber)>=0 ? (aNumber) : (-aNumber))

#define GET_PERCENTAGE(aNumerator, aDenominator) ((aNumerator)*100/(aDenominator))

#define RADIANS_TO_DEGREE(aRadians) \
	(static_cast<float>(((aRadians) * (180 / PI)))		\

#define DEGREE_TO_RADIAN(aDegree)	\
	(static_cast<float>((aDegree) * (PI / 180)))		

#define _60FPSInMicroseconds 16666

#define _30FPSInMircoseconds 33333

#define _60FPSInSeconds 0.0166667f

//#define EXPORT_DLL __declspec(dllexport)
//
//#define IMPORT_DLL __declspec(dllimport)