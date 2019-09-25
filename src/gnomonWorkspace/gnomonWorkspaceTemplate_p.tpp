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

#include <gnomonWidgets/gnomonWidgetsParameter.h>

#include <dtkWidgets>

template <typename T> gnomonWorkspaceTemplatePrivate<T>::gnomonWorkspaceTemplatePrivate(void)
{

}

template <typename T> gnomonWorkspaceTemplatePrivate<T>::~gnomonWorkspaceTemplatePrivate(void)
{
    if (this->command)
        delete this->command;
}

template <typename T> void gnomonWorkspaceTemplatePrivate<T>::configure(const QString& algorithm)
{
    if (this->layout) {

        for(int row = 0, max_row = this->layout->count(); row < max_row; ++row) {
            QLayoutItem *forDeletion = this->layout->takeAt(0);
            forDeletion->widget()->disconnect();
            delete forDeletion->widget();
            delete forDeletion;
        }
    } else {
        this->layout = new QFormLayout;
    }

    this->algorithm = algorithm;

    if (!algorithm.isEmpty()) {

        QMap<QString, gnomonCoreParameter *> parameters = this->command->parameters();

        for(QMap<QString, gnomonCoreParameter*>::iterator it = parameters.begin(), it_end = parameters.end(); it != it_end; ++it) {
            QWidget *widget = gnomonWidgetsParameter::widget(it.value(), 0);
            if (widget)
                this->layout->addRow(it.key(), widget);
        }

        this->layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    }
}

template <typename T> dtkWidgetsMenu *gnomonWorkspaceTemplatePrivate<T>::menu(dtkWidgetsWorkspace *parent)
{
    QComboBox *combo_box = new QComboBox;

    QLabel *doc = new QLabel;
    doc->setWordWrap(true);

    QStringList combo_box_keys = this->keys();

    for (auto it = combo_box_keys.begin(), it_end = combo_box_keys.end(); it != it_end; ++it) {
        combo_box->addItem(*it);
    }
    combo_box->model()->sort(0);

    // TODO: Script
    // this->command = new T(combo_box->currentText());

    QObject::connect(combo_box, &QComboBox::currentTextChanged, [=] (const QString& algorithm)
    {
        if (this->command) {
            delete this->command;
            this->command = nullptr;
        }
        // TODO: Script
        // this->command = new T(algorithm);
        emit algorithmChanged(algorithm);
        this->configure(algorithm);

        // TODO: Script
        // doc->setText(this->command->documentation());
     });

    // TODO: Script
    // doc->setText(this->command->documentation());
    
    QVBoxLayout *pane_item_algorithm_layout = new QVBoxLayout;
    pane_item_algorithm_layout->addWidget(combo_box);
    pane_item_algorithm_layout->addWidget(doc);

    this->layout = new QFormLayout;

    QPushButton *pane_item_button = new QPushButton("Apply");
    pane_item_button->setCheckable(true);

    QWidget *pane_item_algorithm = new QWidget;
    pane_item_algorithm->setLayout(pane_item_algorithm_layout);
    
    QWidget *pane_item_parameters = new QWidget;
    pane_item_parameters->setLayout(this->layout);
    
    dtkWidgetsMenuItem *menu_algorithm = new dtkWidgetsMenuItem("Algorithm", pane_item_algorithm);
    dtkWidgetsMenuItem *menu_parameters = new dtkWidgetsMenuItem("Parameters", pane_item_parameters);
    dtkWidgetsMenuItem *menu_button = new dtkWidgetsMenuItemDIY(this->workspace(), pane_item_button);

    dtkWidgetsMenu *pane = new dtkWidgetsMenu(fa::circlethin, "Generic Gnomon Menu");
    pane->addItem(menu_algorithm);
    pane->addItem(menu_parameters);
    pane->addItem(menu_button);

    QObject::connect(pane_item_button, &QPushButton::clicked, [=] () {
        dtkApp->window()->setCursor(Qt::BusyCursor);
        dynamic_cast<dtkWidgetsWorkspace*>(parent)->apply();
        dtkApp->window()->setCursor(Qt::ArrowCursor);
    });

    configure(combo_box->currentText());

    return pane;
}

//
// gnomonWorkspaceTemplate_p.tpp ends here
