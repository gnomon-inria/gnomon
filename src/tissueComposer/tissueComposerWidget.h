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

#include <tissueComposerExport.h>

class dtkComposerSceneNode;
class dtkComposerWidget;

class tissueComposerWidgetPrivate;

class TISSUECOMPOSER_EXPORT tissueComposerWidget : public QFrame
{
    Q_OBJECT

public:
     tissueComposerWidget(QWidget *parent = 0);
    ~tissueComposerWidget(void);

public:
    dtkComposerWidget* composerWidget(void);

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
    void closeEvent(QCloseEvent *event);

private:
    tissueComposerWidgetPrivate *d;
};

//
// tissueComposerWidget.h ends here
