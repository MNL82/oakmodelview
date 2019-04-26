# OakModelView
Model View C++ library. 
Project is still under continues development.
It is a rather large personal project now with 200+ source files.

# Model
The model is written in C++ with only a sing lightweight dependency.
How data is stored and its definition that describes the format, is sperated to allow flexibility of the storage format.

## Data format
The data structure is tree based like XML, Json or objects.
Only XML is currently supported but code is prepared to support other formats.

## Data definition
The data definition is a two dimentional tree structure mutch like a heiraky of object build from classes that uses inheritence. 
* First dimension is build from node definitions with parent child relations and each node can have leaf definitions.
* Secound dimention comes from node definitions being able to have derived node definition just like class can have derived classes.
The data definition is easy to create and change and can even be created at runtime. 
This gives some interesting possibilities that I will not elaborate on here. 

## Nodes and Leafs
A OakModelView node is therefore a node definition and a data node. A node have a single parent node (unless it is the root node) and any number of child nodes. It can also have a number of leafs that contains the data of the tree.
OakModelView leaf is a leaf definition and a data node object. The definition describes how data can be accessed from the data node. 

Current Leaf Data formats:
* bool
* int
* double
* std::string
* std::chrono::system_clock::time_point

# View
The view is decopled from the model to allow view not Qt based.
Currently all views have been based on Qt. 
I have some view components based on QWidgets but I am currently working on Qml based view.
Qml seams to support far superior flexibility in regards to customization of how the application will look when using the Oak Model to manage data.

# Installation instructions
Missing... 
but you need the latest Qt and I am currently working on the QtQuickTest.pro project in the root folder.

# Contact
If you have any questions or want to contribute you are always welcome to contact me by email: mikkel@oakmodelview.com

# Copyright
Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)

# License
This library is distributed under the MIT License. 
See accompanying file LICENSE in the root folder.

# Dependencies
Thanks to.
 * pugixml parser - http://pugixml.org
