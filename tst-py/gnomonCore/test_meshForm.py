import unittest
import numpy as np
from gnomon.core import initialize, gnomonMesh, meshData_pluginFactory


class TestMesh(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        initialize("",True)

    def setUp(self):
        self.mesh_d_factory = meshData_pluginFactory()
        self.mesh_data = self.mesh_d_factory.create("gnomonMeshDataStdVector")
        self.mesh = gnomonMesh(self.mesh_data)

    def tearDown(self):
        pass

    def test_gnomonMesh_factory(self):
        assert "gnomonMeshDataStdVector" in meshData_pluginFactory().keys()

    def test_gnomonMesh_dataName(self):
        assert self.mesh.data().dataName() == "gnomonMesh"
        assert self.mesh.data().pluginName() == "gnomonMeshDataStdVector"

    def test_gnomonMesh_attribute(self):
        data = np.ones(5, dtype=np.float64)
        att = {'name': 'titi', 'kind': 1, 'support': 1, 'data': data}

        self.mesh.data().addAttribute(att);
        assert(self.mesh.attributesCount() == 1)

        att_back = self.mesh.attribute('titi')
        assert att_back
        assert att_back['kind'] == 1
        assert att_back['support'] == 1
        assert att_back['data'].size == 5
        assert att_back['data'][0] == 1.
