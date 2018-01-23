/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

// Assertion severity level
// 1. warn
// 2. check
// 3. require

#include "Test_XML.h"
//#include "Test_Variant.h"
#include "Test_Union.h"

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    UNUSED(argc);
    UNUSED(argv);
    test_suite* test = BOOST_TEST_SUITE( "Master test suite" );

    test->add(Test_Union());
#ifdef XML_BACKEND
    test->add(Test_XML());
#endif

    return test;
}
