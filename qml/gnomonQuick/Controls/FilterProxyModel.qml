import QtQuick      2.10
import QtQml.Models 2.3

DelegateModel {

    property var filterAccepts: function(item) {
        return true
    }

    onFilterAcceptsChanged: refilter()

    function refilter() {
        if(_hidden.count>0)
            _hidden.setGroups(0, _hidden.count, "default")
        if(items.count>0)
            items.setGroups(0, items.count, "default")
    }

    function filter() {
        while (_default.count > 0) {
            var item = _default.get(0)
            if(filterAccepts(item.model))
                item.groups = "items"
            else
                item.groups = "hidden"
        }
    }

    items.includeByDefault: false
    groups: [
        DelegateModelGroup {
            id: _default
            name: "default"
            includeByDefault: true
            onChanged: filter()
        },
        DelegateModelGroup {
            id: _hidden
            name: "hidden"
        }
    ]
}
