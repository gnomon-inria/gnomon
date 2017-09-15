#include "tissueGrowthSimulatorCodeEditor.h"
#include <set>

class tissueGrowthSimulatorCodeEditorPrivate
{
public:
    QWidget* line_number_area;
    tissueGrowthSimulatorCodeEditorSyntaxHighlighter* highlighter;
    std::set<QString> vocabulary;
    //QTimer* timer;
};

tissueGrowthSimulatorCodeEditor::tissueGrowthSimulatorCodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    d = new tissueGrowthSimulatorCodeEditorPrivate;

    d->line_number_area = new tissueGrowthSimulatorCodeEditorLineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    setFont(QFont("monospace"));

    d->highlighter = new tissueGrowthSimulatorCodeEditorSyntaxHighlighter(document());

    /*
    d->timer = new QTimer(this);
    d->timer->start(1000);
    connect(d->timer, SIGNAL(timeout()), this, SLOT(updateVocabulary()));
    */
}

tissueGrowthSimulatorCodeEditor::~tissueGrowthSimulatorCodeEditor()
{
    delete d;
}

/*
#include <iostream>
#include <string>
void tissueGrowthSimulatorCodeEditor::updateVocabulary()
{
    QRegularExpression re("([a-zA-Z_][a-zA-Z0-9_]+)");

    d->vocabulary.clear();

    QTextBlock block = document()->begin();
    while(block.isValid())
    {
        QRegularExpressionMatchIterator it = re.globalMatch(block.text());
        while(it.hasNext())
        {
            QRegularExpressionMatch match = it.next();

            d->vocabulary.insert( match.captured(1) );
        }

        block = block.next();
    }

    for(QString str : d->vocabulary) std::cout << str.toStdString() << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}
*/

int tissueGrowthSimulatorCodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());

    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    digits = qMax(3, digits);

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void tissueGrowthSimulatorCodeEditor::updateLineNumberAreaWidth(int)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void tissueGrowthSimulatorCodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy) {
        d->line_number_area->scroll(0, dy);
    }
    else {
        d->line_number_area->update(0, rect.y(), d->line_number_area->width(), rect.height());
    }

    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

void tissueGrowthSimulatorCodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    d->line_number_area->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void tissueGrowthSimulatorCodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void tissueGrowthSimulatorCodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(d->line_number_area);
    painter.fillRect(event->rect(), Qt::lightGray);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, d->line_number_area->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

class tissueGrowthSimulatorCodeEditorSyntaxHighlighterPrivate
{
public:
    // 're' suffix stands for Regular Expression.

    QRegularExpression comment_re;
    QRegularExpression word_re;
    QRegularExpression defclass_re;
    QRegularExpression number_re;

    QSet<QString> keywords;
    QSet<QString> builtins;
    QString braces;
    QSet<QString> operators;

    QTextCharFormat keyword_format;
    QTextCharFormat builtin_format;
    QTextCharFormat operator_format;
    QTextCharFormat comment_format;
    QTextCharFormat defclass_format;
    QTextCharFormat brace_format;
    QTextCharFormat number_format;
    QTextCharFormat string_format;
};

tissueGrowthSimulatorCodeEditorSyntaxHighlighter::tissueGrowthSimulatorCodeEditorSyntaxHighlighter(QTextDocument* document) : QSyntaxHighlighter(document)
{
    d = new tissueGrowthSimulatorCodeEditorSyntaxHighlighterPrivate;

    // Declare some useful regular expressions.

    d->comment_re = QRegularExpression("^[[:blank:]]*#");

    d->word_re = QRegularExpression("([a-zA-Z_][a-zA-Z0-9_]+)");

    d->defclass_re = QRegularExpression("^[[:blank:]]*(def|class)[[:blank:]]+([a-zA-Z_][a-zA-Z0-9_]+)");

    d->number_re = QRegularExpression("[+-]?([0-9]*[.])?[0-9]+");

    // Declare list of braces, operators, keywords, builtins, etc.

    d->braces = "{}[]()";

    // We assume that operator == appears before operator = and that operator ** appears before operator * and so on.

    d->operators << "===";
    d->operators << "==";
    d->operators << "+=";
    d->operators << "-=";
    d->operators << "<>";
    d->operators << ">>";
    d->operators << "<<";
    d->operators << "**";
    d->operators << "=";
    d->operators << ">";
    d->operators << "<";
    d->operators << "+";
    d->operators << "-";
    d->operators << "*";
    d->operators << "|";
    d->operators << "^";

    /*
    The list of keywords and the list of builtins were generated by the following python code.

        import keyword

        for k in keyword.kwlist:
            print("d->keywords << \"" + k + "\";")

        for fn in dir(__builtins__):
            print("d->builtins << \"" + fn + "\";")
    */
    
    d->keywords << "False";
    d->keywords << "None";
    d->keywords << "True";
    d->keywords << "and";
    d->keywords << "as";
    d->keywords << "assert";
    d->keywords << "break";
    d->keywords << "class";
    d->keywords << "continue";
    d->keywords << "def";
    d->keywords << "del";
    d->keywords << "elif";
    d->keywords << "else";
    d->keywords << "except";
    d->keywords << "finally";
    d->keywords << "for";
    d->keywords << "from";
    d->keywords << "global";
    d->keywords << "if";
    d->keywords << "import";
    d->keywords << "in";
    d->keywords << "is";
    d->keywords << "lambda";
    d->keywords << "nonlocal";
    d->keywords << "not";
    d->keywords << "or";
    d->keywords << "pass";
    d->keywords << "raise";
    d->keywords << "return";
    d->keywords << "try";
    d->keywords << "while";
    d->keywords << "with";
    d->keywords << "yield";

    d->builtins << "ArithmeticError";
    d->builtins << "AssertionError";
    d->builtins << "AttributeError";
    d->builtins << "BaseException";
    d->builtins << "BlockingIOError";
    d->builtins << "BrokenPipeError";
    d->builtins << "BufferError";
    d->builtins << "BytesWarning";
    d->builtins << "ChildProcessError";
    d->builtins << "ConnectionAbortedError";
    d->builtins << "ConnectionError";
    d->builtins << "ConnectionRefusedError";
    d->builtins << "ConnectionResetError";
    d->builtins << "DeprecationWarning";
    d->builtins << "EOFError";
    d->builtins << "Ellipsis";
    d->builtins << "EnvironmentError";
    d->builtins << "Exception";
    d->builtins << "False";
    d->builtins << "FileExistsError";
    d->builtins << "FileNotFoundError";
    d->builtins << "FloatingPointError";
    d->builtins << "FutureWarning";
    d->builtins << "GeneratorExit";
    d->builtins << "IOError";
    d->builtins << "ImportError";
    d->builtins << "ImportWarning";
    d->builtins << "IndentationError";
    d->builtins << "IndexError";
    d->builtins << "InterruptedError";
    d->builtins << "IsADirectoryError";
    d->builtins << "KeyError";
    d->builtins << "KeyboardInterrupt";
    d->builtins << "LookupError";
    d->builtins << "MemoryError";
    d->builtins << "ModuleNotFoundError";
    d->builtins << "NameError";
    d->builtins << "None";
    d->builtins << "NotADirectoryError";
    d->builtins << "NotImplemented";
    d->builtins << "NotImplementedError";
    d->builtins << "OSError";
    d->builtins << "OverflowError";
    d->builtins << "PendingDeprecationWarning";
    d->builtins << "PermissionError";
    d->builtins << "ProcessLookupError";
    d->builtins << "RecursionError";
    d->builtins << "ReferenceError";
    d->builtins << "ResourceWarning";
    d->builtins << "RuntimeError";
    d->builtins << "RuntimeWarning";
    d->builtins << "StopAsyncIteration";
    d->builtins << "StopIteration";
    d->builtins << "SyntaxError";
    d->builtins << "SyntaxWarning";
    d->builtins << "SystemError";
    d->builtins << "SystemExit";
    d->builtins << "TabError";
    d->builtins << "TimeoutError";
    d->builtins << "True";
    d->builtins << "TypeError";
    d->builtins << "UnboundLocalError";
    d->builtins << "UnicodeDecodeError";
    d->builtins << "UnicodeEncodeError";
    d->builtins << "UnicodeError";
    d->builtins << "UnicodeTranslateError";
    d->builtins << "UnicodeWarning";
    d->builtins << "UserWarning";
    d->builtins << "ValueError";
    d->builtins << "Warning";
    d->builtins << "ZeroDivisionError";
    d->builtins << "__build_class__";
    d->builtins << "__debug__";
    d->builtins << "__doc__";
    d->builtins << "__import__";
    d->builtins << "__loader__";
    d->builtins << "__name__";
    d->builtins << "__package__";
    d->builtins << "__spec__";
    d->builtins << "abs";
    d->builtins << "all";
    d->builtins << "any";
    d->builtins << "ascii";
    d->builtins << "bin";
    d->builtins << "bool";
    d->builtins << "bytearray";
    d->builtins << "bytes";
    d->builtins << "callable";
    d->builtins << "chr";
    d->builtins << "classmethod";
    d->builtins << "compile";
    d->builtins << "complex";
    d->builtins << "copyright";
    d->builtins << "credits";
    d->builtins << "delattr";
    d->builtins << "dict";
    d->builtins << "dir";
    d->builtins << "divmod";
    d->builtins << "enumerate";
    d->builtins << "eval";
    d->builtins << "exec";
    d->builtins << "exit";
    d->builtins << "filter";
    d->builtins << "float";
    d->builtins << "format";
    d->builtins << "frozenset";
    d->builtins << "getattr";
    d->builtins << "globals";
    d->builtins << "hasattr";
    d->builtins << "hash";
    d->builtins << "help";
    d->builtins << "hex";
    d->builtins << "id";
    d->builtins << "input";
    d->builtins << "int";
    d->builtins << "isinstance";
    d->builtins << "issubclass";
    d->builtins << "iter";
    d->builtins << "len";
    d->builtins << "license";
    d->builtins << "list";
    d->builtins << "locals";
    d->builtins << "map";
    d->builtins << "max";
    d->builtins << "memoryview";
    d->builtins << "min";
    d->builtins << "next";
    d->builtins << "object";
    d->builtins << "oct";
    d->builtins << "open";
    d->builtins << "ord";
    d->builtins << "pow";
    d->builtins << "print";
    d->builtins << "property";
    d->builtins << "quit";
    d->builtins << "range";
    d->builtins << "repr";
    d->builtins << "reversed";
    d->builtins << "round";
    d->builtins << "set";
    d->builtins << "setattr";
    d->builtins << "slice";
    d->builtins << "sorted";
    d->builtins << "staticmethod";
    d->builtins << "str";
    d->builtins << "sum";
    d->builtins << "super";
    d->builtins << "tuple";
    d->builtins << "type";
    d->builtins << "vars";
    d->builtins << "zip";

    // Declare how detected entities should be formatted.

    d->keyword_format.setForeground(Qt::blue);

    d->operator_format.setForeground(Qt::red);

    //d->builtin_format.setFontWeight(QFont::Bold);

    d->comment_format.setForeground(Qt::darkGreen);
    d->comment_format.setFontItalic(true);

    d->defclass_format.setFontWeight(QFont::Bold);

    d->brace_format.setForeground(Qt::darkGray);

    d->string_format.setForeground(Qt::magenta);

    d->number_format.setForeground(Qt::magenta);
}

tissueGrowthSimulatorCodeEditorSyntaxHighlighter::~tissueGrowthSimulatorCodeEditorSyntaxHighlighter()
{
    delete d;
}

void tissueGrowthSimulatorCodeEditorSyntaxHighlighter::highlightBlock(const QString& text)
{
    QRegularExpressionMatch match;
    QRegularExpressionMatchIterator it;

    // Highlight comments.

    match = d->comment_re.match(text);

    if(match.hasMatch())
    {
        setFormat(0, text.size(), d->comment_format);
    }
    else
    {
        // Highlight keywords and builtins.

        it = d->word_re.globalMatch(text);
        while(it.hasNext())
        {
            match = it.next();

            const int nth = 1;

            const QString captured = match.captured(nth);
            const int start = match.capturedStart(nth);
            const int length = match.capturedLength(nth);

            if(d->keywords.contains(captured))
            {
                setFormat(start, length, d->keyword_format);
            }
            else if(d->builtins.contains(captured))
            {
                setFormat(start, length, d->builtin_format);
            }
        }

        // Highlight names of functions and classes.

        it = d->defclass_re.globalMatch(text);
        while(it.hasNext())
        {
            match = it.next();

            const int nth = 2;

            const QString captured = match.captured(nth);
            const int start = match.capturedStart(nth);
            const int length = match.capturedLength(nth);

            setFormat(start, length, d->defclass_format);
        }

        // Highlight braces.

        for(int i = 0; i < text.size(); i++)
        {
            if(d->braces.contains(text.at(i)))
            {
                setFormat(i, 1, d->brace_format);
            }
        }

        // Highlight operators.

        for(QString oper : d->operators)
        {
            int j = 0;
            while( (j = text.indexOf(oper,j)) != -1 )
            {
                setFormat(j, oper.size(), d->operator_format);
                j += oper.size();
            }
        }

        // Highlight number litterals.

        it = d->number_re.globalMatch(text);
        while(it.hasNext())
        {
            match = it.next();

            const int nth = 0;

            const QString captured = match.captured(nth);
            const int start = match.capturedStart(nth);
            const int length = match.capturedLength(nth);

            setFormat(start, length, d->number_format);
        }

        // Highlight string litterals.

        {
            const QString species_of_quotes = "\"'";

            QChar kind = 0;
            int start = 0;
            int length = 0;

            for(int j = 0; j < text.size(); j++)
            {
                if(kind == 0)
                {
                    if(species_of_quotes.contains(text.at(j)))
                    {
                        kind = text.at(j);
                        start = j;
                        length = 1;
                    }
                }
                else
                {
                    length++;
                    if(text.at(j) == kind && text.at(j-1) != QChar('\\'))
                    {
                        setFormat(start, length, d->string_format);
                        kind = 0;
                    }
                }
            }

            if(kind != 0)
            {
                setFormat(start, length, d->string_format);
            }
        }
    }
}

