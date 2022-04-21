#pragma once
#include <QtCore>

#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage>
#include <gnomonCoreExport>

class GNOMONCORE_EXPORT gnomonMorphonetHelperCreator {
public:
    virtual ~gnomonMorphonetHelperCreator(void) = default;
    virtual void *create(void) { return nullptr; }
};

class GNOMONCORE_EXPORT gnomonMorphonetHelper {
protected:
    gnomonMorphonetHelper() = default;
    virtual ~gnomonMorphonetHelper() = default;

public:
    static gnomonMorphonetHelper *instance();

public:
    virtual bool connect(QString login, QString password) = 0;
    virtual QString datasetsInfo(QString search = "") = 0;
    virtual bool selectDataset(int id) = 0;
    virtual int startTime() = 0;
    virtual int endTime() = 0;

public:
    virtual gnomonCellImage *loadMnDataAtTime(int time, double voxelsize = 0.1, bool load_infos = true) = 0;
    virtual void loadMnInfos(gnomonCellImageSeries *serie) = 0;

    virtual int createDataset(QString name, gnomonCellImageSeries *serie, int id_NCBI=0, int id_type=0, QString description="") = 0;
    virtual bool deleteDataset(int id) = 0;



private: 
  static gnomonMorphonetHelper *s_instance;
  static gnomonMorphonetHelperCreator *s_creator;
  friend void setMorphonetHelperCreator(gnomonMorphonetHelperCreator *creator);
};

void GNOMONCORE_EXPORT setMorphonetHelperCreator(gnomonMorphonetHelperCreator *creator);

