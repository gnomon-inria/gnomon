import QtQuick          2.15
import QtQuick.Controls 2.15

import crossQuick        1.0 as C
import crossParameters   1.0 as C

import xQuick.Controls  1.0 as X
import xQuick.Fonts     1.0 as X
import xQuick.Style     1.0 as X

import gnomonQuick      1.0 as G

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
        C.Numeric {param: lparam; paramType: lparam.type}
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
        C.InList {param: lparam}
    }

    Component {
        id: _range_component
        C.Range {param: lparam; decimals: 2}
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
        if (type == "dtk::d_real" || type == "dtk::d_bool" || type == "dtk::d_int") {
            return _num_component;
        }
        if (type == "dtk::d_string") {
            return  _string_component;
        }
        if (type == "dtk::d_path") {
            return _path_component;
        }
        if (type == "dtk::d_range_real" || type == "dtk::d_range_int") {
            return _range_component;
        }
        if (type == "dtk::d_inliststring") {
            return _liststring_component;
        }
        if (type == "dtk::d_inliststringlist") {
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
        console.log("UPDATING PARAMETERS", parameters)
        params_model.clear();
        for (var param_name in parameters) {
            var p = parameters[param_name];
            var prop_dict = {};
            console.log("PARAMETER", p.type)
            if(p.type === "dtk::d_inliststring") console.log("LIST", p.list)
            prop_dict["component"] = _self.getComponent(p.type)
            prop_dict["param"] = p;
            params_model.append(prop_dict);
        }
    }

}
