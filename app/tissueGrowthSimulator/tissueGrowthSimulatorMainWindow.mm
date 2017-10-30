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
    NSView *view = (NSView *)(this->winId());

    NSWindow *window = [view window];
    // window.titlebarAppearsTransparent = YES;
    window.appearance = [NSAppearance appearanceNamed:NSAppearanceNameVibrantDark];
}

//
// tissueGrowthSimulatorMainWindow.mm ends here
