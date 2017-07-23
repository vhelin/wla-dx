import codecs
from os import path
from docutils import nodes
from docutils.utils import new_document
from sphinx.builders.text import TextBuilder
from sphinx.util import logging
from sphinx.util.nodes import inline_all_toctrees
from sphinx.util.console import bold, darkgreen
from sphinx import addnodes
from sphinx.writers.text import TextWriter, TextTranslator
from sphinx.environment.adapters.toctree import TocTree

logger = logging.getLogger(__name__)

class SingleFileTextWriter(TextWriter):
    supported = ('singletext',)

class SingleFileTextTranslator(TextTranslator):
    def visit_start_of_file(self, node):
        pass
    def depart_start_of_file(self, node):
        pass

class SingleFileTextBuilder(TextBuilder):
    name = 'singletext'
    default_translator_class = SingleFileTextTranslator

    def get_outdated_docs(self):
        return 'all documents'

    def prepare_writing(self, docnames):
        self.writer = SingleFileTextWriter(self)

    def write(self, *ignored):
        mastername = self.config.master_doc
        mastertree = self.env.get_doctree(mastername)
        mastertitle = u'%(project)s v%(release)s documentation' % \
                {'project': self.config.project, 'release': self.config.release}
        if hasattr(self.config, 'text_title') and self.config.text_title is not None:
            mastertitle = self.config.text_title

        logger.info(bold('preparing documents... '), nonl=True)
        self.prepare_writing(self.env.found_docs)
        logger.info('done')

        logger.info(bold('assembling single document... '), nonl=True)
        tree = inline_all_toctrees(self, set(), mastername,
                mastertree, darkgreen, [mastername])
        tree['docname'] = mastername
        toctree = TocTree(self.env).get_toctree_for(mastername, self, False)
        tree.insert(0, nodes.section() + nodes.title(mastertitle, mastertitle))
        tree.insert(1, toctree)
        self.env.resolve_references(tree, mastername, self)
        logger.info('done')

        logger.info(bold('writing... '), nonl=True)
        self.write_doc_serialized(mastername, tree)
        self.write_doc(mastername, tree)
        logger.info('done')

def setup(app):
    app.add_builder(SingleFileTextBuilder)