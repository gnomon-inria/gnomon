import unittest
from gnomoncore import gnomonSphereSeries, gnomonAbstractForm, gnomonSphereForm


class TestDynamicSphere(unittest.TestCase):
    '''
    Tests the gnomonDiscreteDynamicForm class.
    '''

    def setUp(self):
        self.sphere1 = gnomonSphereForm()
        self.sphere1.setRadius(1)
        self.sphere2 = gnomonSphereForm()
        self.sphere2.setRadius(2)

        self.dynamic_form = gnomonSphereSeries()

    def tearDown(self):
        pass

    def test_gnomonDiscreteDynamicForm_append(self):
        self.dynamic_form.insert(1., self.sphere1)

    def test_gnomonDiscreteDynamicForm_atTime(self):
        self.dynamic_form.insert(2., self.sphere2)
        assert self.dynamic_form.at(2.) is not None
        # assert self.dynamic_form.atTime(gnomonTime(2)) is None
        assert self.dynamic_form.at(2.).asSphere().radius() == 2



#
# test_gnomonDiscreteDynamicForm.py ends here.