
from sphinx.domains import Domain, ObjType
from sphinx.directives import ObjectDescription

class WLADXDirective(ObjectDescription):
    pass

class WLADXDirectiveType(ObjType):
    pass

class WLADXDomain(Domain):
    name = 'wladx'
    label = 'WLA DX'
    object_types = {
            'div': None
    }
    directives = {
        'div': WLADXDirective
    }

def setup(app):
    app.add_domain(WLADXDomain)
