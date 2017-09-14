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

#include <dtkComposer/dtkComposerWriter.h>

#include <QtWidgets>

class dtkComposerSceneNode;

class tissueGrowthSimulatorComposerWidgetPrivate;

class tissueGrowthSimulatorComposerWidget : public QFrame
{
    Q_OBJECT

public:
     tissueGrowthSimulatorComposerWidget(QWidget *parent = 0);
    ~tissueGrowthSimulatorComposerWidget(void);

public slots:
    bool compositionOpen(void);
    bool compositionOpen(const QString& file);
    bool compositionSave(void);
    bool compositionSaveAs(void);
    bool compositionSaveAs(const QString& file, dtkComposerWriter::Type type = dtkComposerWriter::Ascii);
    bool compositionInsert(void);
    bool compositionInsert(const QString& file);

protected slots:
    void showControls(void);
    void onComposerNodeFlagged(dtkComposerSceneNode *);

protected:
    //void closeEvent(QCloseEvent *event);

private:
    tissueGrowthSimulatorComposerWidgetPrivate *d;
};

//
// tissueGrowthSimulatorComposerWidget.h ends here
