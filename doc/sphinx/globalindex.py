# Note: Modified by Neui (Note: sphinx.util.compat.Directive is deprecated)
#
# Copyright (C) 2011 by Matteo Franchin
#
#   This file is free software: you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published
#   by the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This file is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   A copy of the GNU General Public License is available at
#   <http://www.gnu.org/licenses/>.

from sphinx.builders.html import SingleFileHTMLBuilder
from docutils import nodes
from docutils.parsers.rst import Directive, directives
import re

class globalindex(nodes.General, nodes.Element):
    pass

def visit_globalindex_node(self, node):
    self.body.append(node['content'])

def depart_globalindex_node(self, node):
    pass

class GlobalIndexDirective(Directive):
    required_arguments = 0
    optional_arguments = 1
    final_argument_whitespace = True
    option_spec = \
      {'maxdepth': directives.nonnegative_int,
       'collapse': directives.flag,
       'titlesonly': directives.flag}

    def run(self):
        node = globalindex('')
        node['maxdepth'] = self.options.get('maxdepth', 2)
        node['collapse'] = 'collapse' in self.options
        node['titlesonly'] = 'titlesonly' in self.options
        return [node]

def process_globalindex_nodes(app, doctree, fromdocname):
    builder = app.builder
    if builder.name != SingleFileHTMLBuilder.name:
        for node in doctree.traverse(globalindex):
            node.parent.remove(node)

    else:
        docname = builder.config.master_doc
        for node in doctree.traverse(globalindex):
            kwargs = dict(maxdepth=node['maxdepth'],
                          collapse=node['collapse'],
                          titles_only=node['titlesonly'])
            rendered_toctree = builder._get_local_toctree(docname, **kwargs)
            # For some reason, it refers to docname.html#anchor, where just
            # #anchor is enough.
            rendered_toctree = rendered_toctree.replace(docname + ".html", '')
            # Subsections will be #section#subsection, which is invalid.
            # Removing the first #section fixes this.
            rendered_toctree = re.sub('href="(?:#[^#"]+)*(#[^"]+)"', \
                                      'href="\\1"', rendered_toctree)
            node['content'] = rendered_toctree

def setup(app):
    app.add_node(globalindex,
                 html=(visit_globalindex_node, depart_globalindex_node))
    app.add_directive('globalindex', GlobalIndexDirective)
    app.connect('doctree-resolved', process_globalindex_nodes)
