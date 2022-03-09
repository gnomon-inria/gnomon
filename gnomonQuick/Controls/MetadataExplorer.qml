import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Layouts    1.15

import Qt5Compat.GraphicalEffects

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

import gnomonQuick.Controls  1.0 as G

import gnomon.Visualization 1.0 as GV
import gnomon.MetaData    1.0 as GM

Rectangle {
    id: _metadata_explorer;
    property int formId;
    property var dynamicFormMetadata

    onEnabledChanged: {
        if(enabled) {
            load_metadata();
        }
    }

    onFormIdChanged: {
        load_metadata();
    }

    function load_metadata() {
        // saving metadata
        for(let i = 0; i < form_collection_metadata.count; i++) {
            var item_delegate = form_collection_metadata.itemAtIndex(i);
            item_delegate.save();
        }

        console.log("Form changed : ", formId)
        form_collection_metadata.model = undefined;
        form_metadata.model = undefined;
        dynamicFormMetadata = undefined;
        if(GV.World.contains(formId)) {
            // loading thumbnail
            _img.source = "image://thumbnails/" + formId;

            // loading dynamicFormMetadata
            dynamicFormMetadata = GV.World.getDynamicFormMetadata(formId);
            form_collection_metadata.model = dynamicFormMetadata.keys;
            for(let i = 0; i < form_collection_metadata.count; i++) {
                var item_delegate = form_collection_metadata.itemAtIndex(i);
                item_delegate.load();
            }
            // setting slider
            time_slider.times = GV.World.timeKeys(formId);
            time_slider.to = time_slider.times.length - 1;
            current_form_metadata_panel.load_frame();

        } else {
            form_collection_metadata.model = undefined;
            dynamicFormMetadata = undefined;
        }
    }

    Rectangle {
        id: current_form;
        anchors.left: parent.left;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.margins: 12;
        //width: 2*parent.width/3;
        width: _img_rect.width + (parent.width - _img_rect.width)/2


        border.color: X.Style.foregroundColor;
        border.width: 1;
        color: Qt.darker(X.Style.backgroundColor)

        Rectangle {
            id: _img_rect;
            anchors.left: parent.left;
            anchors.top: parent.top;
            anchors.bottom: parent.bottom;
            anchors.margins: 12
            width: Math.min(2*_metadata_explorer.width/3, parent.height) - 12;

            color: parent.color;

            Image {
                id: _img
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.verticalCenter: parent.verticalCenter;
                width: Math.min(parent.width, parent.height);
                fillMode: Image.PreserveAspectFit
                source: "";
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
                        text: "";
                        //focus: true;
                        onAccepted: {

                        }
                    }
                    function load() {
                        var text = GV.World.formMetadataValueAtT(
                            formId,
                            time_slider.t,
                            modelData,
                        );
                        //console.log(text);
                        _form_name.text = text;
                    }
                }
            }

            Slider {
                id: time_slider;
                anchors.bottom: parent.bottom;
                anchors.left: parent.left;
                anchors.right: parent.right;
                height: 40;

                anchors.margins: 12;

                property var times;
                property double t;
                enabled: from != to;
                visible: enabled;

                from: 0;
                value: 0;
                to: 0;
                stepSize: 1;
                snapMode: Slider.SnapAlways;

                onValueChanged: {
                    if(times) {
                        t = times[Math.trunc(value)];
                    }
                }
            }

            function load_frame() {
                //t = times[Math.trunc(value)];
                form_metadata.model = GV.World.formMetadataKeysAtT(formId, time_slider.t);
                for(let i = 0; i < form_metadata.count; i++) {
                    var item_delegate = form_metadata.itemAtIndex(i);
                    item_delegate.load();
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
                text: "";
                //focus: true;
                onEditingFinished: {
                    save();
                }
            }

            function load() {
                _form_name.text = Qt.binding(function() {
                    return dynamicFormMetadata.data[modelData];
                })
            }

            function save() {
                dynamicFormMetadata.set(modelData, _form_name.text);
            }
        }
    }


}
