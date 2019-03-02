import QtQuick 2.0
import Oak.Model 1.0

OakModel {
    name: "Hallo QML :)"

    builder: OakModelBuilder {
        nodeDefs : [
            NodeDef {
                name: "process"
                containers: [
                    ContainerDef {
                        name: "solvents"
                        minCount: 1
                        maxCount: 1
                    },
                    ContainerDef {
                        name: "columns"
                        minCount: 1
                        maxCount: 1
                    }
                ]
            },
            NodeDef {
                name: "solvents"
                displayName: "Solvents"
                //tooltip: "Solvents ToolTip"
                containers: [
                    ContainerDef {
                        name: "solvent"
                        minCount: 1
                    }

                ]
            },
            NodeDef {
                name: "solvent"
                displayName: "Solvent"
                tooltip: "Solvent ToolTip"
                keyLeaf: LeafDef {
                    name: "name"
                    displayName: "Name"
                    dataType: LeafDef.String
                    defaultValue: "Solvent"
                }
                leafs: [
                    LeafDef {
                        name: "pctb"
                        displayName: "%B"
                        dataType: LeafDef.Double
                        defaultValue: 0.0
                    },
                    LeafDef {
                        name: "inletA"
                        displayName: "Inlet A"
                        dataType: LeafDef.String
                        defaultValue: "A1"
                        options: ValueOptions {
                            values: [ "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8" ]
                        }
                    },
                    LeafDef {
                        name: "inletB"
                        displayName: "Inlet B"
                        dataType: LeafDef.String
                        defaultValue: "B1"
                        options: ValueOptions {
                            values: [ "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8" ]
                        }
                    }

                ]
            },
            NodeDef {
                name: "columns"
                displayName: "Columns"
                tooltip: "Columns ToolTip"
                containers: [
                    ContainerDef {
                        name: "column"
                        minCount: 1
                    }
                ]
            },
             NodeDef {
                name: "column"
                displayName: "Column"
                tooltip: "Column ToolTip"
                color: "#3246C8"
                imagePath: ":/images/Resources/doc_64.png"
                keyLeaf: LeafDef {
                    name: "name"
                    displayName: "Name"
                    dataType: LeafDef.String
                    defaultValue: "Column"
                    settings: LeafSettings {
                        unique: TriState.True
                    }
                }
                leafs: LeafDef {
                    name: "volume"
                    displayName: "Volume"
                    dataType: LeafDef.Integer
                    defaultValue: 50
                    settings: LeafSettings {
                        unit: "ml"
                    }
                }
                containers: ContainerDef {
                    name: "step"
                }
            },
            NodeDef {
                name: "step"
                displayName: "Step"
                variantId: ""
                color: "#509632"
                keyLeaf: LeafDef {
                    name: "name"
                    displayName: "Name"
                    dataType: LeafDef.String
                    defaultValue: "Step"
                }
                variantLeaf: LeafDef {
                    name: "type"
                    displayName: "Type"
                    dataType: LeafDef.String
                    defaultValue: "Wash"
                }
                leafs: [
                    LeafDef {
                        name: "flowrate"
                        displayName: "Flowrate"
                        dataType: LeafDef.Double
                        defaultValue: 0.0
                        settings: LeafSettings {
                            unit: "mL/min"
                        }
                    },
                    LeafDef {
                        name: "pumpwash"
                        displayName: "Pump Wash"
                        dataType: LeafDef.Boolean
                        defaultValue: true
                    },
                    LeafDef {
                        name: "cv"
                        displayName: "CV"
                        dataType: LeafDef.Double
                        defaultValue: 0.0
                    },
                    LeafDef {
                        name: "solvent"
                        displayName: "Solvent"
                        dataType: LeafDef.String
                        options: ValueOptions {
                            valueQuery: "P{};P{};P{};C{solvents};C{solvent};name"
                        }
                        settings: LeafSettings {
                            optionsOnly: TriState.True
                        }
                    }
                ]
                derivedNodeDefs: [
                    NodeDef {
                        variantId: "Equil"
                        leafs: LeafDef {
                            name: "test1"
                            displayName: "Test 1"
                            dataType: LeafDef.String
                        }
                    },
                    NodeDef {
                        variantId: "Load"
                        leafs: LeafDef {
                            name: "test2"
                            displayName: "Test 2"
                            dataType: LeafDef.String
                        }
                    },
                    NodeDef {
                        variantId: "Wash"
                        leafs: LeafDef {
                            name: "test3"
                            displayName: "Test 3"
                            dataType: LeafDef.String
                        }
                    },
                    NodeDef {
                        variantId: "Elute"
                        leafs: LeafDef {
                            name: "test4"
                            displayName: "Test 4"
                            dataType: LeafDef.String
                        }
                    },
                    NodeDef {
                        variantId: "CIP"
                        leafs: LeafDef {
                            name: "test5"
                            displayName: "Test 5"
                            dataType: LeafDef.String
                        }
                    }

                ]
            }

        ]
    }
}
