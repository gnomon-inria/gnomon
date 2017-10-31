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

#include "tissueGrowthSimulatorMainWindow.h"

#import <Cocoa/Cocoa.h>

void tissueGrowthSimulatorMainWindow::initialize(void)
{
#if __MAC_OS_X_VERSION_MAX_ALLOWED > 1090
    NSView *view = (NSView *)(this->winId());

    NSWindow *window = [view window];
    window.appearance = [NSAppearance appearanceNamed:NSAppearanceNameVibrantDark];
#endif
}

//
// tissueGrowthSimulatorMainWindow.mm ends here
