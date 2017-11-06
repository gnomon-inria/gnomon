
#pragma once
#include <gnomonOmeroObject>

class gnomonOmeroProject : public gnomonOmeroObject
{
 public:
  bool gnomonOmeroProject::isProject(void);
  bool gnomonOmeroProject::isDataset(void);
  bool gnomonOmeroProject::isImage(void);
};

gnomonOmeroType::type gnomonOmeroProject::type(void)
{
  return gnomonOmeroType::project;
}

inline bool gnomonOmeroProject::isProject(void) {return true;}
inline bool gnomonOmeroProject::isDataset(void) {return false;}
inline bool gnomonOmeroProject::isImage(void)   {return false;}
