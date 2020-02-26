#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonTreeFromLStringCommand : public gnomonAbstractCommand
{
public:
     gnomonTreeFromLStringCommand(void) = delete;
     gnomonTreeFromLStringCommand(const QString&);
    ~gnomonTreeFromLStringCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setLSystem(const QString& lsystem);

    void setInput(gnomonLStringSeries *);
    gnomonLStringSeries *input();

    gnomonTreeSeries *output();

public:
    void setParameter(const QString&, const QVariant&);
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public:
    static bool isEmpty(void);

private:
    class gnomonTreeFromLStringCommandPrivate *d;
};
