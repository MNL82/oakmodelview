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

#include "Item.h"

#include "Test_ServiceFunctions.h"
#include "TreeDataModel.h"

#ifdef XML_BACKEND

using namespace Oak::Model;

void test_Item()
{
    TreeDataModel dataModel;
    dataModel.setRootNodeDefinition(createNodeDefinition());
    dataModel.loadXMLRootNode(std::string(RESOURCE_PATH)+"test_doc.xml");

    Item item_oakModel = dataModel.rootItem();

    BOOST_CHECK(item_oakModel.childCount() == 2);

    Item item_model = item_oakModel.firstChild(nodeId_model);

    int childCount = item_model.childCount();
    BOOST_CHECK(childCount == 5);

    BOOST_CHECK(item_model.canInsertChild(nodeId_nodeDef, childCount));

    item_model.insertChild(nodeId_nodeDef, childCount);

    BOOST_CHECK(item_model.childCount() == 6);

    BOOST_CHECK(item_model.removeChild(childCount));

    Item niItem = item_model.lastChild();

    std::vector<std::string> options;
    niItem.value("name").getOptions(options);

    BOOST_CHECK(!niItem.isNull());

    std::string name;
    BOOST_CHECK(niItem.value(valueId_name).getValue(name));

    BOOST_CHECK(name == "description");
}

test_suite* Test_Item()
{
    test_suite* test = BOOST_TEST_SUITE("Item");

    test->add(BOOST_TEST_CASE(&test_Item));

    return test;
}

#endif // XML_BACKEND

