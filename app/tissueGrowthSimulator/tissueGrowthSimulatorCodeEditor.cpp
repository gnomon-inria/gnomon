#include "tissueGrowthSimulatorCodeEditor.h"

tissueGrowthSimulatorCodeEditor::tissueGrowthSimulatorCodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    m_lineNumberArea = new tissueGrowthSimulatorCodeEditorLineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    setFont(QFont("monospace"));

    m_highlighter = new tissueGrowthSimulatorCodeEditorSyntaxHighlighter(document());
}

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
        m_lineNumberArea->scroll(0, dy);
    }
    else {
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

void tissueGrowthSimulatorCodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
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
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, m_lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

tissueGrowthSimulatorCodeEditorSyntaxHighlighter::tissueGrowthSimulatorCodeEditorSyntaxHighlighter(QTextDocument* document) :
    QSyntaxHighlighter(document),
    m_commentExpr("^[[:blank:]]*#"),
    m_wordExpr("([a-zA-Z_][a-zA-Z0-9_]+)")
{
    // TODO :
    // use a std::set<std::string> with lexicographic ordering rather than QStringList.

    m_keywords << "and";
    m_keywords << "as";
    m_keywords << "assert";
    m_keywords << "break";
    m_keywords << "class";
    m_keywords << "continue";
    m_keywords << "def";
    m_keywords << "del";
    m_keywords << "elif";
    m_keywords << "else";
    m_keywords << "except";
    m_keywords << "exec";
    m_keywords << "finally";
    m_keywords << "for";
    m_keywords << "from";
    m_keywords << "global";
    m_keywords << "if";
    m_keywords << "import";
    m_keywords << "in";
    m_keywords << "is";
    m_keywords << "lambda";
    m_keywords << "not";
    m_keywords << "or";
    m_keywords << "pass";
    m_keywords << "print";
    m_keywords << "raise";
    m_keywords << "return";
    m_keywords << "try";
    m_keywords << "while";
    m_keywords << "with";
    m_keywords << "yield";
    m_builtins << "ArithmeticError";
    m_builtins << "AssertionError";
    m_builtins << "AttributeError";
    m_builtins << "BaseException";
    m_builtins << "BufferError";
    m_builtins << "BytesWarning";
    m_builtins << "DeprecationWarning";
    m_builtins << "EOFError";
    m_builtins << "Ellipsis";
    m_builtins << "EnvironmentError";
    m_builtins << "Exception";
    m_builtins << "False";
    m_builtins << "FloatingPointError";
    m_builtins << "FutureWarning";
    m_builtins << "GeneratorExit";
    m_builtins << "IOError";
    m_builtins << "ImportError";
    m_builtins << "ImportWarning";
    m_builtins << "IndentationError";
    m_builtins << "IndexError";
    m_builtins << "KeyError";
    m_builtins << "KeyboardInterrupt";
    m_builtins << "LookupError";
    m_builtins << "MemoryError";
    m_builtins << "NameError";
    m_builtins << "None";
    m_builtins << "NotImplemented";
    m_builtins << "NotImplementedError";
    m_builtins << "OSError";
    m_builtins << "OverflowError";
    m_builtins << "PendingDeprecationWarning";
    m_builtins << "ReferenceError";
    m_builtins << "RuntimeError";
    m_builtins << "RuntimeWarning";
    m_builtins << "StandardError";
    m_builtins << "StopIteration";
    m_builtins << "SyntaxError";
    m_builtins << "SyntaxWarning";
    m_builtins << "SystemError";
    m_builtins << "SystemExit";
    m_builtins << "TabError";
    m_builtins << "True";
    m_builtins << "TypeError";
    m_builtins << "UnboundLocalError";
    m_builtins << "UnicodeDecodeError";
    m_builtins << "UnicodeEncodeError";
    m_builtins << "UnicodeError";
    m_builtins << "UnicodeTranslateError";
    m_builtins << "UnicodeWarning";
    m_builtins << "UserWarning";
    m_builtins << "ValueError";
    m_builtins << "Warning";
    m_builtins << "ZeroDivisionError";
    m_builtins << "__debug__";
    m_builtins << "__doc__";
    m_builtins << "__import__";
    m_builtins << "__name__";
    m_builtins << "__package__";
    m_builtins << "abs";
    m_builtins << "all";
    m_builtins << "any";
    m_builtins << "apply";
    m_builtins << "basestring";
    m_builtins << "bin";
    m_builtins << "bool";
    m_builtins << "buffer";
    m_builtins << "bytearray";
    m_builtins << "bytes";
    m_builtins << "callable";
    m_builtins << "chr";
    m_builtins << "classmethod";
    m_builtins << "cmp";
    m_builtins << "coerce";
    m_builtins << "compile";
    m_builtins << "complex";
    m_builtins << "copyright";
    m_builtins << "credits";
    m_builtins << "delattr";
    m_builtins << "dict";
    m_builtins << "dir";
    m_builtins << "divmod";
    m_builtins << "enumerate";
    m_builtins << "eval";
    m_builtins << "execfile";
    m_builtins << "exit";
    m_builtins << "file";
    m_builtins << "filter";
    m_builtins << "float";
    m_builtins << "format";
    m_builtins << "frozenset";
    m_builtins << "getattr";
    m_builtins << "globals";
    m_builtins << "hasattr";
    m_builtins << "hash";
    m_builtins << "help";
    m_builtins << "hex";
    m_builtins << "id";
    m_builtins << "input";
    m_builtins << "int";
    m_builtins << "intern";
    m_builtins << "isinstance";
    m_builtins << "issubclass";
    m_builtins << "iter";
    m_builtins << "len";
    m_builtins << "license";
    m_builtins << "list";
    m_builtins << "locals";
    m_builtins << "long";
    m_builtins << "map";
    m_builtins << "max";
    m_builtins << "memoryview";
    m_builtins << "min";
    m_builtins << "next";
    m_builtins << "object";
    m_builtins << "oct";
    m_builtins << "open";
    m_builtins << "ord";
    m_builtins << "pow";
    m_builtins << "print";
    m_builtins << "property";
    m_builtins << "quit";
    m_builtins << "range";
    m_builtins << "raw_input";
    m_builtins << "reduce";
    m_builtins << "reload";
    m_builtins << "repr";
    m_builtins << "reversed";
    m_builtins << "round";
    m_builtins << "set";
    m_builtins << "setattr";
    m_builtins << "slice";
    m_builtins << "sorted";
    m_builtins << "staticmethod";
    m_builtins << "str";
    m_builtins << "sum";
    m_builtins << "super";
    m_builtins << "tuple";
    m_builtins << "type";
    m_builtins << "unichr";
    m_builtins << "unicode";
    m_builtins << "vars";
    m_builtins << "xrange";
    m_builtins << "zip";

    m_keywordFormat.setFontWeight(QFont::Bold);

    m_builtinFormat.setFontWeight(QFont::Bold);

    m_commentFormat.setForeground(Qt::darkGray);
    m_commentFormat.setFontItalic(true);

}

void tissueGrowthSimulatorCodeEditorSyntaxHighlighter::highlightBlock(const QString& text)
{

    QRegularExpressionMatch match = m_commentExpr.match(text);

    if(match.hasMatch())
    {
        setFormat(0, text.size(), m_commentFormat);
    }
    else
    {
        QRegularExpressionMatchIterator it = m_wordExpr.globalMatch(text);
        while(it.hasNext())
        {
            match = it.next();

            const int nth = 1;

            QString captured = match.captured(nth);
            int start = match.capturedStart(nth);
            int length = match.capturedLength(nth);

            if(m_keywords.contains(captured))
            {
                setFormat(start, length, m_keywordFormat);
            }
            else if(m_builtins.contains(captured))
            {
                setFormat(start, length, m_builtinFormat);
            }
        }
    }
}

