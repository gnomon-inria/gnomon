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

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <dtkCore>
#include <dtkWidgets>

#include <gnomonVisualizationExport.h>

class gnomonViewManager;
class gnomonViewPrivate;

class gnomonInspector;

class gnomonActor;

class GNOMONVISUALIZATION_EXPORT gnomonView : public dtkViewWidget
{
    Q_OBJECT

public:
     gnomonView(QWidget *parent = Q_NULLPTR);
    ~gnomonView(void);

signals:
    void focused(void);

public:
    gnomonViewManager *manager(void);

public:
    QWidget *widget(void) override;
    QWidget *inspector(void) override;

protected:
    void mousePressEvent(QMouseEvent *) override;

 private slots:
    void onInserted(gnomonActor *);

private:
    gnomonViewPrivate *d;
};

DTK_DECLARE_OBJECT(gnomonView *)

//
// gnomonView.h ends here
