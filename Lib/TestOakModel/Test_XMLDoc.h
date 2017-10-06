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
using namespace Oak;

void test_OpenXMLDoc()
{
    XML::Document document1;
    BOOST_CHECK(document1.isNull());

    BOOST_REQUIRE(document1.load(std::string(RESOURCE_PATH)+"test_doc.xml"));

    BOOST_CHECK(!document1.isNull());

    XML::Document document2 = document1;

    BOOST_CHECK(document1 == document2);

    document2 = XML::Document();

    document2.clone(document1);

    BOOST_CHECK(document1 != document2);
}


test_suite* Test_XMLDoc()
{
    test_suite* test = BOOST_TEST_SUITE( "XMLDoc" );

    test->add(BOOST_TEST_CASE(&test_OpenXMLDoc));

    return test;
}
#endif XML_BACKEND
