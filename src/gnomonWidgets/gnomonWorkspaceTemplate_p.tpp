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

#include <gnomonCore/gnomonCoreParameter.h>

#include "gnomonWidgetsParameter.h"

template <typename T> gnomonWorkspaceTemplatePrivate<T>::gnomonWorkspaceTemplatePrivate(void)
{

}

template <typename T> gnomonWorkspaceTemplatePrivate<T>::~gnomonWorkspaceTemplatePrivate(void)
{
    if (this->command)
        delete this->command;
}

template <typename T> void gnomonWorkspaceTemplatePrivate<T>::configure(QWidget *parent, const QString& algorithm)
{
    if (this->pane_item_params_layout) {
        for(int row = 0, max_row = this->pane_item_params_layout->count(); row < max_row; ++row) {
            QLayoutItem *forDeletion = this->pane_item_params_layout->takeAt(0);
            forDeletion->widget()->disconnect();
            delete forDeletion->widget();
            delete forDeletion;
        }

    } else {
        this->pane_item_params_layout = new QFormLayout(parent);
    }

    this->algorithm=algorithm;


    if (!algorithm.isEmpty()) {
        QMap<QString, gnomonCoreParameter *> parameters = this->command->parameters();
        for(QMap<QString, gnomonCoreParameter*>::iterator it = parameters.begin(), it_end = parameters.end(); it != it_end; ++it) {
            QWidget *widget = gnomonWidgetsParameter::widget(it.value(), parent);
            if (widget)
                this->pane_item_params_layout->addRow(it.key(), widget);
        }
        this->pane_item_params_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    }
}

template <typename T> gnomonOverlayPane *gnomonWorkspaceTemplatePrivate<T>::pane(QWidget *parent)
{
    QComboBox *combo_box = new QComboBox(parent);
    QStringList combo_box_keys = this->keys();
    for (auto it = combo_box_keys.begin(), it_end = combo_box_keys.end(); it != it_end; ++it) {
        combo_box->addItem(*it);
    }
    combo_box->model()->sort(0);
    this->command = new T(combo_box->currentText());

    QObject::connect(combo_box, &QComboBox::currentTextChanged, [=] (const QString& algorithm) {
        if (this->command) {
            delete this->command;
            this->command = nullptr;
        }
        this->command = new T(algorithm);
        emit algorithmChanged(algorithm);
        this->configure(parent, algorithm);
     });

    gnomonOverlayPaneItem *pane_item_algorithm = new gnomonOverlayPaneItem(parent);
    pane_item_algorithm->setTitle("Algorithm");
    pane_item_algorithm->addWidget(combo_box);
    pane_item_algorithm->toggle();

    this->pane_item_params_layout = new QFormLayout;

    gnomonOverlayPaneItem *pane_item_parameters = new gnomonOverlayPaneItem;
    pane_item_parameters->setTitle("Parameters");
    pane_item_parameters->addLayout(pane_item_params_layout);
    pane_item_parameters->toggle();

    QPushButton *button = new QPushButton("Apply", parent);
    button->setCheckable(true);

    gnomonOverlayPaneItem *pane_item_button = new gnomonOverlayPaneItem(parent);
    pane_item_button->setTitle(this->workspace());
    pane_item_button->addWidget(button);
    pane_item_button->toggle();

    gnomonOverlayPane *pane = new gnomonOverlayPane(parent);
    pane->addWidget(pane_item_algorithm);
    pane->addWidget(pane_item_parameters);
    pane->addWidget(pane_item_button);
    pane->toggle();
    pane->toggle();

    QObject::connect(button, &QPushButton::clicked, [=] () {
        parent->setCursor(Qt::BusyCursor);
        dynamic_cast<gnomonWorkspace*>(parent)->apply();
        parent->setCursor(Qt::ArrowCursor);
    });

    configure(parent, combo_box->currentText());

    return pane;
}

//
// gnomonWorkspaceTemplate_p.tpp ends here
