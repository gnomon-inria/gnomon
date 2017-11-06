
#pragma once
#include <gnomonOmeroObject>

class gnomonOmeroProject : public gnomonOmeroObject
{
 public:
  gnomonOmeroProject(void);
  
 public:
  bool isProject(void);
  bool isDataset(void);
  bool isImage(void);
  gnomonOmeroType::type type(void);
};

inline bool gnomonOmeroProject::isProject(void) {return true;}
inline bool gnomonOmeroProject::isDataset(void) {return false;}
inline bool gnomonOmeroProject::isImage(void)   {return false;}
