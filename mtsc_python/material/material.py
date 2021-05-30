class Material:
    def __init__(self):
        self._type = None

    def get_material_properties(self):
        return 0


class SolidMaterial(Material):
    def __init__(self):
        super().__init__()
        self._type = 'SOLID'
