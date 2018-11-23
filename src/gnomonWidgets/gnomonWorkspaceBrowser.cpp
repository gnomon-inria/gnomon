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

#include "gnomonWorkspaceBrowser.h"

#include <gnomonVisualization/gnomonColorMapEditor.h>

#include "gnomonFinder.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonViewVolumic.h"

class gnomonWorkspaceBrowserPrivate
{
public:
    gnomonFinder *finder;
    gnomonFinderPathBar *path;
    gnomonFinderToolBar *toolbar;

public:
    gnomonViewVolumic *browse_view;

public:
    gnomonColorMapEditor *color_map_editor;
    gnomonOverlayPaneItem *pane_item_channels_lut = nullptr;
    QListWidget *channels_list = nullptr;

    QMap<QString, QMap<double, QColor> >channels_lut;
};

gnomonWorkspaceBrowser::gnomonWorkspaceBrowser(QWidget *parent) : gnomonWorkspace(parent)
{
    d = new gnomonWorkspaceBrowserPrivate;

    d->finder = new gnomonFinder(this);
    d->finder->switchToTreeView();

    d->path = new gnomonFinderPathBar(this);
    d->path->setPath(QDir::currentPath());
    d->path->setFixedHeight(32);

    d->toolbar = new gnomonFinderToolBar(this);
    d->toolbar->setPath(QDir::currentPath());

    d->browse_view = new gnomonViewVolumic(this);

    connect(d->browse_view, &gnomonViewVolumic::channelsChanged, this, &gnomonWorkspaceBrowser::replaceChannels);

    d->channels_list = new QListWidget();

    connect(d->channels_list, &QListWidget::currentItemChanged, this, &gnomonWorkspaceBrowser::replaceChannel);
    connect(d->channels_list, &QListWidget::itemChanged, this, &gnomonWorkspaceBrowser::displayChannels);

    QFormLayout *pane_item_channels_layout = new QFormLayout;
    pane_item_channels_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pane_item_channels_layout->addRow(d->channels_list);

    gnomonOverlayPaneItem *pane_item_channels = new gnomonOverlayPaneItem;
    pane_item_channels->setTitle("Channels");
    pane_item_channels->addLayout(pane_item_channels_layout);
    pane_item_channels->toggle();


    d->color_map_editor = new gnomonColorMapEditor(this);
    QPushButton * color_map_button = new QPushButton("Apply", this);
    connect(color_map_button, &QPushButton::clicked, [=](void) {
            QMap<double, QColor> source = d->color_map_editor->value();

            //if we are editing a single channel image
            QString channel_name = "default";
            if(d->channels_list->currentItem()) {
                channel_name = d->channels_list->currentItem()->text();
            }

            d->channels_lut[channel_name] = source;
            d->browse_view->applyLut(source);
        });

    d->pane_item_channels_lut = new gnomonOverlayPaneItem;
    d->pane_item_channels_lut->setTitle("Channels Lookuptable");
    d->pane_item_channels_lut->addWidget(d->color_map_editor);
    d->pane_item_channels_lut->addWidget(color_map_button);
    d->pane_item_channels_lut->toggle();

    gnomonOverlayPane *pane = new gnomonOverlayPane(this);
    pane->addWidget(pane_item_channels);
    pane->addWidget(d->pane_item_channels_lut);
    pane->toggle();

    QHBoxLayout *toolbar_layout = new QHBoxLayout;
    toolbar_layout->setContentsMargins(0, 0, 0, 0);
    toolbar_layout->setSpacing(0);
    toolbar_layout->addWidget(d->toolbar);
    toolbar_layout->addWidget(d->path);

    QVBoxLayout *finder_layout = new QVBoxLayout;
    finder_layout->setContentsMargins(0, 0, 0, 0);
    finder_layout->setSpacing(0);
    finder_layout->addLayout(toolbar_layout);
    finder_layout->addWidget(d->finder);

    QWidget *finder = new QWidget(this);
    finder->setLayout(finder_layout);

    connect(d->finder, SIGNAL(changed(QString)), d->path,    SLOT(setPath(QString)));
    connect(d->finder, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect(d->path, SIGNAL(changed(QString)), d->finder,  SLOT(setPath(QString)));
    connect(d->path, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect(d->toolbar, SIGNAL(changed(QString)), d->finder, SLOT(setPath(QString)));
    connect(d->toolbar, SIGNAL(changed(QString)), d->path,   SLOT(setPath(QString)));
    connect(d->toolbar, SIGNAL(treeView()),       d->finder, SLOT(switchToTreeView()));
    connect(d->toolbar, SIGNAL(listView()),       d->finder, SLOT(switchToListView()));

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(finder);
    splitter->addWidget(d->browse_view);
    splitter->addWidget(pane);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(splitter);
}

void gnomonWorkspaceBrowser::replaceChannels(QStringList channels_list)
{
    d->channels_list->clear();
    d->channels_list->addItems(channels_list);

    for(int i=0; i < d->channels_list->count(); ++i)
    {
        d->channels_list->item(i)->setFlags(d->channels_list->item(i)->flags() | Qt::ItemIsUserCheckable);
        d->channels_list->item(i)->setCheckState(Qt::Unchecked);
    }
}


void gnomonWorkspaceBrowser::replaceChannel(QListWidgetItem *current_item, QListWidgetItem *previous_item)
{
    if(current_item) {
        current_item->setCheckState(Qt::Checked);
        d->pane_item_channels_lut->setTitle("Channel "+ current_item->text() + " LookUpTable");

        //save old values
        if(previous_item) {
            current_item->setCheckState(Qt::Unchecked);
            d->channels_lut[previous_item->text()] = d->color_map_editor->value();
        }

        //set current value
        if(d->channels_lut.contains(current_item->text())) {
            d->color_map_editor->setValue(d->channels_lut[current_item->text()]);
        }

        this->displayChannels();
    }
}

void gnomonWorkspaceBrowser::apply(void)
{
}

void gnomonWorkspaceBrowser::displayChannels(void)
{
    //count the number of channels ticked
    int nb_channels_ticked = 0;
    for(int i=0; i < d->channels_list->count(); ++i) {
        if(d->channels_list->item(i)->checkState() == Qt::Checked)
            ++nb_channels_ticked;
    }

    switch(nb_channels_ticked) {
    case 0:
        d->browse_view->setBlending(false);
        if(d->channels_list->currentItem())
            d->browse_view->onChannelChanged(d->channels_list->currentItem()->text(),
                                             d->color_map_editor->value());
        break;

    case 1:
        d->browse_view->setBlending(false);
        if(d->channels_list->currentItem())
            d->browse_view->onChannelChanged(d->channels_list->currentItem()->text(),
                                             d->color_map_editor->value());
        break;

    default:
        d->browse_view->setBlending(true);
        for(int i=0; i < d->channels_list->count(); ++i) {
            if(d->channels_list->item(i)->checkState() == Qt::Checked) {
                if(d->channels_lut.contains(d->channels_list->item(i)->text())) {
                    d->browse_view->onChannelChanged(d->channels_list->item(i)->text(),
                                                     d->channels_lut[d->channels_list->item(i)->text()]);
                }
                else {
                    d->browse_view->onChannelChanged(d->channels_list->item(i)->text(),
                                                     d->color_map_editor->value());
                }
            }
        }
    }

}

gnomonWorkspaceBrowser::~gnomonWorkspaceBrowser(void)
{
    delete d;
}

//
// gnomonWorkspaceBrowser.cpp ends here
