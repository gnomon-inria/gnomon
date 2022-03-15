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

#include <gnomonWidgetsExport>

#include <dtkWidgets>

class GNOMONWIDGETS_EXPORT gnomonPythonEditor : public QWidget
{
    Q_OBJECT

public:
     gnomonPythonEditor(QWidget *parent = Q_NULLPTR);
    ~gnomonPythonEditor(void);

protected:
    void resizeEvent(QResizeEvent *);

public:
    QString text(void);
    void setText(const QString& text);

signals:
    void openButtonClicked(void);
    void saveButtonClicked(void);
    void loadButtonClicked(void);

public:
    QSize sizeHint(void) const;

protected:
    class gnomonPythonEditorPrivate *d;
};

//
// gnomonWorkspacePythonAlgorithm.h ends here
