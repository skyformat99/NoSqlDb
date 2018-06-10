#pragma once
///////////////////////////////////////////////////////////////////////
// XmlUtilities.h - Utility class to convert template types to XML   //
// ver 1.1                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017         //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package defines a single class, XmlConvert<T> that supports convering
* instances of the T type to and from XML strings.
*
* Required Files:
* ---------------
*   XmlUtilities.h, XmlUtilities.cpp,
*   XmlDocument static library
*   Utilities.h, Utilities.cpp
*
* Maintenance History:
*---------------------
* ver 1.1 : 22 Feb 2017
* - added these comments
* ver 1.0 : 06 Feb 2017
* - first release
*/


#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../Utilities/Utilities.h"
#include <vector>

namespace XmlProcessing
{
  template<typename T>
  class XmlConvert
  {
  public:
    using SPtr = std::shared_ptr<AbstractXmlElement>;

    static SPtr toXmlElement(const T& t, const std::string& tag);
    static T fromXmlElement(SPtr sElem);
  };

  template<typename T>
  typename XmlConvert<T>::SPtr XmlConvert<T>::toXmlElement(const T& t, const std::string& tag)
  {
    using SPtr = XmlConvert<T>::SPtr;
    SPtr pElem = XmlProcessing::makeTaggedElement(tag);
    SPtr pText = XmlProcessing::makeTextElement(Utilities::Convert<T>::toString(t));
    pElem->addChild(pText);
    return pElem;
  }

  template<typename T>
  T XmlConvert<T>::fromXmlElement(SPtr pElem)
  {
    std::vector<SPtr> children = pElem->children();
    if (children.size() == 0)
      throw std::exception("text node does not exist");
    SPtr pText = children[0];
    std::string text = pText->value();
    T t = Utilities::Convert<T>::fromString(text);
    return t;
  }
}
