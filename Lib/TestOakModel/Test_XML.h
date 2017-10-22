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

#include "Test_XMLDoc.h"
#include "Test_XMLReferences.h"
#include "Test_ValueDefinition.h"
#include "Test_NodeDefinition.h"
#include "Test_Item.h"
#include "Test_ItemQuery.h"

test_suite* Test_XML()
{
    test_suite* test = BOOST_TEST_SUITE( "XML" );

    test->add(Test_XMLDoc());
    test->add(Test_XMLReferences());
    test->add(Test_ValueDefinition());
    test->add(Test_NodeDefinition());
    test->add(Test_Item());
    test->add(Test_ItemQuery());

    return test;
}

#endif XML_BACKEND


