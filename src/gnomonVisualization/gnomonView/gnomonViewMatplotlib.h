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

#include <QtWidgets>

class dtkWidgetsMenu;

class gnomonAbstractDynamicForm;
class gnomonAbstractMatplotlibVisualization;

class GNOMONVISUALIZATION_EXPORT gnomonViewMatplotlib : public QFrame
{
    Q_OBJECT

public:
     gnomonViewMatplotlib(QWidget *parent = nullptr);
    ~gnomonViewMatplotlib(void);

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
    void setInputView(bool);

public:
    void updateVisualizations(void);

public:
    void addWidget(QWidget *);

public:
    static gnomonViewMatplotlib *downcast(QWidget *widget)
    {
        return dynamic_cast<gnomonViewMatplotlib *>(widget);
    }

public:
    int figureNumber(void);

public:
    dtkWidgetsMenu *menu(void);

signals:
    void formAdded(const QString&);
    void formRemoved(const QString&);

signals:
    void exportedForm(gnomonAbstractDynamicForm *);

protected:
    virtual void showEvent(QShowEvent *event) override;

protected:
    virtual void dragEnterEvent(QDragEnterEvent *) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *) override;
    virtual void dragMoveEvent(QDragMoveEvent *) override;
    virtual void dropEvent(QDropEvent *) override;

signals:
    void fileDropped(const QString&);

private:
    class gnomonViewMatplotlibPrivate *d;
};

//
// gnomonViewMatplotlib.h ends here
