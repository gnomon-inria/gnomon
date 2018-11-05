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

#include <QtWidgets>

class gnomonToolBar : public QFrame
{
    Q_OBJECT

public:
     gnomonToolBar(QWidget *parent = nullptr);
    ~gnomonToolBar(void);

signals:
    void indexChanged(int);

public:
    QSize sizeHint(void) const;

private:
    class gnomonToolBarPrivate *d;
};

//
// gnomonToolBar.h ends here
