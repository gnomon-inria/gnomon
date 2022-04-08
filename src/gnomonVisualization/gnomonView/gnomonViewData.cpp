#include "gnomonViewData.h"

#include <gnomonVisualization/gnomonManager/gnomonFormManager>


QString transformMatrixString(QVector<QVector<double> > transform_matrix)
{
    QString matrix_string;

    matrix_string += "[";
    for (int row=0; row<transform_matrix.size(); row++) {
        if (row > 0) matrix_string += "\n ";
        matrix_string += " [";
        for (int col=0; col<transform_matrix[row].size(); col++) {
            if (col > 0) matrix_string += ",";
            if (transform_matrix[row][col]>=0) matrix_string += " ";
            matrix_string += " " + QString::number(transform_matrix[row][col], 'f', 3);
        }
        matrix_string += "]";
    }
    matrix_string += " ]";

    return matrix_string;
}

QVector<QVector<double> > identity_matrix = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };


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
    QString data_dict;
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
        value = qRgb(60, 110, 180);
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
    qDebug()<<Q_FUNC_INFO<<d->acceptForms["gnomonDataDict"]<<dynamic_cast<gnomonDataDictSeries *>(form);

    if(gnomonDataDictSeries *dict = dynamic_cast<gnomonDataDictSeries *>(form)) {
        if(d->acceptForms["gnomonDataDict"]) {
            d->forms["gnomonDataDict"] = dict;

            gnomonDataDictSeries *transformation = dynamic_cast<gnomonDataDictSeries *>(dict->clone());
            if (dict->current()->keys().contains("transform")) {
                QVariant transform = transformation->current()->get("transform");
                QVector<QVector<double>> transform_matrix = transform.value<QVector<QVector<double> > >();
                this->setDataDict(transformMatrixString(transform_matrix));
            }

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

void gnomonViewData::drop(int index)
{
    gnomonAbstractDynamicForm *form = gnomonFormManager::instance()->get(index);
    qDebug()<<Q_FUNC_INFO<<form<<dynamic_cast<gnomonDataDictSeries *>(form);
    this->setForm("formManager", form);
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
    if (input != d->input_view) {
        d->input_view = input;
        emit inputViewChanged(input);
    }
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
    return forms;
}

bool gnomonViewData::inputView()
{
    return d->input_view;
}

QString gnomonViewData::dataDict(void)
{
    return d->data_dict;
}

void gnomonViewData::setDataDict(QString dict)
{
    d->data_dict = dict;
    emit dictChanged();
}
// ///////////////////////////////////////////////////////////////////

#include "gnomonViewData.moc"

//
// gnomonViewData.cpp ends here