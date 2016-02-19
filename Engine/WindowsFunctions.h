#pragma once
#include <Windows.h>
#include <string>
#include "../Common Utilities/GrowingArray.h"

namespace WindowsFunctions
{
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	std::wstring WideStringConvertion(const char* aString);
	std::string GetComError(HRESULT aHresult);
	std::string GetWindowsLastError();

	void AssertComAndWindowsError(HRESULT aHresult, const std::string& aCustomMessage = "");
	void AssertWindowsError(const std::string& aCustomMessage = "");

	CU::GrowingArray<std::string> GetFilesWithExtension(const std::string& aDirectory, const std::string& anExtension);
	std::string GetFileNameWithoutExtension(const std::string& aFilePath);
}

namespace WF = WindowsFunctions;

