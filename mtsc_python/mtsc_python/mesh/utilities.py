import json
import os


def readeleminfos(x):
    """add comments """
    db_path = os.path.join(os.path.dirname(globals().get('__file__')), 'database/gmshdb.json')
    with open(db_path) as fn:
        db = json.load(fp=fn)
        try:
            infos = db[str(int(x))]
        except KeyError:
            infos = None
    if infos:
        return infos
    else:
        raise NotImplementedError(f"Element of type {x} not found in database")





