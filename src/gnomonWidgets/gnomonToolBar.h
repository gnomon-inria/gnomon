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

#pragma once

#include <gnomonWidgetsExport>

#include <QtWidgets>

class GNOMONWIDGETS_EXPORT gnomonToolBar : public QFrame
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
    void createSimulation(void);

public slots:
    void onCreateFusion(void);
    void onCreateSegmentation(void);
    void onCreatePreprocess(void);
    void onCreateRegistration(void);
    void onCreateSimulation(void);

public:
    QSize sizeHint(void) const;

public:
    static QColor browser_color;
    static QColor fusion_color;
    static QColor segmentation_color;
    static QColor preprocess_color;
    static QColor registration_color;
    static QColor simulation_color;

private:
    class gnomonToolBarPrivate *d;
};

//
// gnomonToolBar.h ends here
