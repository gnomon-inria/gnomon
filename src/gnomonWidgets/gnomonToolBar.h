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
    void createPythonSimulator(void);
    void createPreprocess(void);
    void createCellImageQuantification(void);
    void createRegistration(void);
    void createMeshFromImage(void);
    void createPointCloudFromImage(void);
    void createSegmentation(void);
    void createCellComplexFromCellImage(void);
    void createCellImageFilter(void);
    void createSimulation(void);
    void createTreeAnalysis(void);
    void createTreeFromLString(void);
    void createLStringFromTree(void);
    void createPlantScan3D(void);

public slots:
    void onCreateFusion(void);
    void onCreateLSystemSimulator(void);
    void onCreatePythonSimulator(void);
    void onCreatePreprocess(void);
    void onCreateCellImageQuantification(void);
    void onCreateRegistration(void);
    void onCreateMeshFromImage(void);
    void onCreatePointCloudFromImage(void);
    void onCreateSegmentation(void);
    void onCreateCellComplexFromCellImage(void);
    void onCreateCellImageFilter(void);
    void onCreateSimulation(void);
    void onCreateTreeAnalysis(void);
    void onCreateTreeFromLString(void);
    void onCreateLStringFromTree(void);
    void onCreatePlantScan3D(void);

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
    static QColor python_simulation_color;
    static QColor fusion_color;
    static QColor preprocess_color;
    static QColor cellImageQuantification_color;
    static QColor registration_color;
    static QColor meshFromImage_color;
    static QColor pointCloudFromImage_color;
    static QColor segmentation_color;
    static QColor cellComplexFromCellImage_color;
    static QColor cellImageFilter_color;
    static QColor simulation_color;
    static QColor tree_analysis_color;
    static QColor treeFromLString_color;
    static QColor lStringFromTree_color;
    static QColor plant_scan_3D_color;

private:
    class gnomonToolBarPrivate *d;
};

//
// gnomonToolBar.h ends here
