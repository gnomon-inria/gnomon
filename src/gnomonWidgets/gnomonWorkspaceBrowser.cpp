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
    QListWidget *channels_list = nullptr;
    QLineEdit *lut_hue_min;
    QLineEdit *lut_hue_max;
    QLineEdit *lut_sat_min;
    QLineEdit *lut_sat_max;
    QLineEdit *lut_val_min;
    QLineEdit *lut_val_max;

    QMap<QString, std::tuple<double, double, double, double, double, double> >channels_lut;
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

    d->lut_hue_min = new QLineEdit("0.");
    d->lut_hue_max = new QLineEdit("0.5");
    d->lut_sat_min = new QLineEdit("1.");
    d->lut_sat_max = new QLineEdit("1.");
    d->lut_val_min = new QLineEdit("0.");
    d->lut_val_max = new QLineEdit("1.");
    connect(d->lut_hue_min, &QLineEdit::textEdited, this, &gnomonWorkspaceBrowser::applyLut);
    connect(d->lut_hue_max, &QLineEdit::textEdited, this, &gnomonWorkspaceBrowser::applyLut);
    connect(d->lut_sat_min, &QLineEdit::textEdited, this, &gnomonWorkspaceBrowser::applyLut);
    connect(d->lut_sat_max, &QLineEdit::textEdited, this, &gnomonWorkspaceBrowser::applyLut);
    connect(d->lut_val_min, &QLineEdit::textEdited, this, &gnomonWorkspaceBrowser::applyLut);
    connect(d->lut_val_max, &QLineEdit::textEdited, this, &gnomonWorkspaceBrowser::applyLut);

    QFormLayout *pane_item_channels_lut_layout = new QFormLayout;
    pane_item_channels_lut_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pane_item_channels_lut_layout->addRow(QString("lut_hue_min : "), d->lut_hue_min);
    pane_item_channels_lut_layout->addRow(QString("lut_hue_max : "), d->lut_hue_max);
    pane_item_channels_lut_layout->addRow(QString("lut_sat_min : "), d->lut_sat_min);
    pane_item_channels_lut_layout->addRow(QString("lut_sat_max : "), d->lut_sat_max);
    pane_item_channels_lut_layout->addRow(QString("lut_val_min : "), d->lut_val_min);
    pane_item_channels_lut_layout->addRow(QString("lut_val_max : "), d->lut_val_max);

    gnomonOverlayPaneItem *pane_item_channels_lut = new gnomonOverlayPaneItem;
    pane_item_channels_lut->setTitle("Channels Lookuptable");
    pane_item_channels_lut->addLayout(pane_item_channels_lut_layout);
    pane_item_channels_lut->toggle();

    gnomonOverlayPane *pane = new gnomonOverlayPane(this);
    pane->addWidget(pane_item_channels);
    pane->addWidget(pane_item_channels_lut);
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

void gnomonWorkspaceBrowser::applyLut(void)
{
    //if we are editing a single channel image
    QString channel_name = "default";
    if(d->channels_list->currentItem()) {
        channel_name = d->channels_list->currentItem()->text();
    }

    d->channels_lut[channel_name] = std::make_tuple(d->lut_hue_min->text().toDouble(),
                                                    d->lut_hue_max->text().toDouble(),
                                                    d->lut_sat_min->text().toDouble(),
                                                    d->lut_sat_max->text().toDouble(),
                                                    d->lut_val_min->text().toDouble(),
                                                    d->lut_val_max->text().toDouble());

    d->browse_view->applyLut(d->lut_hue_min->text().toDouble(),
                             d->lut_hue_max->text().toDouble(),
                             d->lut_sat_min->text().toDouble(),
                             d->lut_sat_max->text().toDouble(),
                             d->lut_val_min->text().toDouble(),
                             d->lut_val_max->text().toDouble());
}

void gnomonWorkspaceBrowser::replaceChannel(QListWidgetItem *current_item, QListWidgetItem *previous_item)
{
    if(current_item) {
        qDebug() << "current" << current_item->text();
        current_item->setCheckState(Qt::Checked);
        //save old values
        if(previous_item) {
            qDebug() << "previous" << previous_item->text();
            current_item->setCheckState(Qt::Unchecked);
            d->channels_lut[previous_item->text()] = std::make_tuple(d->lut_hue_min->text().toDouble(),
                                                                     d->lut_hue_max->text().toDouble(),
                                                                     d->lut_sat_min->text().toDouble(),
                                                                     d->lut_sat_max->text().toDouble(),
                                                                     d->lut_val_min->text().toDouble(),
                                                                     d->lut_val_max->text().toDouble());
        }

        //set current value
        if(d->channels_lut.contains(current_item->text())) {
            auto saved_lut = d->channels_lut[current_item->text()];
            d->lut_hue_min->setText(QString::number(std::get<0>(saved_lut)));
            d->lut_hue_max->setText(QString::number(std::get<1>(saved_lut)));
            d->lut_sat_min->setText(QString::number(std::get<2>(saved_lut)));
            d->lut_sat_max->setText(QString::number(std::get<3>(saved_lut)));
            d->lut_val_min->setText(QString::number(std::get<4>(saved_lut)));
            d->lut_val_max->setText(QString::number(std::get<5>(saved_lut)));
        }

        this->displayChannels();
    }
}

void gnomonWorkspaceBrowser::displayChannels(void)
{
    //count the number of channels ticked
    int nb_channels_ticked = 0;
    for(int i=0; i < d->channels_list->count(); ++i) {
        if(d->channels_list->item(i)->checkState() == Qt::Checked)
            ++nb_channels_ticked;
    }

    qDebug() << "nb channels ticked" << nb_channels_ticked;

    switch(nb_channels_ticked) {
    case 0:
        d->browse_view->setBlending(false);
        if(d->channels_list->currentItem())
            d->browse_view->onChannelChanged(d->channels_list->currentItem()->text());
        break;

    case 1:
        d->browse_view->setBlending(false);
        if(d->channels_list->currentItem())
            d->browse_view->onChannelChanged(d->channels_list->currentItem()->text());
        break;

    default:
        d->browse_view->setBlending(true);
        for(int i=0; i < d->channels_list->count(); ++i) {
            if(d->channels_list->item(i)->checkState() == Qt::Checked)
                d->browse_view->onChannelChanged(d->channels_list->item(i)->text());
        }
    }

}

gnomonWorkspaceBrowser::~gnomonWorkspaceBrowser(void)
{
    delete d;
}

//
// gnomonWorkspaceBrowser.cpp ends here
