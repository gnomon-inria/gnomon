#include <gnomonCore>

#include <gnomonVisualization/gnomonManager/gnomonFormManager>

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
        QImage image(1500, 1500, QImage::Format_RGB32);
        image.fill(Qt::GlobalColor::black);

        gnomonFormManager::instance()->addForm(this->forms[key],image);
        q->emit exportedForm(this->forms[key]);
    }
}

void gnomonAbstractViewPrivate::removeForm(const QString& key)
{
    this->forms.remove(key);
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

void gnomonAbstractView::setForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    QString form_name = form->formName();
    if(d->acceptForms[form_name]) {
        d->forms[form_name] = form;
        emit formAdded(form_name);
    }
}

std::shared_ptr<gnomonAbstractDynamicForm> gnomonAbstractView::form(const QString& name)
{
    return d->forms[name];
}

void gnomonAbstractView::clearForm(const QString& name)
{
    return d->removeForm(name);
}

void gnomonAbstractView::drop(int index)
{
    std::shared_ptr<gnomonAbstractDynamicForm> form = gnomonFormManager::instance()->get(index);
    this->setForm("formManager", form);
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