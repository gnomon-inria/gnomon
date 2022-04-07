#include "gnomonViewData.h"

#include "gnomonManager/gnomonFormManager.h"

// ///////////////////////////////////////////////////////////////////
// gnomonViewDataPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonViewDataPrivate : public QObject
{
    Q_OBJECT

public:
    gnomonViewDataPrivate(QObject *parent = Q_NULLPTR);
    ~gnomonViewDataPrivate(void);

public:
    gnomonViewData *q = nullptr;

public:
    QColor export_color = QColor("#cccccc");

public slots:
    void exportToManager(void);
    void removeForm(const QString& key);
public: 
    QMap<QString, gnomonAbstractDynamicForm *> forms;

public:
    QMap<QString, bool> acceptForms;

public:
    bool input_view = false;
};

gnomonViewDataPrivate::gnomonViewDataPrivate(QObject *parent): QObject(parent)
{
}
gnomonViewDataPrivate::~gnomonViewDataPrivate(void)
{
}

void gnomonViewDataPrivate::exportToManager(void)
{
    for(const auto& key: this->forms.keys()) {
        QImage image(1, 1, QImage::Format_Indexed8);
        QRgb value;

        value = qRgb(122, 163, 39);
        image.setColor(0, value);

        gnomonFormManager::instance()->addForm(this->forms[key],this->export_color,image);
        q->emit exportedForm(this->forms[key]);
    }
}

void gnomonViewDataPrivate::removeForm(const QString& key)
{
    this->forms.remove(key);
}

// ///////////////////////////////////////////////////////////////////
// gnomonViewData
// ///////////////////////////////////////////////////////////////////

gnomonViewData::gnomonViewData(QObject *parent): QObject(parent)
{
    d = new gnomonViewDataPrivate(this);
    d->q  = this;

    d->acceptForms["gnomonDataDict"] = false;

    connect(this, &gnomonViewData::formAdded, [=]() {
        // this->render();
        emit formsChanged();
    });
}

gnomonViewData::~gnomonViewData(void)
{
    delete d;
}

void gnomonViewData::setForm(const QString& name, gnomonAbstractDynamicForm *form)
{
    if(gnomonDataDictSeries *dict = dynamic_cast<gnomonDataDictSeries *>(form)) {
        if(d->acceptForms["gnomonDataDict"]) {
            d->forms["gnomonDataDict"] = dict;
            emit formAdded("gnomonDataDict");
        }       
    }    
}

gnomonAbstractDynamicForm* gnomonViewData::form(const QString& name)
{
    return d->forms[name];
}

void gnomonViewData::clearForm(const QString& name)
{
    return d->removeForm(name);
}
void gnomonViewData::transmit(void)
{
    d->exportToManager();
}

void gnomonViewData::setAcceptForm(const QString& name, bool accept)
{
    if(d->acceptForms.contains(name)) {
        d->acceptForms[name] = accept;
    }
}

void gnomonViewData::setInputView(bool input)
{
    d->input_view = input;
}

QStringList gnomonViewData::formNames()
{
    return d->forms.keys();
}

QStringList gnomonViewData::acceptedForms()
{
    QStringList forms;
    for(const auto& name : d->acceptForms.keys()){
        if(d->acceptForms[name])
            forms << name; 
    }
}

bool gnomonViewData::inputView()
{
    return d->input_view;
}
// ///////////////////////////////////////////////////////////////////

#include "gnomonViewData.moc"

//
// gnomonViewData.cpp ends here