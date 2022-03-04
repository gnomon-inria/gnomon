import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import xQuick.Controls 1.0 as X
import xQuick.Style    1.0 as X

Gradient {

    id: self

    required property var cmap;

    orientation: Gradient.Horizontal
    stops: gradientStops(self.cmap)

    function gradientStops(cmap) {
        var stops = [];
        for (var c in cmap) {
            var stop_str = "import QtQuick 2.15; GradientStop { position: "+c+"; color: '"+cmap[c]+"'}";
            var stop = Qt.createQmlObject(stop_str, self);
            stops.push(stop);
        }
        return stops
    }

}
