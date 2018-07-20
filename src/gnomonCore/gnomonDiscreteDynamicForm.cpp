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

#include <QtGlobal>

class gnomonDiscreteDynamicFormPrivate
{
public:
    QMap<gnomonTime, gnomonAbstractForm*> forms;
    gnomonTime initialTime;
};


gnomonDiscreteDynamicForm::gnomonDiscreteDynamicForm(void)
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

gnomonAbstractForm* gnomonDiscreteDynamicForm::atTime(gnomonTime t)
{
	// Q_ASSERT_X(d->forms.contains(t), "atTime", "Invalid time position : the form is not defined at this time");
	return d->forms[t];
}

void gnomonDiscreteDynamicForm::appendForm(gnomonAbstractForm* form, gnomonTime t)
{
	// Q_ASSERT_X(!d->forms.contains(t), "appendForm", "Invalid time position : the form is already defined at this time");
	d->forms[t] = form;
}

void gnomonDiscreteDynamicForm::dropForm(gnomonTime t)
{
	// Q_ASSERT_X(d->forms.contains(t), "atTime", "Invalid time position : the form is not defined at this time");
	d->forms.remove(t);
}

void gnomonDiscreteDynamicForm::setInitialTime(gnomonTime initialTime)
{
	d->initialTime = initialTime;
}

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DEFINE_CONCEPT(gnomonDiscreteDynamicForm, discreteDynamicForm, gnomonCore);
}

//
// gnomonDiscreteDynamicForm.cpp ends here
