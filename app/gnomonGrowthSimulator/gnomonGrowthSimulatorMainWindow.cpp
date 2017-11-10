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

#include "gnomonGrowthSimulatorMainWindow.h"

#include <dtkConfig.h>

#if defined(DTK_BUILD_WRAPPERS)
#include <dtkScript>
#endif

#include <dtkComposer>
#include <dtkWidgets>

#include <gnomon/gnomonConfig.h>
#include <gnomonCore>
#include <gnomonStyle>
#include <gnomonComposer>
#include <gnomonWidgets>
#include <gnomonFonts>

#include <functional>

// ///////////////////////////////////////////////////////////////////
// class gnomonGrowthSimulatorMainWindowStatusBarItem
// ///////////////////////////////////////////////////////////////////

class gnomonGrowthSimulatorMainWindowStatusBarItem : public QWidget
{
public:
     gnomonGrowthSimulatorMainWindowStatusBarItem(QWidget *parent = Q_NULLPTR);

public:
    void setPixmap(const QPixmap& pixmap);
    void setText(const QString text);

private:
    QLabel *p;
    QLabel *t;
    QHBoxLayout *layout;
};

// ///////////////////////////////////////////////////////////////////
// class gnomonGrowthSimulatorMainWindowStatusBarItem
// ///////////////////////////////////////////////////////////////////

gnomonGrowthSimulatorMainWindowStatusBarItem::gnomonGrowthSimulatorMainWindowStatusBarItem(QWidget *parent) : QWidget(parent)
{
    this->p = new QLabel(this);
    this->t = new QLabel(this);

    this->layout = new QHBoxLayout(this);
    this->layout->setContentsMargins(0, 0, 0, 0);
    this->layout->setSpacing(0);
    this->layout->addWidget(this->p);
    this->layout->addWidget(this->t);
}

void gnomonGrowthSimulatorMainWindowStatusBarItem::setPixmap(const QPixmap& pixmap)
{
    this->p->setPixmap(pixmap);
}

void gnomonGrowthSimulatorMainWindowStatusBarItem::setText(const QString text)
{
    this->t->setText(text);
}

// ///////////////////////////////////////////////////////////////////
// gnomonGrowthSimulatorMainWindowSidePane
// ///////////////////////////////////////////////////////////////////

class gnomonGrowthSimulatorMainWindowSidePane : public QFrame
{
    Q_OBJECT

public:
     gnomonGrowthSimulatorMainWindowSidePane(QWidget *parent);
    ~gnomonGrowthSimulatorMainWindowSidePane(void);

public:
    void addAction(const QIcon &icon, const QObject *receiver, std::function<void(void)> function);
    void addStretch(void);

private:
    QVBoxLayout *layout;
};

// ///////////////////////////////////////////////////////////////////
// gnomonGrowthSimulatorMainWindowSidePane
// ///////////////////////////////////////////////////////////////////

gnomonGrowthSimulatorMainWindowSidePane::gnomonGrowthSimulatorMainWindowSidePane(QWidget *parent) : QFrame(parent)
{
    this->layout = new QVBoxLayout(this);
    this->layout->addSpacerItem(new QSpacerItem(72, 32));

    this->setFixedWidth(72);
}

gnomonGrowthSimulatorMainWindowSidePane::~gnomonGrowthSimulatorMainWindowSidePane(void)
{

}

void gnomonGrowthSimulatorMainWindowSidePane::addAction(const QIcon &icon, const QObject *receiver, std::function<void(void)> function)
{
    QPushButton *button = new QPushButton(this);
    button->setIconSize(QSize(32, 32));
    button->setFlat(true);
    button->setIcon(icon);

    connect(button, &QPushButton::clicked, receiver, function);

    this->layout->addWidget(button);
}

void gnomonGrowthSimulatorMainWindowSidePane::addStretch(void)
{
    this->layout->addStretch();
}

// ///////////////////////////////////////////////////////////////////
// gnomonGrowthSimulatorMainWindowPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonGrowthSimulatorMainWindowPrivate
{
public:
    gnomonGrowthSimulatorMainWindowSidePane *side_pane;

public:
    gnomonComposerWidget *composer;

public:
    gnomonCodeEditor *editor;

public:
    dtkInterpreter *interpreter_widget;

#if defined(DTK_BUILD_WRAPPERS)
    dtkScriptInterpreterPython *interpreter;
#endif

public:
    dtkViewManager *view_manager;

public:
    QStackedWidget *editors;

public:
    QSplitter *editor_splitter;

public:
    QAction *open_action;
    QAction *run_action;
    QAction *save_action;
    QAction *save_as_action;

public:
    gnomonFontAllTheIcons *font_all_the_icons;
    gnomonFontAwesome *font_awesome;
    gnomonFontFile *font_file_icons;
    gnomonFontOcticons *font_octicons;
    gnomonFontSourceCodePro *font_source_code_pro;
    gnomonFontSourceSansPro *font_source_sans_pro;
};

// ///////////////////////////////////////////////////////////////////
// gnomonGrowthSimulatorMainWindow
// ///////////////////////////////////////////////////////////////////

gnomonGrowthSimulatorMainWindow::gnomonGrowthSimulatorMainWindow(QWidget *parent) : QMainWindow(parent)
{
    d = new gnomonGrowthSimulatorMainWindowPrivate;

    gnomonCoreSettings settings;
    settings.beginGroup("main_window");
    this->move(settings.value("position").toPoint());
    this->resize(settings.value("size").toSize());
    settings.endGroup();

    d->font_awesome = new gnomonFontAwesome(this);
    d->font_awesome->initFontAwesome();
    d->font_awesome->setDefaultOption("color", QColor(GNOMON_STYLE_ACCENTCOLOR));

    d->font_file_icons = new gnomonFontFile(this);
    d->font_file_icons->initFontFile();
    d->font_file_icons->setDefaultOption("color", QColor(GNOMON_STYLE_ACCENTCOLOR));

    d->font_all_the_icons = new gnomonFontAllTheIcons(this);
    d->font_all_the_icons->initFontAllTheIcons();
    d->font_all_the_icons->setDefaultOption("color", QColor(GNOMON_STYLE_ACCENTCOLOR));

    d->font_octicons = new gnomonFontOcticons(this);
    d->font_octicons->initFontOcticons();

    d->font_source_code_pro = new gnomonFontSourceCodePro(this);
    d->font_source_code_pro->initFontSourceCodePro();

    d->font_source_sans_pro = new gnomonFontSourceSansPro(this);
    d->font_source_sans_pro->initFontSourceSansPro();

    d->side_pane = new gnomonGrowthSimulatorMainWindowSidePane(this);

    d->view_manager = new dtkViewManager(this);

    d->composer = new gnomonComposerWidget(this);

    d->editor = new gnomonCodeEditor;
    d->editor->setFont(d->font_source_code_pro->font(12));

    d->interpreter_widget = new dtkInterpreter;
    d->interpreter_widget->setFont(d->font_source_code_pro->font(12));

#if defined(DTK_BUILD_WRAPPERS)
    d->interpreter = new dtkScriptInterpreterPython;

    d->interpreter_widget->registerInterpreter(d->interpreter);
#endif

    d->editor_splitter = new QSplitter(this);
    d->editor_splitter->setHandleWidth(2);
    d->editor_splitter->addWidget(d->editor);
    d->editor_splitter->addWidget(d->interpreter_widget);

    d->editors = new QStackedWidget(this);
    d->editors->setContentsMargins(0, 0, 0, 0);
    d->editors->addWidget(d->composer);
    d->editors->addWidget(d->editor_splitter);

    d->side_pane->addAction(d->font_awesome->icon(fa::folderopen), this, [=] () {
        if (d->editors->currentIndex() == 0)
            d->composer->compositionOpen();
        else
            d->editor->openScript();
    });

    d->side_pane->addAction(d->font_awesome->icon(fa::play), this, [=] () {
        if (d->editors->currentIndex() == 0) {
            d->composer->composerWidget()->run();
        } else {
            int stat;
            if (d->interpreter_widget)
                d->interpreter_widget->output(d->interpreter->interpret(d->editor->toPlainText(), &stat));
            else
                d->interpreter->interpret(d->editor->toPlainText(), &stat);
        }
    });

    d->side_pane->addAction(d->font_awesome->icon(fa::filetext), this, [=] () {
        if (d->editors->currentIndex() == 0) {
            d->composer->compositionSave();
        } else {
            qWarning() << "not implemented";
        }
    });

    d->side_pane->addAction(d->font_awesome->icon(fa::file), this, [=] () {
        if (d->editors->currentIndex() == 0) {
            d->composer->compositionSaveAs();
        } else {
            qWarning() << "not implemented";
        }
    });

    d->side_pane->addStretch();

    d->side_pane->addAction(d->font_all_the_icons->icon(ai::icon::cplusplus), this, [=] () {
            d->editors->setCurrentIndex(0);
    });

    d->side_pane->addAction(d->font_all_the_icons->icon(ai::icon::python), this, [=] () {
            d->editors->setCurrentIndex(1);
    });

    QSplitter *inner = new QSplitter(Qt::Vertical, this);
    inner->setHandleWidth(2);
    inner->addWidget(d->view_manager);
    inner->addWidget(d->editors);
    inner->setSizes(QList<int>() << this->size().height() / 3.0 << this->size().height() / 7.0);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->side_pane);
    layout->addWidget(inner);

    QWidget *central = new QWidget(this);
    central->setLayout(layout);

    this->setCentralWidget(central);
    this->setWindowTitle("gnomon Growth Simulator");

#if defined(Q_OS_MAC)
    this->initialize();
#endif

    this->setStyleSheet(gnomonStyleSheet());

    qApp->setFont(d->font_source_sans_pro->font(11));

    QVariantMap options;
    options.insert("color", QColor(GNOMON_STYLE_GREEN));
    options.insert("scale-factor", 0.5);

    gnomonGrowthSimulatorMainWindowStatusBarItem *git_branch = new gnomonGrowthSimulatorMainWindowStatusBarItem(this);
    git_branch->setPixmap(d->font_awesome->icon(fa::icon::codefork, options).pixmap(32, 32));
    git_branch->setText(GNOMON_GIT_BRANCH);
    git_branch->setStyleSheet(QString("color: %1;").arg(GNOMON_STYLE_GREEN));

    options.insert("color", QColor(GNOMON_STYLE_LIGHTRED));
    options.insert("scale-factor", 0.5);

    gnomonGrowthSimulatorMainWindowStatusBarItem *git_commit = new gnomonGrowthSimulatorMainWindowStatusBarItem(this);
    git_commit->setPixmap(d->font_octicons->icon(oi::icon::gitcommit, options).pixmap(32, 32));
    git_commit->setText(GNOMON_GIT_COMMIT);
    git_commit->setStyleSheet(QString("color: %1;").arg(GNOMON_STYLE_LIGHTRED));

    options.insert("color", QColor(GNOMON_STYLE_LIGHTYELLOW));
    options.insert("scale-factor", 0.5);

    gnomonGrowthSimulatorMainWindowStatusBarItem *git_updates = new gnomonGrowthSimulatorMainWindowStatusBarItem(this);
    git_updates->setPixmap(d->font_octicons->icon(oi::icon::package, options).pixmap(32, 32));
    git_updates->setText(GNOMON_GIT_UPDATES);
    git_updates->setStyleSheet(QString("color: %1;").arg(GNOMON_STYLE_LIGHTYELLOW));

    this->statusBar()->addPermanentWidget(git_branch);
    this->statusBar()->addPermanentWidget(git_commit);
    this->statusBar()->addPermanentWidget(git_updates);
}

gnomonGrowthSimulatorMainWindow::~gnomonGrowthSimulatorMainWindow(void)
{
    gnomonCoreSettings settings;
    settings.beginGroup("main_window");
    settings.setValue("position", this->pos());
    settings.setValue("size", this->size());
    settings.endGroup();

    delete d;
}

void gnomonGrowthSimulatorMainWindow::addEditor(QWidget *editor)
{
    d->interpreter_widget->hide();
    d->interpreter_widget->deleteLater();
    d->interpreter_widget = Q_NULLPTR;

    editor->setStyleSheet(gnomonStyleSheet());

    d->editor_splitter->addWidget(editor);
}

#include "gnomonGrowthSimulatorMainWindow.moc"

//
// gnomonGrowthSimulatorMainWindow.cpp ends here
