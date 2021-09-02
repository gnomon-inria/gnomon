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


#include "gnomonPythonPluginLoader.h"

#include <dtkScript>
#include <QtCore>

void loadPluginGroup (const QString& module)
{
  int stat;
  QString code = "from pkg_resources import iter_entry_points\n";
  code = code + QString("for entry_point in iter_entry_points(group=") + QString("\"") + module  + QString("\"") + QString(", name=None):\n");
  code = code + QString("   entry_point.load()\n");

  dtkScriptInterpreterPython::instance()->interpret(code, &stat);

  //Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);
}
