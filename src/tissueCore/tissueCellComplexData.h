class tissueCellComplexData
{

public:
    tissueCellComplexData(void) {
        ;
    };
    tissueCellComplexData(const tissueCellComplexData& cellcomplex);
    virtual ~tissueCellComplexData(void) {
        ;
    };


public:
    virtual int dimension(void) const;
    virtual bool isValid(void) const;

//  ///////////////////////////////////////////////////////////////////
//  Element concept
//  ///////////////////////////////////////////////////////////////////

    virtual bool hasElement(int dimension, unsigned long id) const;

    virtual unsigned long* elementIds(int dimension) const;

//  ///////////////////////////////////////////////////////////////////
//  Incidence concept
//  ///////////////////////////////////////////////////////////////////

    virtual unsigned long* borders(int dimension, unsigned long id) const;
    virtual int nbBorders(int dimension, unsigned long id) const;
    virtual unsigned long* regions(int dimension, unsigned long id) const;
    virtual int nbRegions(int dimension, unsigned long id) const;

// OR

    virtual unsigned long* incidentElementIds(int dimension, unsigned long id, int incidenceDimension) const;
    virtual int nbIncidentElements(int dimension, unsigned long id, int incidenceDimension) const;

}