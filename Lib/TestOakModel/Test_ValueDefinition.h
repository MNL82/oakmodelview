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

#include "ValueDefinitionBuilder.h"

using namespace Oak::Model;

void test_valueDefinition()
{
    XML::Document document1;
    BOOST_REQUIRE(document1.load(std::string(RESOURCE_PATH)+"test_doc.xml"));
    XML::Element docElement = document1.documentElement();

    XML::ChildRefGroupUPtr childGroupRef = XML::RefFactory::MakeChildRef("model;nodeDefinition;valueDefinition");

    XML::Element tempElement = childGroupRef->getTarget(docElement);

    BOOST_CHECK(tempElement.compareTagName("valueDefinition") == 0);

    ValueDefUPtr vDef = VDB::MakeXML(1, "value");

    int value;
    vDef->getValue(tempElement, value);
    BOOST_CHECK(value == 34);

    double dVal;
    vDef->getValue(tempElement, dVal, true, true);
    BOOST_CHECK(dVal == 34.0);

    BOOST_CHECK(vDef->setValue(tempElement, -7.3241, true));

    std::string sVal;
    vDef->getValue(tempElement, sVal);
    BOOST_CHECK(sVal == "-7");

    BOOST_CHECK(vDef->setValue(tempElement, std::string("Fail"), true) == false);
}


test_suite* Test_ValueDefinition()
{
    test_suite* test = BOOST_TEST_SUITE( "valueDefinition" );

    test->add(BOOST_TEST_CASE(&test_valueDefinition));

    return test;
}

#endif XML_BACKEND
