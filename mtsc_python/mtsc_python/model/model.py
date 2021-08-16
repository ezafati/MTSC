from utilities import read_solid_cata
from utilities import read_fpg_cata


class Model:
    def __init__(self, typem=None):
        self._type = typem
        self.nbcomp = 2  # components total number: default is 2

    @property
    def mtype(self):
        return self._type

    def get_fpg(self, typelem=None, typemat=None):
        db = read_solid_cata()
        elem_list = db[self.mtype]
        if isinstance(typelem, str):
            elem_infos = list(filter(lambda p: p["ElemType"] == typelem, elem_list))[0]
        else:
            raise TypeError(f"typelem argument should be a string. {typelem} is not of type string")

        if elem_infos["MatElem"]:
            fpg = elem_infos["MatElem"].get(typemat, None)
            list_pg = read_fpg_cata().get(fpg, None)
            assert fpg is not None and list_pg is not None
            nbpg = len(list_pg["weights"])
            tmp_res = []
            tmp_res += [nbpg] + list_pg["pgcoords"] + list_pg["weights"]
            return tmp_res
        else:
            return None
