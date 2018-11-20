template<class T>
gnomonWorkspaceTemplatePrivate< T >::gnomonWorkspaceTemplatePrivate()
{
    this->command = nullptr;
    this->pane_item_params_layout = nullptr;
}

template<class T>
gnomonWorkspaceTemplatePrivate< T >::~gnomonWorkspaceTemplatePrivate()
{
    if(this->command)
        delete this->command;
}

template<class T>
void gnomonWorkspaceTemplatePrivate< T >::configure(QWidget* parent, const QString& algorithm)
{
    if(this->pane_item_params_layout)
    {
        for(int row = 0, max_row = this->pane_item_params_layout->count(); row < max_row; ++row)
        {
            QLayoutItem *forDeletion = this->pane_item_params_layout->takeAt(0);
            forDeletion->widget()->disconnect();
            delete forDeletion->widget();
            delete forDeletion;
        }
    }
    else
    {
        this->pane_item_params_layout = new QFormLayout(parent); 
    }
    if(this->command)
        delete this->command;
    this->command = new T(algorithm); 
    QMap<QString, QVariant> parameters = this->command->parameters();
    for(QMap<QString, QVariant>::iterator it = parameters.begin(), it_end = parameters.end(); it != it_end; ++it)
    { 
        QWidget *widget;
        QString key = it.key();
        int type = it.value().type();
        if (type == QMetaType::Int ||
            // type == QMetaType::Uint ||
            type == QMetaType::Long ||
            type == QMetaType::ULong ||
            type == QMetaType::LongLong ||
            type == QMetaType::ULongLong) {
            widget = new QSpinBox(parent);
            static_cast< QSpinBox* >(widget)->setValue(it.value().value<int>());
            parent->connect(static_cast< QSpinBox* >(widget), static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                    [=](int value){ this->command->setParameter(key, value); });
        } else if (type == QMetaType::Float ||
                   type == QMetaType::Double) {
            widget = new QDoubleSpinBox(parent);
            static_cast< QDoubleSpinBox* >(widget)->setValue(it.value().value<double>());
            parent->connect(static_cast< QDoubleSpinBox* >(widget),  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    [=](double value){ this->command->setParameter(key, value); });
        } else if (type == QMetaType::QString) {
            widget = new QLineEdit(parent);
            static_cast< QLineEdit* >(widget)->setText(it.value().value<QString>());
            parent->connect(static_cast< QLineEdit* >(widget), &QLineEdit::textChanged,
                    [=](QString value){ this->command->setParameter(key, value); });
        } else if (type == QMetaType::Bool) {
            widget = new QCheckBox(parent);
            if(it.value().value<bool>())
                static_cast< QCheckBox* >(widget)->setCheckState(Qt::Checked);
            else
                static_cast< QCheckBox* >(widget)->setCheckState(Qt::Unchecked);
            parent->connect(static_cast< QCheckBox* >(widget), &QCheckBox::stateChanged,
                    [=](int value){ this->command->setParameter(key, value > Qt::Unchecked); });
        }
        this->pane_item_params_layout->addRow(it.key(), widget);
    }  
    this->pane_item_params_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
}

template<class T>
gnomonOverlayPane* gnomonWorkspaceTemplatePrivate< T >::pane(QWidget* parent)
{
    QComboBox *combo_box = new QComboBox(parent);
    QStringList combo_box_keys = this->keys();
    for (QStringList::iterator it = combo_box_keys.begin(), it_end = combo_box_keys.end(); it != it_end; ++it)
    { combo_box->addItem(*it); }

    parent->connect(combo_box, SIGNAL(currentIndexChanged(QString)), parent, SLOT(configure(QString)));


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

    gnomonOverlayPaneItem *pane_item_button = new gnomonOverlayPaneItem(parent);
    pane_item_button->setTitle(this->workspace());
    pane_item_button->addWidget(button);
    pane_item_button->toggle();

    gnomonOverlayPane *pane = new gnomonOverlayPane(parent);
    pane->addWidget(pane_item_algorithm);
    pane->addWidget(pane_item_parameters);
    pane->addWidget(pane_item_button);
    pane->toggle();

    parent->connect(button, SIGNAL(clicked()), parent, SLOT(apply()));

    configure(parent, combo_box->currentText());

    return pane;
}