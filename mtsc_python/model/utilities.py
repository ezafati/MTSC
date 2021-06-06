import json
import os


def read_solid_cata():
    """add comments """
    db_path = os.path.join(os.path.dirname(globals().get('__file__')), 'database/elem.json')
    with open(db_path) as fn:
        db = json.load(fp=fn)
        return db


def read_fpg_cata():
    """add comments """
    db_path = os.path.join(os.path.dirname(globals().get('__file__')), 'database/fpg.json')
    with open(db_path) as fn:
        db = json.load(fp=fn)
        return db


def add_elem_solid_cata(physic=None, elemtype=None, matelem=None):
    db_path = os.path.join(os.path.dirname(globals().get('__file__')), 'database/elem.json')
    template = {
        "ElemType": elemtype,
        "MatElem": matelem
    }
    with open(db_path) as fn:
        db = json.load(fn)
        db[physic].append(template)

    with open(db_path, 'w') as fn:
        json.dump(db, fn, sort_keys=True, indent=4)
