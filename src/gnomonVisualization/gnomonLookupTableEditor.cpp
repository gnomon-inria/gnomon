// Version: $Id: b3634632a3e3eac8bcc4a3b15a61a881bb42aa64 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include <gnomonVisualization/gnomonColorMapEditor.h>
#include <gnomonVisualization/gnomonDoubleRangeEditor.h>

#include "gnomonLookupTableEditor.h"
#include <cmath>

#include <QtXml>
#include <gnomonStyle>


// /////////////////////////////////////////////////////////////////
// gnomonLookupTableEditor
// /////////////////////////////////////////////////////////////////

class gnomonLookupTableEditorPrivate
{
public:
    gnomonLookupTable lut;

public:
    QCheckBox *visible;
    gnomonColorMapEditor *colormap;
    gnomonDoubleRangeEditor *value_range;

};

gnomonLookupTableEditor::gnomonLookupTableEditor(QWidget *parent) : QWidget(parent), d(new gnomonLookupTableEditorPrivate)
{
    QPalette palette ;
    palette.setBrush(QPalette::Background, Qt::transparent);

    d->visible = new QCheckBox("Show",this);
    d->colormap = new gnomonColorMapEditor(this);
    d->value_range = new gnomonDoubleRangeEditor(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d->colormap);
    layout->addWidget(d->value_range);
    layout->addWidget(d->visible);

    this->setValue(gnomonLookupTable("grey"));

    // this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(d->colormap, &gnomonColorMapEditor::valueChanged, [=](const QMap<double, QColor>& val) {
        d->lut.setColorMap(val);
        d->lut.setName(d->colormap->name());
        emit valueChanged(d->lut);
    });

    connect(d->value_range, &gnomonDoubleRangeEditor::valueMinChanged, [=](double value) { 
        d->lut.setValueRange(QList<double>({value, d->value_range->valueMax()}));
        emit valueChanged(d->lut);
    });

    connect(d->value_range, &gnomonDoubleRangeEditor::valueMaxChanged, [=](double value) { 
        d->lut.setValueRange(QList<double>({d->value_range->valueMin(), value}));
        emit valueChanged(d->lut);
    });

    connect(d->visible, &QCheckBox::stateChanged, [=](int val) { 
        d->lut.setVisibility(d->visible->checkState() == Qt::Checked);
        emit valueChanged(d->lut);
    });
}

gnomonLookupTableEditor::~gnomonLookupTableEditor(void)
{
    delete d;

    d = NULL;
}


gnomonLookupTable gnomonLookupTableEditor::value(void) const
{
    return d->lut;
}

void gnomonLookupTableEditor::setValue(gnomonLookupTable lut)
{
    d->lut = lut;

    d->colormap->setValue(lut.colorMapName());

    QList<double> valueRange = lut.valueRange();
    d->value_range->setRange(valueRange[0],valueRange[1]);
    d->value_range->setValueMin(valueRange[0]);
    d->value_range->setValueMax(valueRange[1]);

    if(lut.visibility())
        d->visible->setCheckState(Qt::Checked);
    else
        d->visible->setCheckState(Qt::Unchecked);

    // emit valueChanged(value);
}

//
// gnomonLookupTableEditor.cpp ends here
