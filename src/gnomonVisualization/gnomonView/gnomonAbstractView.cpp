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
}

gnomonAbstractViewPrivate::~gnomonAbstractViewPrivate(void)
{
}

void gnomonAbstractViewPrivate::exportToManager(void)
{
    for(const auto& key: this->forms.keys()) {
        QImage image = this->visualizationCommands[key]->visualization()->imageRendering();
        gnomonFormManager::instance()->addForm(this->forms[key], image, this->visualizationCommands[key]->visualization());
        q->emit exportedForm(this->forms[key]);
    }
}

// ///////////////////////////////////////////////////////////////////
// gnomonAbstractView
// ///////////////////////////////////////////////////////////////////


gnomonAbstractView::gnomonAbstractView(QObject *parent): QObject(parent)
{
    d = new gnomonAbstractViewPrivate(this);
    d->q  = this;

    connect(this, &gnomonAbstractView::formAdded, [=]() {
        emit formsChanged();
    });
}

gnomonAbstractView::~gnomonAbstractView(void)
{
    delete d;
}

void gnomonAbstractView::setForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form, std::shared_ptr<gnomonAbstractVisualization> visu)
{
    QString form_name = form->formName();
    if(d->acceptForms[form_name]) {
        d->forms[form_name] = form;
        emit formAdded(form_name);
    }
}

std::shared_ptr<gnomonAbstractDynamicForm> gnomonAbstractView::form(const QString& name)
{
    if (d->forms.contains(name)) {
        return d->forms[name];
    } else {
        return nullptr;
    }
}

void gnomonAbstractView::removeForm(const QString& name)
{
    if (d->forms.contains(name)) {
        d->forms.remove(name);
        emit formRemoved(name);
        emit formsChanged();
    }
}

QStringList gnomonAbstractView::nodePortNames(void)
{
    return d->nodePortNames;
}

void gnomonAbstractView::setNodePortNames(const QStringList& names)
{
    d->nodePortNames = names;
}

void gnomonAbstractView::clear(void)
{
    d->forms.clear();
    emit formsChanged();
}

void gnomonAbstractView::drop(int index)
{
    std::shared_ptr<gnomonAbstractDynamicForm> form = gnomonFormManager::instance()->get(index);
    this->setForm("formManager", form, gnomonFormManager::instance()->getVisualization(index));
    this->render();
    gnomonFormManager::instance()->setFormDropped(form);
}

void gnomonAbstractView::transmit(void)
{
    d->exportToManager();
}

void gnomonAbstractView::setAcceptForm(const QString& name, bool accept)
{
    if(d->acceptForms.contains(name)) {
        d->acceptForms[name] = accept;
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
    for(const auto& name : d->acceptForms.keys()){
        if(d->acceptForms[name])
            forms << name;
    }
    return forms;
}

bool gnomonAbstractView::inputView()
{
    return d->input_view;
}

bool gnomonAbstractView::empty(void) {
    return d->forms.empty();
}

QString gnomonAbstractView::formVisuName(const QString& name)
{
    QString visu_name;
    if (d->forms.contains(name)) {
        visu_name =  d->visualizationCommands[name]->algorithmName();
    }
    return visu_name;
}

QVariantList gnomonAbstractView::formVisualizations(const QString& name)
{
    if (d->forms.contains(name)) {
        return d->visualizationCommands[name]->pluginFactory()->dataList();
    }
    return {};
}

void gnomonAbstractView::setFormVisible(const QString& name, bool visible)
{
    if (d->forms.contains(name)) {
        d->visualizationCommands[name]->setVisible(visible);
    }
    this->render();
}