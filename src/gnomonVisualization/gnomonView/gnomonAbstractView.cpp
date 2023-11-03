#include <gnomonCore>

#include <gnomonManager/gnomonFormManager>
#include <gnomonVisualizations/gnomonAbstractVisualization>
#include <gnomonCommand/gnomonAbstractVisualizationCommand>

#include "gnomonAbstractView.h"
#include "gnomonAbstractView_p.h"


// ///////////////////////////////////////////////////////////////////
// gnomonAbstractViewPrivate
// ///////////////////////////////////////////////////////////////////

gnomonAbstractViewPrivate::gnomonAbstractViewPrivate(QObject *parent): QObject(parent)
{
    this->viewParameters.currentFormIndex = -1;
}

gnomonAbstractViewPrivate::~gnomonAbstractViewPrivate(void)
{
}

void gnomonAbstractViewPrivate::exportToManager(void)
{
    for(const auto& key: this->forms.keys()) {
        QImage image;
        std::shared_ptr<gnomonAbstractVisualization> visualization = nullptr;
        if (this->visualizationCommands.contains(key)) {
            image = this->visualizationCommands[key]->visualization()->imageRendering();
            visualization = this->visualizationCommands[key]->visualization();
        } else {
            image = QImage(1500, 1500, QImage::Format_RGB32);
            image.fill(Qt::GlobalColor::black);
        }
        gnomonFormManager::instance()->addForm(this->forms[key], image, visualization);
        emit q->exportedForm(this->forms[key]);
    }
}

void gnomonAbstractViewPrivate::setFormVisualization(const QString& form_type, const QString& visu_name, const QVariantMap &parameters)
{
    // Saving current parameters before change
    for (const auto& _type : this->forms.keys()) {
        const auto& _visu_name = this->visualizationCommands[_type]->algorithmName();
        this->viewParameters.parameters[_visu_name] = this->visualizationCommands[_type]->visualizationParameters();
    }

    auto visu_parameters = parameters;
    // Use saved parameters if none are provided
    if (this->viewParameters.parameters.contains(visu_name) && parameters.size()==0) {
        visu_parameters = this->viewParameters.parameters[visu_name];
    }

    // Update the form in the command (no update at this stage)
    this->visualizationCommands[form_type]->setForm(this->forms[form_type]);
    // Set the visualization name and parameters and update it
    this->visualizationCommands[form_type]->setFormVisualization(visu_name, visu_parameters);
    emit q->formVisualizationChanged(form_type);

    this->viewParameters.visuSelected[form_type] = visu_name;
    emit q->formVisuParametersChanged();
}


void gnomonAbstractViewPrivate::updateFormsTimes(void)
{
    this->forms_times.clear();

    for (const auto& key : q->formNames()) {
        for(auto time : q->form(key)->times()) {
            this->forms_times.insert(time);
        }
    }

    emit q->timeMaxChanged(q->timeMax());
    q->timesChanged();
}

// ///////////////////////////////////////////////////////////////////
// gnomonAbstractView
// ///////////////////////////////////////////////////////////////////

gnomonAbstractView::gnomonAbstractView(QObject *parent): QObject(parent)
{
    d = new gnomonAbstractViewPrivate(this);
    d->q  = this;

    connect(this, &gnomonAbstractView::formAdded, [=]() {
        d->updateFormsTimes();
        emit formsChanged();
    });

    // just need to find a signal that's actually emitted when a parameter changes :|
    connect(this, &gnomonAbstractView::formVisuParametersChanged, [=] () {
        for (const auto& form_type : d->forms.keys()) {
            const auto& visu_name = d->visualizationCommands[form_type]->algorithmName();
            d->viewParameters.parameters[visu_name] = d->visualizationCommands[form_type]->visualizationParameters();
        }
    });
}

gnomonAbstractView::~gnomonAbstractView(void)
{
    for (auto command : d->visualizationCommands) {
        command->disconnectVisualization();
    }
    delete d;
}

void gnomonAbstractView::setForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form, std::shared_ptr<gnomonAbstractVisualization> visualization)
{
    QString form_type = form->formName();
    if (d->acceptForms.contains(form_type)  && d->acceptForms[form_type]) {
        // If another form of the same type is already in the view, we need to create a new visualization instance
        bool existing_visu = d->forms.contains(form_type) && (form != d->visualizationCommands[form_type]->inputs()[form_type]);
        d->forms[form_type] = form;
        // If the form comes with a visualization (drop from manager) we pass it on to the command (no update)
        if (visualization) {
            d->visualizationCommands[form_type]->setVisualization(visualization);
            emit formVisualizationChanged(form_type);
            d->visualizationCommands[form_type]->setForm(d->forms[form_type]);
            emit formVisuParametersChanged();
        } else {
            QString visu_name = d->visualizationCommands[form_type]->algorithmName();
            QVariantMap parameters;
            if (existing_visu) {
                parameters = d->visualizationCommands[form_type]->visualizationParameters();
                // Create a new visualization instance to avoid changing the form of the current one that can be shared
                d->visualizationCommands[form_type]->newVisualization();
            }
            // Update the command with parameter values (name is identical and will have no effect)
            d->setFormVisualization(form_type, visu_name, parameters);
        }
        emit formAdded(form_type);
    } else {
        // TODO: restore the adaption mechanism
        // this->setAdaptedForm(form_name, form);
        emit badFormDropped(form->formName(), acceptedForms().join(", "));
    }
}

std::shared_ptr<gnomonAbstractDynamicForm> gnomonAbstractView::form(const QString& form_type)
{
    if (d->forms.contains(form_type)) {
        return d->forms[form_type];
    } else {
        return nullptr;
    }
}

void gnomonAbstractView::removeForm(const QString& form_type)
{
    if (d->forms.contains(form_type)) {
        QString visu_name = d->visualizationCommands[form_type]->algorithmName();
        d->viewParameters.parameters.remove(visu_name);
        d->visualizationCommands[form_type]->clear();
        d->forms.remove(form_type);
        d->viewParameters.visuSelected.remove(form_type);
        emit formRemoved(form_type);
        emit formsChanged();
    }
    this->render();
}

QStringList gnomonAbstractView::nodePortNames(void)
{
    return d->nodePortNames;
}

void gnomonAbstractView::setNodePortNames(const QStringList& names)
{
    d->nodePortNames = names;
}

void gnomonAbstractView::update(void)
{
    for (const auto& form_type :  d->forms.keys()) {
        d->visualizationCommands[form_type]->update();
    }
}

void gnomonAbstractView::clear(void)
{
    for (const auto & form_type : d->forms.keys()) {
        QString visu_name = d->visualizationCommands[form_type]->algorithmName();
        d->viewParameters.parameters.remove(visu_name);
        d->visualizationCommands[form_type]->clear();
        d->visualizationCommands[form_type]->setForm(nullptr);
    }
    d->viewParameters.visuSelected.clear();

    d->forms.clear();
    emit formsChanged();
}

void gnomonAbstractView::drop(int index, bool new_visu)
{
    std::shared_ptr<gnomonAbstractDynamicForm> form = gnomonFormManager::instance()->get(index);
    std::shared_ptr<gnomonAbstractVisualization> visu = gnomonFormManager::instance()->getVisualization(index);
    if (new_visu) {
        visu = nullptr;
    }
    this->setForm("formManager", form, visu);
    this->render();
    gnomonFormManager::instance()->setFormDropped(form);
}

void gnomonAbstractView::transmit(void)
{
    d->exportToManager();
}

void gnomonAbstractView::transmitForm(const QString& form_type)
{
    if (d->forms.contains(form_type)) {
        QImage image;
        std::shared_ptr<gnomonAbstractVisualization> visualization = nullptr;
        if (d->visualizationCommands.contains(form_type)) {
            image = d->visualizationCommands[form_type]->visualization()->imageRendering();
            visualization = d->visualizationCommands[form_type]->visualization();
        } else {
            image = QImage(1500, 1500, QImage::Format_RGB32);
            image.fill(Qt::GlobalColor::black);
        }
        gnomonFormManager::instance()->addForm(d->forms[form_type], image, visualization);
        emit exportedForm(d->forms[form_type]);
    }
}

void gnomonAbstractView::restoreState(void)
{
    if (!this->empty()) {
        for (const auto &form_type :  d->forms.keys()) {
            // Ensure tranfer of actors of shared visualizations in the current view
            d->visualizationCommands[form_type]->setView(this);
            d->visualizationCommands[form_type]->setVisualizationParameters(d->viewParameters.parameters[form_type]);
        }
        // Force the renderer to update after new actors are added
        this->render();
    }
}

void gnomonAbstractView::setAcceptForm(const QString& form_type, bool accept)
{
    if(d->acceptForms.contains(form_type)) {
        d->acceptForms[form_type] = accept;
    }
}

void gnomonAbstractView::setInputView(bool input)
{
    if (input != d->input_view) {
        d->input_view = input;
        emit inputViewChanged(input);
    }
}

QStringList gnomonAbstractView::formNames()
{
    return d->forms.keys();
}

QStringList gnomonAbstractView::formNamesAndId(void)
{
    QStringList formNamesAndIndex;
    auto it = d->forms.constBegin();
    while (it != d->forms.constEnd()) {
        int id =  it.value()->thumbnailId();
        formNamesAndIndex.append(it.key() + "," + QString::number(id));
        ++it;
    }
    return formNamesAndIndex;
}

QStringList gnomonAbstractView::acceptedForms()
{
    QStringList forms;
    for(const auto& form_type : d->acceptForms.keys()){
        if(d->acceptForms[form_type])
            forms << form_type;
    }
    return forms;
}

bool gnomonAbstractView::inputView()
{
    return d->input_view;
}

bool gnomonAbstractView::contains(const QString& form_type) {
    return d->forms.contains(form_type);
}

bool gnomonAbstractView::empty(void) {
    return d->forms.empty();
}

QList<double> gnomonAbstractView::times(void)
{
    QList<double> sorted_times = QList<double>(d->forms_times.begin(), d->forms_times.end());
    std::sort(sorted_times.begin(), sorted_times.end());

    return sorted_times;
}

double gnomonAbstractView::currentTime(void) const
{
    return d->current_time;
}

double gnomonAbstractView::timeMax(void)
{
    QList<double> times = this->times();
    if (times.isEmpty()) {
        return -1;
    }  else {
        return this->times().last();
    }
}

void gnomonAbstractView::setCurrentTime(double time)
{
    QList<double> sorted_times = this->times();
    if (d->current_time != time) {
        d->current_time = time;
        emit timeChanged(d->current_time);
    }
}

QString gnomonAbstractView::formVisuName(const QString& form_type)
{
    QString visu_name;
    if (d->forms.contains(form_type)) {
        visu_name =  d->visualizationCommands[form_type]->algorithmName();
    }
    return visu_name;
}

void gnomonAbstractView::setFormVisuName(const QString& form_type, const QString& visu_name)
{
    if (d->forms.contains(form_type)) {
        auto current_visu_name = d->visualizationCommands[form_type]->algorithmName();
        if (visu_name != current_visu_name && !visu_name.isEmpty()) {
            if (d->viewParameters.parameters.contains(visu_name)) {
                // TODO: setting the parameters does not work so ignoring it for now
                // d->setFormVisualization(form_type, visu_name, d->viewParameters.parameters[visu_name]);
                d->setFormVisualization(form_type, visu_name);
            } else {
                d->setFormVisualization(form_type, visu_name);
            }
        }
    }
}

QVariantList gnomonAbstractView::formVisualizations(const QString& form_type)
{
    if (d->forms.contains(form_type)) {
        return d->visualizationCommands[form_type]->pluginFactory()->dataList();
    }
    return {};
}

gnomonDynamicFormMetadata* gnomonAbstractView::formMetadata(const QString &form_type)
{
    if (d->forms.contains(form_type)) {
        gnomonDynamicFormMetadata *ptr = d->forms[form_type]->metadata();
        QQmlEngine::setObjectOwnership(ptr, QQmlEngine::CppOwnership);
        return ptr;
    } else {
        return nullptr;
    }
}

QJSValue gnomonAbstractView::formVisuParameters(const QString& form_type)
{
    if (d->forms.contains(form_type)) {
        QJSValue parameters = dtkCoreParameterCollection(d->visualizationCommands[form_type]->parameters()).toJSValue(this->parent());
        QMap<QString, QString> parameter_groups =  d->visualizationCommands[form_type]->parameterGroups();

        QJSValueIterator it(parameters);
        while (it.hasNext()) {
            it.next();
            QString group = parameter_groups.contains(it.name()) ? parameter_groups[it.name()] : "";
            it.value().setProperty("group", group != "" ? group : nullptr);
        }

        return parameters;
    } else {
        return QJSValue();
    }
}

QVariant gnomonAbstractView::formVisuParameter(const QString& form_type, const QString& parameter_name)
{
    if (d->forms.contains(form_type)) {
        auto params = d->visualizationCommands[form_type]->parameters();
        if (params.keys().contains(parameter_name)) {
            dtkCoreParameter *param = params.value(parameter_name);
            // TODO: More specific cases to handle?
            if (auto string_param = dynamic_cast<dtkCoreParameterSimple<QString> *>(param))
            {
                return QVariant(string_param->value());
            } else {
                return param->variant();
            }
        } else {
            return QVariant();
        }
    } else {
        return QVariant();
    }
}

void gnomonAbstractView::setFormVisuParameter(const QString& form_type, const QString& parameter_name, const QVariant& value)
{
    if (d->forms.contains(form_type)) {
        d->visualizationCommands[form_type]->setParameter(parameter_name, value);
    }
}

void gnomonAbstractView::setFormVisible(const QString& form_type, bool visible)
{
    if (d->forms.contains(form_type)) {
        d->visualizationCommands[form_type]->setVisible(visible);
    }
    this->render();
}

void gnomonAbstractView::notifyFormSelected(int index, QString form_type) {
    d->viewParameters.currentFormIndex = index;
    d->viewParameters.currentFormType = std::move(form_type);
}

int gnomonAbstractView::lastFormIndexSelected() {
    return d->viewParameters.currentFormIndex;
}

QString gnomonAbstractView::lastFromTypeSelected() {
    return d->viewParameters.currentFormType;
}

QString gnomonAbstractView::lastVisuSelected(QString form_type) {
    return d->viewParameters.visuSelected[form_type];
}
