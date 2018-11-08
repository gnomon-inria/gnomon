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

#include "gnomonFinder.h"
#include "gnomonWorkspaceBrowser.h"
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
    QListWidget *channels_list = nullptr;
};

gnomonWorkspaceBrowser::gnomonWorkspaceBrowser(QWidget *parent) : QSplitter(parent)
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
    ///////////////////

    d->channels_list = new QListWidget();

    connect(d->channels_list, &QListWidget::currentItemChanged, [=] (QListWidgetItem *current_item, QListWidgetItem *previous_item) {
        emit channelChanged(current_item->text());
    });

    QFormLayout *pane_item_channels_layout = new QFormLayout;
    pane_item_channels_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pane_item_channels_layout->addRow(d->channels_list);

    gnomonOverlayPaneItem *pane_item_channels = new gnomonOverlayPaneItem;
    pane_item_channels->setTitle("Channels");
    pane_item_channels->addLayout(pane_item_channels_layout);
    pane_item_channels->toggle();

    gnomonOverlayPane *pane = new gnomonOverlayPane(this);
    pane->addWidget(pane_item_channels);
    pane->toggle();

    //////////////////:

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

    this->addWidget(finder);
    this->addWidget(d->browse_view);
    this->addWidget(pane);
}

void gnomonWorkspaceBrowser::replaceChannels(QStringList channels_list)
{
    qDebug() << "Ouh yeeeey";
    qDebug() << channels_list;
    d->channels_list->clear();
    d->channels_list->addItems(channels_list);
}

gnomonWorkspaceBrowser::~gnomonWorkspaceBrowser(void)
{
    delete d;
}

//
// gnomonWorkspaceBrowser.cpp ends here
