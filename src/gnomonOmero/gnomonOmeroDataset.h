
#pragma once
#include <gnomonOmeroObject>

class gnomonOmeroDataset : public gnomonOmeroObject
{
 public:
  bool gnomonOmeroDataset::isProject(void);
  bool gnomonOmeroDataset::isDataset(void);
  bool gnomonOmeroDataset::isImage(void);
};

gnomonOmeroType::type gnomonOmeroDataset::type(void)
{
  return gnomonOmeroType::dataset;
}

inline bool gnomonOmeroDataset::isProject(void) {return false;}
inline bool gnomonOmeroDataset::isDataset(void) {return true;}
inline bool gnomonOmeroDataset::isImage(void)   {return false;}
