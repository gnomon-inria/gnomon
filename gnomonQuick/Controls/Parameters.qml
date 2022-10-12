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
        G.StringParameter {param: lparam}
    }

    Component {
        id: _path_component
        //C.Path {param: lparam}
        G.PathParameter {param: lparam}
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

    Component {
        id: _graphical_component
        G.GraphicalParameter {param: lparam}
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
        if(type ==  "dtkCoreParameterNumeric<bool>") {
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
        if (type == "gnomonCoreParameterGraphical") {
            return _graphical_component
        }

        return _dummy_component;
    }

    function updateParametersModel() {
        params_model.clear();

        //When instantiating the parameters, we have to differentiate between general params
        //and others so that the first group is always "General"
        const params = [];
        const groups = [];
        const general = [];

        for (var param_name in _self.parameters) {
            var p = _self.parameters[param_name];
            var prop_dict = {};
            prop_dict["component"] = _self.getComponent(p.type)
            prop_dict["param"] = p;
            prop_dict["group"] = p.group ? p.group : "General";
            if(!groups.includes(prop_dict["group"]) && prop_dict["group"] !== "General") groups.push(prop_dict["group"])
            if(p.group)
                params.push(prop_dict);
            else
                general.push(prop_dict)
        }

        //First add the general parameters if any
        if(general.length)
            params_model.append({
                "group": "General",
                "parameters": general
            })

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
