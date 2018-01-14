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

#include "QueryBase.h"

#include "Test_ServiceFunctions.h"
#include "OakModel.h"

#ifdef XML_BACKEND

using namespace Oak::Model;

void test_ItemQuery()
{
    OakModel dataModel;
    dataModel.setRootNodeDefinition(createNodeDefinition());
    dataModel.loadXMLRootNode(std::string(RESOURCE_PATH)+"test_doc.xml");

    Item item_oakModel = dataModel.rootItem();

    BOOST_CHECK(item_oakModel.childCount() == 2);

    auto query = QueryBase::MakeSPtr(item_oakModel)->children("model")->children("nodeDefinition");
    BOOST_CHECK(query->count() == 5);

    std::vector<int> list = query->children("valueDefinition")->toList<int>("value");
    BOOST_CHECK(list.size() == 2);
    BOOST_CHECK(list[0] == 34);
    BOOST_CHECK(list[1] == -7);
}

test_suite* Test_ItemQuery()
{
    test_suite* test = BOOST_TEST_SUITE("ItemQuery");

    test->add(BOOST_TEST_CASE(&test_ItemQuery));

    return test;
}

#endif // XML_BACKEND
