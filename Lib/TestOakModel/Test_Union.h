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

#include <iostream>

#include "UnionValue.h"
#include "UnionRef.h"

using namespace Oak::Model;

void test_Union()
{
    std::cout << "Size of UnionValue: " << sizeof(UnionValue) << std::endl;
    std::cout << "Size of UnionRef:   " << sizeof(UnionRef)   << std::endl;

    const char *c = "Hallo World";
    bool b = true;
    int i = 32;
    double d = 3.21;
    std::string s = "Testing...";

    UnionValue uv;
    UnionValue uvc(c);
    UnionValue uvb(b);
    UnionValue uvi(i);
    UnionValue uvd(d);
    UnionValue uvs(s);

    BOOST_CHECK(uv.type() == UnionType::Undefined);

    BOOST_CHECK(uvc.type() == UnionType::String);
    BOOST_CHECK(uvc.getString() == c);

    BOOST_CHECK(uvb.type() == UnionType::Bool);
    BOOST_CHECK(uvb.getBool() == b);

    BOOST_CHECK(uvi.type() == UnionType::Integer);
    BOOST_CHECK(uvi.getInt() == i);

    BOOST_CHECK(uvd.type() == UnionType::Double);
    BOOST_CHECK(uvd.getDouble() == d);

    BOOST_CHECK(uvs.type() == UnionType::String);
    BOOST_CHECK(uvs.getString() == s);

    UnionRef urs(s);
    UnionRef uri(uvi);
    BOOST_CHECK(uri.canConvertTo(urs));
    BOOST_CHECK(!urs.canConvertTo(uri));
}

test_suite* Test_Union()
{
    test_suite* test = BOOST_TEST_SUITE("Union");

    test->add(BOOST_TEST_CASE(&test_Union));

    return test;
}
