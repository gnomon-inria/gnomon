import QtQuick            2.15
import QtQuick.Controls   2.15
import QtGraphicalEffects 1.15
import QtQuick.Layouts    1.15

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

Item {

    id: self;

    // TODO: Start with a flickable

    required property Item view;

    ColumnLayout {

        anchors.fill: parent;

        ComboBox {
            model: view.formNames;

            Layout.fillWidth: true;

            onCurrentIndexChanged: {
                view.updateVisualizations(view.formNames[currentIndex]);
            }
        }

        ComboBox {
            model: self.view.visualizations;

            Layout.fillWidth: true;
        }

        Item {
            Layout.fillHeight: true;
            Layout.fillWidth: true;
        }
    }
}
