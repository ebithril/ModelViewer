#include "Common Utilities_Precompiled.h"
#include "XMLWriter.h"


XMLWriter::XMLWriter()
{
	myHasUnsavedChanges = false;
	myHasCreatedDoc = false;
}

XMLWriter::~XMLWriter()
{
	if (myHasUnsavedChanges == true)
	{
		DL_DEBUG("[XMLWriter]: Destructor got called before everything was saved. File being edited was: %s", mySavePath.c_str());
		DL_ASSERT("[XMLSave]: Dint save all changes to a document");
	}
}

void XMLWriter::CreateEmptyDocument(const std::string& aFilePath)
{
	mySavePath = aFilePath;
	myHasCreatedDoc = true;
}

void XMLWriter::CreateCopyOfDocument(const std::string& aNewFilePath, const std::string& aSourceFilePath)
{
	mySavePath = aNewFilePath;
	
	tinyxml2::XMLError error = myDoc.LoadFile(aSourceFilePath.c_str());
	if (error == 0)
	{
		myDoc.SaveFile(mySavePath.c_str());
		myHasCreatedDoc = true;
	}
	else
	{
#pragma region Error-Codes
		switch (error)
		{
		case tinyxml2::XML_NO_ATTRIBUTE:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_NO_ATTRIBUTE", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_WRONG_ATTRIBUTE_TYPE", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_FILE_NOT_FOUND", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_FILE_COULD_NOT_BE_OPENED", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_FILE_READ_ERROR:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_FILE_READ_ERROR", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_ELEMENT_MISMATCH:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_ELEMENT_MISMATCH", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_ELEMENT:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_ELEMENT", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_ATTRIBUTE", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_IDENTIFYING_TAG:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_IDENTIFYING_TAG", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_TEXT:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_TEXT", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_CDATA:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_CDATA", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_COMMENT:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_COMMENT", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_DECLARATION:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_DECLARATION", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_UNKNOWN:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_UNKNOWN", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_EMPTY_DOCUMENT:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_EMPTY_DOCUMENT", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_MISMATCHED_ELEMENT", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_CAN_NOT_CONVERT_TEXT:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_CAN_NOT_CONVERT_TEXT", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_NO_TEXT_NODE:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_NO_TEXT_NODE", aSourceFilePath.c_str());
			break;
		case tinyxml2::XML_ERROR_COUNT:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: XML_ERROR_COUNT", aSourceFilePath.c_str());
			break;
		default:
			DL_DEBUG("Failed to Open XML File. File: %s, Error: Unknown, non of tinyxml's error codes caught it :(", aSourceFilePath.c_str());
			break;
		}
#pragma endregion

		DL_ASSERT("[XMLWriter]: Failed to Open XML File. Check Debuglog for error");
	}
}

void XMLWriter::SaveDocument()
{
	if (myHasCreatedDoc == false)
	{
		DL_DEBUG("[XMLWriter]: Tried to [SaveDocument] before Creating an document");
		DL_ASSERT("[XMLWriter]: Failed to SaveDocument");
	}

	tinyxml2::XMLError error = myDoc.SaveFile(mySavePath.c_str());

	if (error != 0)
	{
#pragma region Error-Codes
		switch (error)
		{
		case tinyxml2::XML_NO_ATTRIBUTE:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_NO_ATTRIBUTE", mySavePath.c_str());
			break;
		case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_WRONG_ATTRIBUTE_TYPE", mySavePath.c_str());
			break;
		case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_ERROR_FILE_NOT_FOUND", mySavePath.c_str());
			break;
		case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_ERROR_FILE_COULD_NOT_BE_OPENED", mySavePath.c_str());
			break;
		case tinyxml2::XML_ERROR_FILE_READ_ERROR:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_ERROR_FILE_READ_ERROR", mySavePath.c_str());
			break;
		case tinyxml2::XML_ERROR_ELEMENT_MISMATCH:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_ERROR_ELEMENT_MISMATCH", mySavePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_ELEMENT:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_ERROR_PARSING_ELEMENT", mySavePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_ERROR_PARSING_ATTRIBUTE", mySavePath.c_str());
			break;
		case tinyxml2::XML_ERROR_IDENTIFYING_TAG:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_ERROR_IDENTIFYING_TAG", mySavePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_TEXT:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_ERROR_PARSING_TEXT", mySavePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_CDATA:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_ERROR_PARSING_CDATA", mySavePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_COMMENT:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_ERROR_PARSING_COMMENT", mySavePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_DECLARATION:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_ERROR_PARSING_DECLARATION", mySavePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING_UNKNOWN:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_ERROR_PARSING_UNKNOWN", mySavePath.c_str());
			break;
		case tinyxml2::XML_ERROR_EMPTY_DOCUMENT:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_ERROR_EMPTY_DOCUMENT", mySavePath.c_str());
			break;
		case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_ERROR_MISMATCHED_ELEMENT", mySavePath.c_str());
			break;
		case tinyxml2::XML_ERROR_PARSING:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_ERROR_PARSING", mySavePath.c_str());
			break;
		case tinyxml2::XML_CAN_NOT_CONVERT_TEXT:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_CAN_NOT_CONVERT_TEXT", mySavePath.c_str());
			break;
		case tinyxml2::XML_NO_TEXT_NODE:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_NO_TEXT_NODE", mySavePath.c_str());
			break;
		case tinyxml2::XML_ERROR_COUNT:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: XML_ERROR_COUNT", mySavePath.c_str());
			break;
		default:
			DL_DEBUG("Failed to Save XML File. File: %s, Error: Unknown, non of tinyxml's error codes caught it :(", mySavePath.c_str());
			break;
		}
#pragma endregion

		DL_ASSERT("[XMLWriter]: Failed to Save XML File. Check Debuglog for error");
	}
	else
	{
		myHasUnsavedChanges = false;
	}
}

void XMLWriter::ResetDocument()
{
	if (myHasCreatedDoc == false)
	{
		DL_DEBUG("[XMLWriter]: Tried to [ResetDocument] before Creating an document");
		DL_ASSERT("[XMLWriter]: Failed to SaveDocument");
	}

	myDoc.DeleteChildren();
}

XMLElement XMLWriter::InsertElement(const std::string& aElementName)
{
	if (myHasCreatedDoc == false)
	{
		DL_DEBUG("[XMLWriter]: Tried to [InsertElement] before Creating an document");
		DL_ASSERT("[XMLWriter]: Failed to SaveDocument");
	}

	XMLElement newElem = myDoc.NewElement(aElementName.c_str());
	myDoc.InsertEndChild(newElem);
	myHasUnsavedChanges = true;

	return newElem;
}

XMLElement XMLWriter::InsertElement(XMLElement aParentElement, const std::string& aElementName)
{
	if (myHasCreatedDoc == false)
	{
		DL_DEBUG("[XMLWriter]: Tried to [InsertElement] before Creating an document");
		DL_ASSERT("[XMLWriter]: Failed to SaveDocument");
	}

	if (aParentElement == nullptr)
	{
		DL_DEBUG("[XMLWriter]: Tried to [InsertElement ( %s )] to a parentelement that dint exist.", aElementName.c_str());
		DL_ASSERT("[XMLWriter]: Failed to InsertElement");
	}

	XMLElement newElem = myDoc.NewElement(aElementName.c_str());
	aParentElement->InsertEndChild(newElem);
	myHasUnsavedChanges = true;

	return newElem;
}

void XMLWriter::SetAttribute(XMLElement aElement, const std::string& aAttributeName, const char* aValue)
{
	if (myHasCreatedDoc == false)
	{
		DL_DEBUG("[XMLWriter]: Tried to [InsertElement] before Creating an document");
		DL_ASSERT("[XMLWriter]: Failed to SaveDocument");
	}

	if (aElement == nullptr)
	{
		DL_DEBUG("[XMLWriter]: Tried to [SetAttribute] to a Element that dint exist. Attribute: %s, Value: %s", aAttributeName.c_str(), aValue);
		DL_ASSERT("[XMLWriter]: Failed to SetAttribute");
	}

	aElement->SetAttribute(aAttributeName.c_str(), aValue);
	myHasUnsavedChanges = true;

}

void XMLWriter::SetAttribute(XMLElement aElement, const std::string& aAttributeName, const std::string& aValue)
{
	if (myHasCreatedDoc == false)
	{
		DL_DEBUG("[XMLWriter]: Tried to [InsertElement] before Creating an document");
		DL_ASSERT("[XMLWriter]: Failed to SaveDocument");

	}
	if (aElement == nullptr)
	{
		DL_DEBUG("[XMLWriter]: Tried to [SetAttribute] to a Element that dint exist. Attribute: %s, Value: %s", aAttributeName.c_str(), aValue.c_str());
		DL_ASSERT("[XMLWriter]: Failed to SetAttribute");
	}

	aElement->SetAttribute(aAttributeName.c_str(), aValue.c_str());
	myHasUnsavedChanges = true;
}

void XMLWriter::SetAttribute(XMLElement aElement, const std::string& aAttributeName, const int aValue)
{
	if (myHasCreatedDoc == false)
	{
		DL_DEBUG("[XMLWriter]: Tried to [InsertElement] before Creating an document");
		DL_ASSERT("[XMLWriter]: Failed to SaveDocument");
	}

	if (aElement == nullptr)
	{
		DL_DEBUG("[XMLWriter]: Tried to [SetAttribute] to a Element that dint exist. Attribute: %s, Value: %i", aAttributeName.c_str(), aValue);
		DL_ASSERT("[XMLWriter]: Failed to SetAttribute");
	}

	aElement->SetAttribute(aAttributeName.c_str(), aValue);
	myHasUnsavedChanges = true;
}

void XMLWriter::SetAttribute(XMLElement aElement, const std::string& aAttributeName, const float aValue)
{
	if (myHasCreatedDoc == false)
	{
		DL_DEBUG("[XMLWriter]: Tried to [InsertElement] before Creating an document");
		DL_ASSERT("[XMLWriter]: Failed to SaveDocument");
	}

	if (aElement == nullptr)
	{
		DL_DEBUG("[XMLWriter]: Tried to [SetAttribute] to a Element that dint exist. Attribute: %s, Value: %f", aAttributeName.c_str(), aValue);
		DL_ASSERT("[XMLWriter]: Failed to SetAttribute");
	}

	aElement->SetAttribute(aAttributeName.c_str(), aValue);
	myHasUnsavedChanges = true;
}

void XMLWriter::SetAttribute(XMLElement aElement, const std::string& aAttributeName, const double aValue)
{
	if (myHasCreatedDoc == false)
	{
		DL_DEBUG("[XMLWriter]: Tried to [InsertElement] before Creating an document");
		DL_ASSERT("[XMLWriter]: Failed to SaveDocument");
	}

	if (aElement == nullptr)
	{
		DL_DEBUG("[XMLWriter]: Tried to [SetAttribute] to a Element that dint exist. Attribute: %s, Value: %d", aAttributeName.c_str(), aValue);
		DL_ASSERT("[XMLWriter]: Failed to SetAttribute");
	}

	aElement->SetAttribute(aAttributeName.c_str(), aValue);
	myHasUnsavedChanges = true;
}

void XMLWriter::SetAttribute(XMLElement aElement, const std::string& aAttributeName, const bool aValue)
{
	if (myHasCreatedDoc == false)
	{
		DL_DEBUG("[XMLWriter]: Tried to [InsertElement] before Creating an document");
		DL_ASSERT("[XMLWriter]: Failed to SaveDocument");
	}

	if (aElement == nullptr)
	{
		if (aValue == true)
			DL_DEBUG("[XMLWriter]: Tried to [SetAttribute] to a Element that dint exist. Attribute: %s, Value: %s", aAttributeName.c_str(), "true");
		else
			DL_DEBUG("[XMLWriter]: Tried to [SetAttribute] to a Element that dint exist. Attribute: %s, Value: %s", aAttributeName.c_str(), "false");

		DL_ASSERT("[XMLWriter]: Failed to SetAttribute");
	}

	aElement->SetAttribute(aAttributeName.c_str(), aValue);
	myHasUnsavedChanges = true;
}