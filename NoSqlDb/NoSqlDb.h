#pragma once

///////////////////////////////////////////////////////////////////////
// NoSqlDb.cpp - Store/Persist/Retrieve Key/Value Database           //
// ver 3.3                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Key/Value Database, CSE687 - Object Oriented Design  //
// Author:      Nupur Kulkarni, Syracuse University,                 //
//              nvkulkar@syr.edu                                     //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public NoSqlDb class.
* NoSqlDb supports functionalities like addition, deletion and edition of elements in database. 
* 
* Required Files:
* ---------------
* NoSqlDb.h, NoSqlDb.cpp, CppProperties.h, CppProperties.cpp
* 
* Public Interface:
* ---------------------
* Element::saveChild - to save on which value the key depends on 
* Element::show - to show a single element in database
* NoSqlDb::hasKey - to check if key is present in database
* NoSqlDb::keys - to retrieve all the keys from database
* NoSqlDb::value - to retrieve value of key
* NoSqlDb::saveRecord - to save a record in database
* NoSqlDb::removeRecord - to delete a record from database
* NoSqlDb::count - to count total number of records in database
* 
*
* Maintenance History:
* ---------------------
* Ver 1.0 : 25 Jan 2017
* - first release
*/

#include <map>
#include "../DateTime/DateTime.h"
#include "../CppProperties/CppProperties.h"
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>

namespace NoSqlDb {

	using Key = std::string;
	using Keys = std::vector<Key>;
	using Name = std::string;
	using Category = std::string;
	using Children = std::vector<Key>;

	/////////////////////////////////////////////////////////////////////
	// Element class represents a data record in our NoSql database
	// - in our NoSql database that is just the value in a key/value pair
	// - it needs to store child data
	
	template<typename Data>
	class Element
	{
	public:

		Element() {}
		Element(Name aName, Category aCat) : name(aName), category(aCat) {};

		Property<Name> name;                  // metadata
		Property<DateTime> dateTime;          // metadata
		Property<Children> children;          // metadata
		Property<Category> category;          //matadata
		Property<Data> data;                  // data

		void saveChild(Key);
		std::string show();
	};

	template<typename Data>
	void Element<Data>::saveChild(Key key)
	{
		children.getValue().push_back(key);
	}

	template<typename Data>
	std::string Element<Data>::show()
	{
		std::ostringstream out;
		out.setf(std::ios::adjustfield, std::ios::left);
		out << "\n    " << std::setw(8) << "name" << " : " << name;
		out << "\n	  " << std::setw(8) << "category" << " : " << category;
		out << "\n    " << std::setw(8) << "dateTime" << " : " << DateTime(dateTime).time();
		Children children_ = static_cast<Children>(children);
		if (children_.size() > 0)
		{
			out << "\n    " << std::setw(8) << "children" << " : ";
			for (size_t i = 0; i < children_.size(); ++i)
			{
				out << children_[i];
				if (i < children_.size())
					out << ", ";
			}
		}
		out << "\n    " << std::setw(8) << "data" << " : " << data;
		out << "\n";
		return out.str();
	}

	/////////////////////////////////////////////////////////////////////
	// NoSqlDb class is a key/value pair in-memory database
	// - stores and retrieves elements

	template<typename Data>
	class NoSqlDb
	{
	public:
		using Key = std::string;
		using Keys = std::vector<Key>;

		Keys keys();
		bool hasKey(Key key);
		bool saveRecord(Key key, Element<Data> elem);
		bool removeRecord(Key key);
		Element<Data> value(Key key);
		size_t count();
		bool saveValue(Key key, Element<Data> elem);
	private:
		using Record = std::pair<Key, Element<Data>>;

		std::map<Key, Element<Data>> store;
	};

	template<typename Data>
	typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
	{
		Keys keys;
		for (Record record : store)
		{
			keys.push_back(record.first);
		}
		return keys;
	}

	template<typename Data>
	bool NoSqlDb<Data>::hasKey(Key key)
	{
		if (store.find(key) == store.end())
			return false;
		return true;
	}

	template<typename Data>
	bool NoSqlDb<Data>::saveRecord(Key key, Element<Data> elem)
	{
		if (store.find(key) != store.end())
			return false;
		store[key] = elem;
		return true;
	}

	template<typename Data>
	bool NoSqlDb<Data>::removeRecord(Key key)
	{
		if (store.find(key) == store.end())
			return false;
		size_t numErased = store.erase(key);
		if (numErased == 1)
		{
			// remove key from all other element's children collections
			for (Key k : keys())
			{
				Keys& children = store[k].children.getValue();  // note Keys& - we don't want copy of children
				Keys::iterator iter;
				for (iter = children.begin(); iter != children.end(); ++iter)
				{
					if ((*iter) == key)
					{
						children.erase(iter);
						break;
					}
				}
			}
			return true;
		}
		return false;
	}

	template<typename Data>
	Element<Data> NoSqlDb<Data>::value(Key key)
	{
		if (store.find(key) != store.end())
			return store[key];
		return Element<Data>();
	}

	template<typename Data>
	size_t NoSqlDb<Data>::count()
	{
		return store.size();
	}
	template<typename Data>
	bool NoSqlDb<Data>::saveValue(Key key, Element<Data> elem)
	{
		if (hasKey(key)) {
			store[key] = elem;
			return true;
		}
		return false;
	}
}