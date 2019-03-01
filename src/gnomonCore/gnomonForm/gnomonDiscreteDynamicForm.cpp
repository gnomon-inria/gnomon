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

#include "gnomonCore.h"
#include "gnomonDiscreteDynamicForm.h"

#include "gnomonTime.h"

#include <QtGlobal>

class gnomonDiscreteDynamicFormPrivate
{
public:
    QMap<gnomonTime, gnomonAbstractForm*> forms;
    gnomonTime initialTime;
};


gnomonDiscreteDynamicForm::gnomonDiscreteDynamicForm(void) : d(new gnomonDiscreteDynamicFormPrivate)
{
}

gnomonDiscreteDynamicForm::gnomonDiscreteDynamicForm(const gnomonDiscreteDynamicForm& o) : d(new gnomonDiscreteDynamicFormPrivate)
{
    d->forms = o.d->forms;
    d->initialTime = o.d->initialTime;
}

gnomonDiscreteDynamicForm::~gnomonDiscreteDynamicForm(void)
{
    delete d;
}


void gnomonDiscreteDynamicForm::setInitialTime(gnomonTime initialTime)
{
    d->initialTime = initialTime;
}

gnomonAbstractForm* gnomonDiscreteDynamicForm::atTime(gnomonTime t)
{
    // Q_ASSERT_X(d->forms.contains(t), "atTime", "Invalid time position : the form is not defined at this time");
    //if(!d->forms.contains(t))
    //    d->forms.insert(t, d->forms.last()->clone());

    return d->forms[t];
}

gnomonDiscreteDynamicForm& gnomonDiscreteDynamicForm::operator = (const gnomonDiscreteDynamicForm& o)
{
    if (this == &o)
        return *this;

    d->forms = o.d->forms;
    d->initialTime = o.d->initialTime;

    return (*this);
}

void gnomonDiscreteDynamicForm::insert(gnomonAbstractForm* form, gnomonTime t)
{
    // Q_ASSERT_X(!d->forms.contains(t), "appendForm", "Invalid time position : the form is already defined at this time");
    d->forms[t] = form;
}

void gnomonDiscreteDynamicForm::drop(gnomonTime t)
{
    // Q_ASSERT_X(d->forms.contains(t), "atTime", "Invalid time position : the form is not defined at this time");
    d->forms.remove(t);
}

QList<gnomonTime> gnomonDiscreteDynamicForm::availableTimes(void)
{
    return d->forms.keys();
}

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DEFINE_CONCEPT(gnomonDiscreteDynamicForm, discreteDynamicForm, gnomonCore);
}

//
// gnomonDiscreteDynamicForm.cpp ends here
