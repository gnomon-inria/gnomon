#pragma once

class gnomonSessionProcess
{

public:
    gnomonSessionProcess(/* gnomonSession *session */);
    ~gnomonSessionProcess();

public:
    void save_session(/* */);
    void load_session(const string&, const string&);

private:
    class gnomonSessionProcessPrivate *d;

};

//
// gnomonSessionProcess.h ends here
