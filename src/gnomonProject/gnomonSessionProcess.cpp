#include "gnomonSessionProcess.h"

// /////////////////////////////////////////////////////////////////
// gnomonSessionProcessPrivate
// /////////////////////////////////////////////////////////////////

class gnomonSessionProcessPrivate
{
public:
    gnomonSessionProcessPrivate();
    ~gnomonSessionProcessPrivate();
};

gnomonSessionProcessPrivate::gnomonSessionProcessPrivate(/* args */)
{
}

gnomonSessionProcessPrivate::~gnomonSessionProcessPrivate()
{
}


// /////////////////////////////////////////////////////////////////
// gnomonSessionProcess
// /////////////////////////////////////////////////////////////////

gnomonSessionProcess::gnomonSessionProcess(/* gnomonSession *session */)
{
    d = new gnomonSessionProcessPrivate;
}

gnomonSessionProcess::~gnomonSessionProcess()
{
}

void gnomonSessionProcess::save_session(/**/)
{
}

void gnomonSessionProcess::load_session(const string&, const string&)
{
}
//
// gnomonSessionProcess.cpp ends here
