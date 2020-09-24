#include <dtkWidgetsMenuBar>

class gnomonMenuBarPrivate;

class gnomonMenuBar: public dtkWidgetsMenuBar
{
public:
    gnomonMenuBar(QWidget *parent, QWidget *child_below=nullptr);
    virtual ~gnomonMenuBar(void);

public:
    void addRootMenu(QWidget *parent, QMenu *menu);
    void resizeEvent(void);

private:
    gnomonMenuBarPrivate *d;
};
