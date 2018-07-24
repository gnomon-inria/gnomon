# Version: $Id$
#
#

# Commentary:
#
#

# Change Log:
#
#

# Code:

import sys
from PyQt5.QtCore import QSettings

settings = QSettings(QSettings.IniFormat,QSettings.UserScope,"inria","dtk-script")
settings.beginGroup("modules");
paths = settings.value("path")
settings.endGroup()

for path in paths.split(":"):
    sys.path.append(path)


import gnomoncore
from gnomoncore import gnomonCellComplex
from vplants.cellcomplex.property_topomesh.property_topomesh_creation import triangle_topomesh

import gnomonCellComplexDataPropertyTopomesh


class TestGnomonCellComplex:
    '''
    Tests the gnomonCellComplex class.
    '''

    def setUp(self):    
        self.topomesh  = triangle_topomesh([[0, 1, 2]], {0:[0,0,0], 1:[0,1,0], 2:[1,1,0]})
        self.nbr_elements_topomesh = {dimension: len(list(self.topomesh.wisps(dimension))) for dimension in range(4)}

        self.c_cplx = gnomonCellComplex()
        self.c_cplx_data = gnomoncore.cellcomplexdata_pluginFactory().create("gnomonCellComplexDataPropertyTopomesh").from_property_topomesh(self.topomesh)
        self.c_cplx.setData(self.c_cplx_data)


    def tearDown(self):
        self.c_cplx.this.disown()
        self.c_cplx_data.this.disown()


    def test_gnomonCellComplex_element_properties(self):

        # assert c_cplx.dimension() == 3 # WARNING: This method makes gnomon crashes

        assert self.c_cplx.hasElementProperty(0, 'barycenter')


        for dimension in range(4):
            assert self.c_cplx.elementPropertyNames(dimension) == list(self.topomesh.wisp_property_names(dimension))

             # -- Testing that properties have been properly transfered from the propertyTopomesh to the cell complex
            self.c_cplx.elementPropertyNames(dimension) == self.topomesh.wisp_properties(dimension).keys()

            # -- Testing the property manipulation methods
            prop = {wid: 'abcdefg'[wid] for wid in self.topomesh.wisps(dimension)}
            
            self.c_cplx.addElementProperty(dimension, 'prop_dimension_'+str(dimension))
            # self.c_cplx.updateElementProperty(dimension, 'prop_dimension_'+str(dimension), prop) # WARNING: This function does not work !
            # assert self.c_cplx.elementProperty(dimension, 'prop_dimension_'+str(dimension)) == prop
            # self.c_cplx.removeElementProperty(dimension, 'prop_dimension_'+str(dimension))

            
    def test_gnomonCellComplex_elements(self):
        for dimension in range(4):
            for eid in self.topomesh.wisps(dimension):
                assert self.c_cplx.hasElement(dimension, eid)

        for dimension, nbr_elmts_tpmsh in self.nbr_elements_topomesh.iteritems():
            
            # -- Testing that elements in the cell complex matches the ones in the propertyTopomesh
            assert self.c_cplx.elementCount(dimension) == nbr_elmts_tpmsh  
            assert self.c_cplx.elementIds(dimension) == list(self.topomesh.wisps(dimension))


    def test_gnomonCellComplex_edition(self):

        # -- Testing the cell complex edition tools
        # -- Adding elements
        self.c_cplx.addElement(0, 3)

        self.c_cplx.addElement(1, 3)
        self.c_cplx.linkElements(1, 3, 2)
        self.c_cplx.linkElements(1, 3, 3)

        # -- Removing elements
        self.c_cplx.removeElement(1, 3)
        self.c_cplx.removeElement(0, 3)

        assert self.c_cplx.isValid()


    def test_gnomonCellComplex_oriented_vertices(self):
        # -- Testing the computation of the oriented face
        assert self.c_cplx.orientedFaceVertexIds(0) == [0, 1, 2]


    def test_gnomonCellComplex_neighbors(self):
        # -- Testing methods counting and iding the neighbors
        for dimension in range(4)[1:]:
            for eid in self.c_cplx.elementIds(dimension):
                assert eid in list(self.topomesh.wisps(dimension))
                assert self.c_cplx.incidentElementIds(dimension, eid, dimension-1)    == list(self.topomesh.borders(dimension, eid, 1))
                assert self.c_cplx.incidentElementCount( dimension, eid, dimension-1) == len(list(self.topomesh.borders(dimension, eid, 1)))
                assert self.c_cplx.adjacentElementIds(dimension, eid, dimension-1)    == list(self.topomesh.border_neighbors(dimension, eid))
                assert self.c_cplx.adjacentElementCount(dimension, eid, dimension-1)  == len(list(self.topomesh.border_neighbors(dimension, eid)))



# -- Remaing methods to test:
# elementProperty(self, *args)
# 
# setData(self, data)
# 
# setDimension(self, dimension=3)




#
# test_gnomonCellComplex.py ends here.