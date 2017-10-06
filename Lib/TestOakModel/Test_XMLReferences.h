/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#pragma once

#include <boost/test/included/unit_test.hpp>
using boost::unit_test_framework::test_suite;

#ifdef XML_BACKEND

#include "XMLDocument.h"
#include "XMLRefFactory.h"
#include "XMLChildRef.h"
#include "XMLRefGroup.h"
#include "XMLParentRef.h"
#include "XMLValueRef.h"
#include "XMLListRef.h"

using namespace Oak;

void test_elementRefCopyMove()
{
    XML::Ref * eUPtr1 = new XML::ChildRef("model");
    XML::Ref * eUPtr2 = std::move(eUPtr1);
    XML::ChildRef *eChildPtr = dynamic_cast<XML::ChildRef*>(eUPtr2);
    BOOST_CHECK(eChildPtr != 0);
    delete eUPtr2;
}

void test_elementRef()
{
    XML::Document document1;

    BOOST_REQUIRE(document1.load(std::string(RESOURCE_PATH)+"test_doc.xml"));

    XML::Element docElement = document1.documentElement();

    BOOST_CHECK(!docElement.isNull());

    XML::ChildRef refChild1("model");
    XML::Element element = refChild1.getTarget(docElement);

    BOOST_CHECK(!element.isNull());
    BOOST_CHECK(element.compareTagName("model") == 0);

    XML::RefUPtr refGroup1 = XML::RefFactory::MakeRef("p:;c:data");

    element = refGroup1->getTarget(element);

    BOOST_CHECK(!element.isNull());
    BOOST_CHECK(element.compareTagName("data") == 0);

    XML::RefUPtr refGroupP = XML::RefFactory::MakeRef("p:");

    element = refGroupP->getTarget(element);

    BOOST_CHECK(!element.isNull());
    BOOST_CHECK(element.compareTagName("oakModel") == 0);
}

void test_valueRef()
{

    XML::Document document1;
    BOOST_REQUIRE(document1.load(std::string(RESOURCE_PATH)+"test_doc.xml"));
    XML::Element docElement = document1.documentElement();

    XML::ValueRefUPtr valRef = XML::RefFactory::MakeValueRef("model;nodeDefinition", "name");

    std::string value;
    valRef->getValue(docElement, value);

    BOOST_CHECK(value == "root");

    XML::RefGroup * eGroupRef = valRef->elementRef<XML::RefGroup>();

    BOOST_CHECK(eGroupRef != 0);

    XML::ChildRef* eChildRef = eGroupRef->at<XML::ChildRef>(1);

    BOOST_CHECK(eChildRef != 0);

    eChildRef->setIndex(1);

    valRef->getValue(docElement, value);
    BOOST_CHECK(value == "class");

    eChildRef->setIndex(5);
    valRef->getValue(docElement, value);

    BOOST_CHECK(value.empty());

    valRef->setValue(docElement, "magic");

    valRef->getValue(docElement, value);
    BOOST_CHECK(value == "magic");

    valRef->clearValue(docElement);
    BOOST_CHECK(!valRef->elementRef()->hasTarget(docElement));

    eChildRef->setIndex(1);

    valRef->getValue(docElement, value);
    BOOST_CHECK(value == "class");
}

void test_elementListRef()
{
    XML::Document document1;

    BOOST_REQUIRE(document1.load(std::string(RESOURCE_PATH)+"test_doc.xml"));

    XML::Element docElement = document1.documentElement();

    BOOST_CHECK(!docElement.isNull());

    XML::ListRef eListRef(XML::ChildRef::MakeUPtr("model"), "nodeDefinition");
    XML::ValueRef vRef("name");

    std::vector<XML::Element> eList = eListRef.list(docElement);

    BOOST_CHECK(eList.size() == 5);

    BOOST_CHECK(vRef.compareValue(eList.at(0), "root") == 0);

    BOOST_CHECK(vRef.compareValue(eList.at(3), "variable") == 0);

    // Can not insert elements at position larger than count()
    BOOST_CHECK(eListRef.insert(docElement, 10).isNull());

    XML::Element element = eListRef.insert(docElement, 5);

    BOOST_CHECK(!element.isNull());

    BOOST_CHECK(element == eListRef.last(docElement));
}

test_suite* Test_XMLReferences()
{
    test_suite* test = BOOST_TEST_SUITE( "XMLReferences" );

    test->add(BOOST_TEST_CASE(&test_elementRefCopyMove));
    test->add(BOOST_TEST_CASE(&test_elementRef));
    test->add(BOOST_TEST_CASE(&test_valueRef));
    test->add(BOOST_TEST_CASE(&test_elementListRef));

    return test;
}

#endif XML_BACKEND
