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
    void indexDeleted(int);

signals:
    void createFusion(void);
    void createLSystemSimulator(void);
    void createPreprocess(void);
    void createRegistration(void);
    void createMeshFromImage(void);
    void createSegmentation(void);
    void createSimulation(void);
    void createTreeAnalysis(void);

public slots:
    void onCreateFusion(void);
    void onCreateLSystemSimulator(void);
    void onCreatePreprocess(void);
    void onCreateRegistration(void);
    void onCreateMeshFromImage(void);
    void onCreateSegmentation(void);
    void onCreateSimulation(void);
    void onCreateTreeAnalysis(void);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

public:
    void setCurrentIndex(int);

public:
    QSize sizeHint(void) const;

public:
    static QColor browser_color;
    static QColor lsystem_color;
    static QColor fusion_color;
    static QColor preprocess_color;
    static QColor registration_color;
    static QColor meshFromImage_color;
    static QColor segmentation_color;
    static QColor simulation_color;
    static QColor tree_analysis_color;

private:
    class gnomonToolBarPrivate *d;
};

//
// gnomonToolBar.h ends here
