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

#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>

class gnomonAbstractDynamicForm;
class gnomonAbstractMatplotlibVisualization;

class GNOMONVISUALIZATION_EXPORT gnomonViewMatplotlib  : public QObject
{
    Q_OBJECT

public:
     gnomonViewMatplotlib(QObject *parent = nullptr);
    ~gnomonViewMatplotlib(void);

public:
    Q_PROPERTY(int figureNumber READ figureNumber WRITE setFigureNumber NOTIFY figureNumberChanged);
    Q_PROPERTY(QStringList formNames READ formNames NOTIFY formsChanged);
    Q_PROPERTY(QStringList acceptedForms READ acceptedForms);
    Q_PROPERTY(bool inputView READ inputView WRITE setInputView);

public:
    void setForm(const QString&, gnomonAbstractDynamicForm *, gnomonAbstractMatplotlibVisualization *  = nullptr);
    gnomonAbstractDynamicForm *form (const QString&);
    void clearForm(const QString&);

public:
    void setAdaptedForm(const QString&, gnomonAbstractDynamicForm *, gnomonAbstractMatplotlibVisualization * = nullptr);

public slots:
    void setAcceptForm(const QString&, bool);

public:
    void setIsModifiedForm(const QString&);

public slots:
    void setFigureNumber(int num);
    void setInputView(bool);

public:
    int figureNumber(void);
    QStringList formNames(void);
    QStringList acceptedForms(void);
    bool inputView(void);

signals:
    void formsChanged(void);

public:
    void updateVisualizations(void);


signals:
    void formAdded(const QString&);
    void formRemoved(const QString&);
    void figureNumberChanged(int);

signals:
    void exportedForm(gnomonAbstractDynamicForm *);

private:
    class gnomonViewMatplotlibPrivate *d;
};

//
// gnomonViewMatplotlib.h ends here
