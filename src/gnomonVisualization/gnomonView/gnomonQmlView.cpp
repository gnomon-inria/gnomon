#include "gnomonQmlView.h"
#include "gnomonAbstractView_p.h"

#include <gnomonVisualization/gnomonManager/gnomonFormManager>

#include <gnomonCommand/gnomonAbstractQmlVisualizationCommand>
#include <gnomonCommand/gnomonDataDict/gnomonDataDictQmlVisualizationCommand>
#include <gnomonCommand/gnomonLString/gnomonLStringQmlVisualizationCommand>

QString transformMatrixString(QVector<QVector<double> > transform_matrix)
{
    QString matrix_string;

    matrix_string += "[";
    for (int row=0; row<transform_matrix.size(); row++) {
        if (row > 0) matrix_string += ",\n ";
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
// gnomonQmlViewPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonQmlViewPrivate : public QObject
{
    Q_OBJECT

public:
    gnomonQmlViewPrivate(QObject *parent = Q_NULLPTR);
    ~gnomonQmlViewPrivate(void);

public:
    QString display_text;
    int font_size;
};

gnomonQmlViewPrivate::gnomonQmlViewPrivate(QObject *parent): QObject(parent)
{
}

gnomonQmlViewPrivate::~gnomonQmlViewPrivate(void)
{
}

// ///////////////////////////////////////////////////////////////////
// gnomonQmlView
// ///////////////////////////////////////////////////////////////////

gnomonQmlView::gnomonQmlView(QObject *parent): gnomonAbstractView(parent)
{
    dd = new gnomonQmlViewPrivate(this);
    d->q  = this;

    d->visualizationCommands["gnomonLString"] = new gnomonLStringQmlVisualizationCommand;
    d->visualizationCommands["gnomonDataDict"] = new gnomonDataDictQmlVisualizationCommand;

    for (const auto &form_type: d->visualizationCommands.keys()) {
        d->visualizationCommands[form_type]->setView(this);
        connect(d->visualizationCommands[form_type], &gnomonAbstractVisualizationCommand::visuParametersChanged, [=] () {
            emit formVisuParametersChanged();
        });
        d->acceptForms[form_type] = false;
    }
    d->acceptForms["gnomonDataDict"] = false;
}

gnomonQmlView::~gnomonQmlView(void)
{
    delete dd;
}

const QString& gnomonQmlView::displayText(void)
{
    return dd->display_text;
}

void gnomonQmlView::setDisplayText(const QString& text)
{
    dd->display_text = text;
    emit displayTextChanged();
}

int gnomonQmlView::fontSize(void)
{
    return dd->font_size;
}

void gnomonQmlView::setFontSize(int size)
{
    if (size != dd->font_size && size >= 0) {
        dd->font_size = size;
        emit fontSizeChanged();
    }
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonQmlView.moc"

//
// gnomonQmlView.cpp ends here