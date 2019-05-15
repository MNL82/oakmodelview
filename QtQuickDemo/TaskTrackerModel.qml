import QtQuick 2.0
import Oak.Model 1.0

OakModel {
    name: "Task Tracker"

    builder: OakModelBuilder {
        nodeDefs : [
            NodeDef {
                name: "document"
                containers: [
                    ContainerDef {
                        name: "lists"
                        minCount: 1
                        maxCount: 1
                    },
                    ContainerDef {
                        name: "backlog"
                        minCount: 1
                        maxCount: 1
                    }
                ]
            },
            NodeDef {
                name: "lists"
                displayName: "Lists"
                containers: [
                    ContainerDef {
                        name: "list"
                    }
                ]
            },
            NodeDef {
                name: "list"
                displayName: "List"
                keyLeaf: LeafDef {
                    name: "name"
                    displayName: "Name"
                    dataType: LeafDef.String
                    defaultValue: "List Name"
                }
                variantLeaf: LeafDef {
                    name: "type"
                    displayName: "Type"
                    dataType: LeafDef.String
                    defaultValue: "Text"
                }
                derivedNodeDefs: [
                    NodeDef {
                        variantId: "Text"
                        leafs: LeafDef {
                            name: "text"
                            displayName: "Text"
                            dataType: LeafDef.String
                        }
                    },
                    NodeDef {
                        variantId: "Number"
                        leafs: LeafDef {
                            name: "number"
                            displayName: "Number"
                            dataType: LeafDef.Integer
                        }
                    },
                    NodeDef {
                        variantId: "Decimal"
                        leafs: LeafDef {
                            name: "decimal"
                            displayName: "Decimal"
                            dataType: LeafDef.Double
                        }
                    },
                    NodeDef {
                        variantId: "Logical"
                        leafs: LeafDef {
                            name: "logical"
                            displayName: "Logical"
                            dataType: LeafDef.Boolean
                        }
                    },
                    NodeDef {
                        variantId: "DateTime"
                        leafs: LeafDef {
                            name: "dateTime"
                            displayName: "Date Time"
                            dataType: LeafDef.DateTime
                        }
                    }
                ]
            },
            NodeDef {
                name: "backlog"
                displayName: "Backlog"
                tooltip: "Contains all tasks"
                containers: [
                    ContainerDef {
                        name: "task"
                    }
                ]
            },
            NodeDef {
                name: "task"
                displayName: "Task"
                tooltip: "Tasks can be nested endlessly"
                containers: [
                    ContainerDef {
                        name: "task"
                    }
                ]
                keyLeaf: LeafDef {
                    name: "name"
                    displayName: "Name"
                    dataType: LeafDef.String
                    defaultValue: "Task"
                }
//                variantLeaf: LeafDef {
//                    name: "type"
//                    displayName: "Type"
//                    dataType: LeafDef.String
//                    defaultValue: "Wash"
//                }
                leafs: [
                    LeafDef {
                        name: "description"
                        displayName: "Description"
                        dataType: LeafDef.String
                    },
                    LeafDef {
                        name: "duration"
                        displayName: "Duration"
                        dataType: LeafDef.String
                        defaultValue: "1 hour"
                    }
                ]
            }
        ]
    }
}
