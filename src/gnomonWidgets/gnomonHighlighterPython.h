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

#include <dtkMacsHighlighter.h>


class GNOMONWIDGETS_EXPORT gnomonHighlighterPython : public dtkMacsHighlighter
{
    Q_OBJECT

public:
     gnomonHighlighterPython(QObject *parent = 0);
    ~gnomonHighlighterPython(void);

public slots:
    void rehighlight(void) override;
    void rehighlightBlock(const QTextBlock&) override;

protected:
    void highlightBlock(const QString& text) override;

private:
    class gnomonHighlighterPythonPrivate *d;

private:
    friend class gnomonHighlighterPythonPrivate;
};

//
// gnomonHighlighterPython.h ends here
