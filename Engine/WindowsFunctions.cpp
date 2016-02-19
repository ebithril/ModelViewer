#include "Engine_Precompiled.h"
#include "WindowsFunctions.h"
#include "../Common Utilities/EventManager.h"
#include <pplinterface.h>
#include <codecvt>
#include <comdef.h>
#include <thread>
#include <filesystem>
#include <iostream>
#include "../Common Utilities/CUString.h"

namespace WindowsFunctions
{
	bool locIsResizing;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		hWnd;
		wParam;
		lParam;
		PAINTSTRUCT ps;
		HDC hdc;

		switch (uMsg)
		{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			CU::EventManager::GetInstance()->AddEvent(CU::Event(CU::eEvent::QUIT));
			break;

		case WM_ACTIVATE:

			if (LOWORD(wParam) == WA_INACTIVE)
			{
				CU::EventManager::GetInstance()->AddEvent(CU::Event(CU::eEvent::PAUSE));
			}
			else
			{
				CU::EventManager::GetInstance()->AddEvent(CU::Event(CU::eEvent::RESUME));
			}

			break;

		case WM_SIZE:
		{
			if (wParam == SIZE_MINIMIZED)
			{
				CU::EventManager::GetInstance()->AddEvent(CU::Event(CU::eEvent::PAUSE));
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				CU::EventManager::GetInstance()->AddEvent(CU::Event(CU::eEvent::RESUME));

				CU::EventManager::GetInstance()->AddEvent(CU::Event(CU::eEvent::RESIZE));
			}
			else if (wParam == SIZE_RESTORED)
			{

				//// Restoring from minimized state?
				if (locIsResizing == true)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					CU::EventManager::GetInstance()->AddEvent(CU::Event(CU::eEvent::RESUME));
					CU::EventManager::GetInstance()->AddEvent(CU::Event(CU::eEvent::RESIZE));
				}
			}
		}
			break;

		case WM_ENTERSIZEMOVE:
			CU::EventManager::GetInstance()->AddEvent(CU::Event(CU::eEvent::PAUSE));
			locIsResizing = true;
			break;

			// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
			// Here we reset everything based on the new window dimensions.
		case WM_EXITSIZEMOVE:
		{
			CU::EventManager::GetInstance()->AddEvent(CU::Event(CU::eEvent::RESUME));
			locIsResizing = false;
			CU::EventManager::GetInstance()->AddEvent(CU::Event(CU::eEvent::RESIZE));
			break;
		}

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
			break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	std::wstring WideStringConvertion(const char* aString)
	{
		/*std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(aString);*/

		// newsize describes the length of the 
		// wchar_t string called wcstring in terms of the number 
		// of wide characters, not the number of bytes.
		size_t newsize = strlen(aString) + 1;

		// The following creates a buffer large enough to contain 
		// the exact number of characters in the original string
		// in the new format. If you want to add more characters
		// to the end of the string, increase the value of newsize
		// to increase the size of the buffer.
		wchar_t wcstring[4096];

		// Convert char* string to a wchar_t* string.
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, aString, _TRUNCATE);
		// Display the result and indicate the type of string that it is.

		std::wstring wString(wcstring);

		return wString;
	}

	std::string GetComError(HRESULT aHresult)
	{
		_com_error comError(aHresult);
		return comError.ErrorMessage();
	}

	std::string GetWindowsLastError()
	{
		//Get the error message, if any.
		DWORD errorMessageID = ::GetLastError();
		if (errorMessageID == 0)
			return "No error message has been recorded from the operating system.";

		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		std::string message(messageBuffer, size);

		//Free the buffer.
		LocalFree(messageBuffer);

		return message;
	}

	void AssertComAndWindowsError(HRESULT aHresult, const std::string& aCustomMessage /*= ""*/)
	{
		std::string errorMessage = aCustomMessage + "\nCom error message: " + GetComError(aHresult) + "\nWindows error Message: " + GetWindowsLastError();

		DL_ASSERT(errorMessage.c_str());
	}

	void AssertWindowsError(const std::string& aCustomMessage /*= ""*/)
	{
		std::string errorMessage = aCustomMessage + "\nWindows error message: " + GetWindowsLastError();

		DL_ASSERT(errorMessage.c_str());
	}

	CU::GrowingArray<std::string> GetFilesWithExtension(const std::string& aDirectory, const std::string& anExtension)
	{
		CU::GrowingArray<std::string> myfiles(4);

		for (std::tr2::sys::recursive_directory_iterator i(aDirectory.c_str()), end; i != end; ++i)
		{
			if (!is_directory(i->path()))
			{
				if (strstr(i->path().filename().c_str(), anExtension.c_str()))
				{
					myfiles.Add(i->path().relative_path());
				}
			}
		}
			

		return myfiles;
	}

	std::string GetFileNameWithoutExtension(const std::string& aFilePath)
	{
		CU::String<240> fileName;

		std::tr2::sys::path filePath = aFilePath;

		fileName = filePath.filename().c_str();

		int pointIndex = fileName.Find(".");

		fileName = fileName.SubStr(0, pointIndex);

		return std::string(fileName.c_str());
	}
}