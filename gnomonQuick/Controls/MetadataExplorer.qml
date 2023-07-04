import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Layouts    1.15

import Qt5Compat.GraphicalEffects

import gnomonQuick.Controls  1.0 as G
import gnomonQuick.Style  1.0 as G

import gnomon.Visualization 1.0 as GV
import gnomon.MetaData    1.0 as GM

Rectangle {
    id: _self;

    property int formId;
    property var dynamicFormMetadata : GV.World.contains(_self.formId) ? GV.World.getDynamicFormMetadata(_self.formId) : undefined;

    Rectangle {
        id: current_form;
        anchors.left: parent.left;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.margins: 12;
        //width: 2*parent.width/3;
        width: _img_rect.width + (parent.width - _img_rect.width)/2

        border.color: G.Style.colors.fgColor;
        border.width: 1;
        color: Qt.darker(G.Style.colors.bgColor)

        Rectangle {
            id: _img_rect;
            anchors.left: parent.left;
            anchors.top: parent.top;
            anchors.bottom: parent.bottom;
            anchors.margins: 12
            width: Math.min(2*_self.width/3, parent.height) - 12;

            color: parent.color;

            Image {
                id: _img
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.verticalCenter: parent.verticalCenter;

                width: Math.min(parent.width, parent.height);
                fillMode: Image.PreserveAspectFit

                source: "image://thumbnails/" + _self.formId
            }
        }

        Rectangle {
            id: current_form_metadata_panel;

            anchors.left: _img_rect.right;
            anchors.top: parent.top;
            anchors.bottom: parent.bottom;
            anchors.right: parent.right;
            anchors.margins: 12;

            color: parent.color;

            ListView {
                id: form_metadata;

                anchors.top: parent.top;
                anchors.bottom: time_slider.top;
                anchors.right: parent.right;
                anchors.left: parent.left;
                anchors.margins: 12;

                model : GV.World.contains(_self.formId) ? GV.World.formMetadataKeysAtT(_self.formId, time_slider.t) : undefined;

                delegate: ItemDelegate {
                    id: _f_delegate;
                    width: form_metadata.width;

                    Label {
                        id: _name_label
                        anchors.left: parent.left;
                        anchors.bottom: parent.bottom;
                        anchors.verticalCenter: _form_name.verticalCenter;
                        anchors.margins: 5;

                        width: Math.max(100, _name_label.contentWidth);

                        font.pointSize: 14;
                        font.bold: true;
                        verticalAlignment: Text.AlignVCenter
                        text: modelData;
                    }

                    TextField {
                        id: _form_name;

                        anchors.left: _name_label.right;
                        anchors.right: parent.right;
                        anchors.top: parent.top;
                        anchors.bottom: parent.bottom;
                        anchors.margins: 5;

                        readOnly: true;
                        font.pointSize: 14;
                        text: GV.World.formMetadataValueAtT(_self.formId, time_slider.t, modelData);
                    }
                }
            }

            G.TimeSeriesSlider {
                id: time_slider;
                property var times : GV.World.contains(_self.formId) ? GV.World.timeKeys(_self.formId) : [0.];
                property double t

                to: time_slider.times.length - 1;
                enabled: from != to;
                visible: enabled;

                onValueChanged: {
                    t = time_slider.times[Math.trunc(value)];
                }
            }
        }
    }


    ListView {
        id: form_collection_metadata;

        anchors.left: current_form.right;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
        anchors.margins: 12;
        anchors.rightMargin: 24

        model: _self.dynamicFormMetadata? _self.dynamicFormMetadata.keys : undefined

        delegate: ItemDelegate {
            id: _fc_delegate;
            width: form_collection_metadata.width;

            Label {
                id: _name_label
                anchors.left: parent.left;
                anchors.bottom: parent.bottom;
                anchors.verticalCenter: _form_name.verticalCenter;
                anchors.margins: 5;

                width: Math.max(100, _name_label.contentWidth);

                font.pointSize: 14;
                font.bold: true;
                verticalAlignment: Text.AlignVCenter
                text: modelData;
            }

            TextField {
                id: _form_name;

                anchors.left: _name_label.right;
                anchors.right: parent.right;
                anchors.top: parent.top;
                anchors.bottom: parent.bottom;
                anchors.margins: 5;

                font.pointSize: 14;
                text: _self.dynamicFormMetadata.data[modelData];
                //focus: true;
                onEditingFinished: {
                    _self.dynamicFormMetadata.set(modelData, _form_name.text);
                }
            }
        }
    }
}
