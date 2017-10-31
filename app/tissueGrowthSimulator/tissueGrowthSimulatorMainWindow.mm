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
    if (NSAppKitVersionNumber <= NSAppKitVersionNumber10_12)
        return;

    NSView *view = (NSView *)(this->winId());

    NSWindow *window = [view window];
    window.appearance = [NSAppearance appearanceNamed:NSAppearanceNameVibrantDark];
}

//
// tissueGrowthSimulatorMainWindow.mm ends here
