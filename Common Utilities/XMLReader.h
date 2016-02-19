#pragma once

#include "tinyxml2.h"
#include <string>

#define XMLElement tinyxml2::XMLElement*

class XMLReader
{
public:
	XMLReader();
	~XMLReader();

	void OpenDocument(const std::string& aFilePath);
	void CloseDocument();

	const std::string& GetMyFilePath()const;
	std::string GetMyFileName()const;

	void InvalidTag(XMLElement anElement) const;

	std::string GetElementName(XMLElement aElement) const; 

	/*
	---------------------------
	  Normal Find Operations
	---------------------------

	These will try to find the requested element, if the element is not found
	a nullptr will be returned
	*/
	XMLElement FindFirstChild()const;
	XMLElement FindFirstChild(const std::string& aChildName) const;
	XMLElement FindFirstChild(XMLElement aParent) const;
	XMLElement FindFirstChild(XMLElement aParent, const std::string& aChildName) const;
	XMLElement FindNextElement(const std::string& aElementName);
	XMLElement FindNextElement(XMLElement aPrevElement) const;
	XMLElement FindNextElement(XMLElement aPrevElement, const std::string& aElementName) const;

	/*
	---------------------------
	Force Find Operations
	---------------------------

	These will try to find the requested element, however it will
	not return a nullptr if the element isnt found.
	Instead it will crash and print an errormessage into
	the DebugLogger about the involved Parent, Child, Element and Document
	*/

	XMLElement ForceFindFirstChild(const std::string& aChildName) const;
	XMLElement ForceFindFirstChild(XMLElement aParent) const;
	XMLElement ForceFindFirstChild(XMLElement aParent, const std::string& aChildName) const;
	XMLElement ForceFindNextElement(const std::string& aElementName);
	XMLElement ForceFindNextElement(XMLElement aPrevElement) const;
	XMLElement ForceFindNextElement(XMLElement aPrevElement, const std::string& aElementName) const;

	/*
	---------------------------
	   Normal Read Operations
	---------------------------

	These will try to read the attribute from the element and store the value
	into the targetvariable.
	If the attribute is not found, the targetvariable simply wont be changed,
	the program wont crash or even produce an error.
	*/

	bool ReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, std::string& aTargetVariable) const;
	bool ReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, int* aTargetVariable)const;
	bool ReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, float* aTargetVariable) const;
	bool ReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, double* aTargetVariable) const;
	bool ReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, bool* aTargetVariable) const;

	/*
	---------------------------
	   Force Read Operations
	---------------------------

	These will try to read the attribute from the element and store the value
	into the targetvariable.
	If the attribute is not found, the program will crash and print an
	error message to the DebugLogger about the involved Element, Attribute and Document.
	*/

	bool ForceReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, std::string& aTargetVariable)const;
	bool ForceReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, int* aTargetVariable)const;
	bool ForceReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, float* aTargetVariable)const;
	bool ForceReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, double* aTargetVariable)const;
	bool ForceReadAttribute(const XMLElement aElementToReadFrom, const std::string& aAttributeToRead, bool* aTargetVariable)const;
	
private:

	/*
	---------------------------
	     Debug Operations
	---------------------------

	These are used to verify that the FindFirstChild and FindNextSiblingElement
	functions return valid pointers, that is, pointers that isnt nullptr.

	The ForceFind-functions all use these debugfunctions to verify that 
	the requested Child/Element was found and crashes otherwise.
	*/

	void DebugFirstChild(const std::string& aChildName)const;
	void DebugFirstChild(XMLElement aParent) const;
	void DebugFirstChild(XMLElement aParent, const std::string& aChildName) const;

	void DebugNextElement(const std::string& aChildName)const;
	void DebugNextElement(XMLElement aParent) const;
	void DebugNextElement(XMLElement aParent, const std::string& aChildName) const;

	tinyxml2::XMLDocument* myDoc;
	std::string myFilePath;
	bool myHasOpenedDoc;
};

