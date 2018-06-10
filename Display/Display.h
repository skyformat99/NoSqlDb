#pragma once

#include "../NoSqlDb/NoSqlDb.h"
#include "../Utilities/Utilities.h"
#include <string>
#include <iostream>

namespace NoSqlDb
{
	template <typename Data>
	class Display
	{
		NoSqlDb<Data>& db_;
		void showSummary(Key key, size_t fieldwidth, std::ostream& out);
		void showHeader(size_t fieldwidth = 10, std::ostream& out = std::cout);
	public:
		Display(NoSqlDb<Data>& db) : db_(db) {}
		void show(size_t fieldwidth = 10, std::ostream& out = std::cout);

	};


	template<typename Data>
	void Display<Data>::show(size_t fieldwidth, std::ostream& out)
	{
		Keys keys = db_.keys();
		showHeader(fieldwidth, out);
		for (Key k : keys)
			showSummary(k, fieldwidth, out);
	}

	//----< show single record in one-line format >----------------------

	template<typename Data>
	void Display<Data>::showSummary(Key key, size_t fieldwidth, std::ostream& out)
	{
		if (!db_.hasKey(key))
			return;
		Element<Data> elem = db_.value(key);
		int save = out.setf(std::ios::left, std::ios::adjustfield);
		out << "  ";
		out << std::setw(fieldwidth) << key.substr(0, fieldwidth) << " ";
		out << std::setw(fieldwidth) << static_cast<std::string>(elem.name).substr(0, fieldwidth) << " ";
		out << std::setw(fieldwidth) << static_cast<std::string>(elem.category).substr(0, fieldwidth) << " ";
		DateTime dt = elem.dateTime;
		out << std::setw(fieldwidth) << static_cast<std::string>(dt).substr(0, fieldwidth) << " ";
		std::string temp = Utilities::Convert<Data>::toString(elem.data.getValue());
		out << std::setw(fieldwidth) << temp.substr(0, fieldwidth) << " ";
		if (elem.children.getValue().size() > 0)
		{
			for (Key k : elem.children.getValue())
				out << std::setw(fieldwidth) << static_cast<std::string>(k).substr(0, fieldwidth) << " ";
		}
		out << "\n";
		out.setf(save);
	}

	//----< shows underlined names of each field for summary output >----

	template<typename Data>
	void Display<Data>::showHeader(size_t fieldwidth, std::ostream& out)
	{
		size_t numChildren = 0;
		Keys keys = db_.keys();
		for (Key k : keys)
		{
			size_t size = db_.value(k).children.getValue().size();
			if (numChildren < size)
				numChildren = size;
		}
		int save = out.setf(std::ios::left, std::ios::adjustfield);
		out << "  ";
		out << std::setw(fieldwidth) << std::string("Key").substr(0, fieldwidth) << " ";
		out << std::setw(fieldwidth) << std::string("Name").substr(0, fieldwidth) << " ";
		out << std::setw(fieldwidth) << std::string("Category").substr(0, fieldwidth) << " ";
		out << std::setw(fieldwidth) << std::string("TimeDate").substr(0, fieldwidth) << " ";
		out << std::setw(fieldwidth) << std::string("Data").substr(0, fieldwidth) << " ";
		if (numChildren > 0)
		{
			for (size_t i = 0; i < numChildren; ++i)
				out << std::setw(fieldwidth) << std::string("Child").substr(0, fieldwidth) << " ";
		}
		out << "\n  ";
		for (size_t i = 0; i < numChildren + 5; ++i)
			out << std::setw(fieldwidth) << std::string("----------").substr(0, fieldwidth) << " ";
		out << "\n";
		out.setf(save);
	}
}