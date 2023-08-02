import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

import gnomon.Visualization 1.0 as GV
import gnomon.MetaData    1.0 as GM


G.Dialog {
    id: _self
    anchors.centerIn: Overlay.overlay
    width: Math.round(G.Style.largeDialogWidth*1.25);
    height: Math.round(G.Style.largeDialogHeight*1.25);
    focus: true
    modal: true
    title: "Edit Metadata"
    standardButtons: Dialog.Ok

    property int formId;
    property var dynamicFormMetadata : GV.World.contains(_self.formId) ? GV.World.getDynamicFormMetadata(_self.formId) : undefined;

    Rectangle {
        id: current_form;
        anchors.left: parent.left;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        //anchors.margins: G.Style.mediumPadding;
        width: parent.width/2;
        radius: G.Style.panelRadius;

        color: G.Style.colors.fgColor

        Label {
            id: current_form_header
            anchors.left: parent.left;
            anchors.top: parent.top;
            anchors.right: parent.right;
            text: _text_provider()
            function _text_provider() {
                return "Metadata at time " + time_slider.t
            }

            font: G.Style.fonts.header
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            topPadding: G.Style.smallPadding;
            bottomPadding: G.Style.smallPadding;
            leftPadding: G.Style.mediumPadding
        }

        G.Gutter {
            id: current_form_metadata_panel;

            anchors.left: parent.left;
            anchors.top: current_form_header.bottom;
            anchors.bottom: parent.bottom;
            anchors.right: parent.right;
            anchors.margins: G.Style.mediumPadding;
            anchors.topMargin: 0

            ListView {
                id: form_metadata;

                anchors.top: parent.top;
                anchors.bottom: time_slider.top;
                anchors.right: parent.right;
                anchors.left: parent.left;

                model : GV.World.contains(_self.formId) ? GV.World.formMetadataKeysAtT(_self.formId, time_slider.t) : undefined;

                delegate: G.ListItemDelegate {
                    id: _f_delegate;
                    width: form_metadata.width;
                    hoverEnabled: false

                    Label {
                        id: _name_label
                        anchors.left: parent.left;
                        anchors.bottom: parent.bottom;
                        anchors.verticalCenter: _form_name.verticalCenter;
                        anchors.margins: G.Style.smallPadding;

                        width: Math.max(100, _name_label.contentWidth);

                        font: G.Style.fonts.formLabel;
                        verticalAlignment: Text.AlignVCenter
                        text: modelData;
                    }

                    G.TextField {
                        id: _form_name;

                        anchors.left: _name_label.right;
                        anchors.right: parent.right;
                        anchors.top: parent.top;
                        anchors.bottom: parent.bottom;
                        anchors.margins: G.Style.smallPadding;

                        readOnly: true;
                        font: G.Style.fonts.value;
                        text: GV.World.formMetadataValueAtT(_self.formId, time_slider.t, modelData);
                    }
                }
            }

            G.TimeSeriesSlider {
                id: time_slider;
                anchors.left: parent.left;
                anchors.right: parent.right;
                anchors.bottom: parent.bottom;
                anchors.margins: G.Style.smallPadding
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

    Rectangle {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: current_form.right
        anchors.right: parent.right
        radius: G.Style.panelRadius;
        color: G.Style.colors.bgColor

        Label {
            id: series_metadata_header
            anchors.left: parent.left;
            anchors.top: parent.top;
            anchors.right: parent.right;
            text: "Series Metadata"
            font: G.Style.fonts.header
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            topPadding: G.Style.smallPadding;
            bottomPadding: G.Style.smallPadding;
            leftPadding: G.Style.mediumPadding
        }

        G.Gutter {
            anchors.left: parent.left;
            anchors.top: series_metadata_header.bottom;
            anchors.bottom: parent.bottom;
            anchors.right: parent.right;
            anchors.margins: G.Style.mediumPadding;
            anchors.topMargin: 0

            ListView {
                id: form_collection_metadata;
                anchors.fill: parent


                model: _self.dynamicFormMetadata? _self.dynamicFormMetadata.keys : undefined

                delegate: G.ListItemDelegate {
                    id: _fc_delegate;
                    width: form_collection_metadata.width;
                    hoverEnabled: false

                    Label {
                        id: _name_label
                        anchors.left: parent.left;
                        anchors.bottom: parent.bottom;
                        anchors.verticalCenter: _form_name.verticalCenter;
                        anchors.margins: G.Style.smallPadding;

                        width: Math.max(100, _name_label.contentWidth);

                        font: G.Style.fonts.formLabel;
                        verticalAlignment: Text.AlignVCenter
                        text: modelData;
                    }

                    G.TextField {
                        id: _form_name;

                        anchors.left: _name_label.right;
                        anchors.right: parent.right;
                        anchors.top: parent.top;
                        anchors.bottom: parent.bottom;
                        anchors.margins: G.Style.smallPadding;

                        font: G.Style.fonts.value;
                        text: _self.dynamicFormMetadata.data[modelData];
                        //focus: true;
                        onEditingFinished: {
                            _self.dynamicFormMetadata.set(modelData, _form_name.text);
                        }
                    }
                }
            }
        }
    }
}
