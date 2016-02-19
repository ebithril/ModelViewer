#include "Common Utilities_Precompiled.h"
#include "XMLReader.h"
#include "Macros.h"
#include "DL_Debug.h"

const std::string locNoFilePath = "No document loaded.";

XMLReader::XMLReader()
{
	myHasOpenedDoc = false;
	myDoc = nullptr;
	myFilePath = locNoFilePath;
}

XMLReader::~XMLReader()
{
	CloseDocument();
}

void XMLReader::OpenDocument(const std::string& aFilePath)
{
	CloseDocument();

	myDoc = new tinyxml2::XMLDocument();
	
	tinyxml2::XMLError error;
	int testTimes = 0;
	do
	{
		error = myDoc->LoadFile(aFilePath.c_str());
		testTimes++;
	} while (error != tinyxml2::XMLError::XML_SUCCESS && testTimes < 1000);

	if (error == tinyxml2::XMLError::XML_SUCCESS)
	{
		myHasOpenedDoc = true;
		myFilePath = aFilePath;
	}
	else
	{
#pragma region Error-Codes
		switch (error)
		{
		case tinyxml2::XML_NO_ATTRIBUTE:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_NO_ATTRIBUTE", aFilePath.c_str());
			break;
		case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_WRONG_ATTRIBUTE_TYPE", aFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_FILE_NOT_FOUND", aFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_FILE_COULD_NOT_BE_OPENED", aFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_FILE_READ_ERROR:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_FILE_READ_ERROR", aFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_ELEMENT_MISMATCH:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_ELEMENT_MISMATCH", aFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_ELEMENT:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_ELEMENT", aFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_ATTRIBUTE", aFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_IDENTIFYING_TAG:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_IDENTIFYING_TAG", aFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_TEXT:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_TEXT", aFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_CDATA:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_CDATA", aFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_COMMENT:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_COMMENT", aFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_DECLARATION:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_DECLARATION", aFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_UNKNOWN:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_UNKNOWN", aFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_EMPTY_DOCUMENT:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_EMPTY_DOCUMENT", aFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_MISMATCHED_ELEMENT", aFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING", aFilePath.c_str());
			break;
		case tinyxml2::XML_CAN_NOT_CONVERT_TEXT:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_CAN_NOT_CONVERT_TEXT", aFilePath.c_str());
			break;
		case tinyxml2::XML_NO_TEXT_NODE:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_NO_TEXT_NODE", aFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_COUNT:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_COUNT", aFilePath.c_str());
			break;
		default:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: Unknown, non of tinyxml's error codes caught it :(", aFilePath.c_str());
			break;
		}
#pragma endregion

		DL_ASSERT("[XMLReader]: Failed to Open XML File. Check Debuglog for error");
	}
}

void XMLReader::CloseDocument()
{
	if (myHasOpenedDoc == true)
	{
		myHasOpenedDoc = false;
		myFilePath = locNoFilePath;
		SAFE_DELETE(myDoc);
	}
}

const std::string& XMLReader::GetMyFilePath()const
{
	return myFilePath;
}

std::string XMLReader::GetMyFileName()const
{
	std::string fileName = myFilePath;

	size_t lastDot = fileName.find_last_of(".");
	if (lastDot != std::string::npos)
	{
		fileName = fileName.substr(0, lastDot);
	}

	size_t lastSlash = fileName.find_last_of("/");
	if (lastSlash != std::string::npos)
	{
		fileName = fileName.substr(lastSlash + 1);
	}

	return fileName;
}

void XMLReader::InvalidTag(XMLElement anElement) const
{
	std::string elementName = anElement->Value();
	std::string errorText = "The <" + elementName + "> element tag is not a valid tag in " + GetMyFilePath();
	DL_ASSERT(errorText.c_str());
}

std::string XMLReader::GetElementName(XMLElement aElement) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [GetElementName] before Opening the document");

	return aElement->Name();
}

/*
	---------------------------
	Normal Find Operations
	---------------------------
	*/

XMLElement XMLReader::FindFirstChild() const
{
	if (myHasOpenedDoc == false)
	{
		DL_ASSERT("[XMLReader]: Tried to [FindFirstChild] before Opening the document");
	}
	return myDoc->FirstChildElement();
}

XMLElement XMLReader::FindFirstChild(const std::string& aChildName) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindFirstChild] before Opening the document");

	return myDoc->FirstChildElement(aChildName.c_str());
}

XMLElement XMLReader::FindFirstChild(XMLElement aParent) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindFirstChild] before Opening the document");
	if (aParent != nullptr)
	{
		return aParent->FirstChildElement();
	}
	else
	{
		return nullptr;
	}
}

XMLElement XMLReader::FindFirstChild(XMLElement aParent, const std::string& aChildName) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindFirstChild] before Opening the document");


	return aParent->FirstChildElement(aChildName.c_str());
}

XMLElement XMLReader::FindNextElement(const std::string& aElementName)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindNextElement] before Opening the document");

	return myDoc->NextSiblingElement(aElementName.c_str());

}

XMLElement XMLReader::FindNextElement(XMLElement aPrevElement) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindNextElement] before Opening the document");

	return aPrevElement->NextSiblingElement();
}

XMLElement XMLReader::FindNextElement(XMLElement aPrevElement, const std::string& aElementName) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindNextElement] before Opening the document");

	return aPrevElement->NextSiblingElement(aElementName.c_str());
}

/*
	---------------------------
	Force Find Operations
	---------------------------
	*/

XMLElement XMLReader::ForceFindFirstChild(const std::string& aChildName) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindFirstChild] before Opening the document");

	DebugFirstChild(aChildName);

	return myDoc->FirstChildElement(aChildName.c_str());
}

XMLElement XMLReader::ForceFindFirstChild(XMLElement aParent) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindFirstChild] before Opening the document");

	DebugFirstChild(aParent);

	return aParent->FirstChildElement();
}

XMLElement XMLReader::ForceFindFirstChild(XMLElement aParent, const std::string& aChildName) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindFirstChild] before Opening the document");


	DebugFirstChild(aParent, aChildName);

	return aParent->FirstChildElement(aChildName.c_str());
}

XMLElement XMLReader::ForceFindNextElement(const std::string& aElementName)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindNextElement] before Opening the document");

	DebugNextElement(aElementName);

	return myDoc->NextSiblingElement(aElementName.c_str());

}

XMLElement XMLReader::ForceFindNextElement(XMLElement aPrevElement) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindNextElement] before Opening the document");

	DebugNextElement(aPrevElement);

	return aPrevElement->NextSiblingElement();
}

XMLElement XMLReader::ForceFindNextElement(XMLElement aPrevElement, const std::string& aElementName) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindNextElement] before Opening the document");

	DebugNextElement(aPrevElement, aElementName);

	return aPrevElement->NextSiblingElement(aElementName.c_str());
}

/*
	---------------------------
	Normal Read Operations
	---------------------------
	*/

bool XMLReader::ReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, std::string& aTargetVariable) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ReadAttribute(string)] before Opening the document");

	if (aElementToReadFrom == nullptr)
		return false;

	if (aElementToReadFrom->FindAttribute(aAttributeToRead.c_str()) != 0)
	{
		aTargetVariable = aElementToReadFrom->Attribute(aAttributeToRead.c_str());
		return true;
	}

	return false;
}

bool XMLReader::ReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, int* aTargetVariable) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ReadAttribute(int)] before Opening the document");

	if (aElementToReadFrom == nullptr)
		return false;

	if (aElementToReadFrom->QueryIntAttribute(aAttributeToRead.c_str(), aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	return false;
}

bool XMLReader::ReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, float* aTargetVariable) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ReadAttribute(float)] before Opening the document");

	if (aElementToReadFrom == nullptr)
		return false;

	if (aElementToReadFrom->QueryFloatAttribute(aAttributeToRead.c_str(), aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	return false;
}

bool XMLReader::ReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, double* aTargetVariable) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ReadAttribute(double)] before Opening the document");

	if (aElementToReadFrom == nullptr)
		return false;

	if (aElementToReadFrom->QueryDoubleAttribute(aAttributeToRead.c_str(), aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	return false;
}

bool XMLReader::ReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, bool* aTargetVariable) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ReadAttribute(bool)] before Opening the document");

	if (aElementToReadFrom == nullptr)
		return false;

	if (aElementToReadFrom->QueryBoolAttribute(aAttributeToRead.c_str(), aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	return false;
}

/*
	---------------------------
	Force Read Operations
	---------------------------
	*/

bool XMLReader::ForceReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, std::string& aTargetVariable) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ForceReadAttribute(string)] before Opening the document");

	if (aElementToReadFrom->FindAttribute(aAttributeToRead.c_str()) != 0)
	{
		aTargetVariable = aElementToReadFrom->Attribute(aAttributeToRead.c_str());
		return true;
	}

	DL_DEBUG("Failed to read Attribute: [ %s ], from Element: [ %s ], in Document: [ %s ]", aAttributeToRead.c_str(), aElementToReadFrom->Name(), myFilePath.c_str());
	DL_ASSERT("Failed to [ForceReadAttribute(string)], check DebugLog for more info");
	return false;
}

bool XMLReader::ForceReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, int* aTargetVariable) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ForceReadAttribute(int)] before Opening the document");

	if (aElementToReadFrom->QueryIntAttribute(aAttributeToRead.c_str(), aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	DL_DEBUG("Failed to read Attribute: [ %s ], from Element: [ %s ], in Document: [ %s ]", aAttributeToRead.c_str(), aElementToReadFrom->Name(), myFilePath.c_str());
	DL_ASSERT("Failed to [ForceReadAttribute(int)], check DebugLog for more info");
	return false;
}

bool XMLReader::ForceReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, float* aTargetVariable) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ReadAttribute(float)] before Opening the document");

	if (aElementToReadFrom->QueryFloatAttribute(aAttributeToRead.c_str(), aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	DL_DEBUG("Failed to read Attribute: [ %s ], from Element: [ %s ], in Document: [ %s ]", aAttributeToRead.c_str(), aElementToReadFrom->Name(), myFilePath.c_str());
	DL_ASSERT("Failed to [ForceReadAttribute(float)], check DebugLog for more info");
	return false;
}

bool XMLReader::ForceReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, double* aTargetVariable) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ReadAttribute(double)] before Opening the document");

	if (aElementToReadFrom->QueryDoubleAttribute(aAttributeToRead.c_str(), aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	DL_DEBUG("Failed to read Attribute: [ %s ], from Element: [ %s ], in Document: [ %s ]", aAttributeToRead.c_str(), aElementToReadFrom->Name(), myFilePath.c_str());
	DL_ASSERT("Failed to [ForceReadAttribute(double)], check DebugLog for more info");
	return false;
}

bool XMLReader::ForceReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, bool* aTargetVariable) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ReadAttribute(bool)] before Opening the document");

	if (aElementToReadFrom->QueryBoolAttribute(aAttributeToRead.c_str(), aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	DL_DEBUG("Failed to read Attribute: [ %s ], from Element: [ %s ], in Document: [ %s ]", aAttributeToRead.c_str(), aElementToReadFrom->Name(), myFilePath.c_str());
	DL_ASSERT("Failed to [ForceReadAttribute(bool)], check DebugLog for more info");
	return false;
}

/*
	---------------------------
	Debug Operations
	---------------------------
	*/

void XMLReader::DebugFirstChild(const std::string& aChildName) const
{
	if (myDoc->FirstChildElement(aChildName.c_str()) == nullptr)
	{
		DL_DEBUG("Failed to [ForceFindFirstChild]. Parent: [ Document ], Child: [ %s ], File: [ %s ]", aChildName.c_str(), myFilePath.c_str());
		DL_ASSERT("Failed to [ForceFindFirstChild], check DebugLog for more info");
	}
}

void XMLReader::DebugFirstChild(XMLElement aParent) const
{
	if (aParent->FirstChildElement() == nullptr)
	{
		DL_DEBUG("Failed to [ForceFindFirstChild]. Parent: [ %s ], Child: [ First ], File:  [ %s ]", aParent->Name(), myFilePath.c_str());
		DL_ASSERT("Failed to [ForceFindFirstChild], check DebugLog for more info");
	}
}

void XMLReader::DebugFirstChild(XMLElement aParent, const std::string& aChildName) const
{
	if (aParent->FirstChildElement(aChildName.c_str()) == nullptr)
	{
		DL_DEBUG("Failed to [ForceFindFirstChild]. Parent: [ %s ], Child: [ %s ], File: [ %s ]", aParent->Name(), aChildName.c_str(), myFilePath.c_str());
		DL_ASSERT("Failed to [ForceFindFirstChild], check DebugLog for more info");
	}
}

void XMLReader::DebugNextElement(const std::string& aChildName) const
{
	if (myDoc->NextSiblingElement(aChildName.c_str()) == nullptr)
	{
		DL_DEBUG("Failed to [ForceFindNextElement]. Parent: [ Document ], Child: [ %s ], File: [ %s ]", aChildName.c_str(), myFilePath.c_str());
		DL_ASSERT("Failed to [ForceFindNextElement], check DebugLog for more info");
	}
}

void XMLReader::DebugNextElement(XMLElement aParent) const
{
	if (aParent->NextSiblingElement() == nullptr)
	{
		DL_DEBUG("Failed to [ForceFindNextElement]. Parent: [ %s ], Child: [ First ], File: [ %s ]", aParent->Name(), myFilePath.c_str());
		DL_ASSERT("Failed to [ForceFindNextElement], check DebugLog for more info");
	}
}

void XMLReader::DebugNextElement(XMLElement aParent, const std::string& aChildName) const
{
	if (aParent->NextSiblingElement(aChildName.c_str()) == nullptr)
	{
		DL_DEBUG("Failed to [ForceFindNextElement]. Parent: [ %s ], Child: [ %s ], File: [ %s ]", aParent->Name(), aChildName.c_str(), myFilePath.c_str());
		DL_ASSERT("Failed to [ForceFindNextElement], check DebugLog for more info");
	}
}