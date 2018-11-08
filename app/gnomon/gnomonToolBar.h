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

signals:
    void createFusion(void);
    void createSegmentation(void);
    void createPreprocess(void);
    void createRegistration(void);

public slots:
    void onCreateFusion(void);
    void onCreateSegmentation(void);
    void onCreatePreprocess(void);
    void onCreateRegistration(void);

public:
    QSize sizeHint(void) const;

private:
    class gnomonToolBarPrivate *d;
};

//
// gnomonToolBar.h ends here
