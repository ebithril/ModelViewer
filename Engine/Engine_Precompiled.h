// Engine_Precompiled.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// TODO: reference additional headers your program requires here

#include "../Common Utilities/GrowingArray.h"
#include "../Common Utilities/StaticArray.h"
#include "../Common Utilities/Map.h"
#include "../Common Utilities/Vector.h"
#include "../Common Utilities/Matrix.h"
#include "GfxFactoryWrapper.h"
#include "COMObjectPointer.h"
#include "Instance.h"
#include "Model.h"
#include "Effect.h"
#include <string>
#include "DynamicVertexBufferWrapper.h"
#include "VertexDataWrapper.h"
#include <unordered_map>
#include "Texture.h"
#include "DirectXEngine.h"
#include "Engine.h"
#include "WindowsFunctions.h"
#include "../Common Utilities/ObjectPool.h"
#include "../Common Utilities/EventManager.h"