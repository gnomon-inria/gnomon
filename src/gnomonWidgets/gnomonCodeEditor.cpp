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

#include <gnomonCore>
#include <gnomonStyle>

#include "gnomonCodeEditor.h"

#include <set>

// ///////////////////////////////////////////////////////////////////
// Python keywords
// ///////////////////////////////////////////////////////////////////

static const char *s_python_keywords[] = {
    "False",
    "None",
    "True",
    "and",
    "as",
    "assert",
    "break",
    "class",
    "continue",
    "def",
    "del",
    "elif",
    "else",
    "except",
    "finally",
    "for",
    "from",
    "global",
    "if",
    "import",
    "in",
    "is",
    "lambda",
    "nonlocal",
    "not",
    "or",
    "pass",
    "raise",
    "return",
    "try",
    "while",
    "with",
    "yield",
    nullptr };

// ///////////////////////////////////////////////////////////////////
// Python builtins
// ///////////////////////////////////////////////////////////////////

static const char *s_python_builtins[] = {
    "ArithmeticError",
    "AssertionError",
    "AttributeError",
    "BaseException",
    "BlockingIOError",
    "BrokenPipeError",
    "BufferError",
    "BytesWarning",
    "ChildProcessError",
    "ConnectionAbortedError",
    "ConnectionError",
    "ConnectionRefusedError",
    "ConnectionResetError",
    "DeprecationWarning",
    "EOFError",
    "Ellipsis",
    "EnvironmentError",
    "Exception",
    "False",
    "FileExistsError",
    "FileNotFoundError",
    "FloatingPointError",
    "FutureWarning",
    "GeneratorExit",
    "IOError",
    "ImportError",
    "ImportWarning",
    "IndentationError",
    "IndexError",
    "InterruptedError",
    "IsADirectoryError",
    "KeyError",
    "KeyboardInterrupt",
    "LookupError",
    "MemoryError",
    "ModuleNotFoundError",
    "NameError",
    "None",
    "NotADirectoryError",
    "NotImplemented",
    "NotImplementedError",
    "OSError",
    "OverflowError",
    "PendingDeprecationWarning",
    "PermissionError",
    "ProcessLookupError",
    "RecursionError",
    "ReferenceError",
    "ResourceWarning",
    "RuntimeError",
    "RuntimeWarning",
    "StopAsyncIteration",
    "StopIteration",
    "SyntaxError",
    "SyntaxWarning",
    "SystemError",
    "SystemExit",
    "TabError",
    "TimeoutError",
    "True",
    "TypeError",
    "UnboundLocalError",
    "UnicodeDecodeError",
    "UnicodeEncodeError",
    "UnicodeError",
    "UnicodeTranslateError",
    "UnicodeWarning",
    "UserWarning",
    "ValueError",
    "Warning",
    "ZeroDivisionError",
    "__build_class__",
    "__debug__",
    "__doc__",
    "__import__",
    "__loader__",
    "__name__",
    "__package__",
    "__spec__",
    "abs",
    "all",
    "any",
    "ascii",
    "bin",
    "bool",
    "bytearray",
    "bytes",
    "callable",
    "chr",
    "classmethod",
    "compile",
    "complex",
    "copyright",
    "credits",
    "delattr",
    "dict",
    "dir",
    "divmod",
    "enumerate",
    "eval",
    "exec",
    "exit",
    "filter",
    "float",
    "format",
    "frozenset",
    "getattr",
    "globals",
    "hasattr",
    "hash",
    "help",
    "hex",
    "id",
    "input",
    "int",
    "isinstance",
    "issubclass",
    "iter",
    "len",
    "license",
    "list",
    "locals",
    "map",
    "max",
    "memoryview",
    "min",
    "next",
    "object",
    "oct",
    "open",
    "ord",
    "pow",
    "print",
    "property",
    "quit",
    "range",
    "repr",
    "reversed",
    "round",
    "set",
    "setattr",
    "slice",
    "sorted",
    "staticmethod",
    "str",
    "sum",
    "super",
    "tuple",
    "type",
    "vars",
    "zip",
    nullptr };

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonCodeEditorPrivate
{
public:
    QWidget *line_number_area;

    gnomonCodeEditorSyntaxHighlighter *highlighter;

    bool autocompletion_enabled;
    QStringListModel *vocabulary;
    QTimer *autocompletion_timer;
    QCompleter *completer;
};

gnomonCodeEditor::gnomonCodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    d = new gnomonCodeEditorPrivate;

    d->line_number_area = new gnomonCodeEditorLineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    setFont(QFont("monospace"));

    d->highlighter = new gnomonCodeEditorSyntaxHighlighter(document());

    d->vocabulary = new QStringListModel(this);

    d->autocompletion_timer = new QTimer(this);
    connect(d->autocompletion_timer, SIGNAL(timeout()), this, SLOT(updateVocabulary()));

    d->completer = new QCompleter(this);
    d->completer->setModel(d->vocabulary);
    d->completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    d->completer->setWrapAround(false);
    d->completer->setWidget(this);
    d->completer->setCompletionMode(QCompleter::PopupCompletion);
    connect(d->completer, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));

    d->autocompletion_enabled = false;
    enableAutocompletion(true);
}

void gnomonCodeEditor::openScript(void)
{
    QString path = QDir::homePath();

    gnomonCoreSettings settings;
    settings.beginGroup("editor");

    path = settings.value("last_open_script_path").toString();

    QString file_name = QFileDialog::getOpenFileName(this, "Open Script", path, "*.py");

    if(file_name.isEmpty())
        return;

    QFile file(file_name);

    if(!file.open(QIODevice::ReadOnly))
        return;

    this->setPlainText(file.readAll());

    file.close();

    QFileInfo info(file_name);

    settings.setValue("last_open_script_path", info.absolutePath());

    settings.endGroup();
}

void gnomonCodeEditor::enableAutocompletion(bool enabled)
{
    d->autocompletion_enabled = enabled;
    if(enabled)
    {
        updateVocabulary();
        d->autocompletion_timer->start(1000);
    }
    else
    {
        d->autocompletion_timer->stop();
        d->completer->popup()->hide();
    }
}

QString gnomonCodeEditor::textUnderCursor(void)
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void gnomonCodeEditor::insertCompletion(QString completion)
{
    QTextCursor tc = textCursor();
    int extra = completion.length() - d->completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

void gnomonCodeEditor::keyPressEvent(QKeyEvent *e)
{
    if(d->autocompletion_enabled == false)
    {
        QPlainTextEdit::keyPressEvent(e);
    }
    else
    {
        if (d->completer->popup()->isVisible())
        {
           switch (e->key())
           {
           case Qt::Key_Enter:
           case Qt::Key_Return:
           case Qt::Key_Escape:
           case Qt::Key_Tab:
           case Qt::Key_Backtab:
                e->ignore();
                return;
           default:
               break;
           }
        }

        bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
        if (!isShortcut)
            QPlainTextEdit::keyPressEvent(e);

        const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
        if ( ctrlOrShift && e->text().isEmpty() )
            return;

        static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
        bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
        QString completionPrefix = textUnderCursor();

        if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3 || eow.contains(e->text().right(1)))) {
            d->completer->popup()->hide();
            return;
        }

        if (completionPrefix != d->completer->completionPrefix()) {
            d->completer->setCompletionPrefix(completionPrefix);
            d->completer->popup()->setCurrentIndex(d->completer->completionModel()->index(0, 0));
        }
        QRect cr = cursorRect();
        cr.setWidth(d->completer->popup()->sizeHintForColumn(0) + d->completer->popup()->verticalScrollBar()->sizeHint().width());
        d->completer->complete(cr);
    }
}

gnomonCodeEditor::~gnomonCodeEditor(void)
{
    delete d;
}

void gnomonCodeEditor::updateVocabulary(void)
{
    if(d->completer->popup()->isVisible() == false)
    {
        QRegularExpression re("(\\w+)");

        std::set< QString > words;

        // first add python keywords and builtins.

        auto add_words_from_list = [&words] (const char **list)
        {
            for(const char **ptr = list; *ptr != nullptr; ptr++)
            {
                words.insert(QString(*ptr));
            }
        };

        add_words_from_list(s_python_keywords);
        add_words_from_list(s_python_builtins);

        // add all the words of the document.

        QTextBlock block = document()->begin();

        while(block.isValid())
        {
            QRegularExpressionMatchIterator it = re.globalMatch(block.text());

            while(it.hasNext())
            {
                QRegularExpressionMatch match = it.next();

                QString cap = match.captured(1);

                if(cap.length() >= 3)
                {
                    words.insert( cap );
                }
            }

            block = block.next();
        }

        // convert the set to a QStringList.

        QStringList string_list;

        for(const QString& str : words)
        {
            string_list << str;
        }

        d->vocabulary->setStringList(string_list);
    }
}

int gnomonCodeEditor::lineNumberAreaWidth(void)
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

void gnomonCodeEditor::updateLineNumberAreaWidth(int)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void gnomonCodeEditor::updateLineNumberArea(const QRect &rect, int dy)
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

void gnomonCodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    d->line_number_area->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void gnomonCodeEditor::highlightCurrentLine(void)
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(GNOMON_STYLE_LIGHTBLACK);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void gnomonCodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(d->line_number_area);

    painter.fillRect(event->rect(), QColor(GNOMON_STYLE_BACKGROUNDCOLOR));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(GNOMON_STYLE_FOREGROUNDCOLOR));
            painter.drawText(0, top, d->line_number_area->width(), fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

class gnomonCodeEditorSyntaxHighlighterPrivate
{
public:
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

gnomonCodeEditorSyntaxHighlighter::gnomonCodeEditorSyntaxHighlighter(QTextDocument* document) : QSyntaxHighlighter(document)
{
    d = new gnomonCodeEditorSyntaxHighlighterPrivate;

    d->comment_re = QRegularExpression("^[[:blank:]]*#");
    d->word_re = QRegularExpression("([a-zA-Z_][a-zA-Z0-9_]+)");
    d->defclass_re = QRegularExpression("^[[:blank:]]*(def|class)[[:blank:]]+([a-zA-Z_][a-zA-Z0-9_]+)");
    d->number_re = QRegularExpression("[+-]?([0-9]*[.])?[0-9]+");

    d->braces = "{}[]()";

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

    auto add_from_list = [] (const char** list, QSet<QString>& set)
    {
        for(const char** ptr=list; *ptr!=nullptr; ptr++)
        {
            set << QString(*ptr);
        }
    };

    add_from_list(s_python_keywords, d->keywords);
    add_from_list(s_python_builtins, d->builtins);

    d->keyword_format.setForeground(QColor(GNOMON_STYLE_BLUE));
    d->operator_format.setForeground(QColor(GNOMON_STYLE_RED));
    d->comment_format.setForeground(QColor(GNOMON_STYLE_GREEN));
    d->comment_format.setFontItalic(true);
    d->defclass_format.setFontWeight(QFont::Bold);
    d->brace_format.setForeground(QColor(GNOMON_STYLE_FOREGROUNDCOLOR));
    d->string_format.setForeground(QColor(GNOMON_STYLE_LIGHTMAGENTA));
    d->number_format.setForeground(QColor(GNOMON_STYLE_LIGHTMAGENTA));
}

gnomonCodeEditorSyntaxHighlighter::~gnomonCodeEditorSyntaxHighlighter(void)
{
    delete d;
}

void gnomonCodeEditorSyntaxHighlighter::highlightBlock(const QString& text)
{
    QRegularExpressionMatch match;
    QRegularExpressionMatchIterator it;

    match = d->comment_re.match(text);

    if(match.hasMatch())
    {
        setFormat(0, text.size(), d->comment_format);
    }
    else
    {
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

        for(int i = 0; i < text.size(); i++)
        {
            if(d->braces.contains(text.at(i)))
            {
                setFormat(i, 1, d->brace_format);
            }
        }

        for(QString oper : d->operators)
        {
            int j = 0;
            while( (j = text.indexOf(oper,j)) != -1 )
            {
                setFormat(j, oper.size(), d->operator_format);
                j += oper.size();
            }
        }

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

//
// gnomonCoreEditor.cpp ends here
