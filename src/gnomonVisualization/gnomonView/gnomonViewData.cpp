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
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > forms;

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
        QImage image(1500, 1500, QImage::Format_RGB32);
        QRgb value = qRgb(168, 168, 168);
        for(int w=0; w<image.width(); w++) {
            for(int h=0; h<image.height(); h++) {
                image.setPixel(w,h,value);
            }
        }

        int n_lines = 1;
        QString dict_string = " {";
        if (this->forms.contains("gnomonDataDict")) {
            auto dict = std::dynamic_pointer_cast<gnomonDataDictSeries>(this->forms["gnomonDataDict"]);
            if (dict->current()->keys().size() > 0) {
                dict_string += "\n";
                n_lines += 1;
            }
            for (const auto& key : dict->current()->keys()) {
                dict_string += "     " + key + "\n";
                n_lines += 1;
            }
        }
        dict_string += " }";
        int font_size = image.height() / (2.5*(n_lines+1));

        QPainter p;
        p.begin(&image);
        p.setPen(QPen(Qt::white));
        p.setFont(QFont("Times", font_size, QFont::Bold));
        p.drawText(image.rect(), Qt::AlignLeft|Qt::AlignVCenter, dict_string);
        p.end();

        gnomonFormManager::instance()->addForm(this->forms[key],image);
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
        emit formsChanged();
    });
}

gnomonViewData::~gnomonViewData(void)
{
    delete d;
}

void gnomonViewData::setForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    if(std::shared_ptr<gnomonDataDictSeries> dict = std::dynamic_pointer_cast<gnomonDataDictSeries>(form)) {
        if(d->acceptForms["gnomonDataDict"]) {
            d->forms["gnomonDataDict"] = dict;
            if (dict->current()->keys().contains("transform")) {
                QVariant transform = dict->current()->get("transform");
                QVector<QVector<double>> transform_matrix = transform.value<QVector<QVector<double> > >();
                this->setDataDict(transformMatrixString(transform_matrix));
            }

            emit formAdded("gnomonDataDict");
        }       
    }    
}

std::shared_ptr<gnomonAbstractDynamicForm> gnomonViewData::form(const QString& name)
{
    return d->forms[name];
}

void gnomonViewData::clearForm(const QString& name)
{
    return d->removeForm(name);
}

void gnomonViewData::drop(int index)
{
    std::shared_ptr<gnomonAbstractDynamicForm> form = gnomonFormManager::instance()->get(index);
    this->setForm("formManager", form);
    gnomonFormManager::instance()->setFormDropped(form);
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

bool gnomonViewData::contains(const QString& key) {
    return d->forms.contains(key);
}

bool gnomonViewData::empty() {
    return d->forms.empty();
}
// ///////////////////////////////////////////////////////////////////

#include "gnomonViewData.moc"

//
// gnomonViewData.cpp ends here