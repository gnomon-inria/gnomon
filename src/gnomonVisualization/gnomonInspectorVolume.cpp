// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonInspectorVolume.h"
#include "gnomonClutEditor.h"

// /////////////////////////////////////////////////////////////////
// gnomonInspectorVolumePrivate
// /////////////////////////////////////////////////////////////////

class gnomonInspectorVolumePrivate
{
public:
    gnomonClutEditor *editor;

public:
    QPushButton *editor_bt;
    QComboBox *clut_cb;

};

// /////////////////////////////////////////////////////////////////
// gnomonInspectorVolume
// /////////////////////////////////////////////////////////////////

gnomonInspectorVolume::gnomonInspectorVolume(QWidget *parent) : gnomonInspector(parent), d(new gnomonInspectorVolumePrivate)
{
    d->editor = new gnomonClutEditor();

    QVBoxLayout *layout = new QVBoxLayout;
    d->editor_bt = new QPushButton(this);
    d->editor_bt->setText("Editor");
    d->clut_cb = new QComboBox(this);
    d->clut_cb->addItem("Clut 1");
    d->clut_cb->addItem("Clut 2");
    d->clut_cb->addItem("Clut 3");
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    QHBoxLayout * hlayout = new QHBoxLayout;
    hlayout->addWidget(d->clut_cb);
    hlayout->addWidget(d->editor_bt);
    layout->addLayout(hlayout);
    this->setLayout(layout);

    connect(d->editor_bt, &QPushButton::clicked, [=]() {d->editor->show();});
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonInspectorVolume::~gnomonInspectorVolume(void)
{
    delete d;

    d = NULL;
}

gnomonClutEditor *gnomonInspectorVolume::editor(void) const
{
    return d->editor;
}

//
// gnomonInspectorVolume.cpp ends here
