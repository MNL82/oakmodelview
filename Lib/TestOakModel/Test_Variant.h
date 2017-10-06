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

#include "Variant.h"
#include "VariantRef.h"
#include "VariantCRef.h"

using namespace Oak::Model;

void test_Variant()
{
    std::cout << "Size of variant:     " << sizeof(Variant) << std::endl;
    std::cout << "Size of variantRef:  " << sizeof(VariantRef) << std::endl;
    std::cout << "Size of variantCRef: " << sizeof(VariantCRef) << std::endl;

    std::string resStr;
    int resInt = 77;

    Variant value("test");
    VariantRef valueRef(value);

    valueRef.get(resStr);
    BOOST_CHECK(resStr == "test");

    valueRef.set("test2");

    BOOST_CHECK(resStr == "test");
    valueRef.get(resStr);
    BOOST_CHECK(resStr == "test2");

    VariantCRef valueCRef(resInt);
    BOOST_CHECK(valueCRef.isEqual(resInt));

    std::vector<int> intList = {34,25,1,-46};

    std::vector<std::string> strList;

    VariantCRef vRefList(intList);

    BOOST_CHECK(vRefList.get(strList));
    BOOST_CHECK(strList.size() == 4);

    std::cout << "Double: " <<  VariantRelations::displayName(typeid(double)) << std::endl;
}

test_suite* Test_Variant()
{
    test_suite* test = BOOST_TEST_SUITE("Varinat");

    test->add(BOOST_TEST_CASE(&test_Variant));

    return test;
}

