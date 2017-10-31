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
    if (![NSProcessInfo.processInfo isOperatingSystemAtLeastVersion:(NSOperatingSystemVersion){10,10,0}])
        return;

    NSView *view = (NSView *)(this->winId());

    NSWindow *window = [view window];
    window.appearance = [NSAppearance appearanceNamed:NSAppearanceNameVibrantDark];
}

//
// tissueGrowthSimulatorMainWindow.mm ends here
