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

#include "gnomonWorkspaceTreeFromLString.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonTree/gnomonTreeFromLStringCommand>
#include <gnomonWidgets>
#include <gnomonVisualization>

#include <dtkImagingCore>
#include <dtkScript>
#include <dtkMacs>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceTreeFromLStringPrivate : public gnomonWorkspaceTemplatePrivate<gnomonTreeFromLStringCommand>
{
public:
     gnomonWorkspaceTreeFromLStringPrivate(void);
    ~gnomonWorkspaceTreeFromLStringPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewMatplotlib *source = nullptr;
    gnomonViewMatplotlib *target = nullptr;

public:
    QStackedWidget *target_stack = nullptr;
    gnomonMessageBoard *target_message = nullptr;

    QSplitter *splitter = nullptr;

public:
    dtkMacsWidget *editor = nullptr;

public:
    dtkWidgetsMenu *menu_;

public:
    dtkWidgetsMenuBarContainer *dashboard;
};

gnomonWorkspaceTreeFromLStringPrivate::gnomonWorkspaceTreeFromLStringPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonTreeFromLStringCommand >()
{

}

gnomonWorkspaceTreeFromLStringPrivate::~gnomonWorkspaceTreeFromLStringPrivate(void)
{

}

QString gnomonWorkspaceTreeFromLStringPrivate::workspace(void) const
{
    return "LString Conversion";
}

QStringList gnomonWorkspaceTreeFromLStringPrivate::keys(void) const
{
    return gnomonCore::treeFromLString::pluginFactory().keys();
}

gnomonWorkspaceTreeFromLString::gnomonWorkspaceTreeFromLString(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("treeFromLString");
    
    d = new gnomonWorkspaceTreeFromLStringPrivate;

    d->source = new gnomonViewMatplotlib(this);
//    d->source->setExportColor(gnomonToolBar::treeFromLString_color);

    d->target = new gnomonViewMatplotlib(this);
//    d->target->setExportColor(gnomonToolBar::treeFromLString_color);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: LSystem Editor
// /////////////////////////////////////////////////////////////////////////////

    d->editor = new dtkMacsWidget(this);

    QString default_lsystem = "";
    default_lsystem += "derivation length: 1\n";
    default_lsystem += "\n";
    default_lsystem += "production:\n";
    default_lsystem += "B(t, s, a, dur, alpha,order):\n";
    default_lsystem += "  nproduce\n";
    default_lsystem += "\n";
    default_lsystem += "A(a):\n";
    default_lsystem += "  nproduce\n";
    default_lsystem += "\n";
    default_lsystem += "I(t,s):\n";
    default_lsystem += "  nproduce S(t,s)\n";
    default_lsystem += "\n";
    default_lsystem += "/(x) -->\n";
    default_lsystem += "f(x) -->\n";
    default_lsystem += "+(x) -->\n";
    default_lsystem += "-(x) -->\n";
    default_lsystem += "@Ts(x) --> \n";
    default_lsystem += "\n";
    default_lsystem += "endlsystem\n";
    d->editor->setText(default_lsystem);

    QVBoxLayout *input_layout = new QVBoxLayout;
    input_layout->setContentsMargins(0, 0, 0, 0);
    input_layout->setSpacing(0);
    input_layout->addWidget(d->source);
    input_layout->addWidget(d->editor);

    QWidget *input_widget = new QWidget(this);
    input_widget->setLayout(input_layout);
    input_widget->resize(800, input_widget->height());

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Stacked target view
// /////////////////////////////////////////////////////////////////////////////

    d->target_message = new gnomonMessageBoard(this);
    d->target_message->setMessage("Result will be displayed here");

    d->target_stack = new QStackedWidget(this);
    d->target_stack->addWidget(d->target_message);
    d->target_stack->addWidget(d->target);

    d->splitter = new QSplitter(this);
    d->splitter->addWidget(input_widget);
    d->splitter->addWidget(d->target_stack);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Dashboard inception
// /////////////////////////////////////////////////////////////////////////////

    d->dashboard = new dtkWidgetsMenuBarContainer(this);
    d->dashboard->navigator->deleteLater();
    d->dashboard->build(QVector<dtkWidgetsMenu *>() << d->menu(this));
    d->dashboard->setFixedWidth(300);

// /////////////////////////////////////////////////////////////////////////////

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->splitter);
    layout->addWidget(d->dashboard);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    connect(d->source, &gnomonViewMatplotlib::formAdded, [=] ()
    {
        if (d->command->input() != dynamic_cast<gnomonLStringSeries *>(d->source->form("gnomonLString"))) {
            if (dynamic_cast<gnomonLStringSeries *>(d->source->form("gnomonLString"))) {
                d->command->setInput(dynamic_cast<gnomonLStringSeries *>(d->source->form("gnomonLString")));
            }
        } else {
            qDebug() << "Not changed";
        }
        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspaceTreeFromLStringPrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        if (dynamic_cast<gnomonLStringSeries *>(d->source->form("gnomonLString"))) {
            d->command->setInput(dynamic_cast<gnomonLStringSeries *>(d->source->form("gnomonLString")));
        }
        d->configure(algorithm);
    });

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspaceTreeFromLString::~gnomonWorkspaceTreeFromLString(void)
{
    delete d;
}

void gnomonWorkspaceTreeFromLString::enter(void)
{
//    dtkApp->window()->menubar()->addMenu(d->source->menu());
//    dtkApp->window()->menubar()->addMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceTreeFromLString::leave(void)
{
//    dtkApp->window()->menubar()->removeMenu(d->source->menu());
//    dtkApp->window()->menubar()->removeMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceTreeFromLString::apply(void)
{
    Q_ASSERT(d->command);

    if (d->command->input() != dynamic_cast<gnomonLStringSeries *>(d->source->form("gnomonLString"))) {
        if (dynamic_cast<gnomonLStringSeries *>(d->source->form("gnomonLString"))) {
            d->command->setInput(dynamic_cast<gnomonLStringSeries *>(d->source->form("gnomonLString")));
        }
    } else {
        qDebug() << "Not changed";
    }

    d->command->setLSystem(d->editor->toPlainText());

    d->command->redo();

    if (d->command->output()) {
        d->target->setForm("gnomonTree",d->command->output());
        d->target_stack->setCurrentWidget(d->target);
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
    }
}

void gnomonWorkspaceTreeFromLString::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

//
// gnomonWorkspaceTreeFromLString.cpp ends here
