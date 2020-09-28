#include "gnomonMenuBar.h"

#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>

class gnomonMenuBarPrivate
{
public:
    QWidget *parent = nullptr;
    bool inside_any_submenu = false;
    dtkWidgetsMenu *deferred_change_menu = nullptr;
    gnomonMenuBar *bar = nullptr;

public:
    dtkWidgetsMenu *buildRootItem(int icon, QMenu *menu, int index);
    void insertMenuItem(QAction *action, dtkWidgetsMenu *w_menu, dtkWidgetsMenu **sub_menu=nullptr);
    void registerActionsShortcuts(QMenu *menu, QWidget *parent);
    void deferredChangeMenu(void);
    void cleanUpLabels(QMenu *menu);
};

gnomonMenuBar::gnomonMenuBar(QWidget *parent, QWidget *child_below) :
    dtkWidgetsMenuBar(parent),
    d(new gnomonMenuBarPrivate)
{
    this->show();
    this->setInteractive(false);
    this->setWidth(32);
    this->setMargins(6);
    this->touch();

    auto *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(this);

    parent->setLayout(layout);

    if (child_below != nullptr) {
        layout->addWidget(child_below);
        child_below->stackUnder(this);
    }

    QTimer::singleShot(1000, [=] () {
        // workaround bug: the first resize event will be called
        // before we are fully created, but it needs to be called
        // after we've been fully created to compute the correct size
        this->setFixedHeight(parent->height());
    });


    d->bar = this;
    d->parent = parent;

    connect(this, &dtkWidgetsMenuBar::clicked, [=] (int index) {

            auto *as_menu = this->menus()[index];
            if (as_menu == nullptr ||as_menu->menus().count() == 0) { return; }
            auto *sub_menu = as_menu->menus()[0];
            if (sub_menu == nullptr) { return; }

            auto *container = dynamic_cast<dtkWidgetsMenuBarContainer*>(this->container());
            std::function<void()> no_op_callback = [=] () { };
            container->switchToRoot(no_op_callback);

            d->deferred_change_menu = sub_menu;
            QTimer::singleShot(10, [=] () { d->deferredChangeMenu(); });
        });

    connect(this, &dtkWidgetsMenuBar::entered, [=] (dtkWidgetsMenu *menu) {
            d->inside_any_submenu = true;
            const auto& menus = this->menus();
            for (int i=0; i<menus.size(); ++i) {
                const auto& submenus = menus[i]->menus();
                if (submenus.size() == 1 && submenus[0] == menu) {
                    this->setCurrentIndex(i);
                    break;
                }
            }
        });

    connect(this, &dtkWidgetsMenuBar::left, [=] (dtkWidgetsMenu *menu) {
            d->inside_any_submenu = false;
            QTimer::singleShot(10, [=] () {
                if (!d->inside_any_submenu) {
                    // still not inside any submenu in the near future
                    // <-> we went back to the root main menu
                    // <-> no root item should be selected
                    this->setCurrentIndex(-1);
                }
            });
        });
}
gnomonMenuBar::~gnomonMenuBar()
{
    delete d;
}

void gnomonMenuBar::resizeEvent(void)
{
    this->setFixedHeight(d->parent->height());
}

void gnomonMenuBarPrivate::deferredChangeMenu(void)
{
    auto *container = dynamic_cast<dtkWidgetsMenuBarContainer*>(bar->container());
    if (container->slider->is_in_transition) {
        QTimer::singleShot(10, [=] () { deferredChangeMenu(); });
    } else {
        container->switchToNextSlide(deferred_change_menu);
    }
}

void gnomonMenuBarPrivate::insertMenuItem(QAction *action, dtkWidgetsMenu *w_menu, dtkWidgetsMenu **sub_menu)
{
    if(QMenu *s_menu = action->menu()) {

        const int icon = sub_menu ? 0 : fa::circle;
        // if sub_menu, then this is a root item
        // it already has a category icon (for the menubar quick-access before expand)
        // -> do not give it an icon
        dtkWidgetsMenu *w_s_menu = w_menu->addMenu(icon, action->text());

        if (sub_menu != nullptr) {
            *sub_menu = w_s_menu;
        }

        foreach(QAction *action, s_menu->actions()) {
            this->insertMenuItem(action, w_s_menu);
        }
    } else {

        if(action->isSeparator()) {
            w_menu->addSeparator();
        } else {

            dtkWidgetsMenuItem *item = w_menu->addItem(fa::dashcube, action->text());

            QObject::connect(item, SIGNAL(clicked()), action, SLOT(trigger()));
            QObject::connect(item, SIGNAL(clicked()), bar, SLOT(collapse()));
        }
    }
}

dtkWidgetsMenu *gnomonMenuBarPrivate::buildRootItem(int icon, QMenu *menu, int index)
{
    QString unique_blank_title;
    for (int i=0; i<index; ++i) {
        unique_blank_title += " ";
    }
    dtkWidgetsMenu *w_menu = new dtkWidgetsMenu(icon, unique_blank_title);

    auto *root_item = new QAction;
    root_item->setText(menu->title());
    root_item->setMenu(new QMenu);
    dtkWidgetsMenu *sub_menu = nullptr;
    this->insertMenuItem(root_item, w_menu, &sub_menu);
    Q_ASSERT(sub_menu != nullptr);

    foreach(QAction *action, menu->actions()) {
        if(action->text().isEmpty())
            continue;

        this->insertMenuItem(action, sub_menu);
    }

    return w_menu;
}


void gnomonMenuBarPrivate::registerActionsShortcuts(QMenu *menu, QWidget *parent)
{
    // this function is needed for these 2 purposes :
    // [1] enable its shortcut by adding it do the parent widget
    // [2] expose its shortcut to the user by adding it to its text
    for (auto * reaction : menu->actions()) {
        const auto& shortcut = reaction->shortcut();
        if (shortcut.isEmpty()) {
            continue;
        }
        parent->addAction(reaction);
        reaction->setText(reaction->text() + " (" + shortcut.toString() + ")");
    }
}

void gnomonMenuBarPrivate::cleanUpLabels(QMenu *menu)
{
    menu->setTitle(menu->title().remove("&"));
    for (auto *action: menu->actions()) {
        action->setText(action->text().remove("&"));
        if (action->menu() != nullptr) {
            cleanUpLabels(action->menu());
        }
    }
}

void gnomonMenuBar::addRootMenu(QWidget *parent, QMenu *menu)
{
    d->cleanUpLabels(menu);

    d->registerActionsShortcuts(menu, parent);

    const auto& menu_title = menu->title();
    static const QMap<QString, int> menu_index = {
        { "File", 0 },
        { "Edit", 1 },
        { "View", 2 },
    };
    auto insert_index = this->size();
    if (menu_index.contains(menu_title)) {
        insert_index = menu_index[menu_title];
    }

    static const QMap<QString, int> icons =
    {
        { "File",  fa::file     },
        { "Edit",  fa::edit     },
        { "View",  fa::eye      },
        { "Tools", fa::gears    },
        { "Help",  fa::question },
    };
    const int icon = icons.contains(menu->title()) ? icons[menu->title()] : fa::file;

    auto *dtk_menu = d->buildRootItem(icon, menu, this->size());
    if (insert_index > this->size()) {
        this->addMenu(dtk_menu);
    } else {
        this->insertMenu(insert_index, dtk_menu);
    }
    this->touch();
}
