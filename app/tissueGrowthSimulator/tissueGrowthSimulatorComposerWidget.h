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

class tissueGrowthSimulatorComposerWidget : public QMainWindow
{
    Q_OBJECT

public:
     tissueGrowthSimulatorComposerWidget(QWidget *parent = 0);
    ~tissueGrowthSimulatorComposerWidget(void);

    void readSettings(void);
    void writeSettings(void);

public slots:
    bool compositionOpen(void);
    bool compositionOpen(const QString& file);
    bool compositionSave(void);
    bool compositionSaveAs(void);
    bool compositionSaveAs(const QString& file, dtkComposerWriter::Type type = dtkComposerWriter::Ascii);
    bool compositionInsert(void);
    bool compositionInsert(const QString& file);

protected slots:
    void switchToCompo(void);
    void switchToDstrb(void);
    void switchToDebug(void);
    void switchToView(void);

protected slots:
    void showControls(void);

protected slots:
    void onComposerNodeFlagged(dtkComposerSceneNode *);

protected:
    void closeEvent(QCloseEvent *event);

private:
    tissueGrowthSimulatorComposerWidgetPrivate *d;
};

//
// tissueGrowthSimulatorComposerWidget.h ends here
