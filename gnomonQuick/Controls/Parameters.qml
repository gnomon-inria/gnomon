import QtQuick          2.15
import QtQuick.Controls 2.15

import crossQuick        1.0 as C
import crossParameters   1.0 as C

import xQuick.Controls  1.0 as X
import xQuick.Fonts     1.0 as X
import xQuick.Style     1.0 as X

import gnomonQuick.Controls      1.0 as G

Item {

    id: _self;

    property var parameters;
    property alias params_model: params_model;

    Component {
        id: _dummy_component
        Text {text: lparam.label}
    }

    Component {
        id: _num_component
        //C.Numeric {param: lparam; paramType:  lparam? lparam.type : ""}
        G.NumericParameter {param: lparam}
    }

    Component {
        id: _bool_component
        //C.Numeric {param: lparam; paramType:  lparam? lparam.type : ""}
        G.BoolParameter {param: lparam}
    }

    Component {
        id: _string_component
        C.Simple {param: lparam}
    }

    Component {
        id: _path_component
        C.Path {param: lparam}
    }

    Component {
        id: _liststringlist_component
        C.InListStringList {param: lparam}
    }

    Component {
        id: _liststring_component
        G.InList {param: lparam}
    }

    Component {
        id: _range_component
        G.RangeParameter {param: lparam; decimals: 2}
    }

    Component {
        id: _colormap_component
        G.Colormap {param: lparam}
    }

    Component {
        id: _lookuptable_component
        G.LookupTable {param: lparam}
    }

    ListModel {
        id: params_model;
        dynamicRoles: true;
    }

    function getComponent(type) {
        type = type.replace(',void', '')
        if (type == "dtkCoreParameterNumeric<qlonglong>" || type == "dtkCoreParameterNumeric<double>") {
            return _num_component;
        }
        if(type == "dtkCoreParameterNumeric<bool>") {
            console.log("RETURNING BOOL COMPONENT")
            return _bool_component;
        }
        if (type == "dtkCoreParameterSimple<QString>") {
            return  _string_component;
        }
        if (type == "dtkCoreParameterPath") {
            return _path_component;
        }
        if (type == "dtkCoreParameterRange<qlonglong>" || type == "dtkCoreParameterRange<double>") {
            return _range_component;
        }
        if (type == "dtkCoreParameterInList<QString>") {
            return _liststring_component;
        }
        if (type == "dtkCoreParameterInListStringList") {
            return _liststringlist_component;
        }
        if (type == "gnomonCoreParameterColorMap") {
            return _colormap_component
        }
        if (type == "gnomonCoreParameterLookupTable") {
            return _lookuptable_component
        }

        return _dummy_component;
    }

    function updateParametersModel() {
        params_model.clear();
        const params = [];
        const groups = [];

        for (var param_name in _self.parameters) {
            var p = _self.parameters[param_name];
            var prop_dict = {};
            prop_dict["component"] = _self.getComponent(p.type)
            prop_dict["param"] = p;
            console.log("GETTING PARAM: " + p.type)
            prop_dict["group"] = p.group ? p.group : "General";
            if(!groups.includes(prop_dict["group"])) groups.push(prop_dict["group"])
            params.push(prop_dict);
        }

        //Instantiating the parameters by groups
        for(let i in groups) {
            const group_params = params.filter(p => p.group === groups[i])

            const element = {}
            element["group"] = groups[i]
            element["parameters"] = group_params
            params_model.append(element)
        }
    }

}
