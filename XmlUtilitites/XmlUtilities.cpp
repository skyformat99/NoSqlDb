/////////////////////////////////////////////////////////////////////////
// XmlUtilities.cpp - Utility class to convert template types to XML   //
// ver 1.1                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017           //
/////////////////////////////////////////////////////////////////////////

#include "XmlUtilities.h"
#include "../CppProperties/CppProperties.h"
#include <iostream>

using namespace XmlProcessing;

class Widget
{
public:
  using SPtr = std::shared_ptr<Widget>;

  Property<std::string> name;
  Property<double> pi;
};

template<>
typename XmlConvert<Widget>::SPtr XmlConvert<Widget>::toXmlElement(const Widget& w, const std::string& tag)
{
  using SPtr = XmlConvert<Widget>::SPtr;
  SPtr pElem = makeTaggedElement("widget");
  SPtr pName = makeTaggedElement("name");
  SPtr pText = makeTextElement(static_cast<const std::string>((w.name)));
  pName->addChild(pText);
  SPtr pPi = makeTaggedElement("pi");
  pText = makeTextElement(Utilities::Convert<double>::toString(w.pi));
  pPi->addChild(pText);
  pElem->addChild(pName);
  pElem->addChild(pPi);
  return pElem;
}

template<>
typename Widget XmlConvert<Widget>::fromXmlElement(SPtr pElem)
{
  std::vector<SPtr> children = pElem->children();
  if (children.size() == 0)
    throw std::exception("malformed Widget");
  SPtr pName = children[0];
  SPtr pText = pName->children()[0];
  std::string text = pText->value();
  Widget temp;
  temp.name = text;
  SPtr pPi = children[1];
  pText = pPi->children()[0];
  text = pText->value();
  temp.pi = Utilities::Convert<double>::fromString(text);
  return temp;
}

#ifdef TEST_UTILS




using SPtr = XmlConvert<std::string>::SPtr;

int main()
{
  Utilities::Title("Testing XmlConvert<T>");

  SPtr pElem = XmlConvert<std::string>::toXmlElement("a string body", "str");
  std::cout << "\n  " << pElem->toString() << "\n";

  std::string newStr = XmlConvert<std::string>::fromXmlElement(pElem);
  std::cout << "\n  value of new string = \"" << newStr << "\"";

  pElem = XmlConvert<double>::toXmlElement(3.1415927, "dbl");
  std::cout << "\n  " << pElem->toString() << "\n";

  double pi = XmlConvert<double>::fromXmlElement(pElem);
  std::cout << "\n  value of new double = " << pi;

  Widget widget;
  widget.name = "aWidget";
  widget.pi = 3.1415927;
  pElem = XmlConvert<Widget>::toXmlElement(widget, "widget");
  std::cout << "\n  " << pElem->toString() << "\n";

  Widget newWidget = XmlConvert<Widget>::fromXmlElement(pElem);
  std::cout << "\n  newWidget = { " << newWidget.name << ", " << newWidget.pi << " }";

  std::cout << "\n\n";
}

#endif // TEST_UTILS