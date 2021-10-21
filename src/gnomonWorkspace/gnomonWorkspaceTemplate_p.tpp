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

#include <dtkCore/dtkCoreParameter.h>

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand.h>
#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand.h>
#include <gnomonCore/gnomonCommand/gnomonAbstractConstructorCommand.h>

//#include <gnomonWidgets/gnomonWidgetsParameter.h>

//#include <gnomonComposer>

template <typename T> gnomonWorkspaceTemplatePrivate<T>::gnomonWorkspaceTemplatePrivate(void)
{
//    this->pipeline = gnomonPipeline::instance();
}

template <typename T> gnomonWorkspaceTemplatePrivate<T>::~gnomonWorkspaceTemplatePrivate(void)
{
    if (this->command)
        delete this->command;
}

template <typename T> void gnomonWorkspaceTemplatePrivate<T>::configure(const QString& algorithm)
{
    // if (this->layout) {
    //     for(int row = 0, max_row = this->layout->count(); row < max_row; ++row) {
    //         QLayoutItem *forDeletion = this->layout->takeAt(0);
    //         dtkWidgetsParameter *parameter_widget = dynamic_cast<dtkWidgetsParameter *>(forDeletion->widget());
    //         if (parameter_widget) {
    //             delete parameter_widget;
    //         } else {
    //             forDeletion->widget()->disconnect();
    //             delete forDeletion->widget();
    //         }
    //         delete forDeletion;
    //     }
    // } else {
    //     this->layout = new QFormLayout;
    // }

    this->command->setAlgorithmName(algorithm);
    this->algorithm = algorithm;

    if (!algorithm.isEmpty()) {

        dtkCoreParameters parameters = this->command->parameters();
        QList<QString> keys = parameters.keys();
        keys.sort();
        // for(QString key : keys) {
        //     QWidget *widget = gnomonWidgetsParameter::widget(parameters[key], 0);
        //     if (widget) {
        //         this->layout->addRow(key, widget);
        //     }
        // }

        // this->layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    }
}

template <typename T> void gnomonWorkspaceTemplatePrivate<T>::registerPipeline(void)
{
    if (this->command) {
        if (gnomonAbstractAlgorithmCommand *algorithm_command = dynamic_cast<gnomonAbstractAlgorithmCommand *>(this->command)) {
//TODO:            this->pipeline->addAlgorithm(algorithm_command);
        } else if (gnomonAbstractConstructorCommand *constructor_command = dynamic_cast<gnomonAbstractConstructorCommand *>(this->command)) {
//TODO:            this->pipeline->addConstructor(constructor_command);
        }
    }
}

// template <typename T> dtkWidgetsMenu *gnomonWorkspaceTemplatePrivate<T>::menu(dtkWidgetsWorkspace *parent)
// {
//     QComboBox *combo_box = new QComboBox;

//     QLabel *doc = new QLabel;
//     doc->setWordWrap(true);

//     QStringList combo_box_keys = this->keys();

//     for (auto it = combo_box_keys.begin(), it_end = combo_box_keys.end(); it != it_end; ++it) {
//         combo_box->addItem(*it);
//     }
//     combo_box->model()->sort(0);

//     this->command = new T(combo_box->currentText());

//     QObject::connect(combo_box, &QComboBox::currentTextChanged, [=] (const QString& algorithm)
//     {
//         if (this->command) {
//             delete this->command;
//             this->command = nullptr;
//         }
//         this->command = new T(algorithm);
//         emit algorithmChanged(algorithm);
//         this->configure(algorithm);

//         doc->setText(this->command->documentation());
//      });

//     doc->setText(this->command->documentation());

//     QVBoxLayout *pane_item_algorithm_layout = new QVBoxLayout;
//     pane_item_algorithm_layout->addWidget(combo_box);
//     pane_item_algorithm_layout->addWidget(doc);

//     this->layout = new QFormLayout;

//     QPushButton *pane_item_button = new QPushButton("Apply");
//     pane_item_button->setCheckable(true);

//     QWidget *pane_item_algorithm = new QWidget;
//     pane_item_algorithm->setLayout(pane_item_algorithm_layout);

//     QWidget *pane_item_parameters = new QWidget;
//     pane_item_parameters->setLayout(this->layout);

//     dtkWidgetsMenuItemDIY *menu_algorithm = new dtkWidgetsMenuItemDIY("Algorithm");
//     menu_algorithm->addWidget(pane_item_algorithm);
//     menu_algorithm->setSizePolicy(QSizePolicy::Expanding);

//     dtkWidgetsMenuItemDIY *menu_parameters = new dtkWidgetsMenuItemDIY("Parameters");
//     menu_parameters->addWidget(pane_item_parameters);
//     menu_parameters->setSizePolicy(QSizePolicy::Expanding);

//     dtkWidgetsMenuItemDIY *menu_button = new dtkWidgetsMenuItemDIY(this->workspace(), pane_item_button);
//     menu_button->setShowTitle(false);
//     menu_button->setSizePolicy(QSizePolicy::Expanding);

//     dtkWidgetsMenu *pane = new dtkWidgetsMenu(fa::circlethin, this->workspace());
//     pane->addItem(menu_algorithm);
//     pane->addItem(menu_parameters);
//     pane->addItem(menu_button);

//     QObject::connect(pane_item_button, &QPushButton::clicked, [=] () {
//         dtkApp->window()->setCursor(Qt::BusyCursor);
//         dynamic_cast<dtkWidgetsWorkspace*>(parent)->apply();
//         dtkApp->window()->setCursor(Qt::ArrowCursor);
//     });

//     configure(combo_box->currentText());

//     return pane;
// }

template <typename T>  bool gnomonWorkspaceTemplatePrivate<T>::isEmpty(void)
{
    return T::isEmpty();
}

//
// gnomonWorkspaceTemplate_p.tpp ends here
