
#pragma once
#include <gnomonOmeroObject>

class gnomonOmeroDataset : public gnomonOmeroObject
{
 public:
  bool isProject(void);
  bool isDataset(void);
  bool isImage(void);
  gnomonOmeroType::type type(void);
};

inline bool gnomonOmeroDataset::isProject(void) {return false;}
inline bool gnomonOmeroDataset::isDataset(void) {return true;}
inline bool gnomonOmeroDataset::isImage(void)   {return false;}
