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

#include "gnomonGrowthSimulatorMainWindow.h"

#import <Cocoa/Cocoa.h>

void gnomonGrowthSimulatorMainWindow::initialize(void)
{
#if __MAC_OS_X_VERSION_MAX_ALLOWED > 1090
    NSView *view = (NSView *)(this->winId());

    NSWindow *window = [view window];
    window.appearance = [NSAppearance appearanceNamed:NSAppearanceNameVibrantDark];
#endif
}

//
// gnomonGrowthSimulatorMainWindow.mm ends here
