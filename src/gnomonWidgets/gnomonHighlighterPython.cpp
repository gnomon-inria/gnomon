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

#include "gnomonHighlighterPython.h"


#include <dtkCore>
#include <dtkMacs>
#include <dtkThemes>
#include <dtkWidgets>


// ///////////////////////////////////////////////////////////////////
// gnomonHighlighterPythonPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonHighlighterPythonPrivate
{
public:
    void initalize(void);

public:
    QVector<dtkMacsHighlightRule> highlight_rules;
    QHash<QString, QColor> highlight_colors;

public:
    QRegularExpression func_pattern;
//    QRegularExpression type_pattern;
    QRegularExpression comment_pattern;
    QRegularExpression decorator_pattern;
    QRegularExpression cstt_pattern;
    QRegularExpression cend_pattern;

public:
    gnomonHighlighterPython *q;
};

void gnomonHighlighterPythonPrivate::initalize(void)
{
    q->load(":gnomon/gnomonHighlighterPython.xml");

    auto keys = q->keys();

    for (auto&& key : keys) {

        auto names = q->names(key);

        for (auto&& name : names)
            this->highlight_rules.append({QRegularExpression(QString(R"(\b%1\b)").arg(name)), key});
    }

    this->highlight_colors["Keyword"] = dtkThemesEngine::instance()->color("@orange");
    this->highlight_colors["SpecialKeyword"] = dtkThemesEngine::instance()->color("@red");
    this->highlight_colors["ClassMethod"] = dtkThemesEngine::instance()->color("@cyan");
    this->highlight_colors["PrimitiveType"] = dtkThemesEngine::instance()->color("@violet");

}

// ///////////////////////////////////////////////////////////////////
// gnomonHighlighterPython
// ///////////////////////////////////////////////////////////////////

gnomonHighlighterPython::gnomonHighlighterPython(QObject *parent) : dtkMacsHighlighter(parent)
{
    d = new gnomonHighlighterPythonPrivate;
    d->q = this;

    d->func_pattern = QRegularExpression(R"(\bdef\b\s*(\w+))");
    d->comment_pattern = QRegularExpression(R"(#[^\n]*)");
    d->cstt_pattern = QRegularExpression(R"(\"\"\")");
    d->cend_pattern = QRegularExpression(R"(\"\"\")");
    d->decorator_pattern = QRegularExpression(R"(@[^@]+)");

    d->highlight_rules.append({QRegularExpression(R"(\b(0b|0x){0,1}[\d.']+\b)"), "Number"});
    d->highlight_colors["Number"] = dtkThemesEngine::instance()->color("@blue");
    d->highlight_rules.append({QRegularExpression(R"(\"[^\"\\]*(\\.[^\"\\]*)*\")"), "String"});
    d->highlight_colors["String"] = dtkThemesEngine::instance()->color("@green");
    d->highlight_rules.append({QRegularExpression(R"('[^'\\]*(\\.[^'\\]*)*')"), "Single-quoted String"});
    d->highlight_colors["Single-quoted String"] = dtkThemesEngine::instance()->color("@green");

    d->initalize();
}

gnomonHighlighterPython::~gnomonHighlighterPython(void)
{
    delete d;
}

void gnomonHighlighterPython::rehighlight(void)
{
    QSyntaxHighlighter::rehighlight();
}

void gnomonHighlighterPython::rehighlightBlock(const QTextBlock& block)
{

}

void gnomonHighlighterPython::highlightBlock(const QString& text)
{
    { // decorators

        auto matchIterator = d->decorator_pattern.globalMatch(text);

        while (matchIterator.hasNext())
        {
            auto match = matchIterator.next();

            setFormat(
                match.capturedStart(),
                match.capturedLength(),
                dtkThemesEngine::instance()->color("@magenta")
            );
        }
    }

    { // functions

        auto matchIterator = d->func_pattern.globalMatch(text);

        while (matchIterator.hasNext())
        {
            auto match = matchIterator.next();

            setFormat(
                match.capturedStart(1),
                match.capturedLength(1),
                dtkThemesEngine::instance()->color("@yellow")
            );
        }
    }

    for (auto& rule : d->highlight_rules)
    {
        auto matchIterator = rule.pattern.globalMatch(text);
        auto format = rule.format;

        while (matchIterator.hasNext())
        {
            auto match = matchIterator.next();

            this->setFormat(
                match.capturedStart(),
                match.capturedLength(),
                d->highlight_colors[format]
            );
        }
    }


    { // comments

        auto matchIterator = d->comment_pattern.globalMatch(text);

        while (matchIterator.hasNext())
        {
            auto match = matchIterator.next();

            setFormat(
                match.capturedStart(),
                match.capturedLength(),
                dtkThemesEngine::instance()->color("@base6")
            );
        }
    }
    
    setCurrentBlockState(0);

    int startIndex = 0;

    if (previousBlockState() != 1)
    {
        startIndex = text.indexOf(d->cstt_pattern);
    }

    while (startIndex >= 0)
    {
        auto match = d->cend_pattern.match(text, startIndex+1);

        int endIndex = match.capturedStart();
        int commentLength = 0;

        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + match.capturedLength();
        }

        setFormat(
            startIndex,
            commentLength,
            dtkThemesEngine::instance()->color("@green")
        );

        startIndex = text.indexOf(d->cstt_pattern, startIndex + commentLength);
    }
}


//
// gnomonHighlighterPython.cpp ends here
