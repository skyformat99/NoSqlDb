#pragma once

#include "../NoSqlDb/NoSqlDb.h"
#include "../Persist/Persist.h"
#include "../Utilities/Utilities.h"
#include "../Display/Display.h"
#include <regex>
#include <string>
#include <vector>
#include <fstream>

class TestDB 
{
public:
	void Req1();
	void Req2();
	void Req3();
	void Req4();
private:
	NoSqlDb::NoSqlDb<std::string> db_;
};

void TestDB::Req1()
{
	Utilities::subTitle("Demonstrating Req #1 - is C++");
	std::cout << "\n  Examine code in Visual Studio to verify that it is C++, using standard libraries.\n";
}

void TestDB::Req2()
{
	Utilities::subTitle("Demonstrating Req #2 - template database");
	Utilities::putLine();
	std::cout << "\n  type of data = " << typeid(std::string).name() << "\n\n";
	NoSqlDb::Element<std::string> elem1;
	elem1.name = "Joe";
	elem1.category = "test";
	elem1.dateTime = DateTime();
	elem1.saveChild("Nupur");
	elem1.saveChild("Kulkarni");
	elem1.saveChild("Nachiket");
	elem1.data = "test1";

	NoSqlDb::Element<std::string> elem2;
	elem2.name = "Nachiket";
	elem2.category = "test";
	elem2.dateTime = DateTime();
	elem2.saveChild("Ninad");
	elem2.saveChild("Priya");
	elem2.data = "test2";

	static NoSqlDb::NoSqlDb<std::string> db;
	db.saveRecord(elem1.name, elem1);
	db.saveRecord(elem2.name, elem2);

	db_ = db;

	NoSqlDb::Display<std::string> display(db_);
	display.show();

}

void TestDB::Req3()
{
	using Key = std::string;
	using Keys = std::vector<Key>;
	std::cout << "\n  Addition of new Key/Value pair\n";
	NoSqlDb::Element<std::string> elem4;
	elem4.name = "elem4";
	elem4.category = "category";
	elem4.dateTime = DateTime();
	elem4.data = "elem4's StrData";
	db_.saveRecord(elem4.name, elem4);

	std::cout << "\n  Deletion of new Key/Value pair\n";
	std::cout << "\n  Deleting elem3:\n  Before Deletion\n";
	NoSqlDb::Display<std::string> display(db_);
	display.show();
	bool res = db_.removeRecord("elem4");
	std::cout << "\n  After Deletion\n";
	if (res) {
		NoSqlDb::Display<std::string> display(db_);
		display.show();
	}
	else {
		std::cout << "\n  Can not delete a key which does not exist.";
	}
	std::cout << "\n  Deleting elem5 which does not exist:";
	res = db_.removeRecord("elem5");
	if (res) {
		NoSqlDb::Display<std::string> display(db_);
		display.show();
	}
	else {
		std::cout << "\n  Can not delete a key which does not exist.\n";
	}
}

void TestDB::Req4()
{
	Utilities::subTitle("Demonstrating Req #4 - editing records");
	std::cout << "\n  editing element with key \"Nachiket\"\n";
	NoSqlDb::Element<std::string> elem = db_.value("Nachiket");
	elem.name = "Nachi";
	elem.category = "test R4";
	elem.data = "Test Data for R4";
	elem.saveChild("Suyash");
	db_.saveValue("Nachiket", elem);
	NoSqlDb::Display<std::string> display(db_);
	display.show();
}
