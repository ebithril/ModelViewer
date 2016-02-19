#pragma once

#include "DL_Debug.h"
#include "tinyxml2.h"
#include <string>

#define XMLElement tinyxml2::XMLElement*

class XMLWriter
{
public:
	XMLWriter();
	~XMLWriter();

	/*
	"Creates" an empty document. Dosnt acctualy save the document to disk before calling SaveDocument though
	*/
	void CreateEmptyDocument(const std::string& aFilePath);

	/*
	Creates an copy of the SourceFile, saves it to disk at NewFilePath and sets the new document as the document that will get edited.
	Comes with all the normal error-checking when it comes to reading the SourceFile
	*/
	void CreateCopyOfDocument(const std::string& aNewFilePath, const std::string& aSourceFilePath);

	/*
	Saves all the added Elements/Attributes to disk.
	*/
	void SaveDocument();


	/*
	Removes everything that has been added to the document. It will NOT affect the changes that has been saved to disk,
	unless you call SaveDocument() after reseting, then it will save an empty document.
	*/
	void ResetDocument();


	/*
	Inserts an element under the selected parent. If no parent is entered the element is inserted as an
	root element of the document.
	Returns the newly created element so that the user to save a pointer to it and later Inserts more
	elements under it, or to SetAttributes to the element.
	*/
	XMLElement InsertElement(const std::string& aElementName);
	XMLElement InsertElement(XMLElement aParentElement, const std::string& aElementName);


	/*
	Adds a new Attribute to the element.
	If aElement is an element named "Test", then SetAttribute(element, "NewAttrib", 123.f) will generate:
	<Test NewAttrib="123"/>
	calling SetAttribute(element, "NewAttrib", 123.f) again will generate:
	<Test NewAttrib="123" NewAttrib="123"/>
	*/
	void SetAttribute(XMLElement aElement, const std::string& aAttributeName, const char* aValue);
	void SetAttribute(XMLElement aElement, const std::string& aAttributeName, const std::string& aValue);
	void SetAttribute(XMLElement aElement, const std::string& aAttributeName, const int aValue);
	void SetAttribute(XMLElement aElement, const std::string& aAttributeName, const float aValue);
	void SetAttribute(XMLElement aElement, const std::string& aAttributeName, const double aValue);
	void SetAttribute(XMLElement aElement, const std::string& aAttributeName, const bool aValue);

private:
	tinyxml2::XMLDocument myDoc;
	std::string mySavePath;
	bool myHasUnsavedChanges;
	bool myHasCreatedDoc;
};

