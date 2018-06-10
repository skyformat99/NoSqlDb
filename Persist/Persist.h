#pragma once

#include "../NoSqlDb/NoSqlDb.h"
#include "../CppProperties/CppProperties.h"
#include "../XmlUtilitites/XmlUtilities.h"
#include <fstream>
#include <vector>
#include <string>
 
namespace NoSqlDb {
	template<typename Data>
	class Persist {
		using Xml = std::string;
		using SPtr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;
		using SPtrs = std::vector<SPtr>;

		Persist(NoSqlDb<Data>& db) : db_(db) {}
		Xml save();
		void restore(const Xml& xml);
		bool saveToFile(const std::string& fileSpec);
		bool restoreFromFile(const std::string& fileSpec);
		std::string toString();
	public:
		NoSqlDb<Data>& db_;
		std::string getTextBodyFromElement(SPtr sPtr);
		std::string getTextBodyFromElement(SPtrs sPtrs);
	};

	/*
	* Private helper function - provides safe extraction of element text
	* from single element pointer.
	*/
	template<typename Data>
	std::string Persist<Data>::getTextBodyFromElement(SPtr sPtr)
	{
		if (sPtr->children().size() == 0)
			return "";
		return Utilities::trim(sPtr->children()[0]->value());
	}
	/*
	* Private helper function - provides safe extraction of element text
	* from the first element pointer in a vector of element pointers.
	*/
	template<typename Data>
	std::string Persist<Data>::getTextBodyFromElement(SPtrs sPtrs)
	{
		if (sPtrs.size() == 0)
			return "";
		SPtr sPtr = sPtrs[0];
		if (sPtr->children().size() == 0)
			return "";
		return Utilities::trim(sPtr->children()[0]->value());
	}


	//----< saves db contents to file, using save >--------------------

	template<typename Data>
	bool Persist<Data>::saveToFile(const std::string& fileSpec)
	{
		std::ofstream out(fileSpec);
		if (!out.good())
			return false;
		Persist<Data> persist(db_);
		Xml xml = persist.save();
		out << xml;
		out.close();
		return true;
	}

	//----< restores db contents from file, using restore >------------

	template<typename Data>
	bool Persist<Data>::restoreFromFile(const std::string& fileSpec)
	{
		std::ifstream in(fileSpec);
		if (!in.good())
		{
			std::cout << "\n  failed to open file";
			return false;
		}
		Xml xml;

		// build XML string from file

		while (in.good())
		{
			char ch = in.get();

			/////////////////////////////////////////////////////
			// This code removes whitespace from XML contents
			// but is unnecessart and adds a small CPU load.
			// I left it here because I intent to use it in
			// another package, for display.
			/////////////////////////////////////////////////////
			// if (ch == '\n' || ch == '\t' || ch == ' ')
			//   continue;
			/////////////////////////////////////////////////////

			if (!in.good())
				break;
			xml += ch;
		}
		in.close();

		restore(xml);
		return true;
	}
	//----< save db contents to XML string >---------------------------

	template<typename Data>
	typename Persist<Data>::Xml Persist<Data>::save()
	{
		using SPtr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;

		SPtr pDbRoot = XmlProcessing::makeTaggedElement("noSqlDb");
		XmlProcessing::XmlDocument doc;
		doc.docElement()->addChild(pDbRoot);

		Keys keys = db_.keys();
		for (Key k : keys)
		{
			SPtr pRecord = XmlProcessing::makeTaggedElement("record");
			SPtr pKey = XmlProcessing::makeTaggedElement("key");
			SPtr pText = XmlProcessing::makeTextElement(Utilities::Convert<Key>::toString(k));
			pKey->addChild(pText);
			pRecord->addChild(pKey);

			Element<Data> elem = db_.value(k);
			SPtr pName = XmlProcessing::makeTaggedElement("name", static_cast<std::string>(elem.name));
			pRecord->addChild(pName);

			SPtr pCata = XmlProcessing::makeTaggedElement("category", static_cast<std::string>(elem.category));
			pRecord->addChild(pCata);

			DateTime dt = elem.dateTime;
			SPtr pDateTime = XmlProcessing::makeTaggedElement("dateTime", dt.time());
			pRecord->addChild(pDateTime);

			SPtr pChildren = XmlProcessing::makeTaggedElement("children");
			Keys childKeys = elem.children;
			for (Key child : childKeys)
			{
				SPtr pChild = XmlProcessing::makeTaggedElement("childKey", static_cast<std::string>(child));
				pChildren->addChild(pChild);
			}
			pRecord->addChild(pChildren);

			Data data = elem.data;
			SPtr pData = XmlConvert<Data>::toXmlElement(data, "data");
			pRecord->addChild(pData);

			pDbRoot->addChild(pRecord);
		}
		return doc.toString();
	}

	/*
	* This is where the restoration takes place.
	*/
	template<typename Data>
	void Persist<Data>::restore(const Xml& xml)
	{
		using SPtr = std::shared_ptr<AbstractXmlElement>;
		using SPtrs = std::vector<SPtr>;

		XmlProcessing::XmlDocument doc(xml, XmlProcessing::XmlDocument::str);
		SPtrs pRecords = doc.descendents("record").select();
		for (SPtr pRecord : pRecords)
		{
			XmlProcessing::XmlDocument doc;
			doc.docElement()->addChild(pRecord);

			Key key = getTextBodyFromElement(doc.element("key").select());

			Name name = getTextBodyFromElement(doc.element("name").select());

			Category category = getTextBodyFromElement(doc.element("category").select());

			Children children;
			SPtrs pChildren = doc.descendents("childKey").select();
			for (SPtr pChild : pChildren)
			{
				children.push_back(getTextBodyFromElement(pChild));
			}

			SPtr pData = doc.element("data").select()[0];

			Element<Data> elem(name, category);
			elem.children = children;
			elem.data = XmlConvert<Data>::fromXmlElement(pData);

			bool saved = db_.saveRecord(key, elem);  // save new record
			if (!saved)
				db_.saveValue(key, elem);  // overwrite old record element
		}
	}

}