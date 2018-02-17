# -*- coding: utf-8 -*-

import sys
import os
import re
import subprocess

# -- General configuration ------------------------------------------------

#needs_sphinx = '1.0'
sys.path.insert(0, os.path.abspath('./sphinx'))
extensions = [
    'singletext',
    'globalindex' # fnch.users.sourceforge.net/sphinxindexinsinglehtml.html (modified)
]
templates_path = ['_templates']
source_suffix = '.rst'
#source_encoding = 'utf-8-sig'
master_doc = 'wla-dx'
project = u'wla-dx'
copyright = u'2016, vhelin'
# Set later in the code!
version = '1.0' # The short X.Y version, can be used with |version|
release = '1.0' # The full version, including alpha/beta/rc tags, |release|
language = None
#today = ''
#today_fmt = '%B %d, %Y'
exclude_patterns = []
#default_role = None # Default reST role (`text`) to use for all documents
#add_function_parentheses = True # Append () to :func: etc.
#add_module_names = True
#show_authors = False # sectionauthor and moduleauthor directives will be shown
pygments_style = 'sphinx' # The name of the syntax highlighting style to use
#modindex_common_prefix = [] # List of ignored prefixes for module index sorting
#keep_warnings = False # Keep warnings in documents

repo_dir = os.getenv('REPO_DIR', '..')
version_file_name = os.getenv('VERSION_FILE_NAME', 'VERSION')
version_file = os.getenv('VERSION_FILE', repo_dir + '/' + version_file_name)
use_git_version = os.getenv('USE_GIT_VERSION', 'true') in ('true', 'on', 'ON')
git_repo_dir = os.getenv('GIT_REPO_DIR', repo_dir)
version_info_h_file = os.getenv('VERSION_INFO_H_FILE', '')

if version_info_h_file:
    with open(version_info_h_file, 'rt') as h_file:
        release = re.search('#define\s+VERSION_FULL_STRING\s+\"v?([^"]+)\"',
                            h_file.read()).group(1)
elif use_git_version:
    proc = subprocess.Popen('git describe --always --dirty'.split(' '),
            cwd=repo_dir, stdout=subprocess.PIPE)
    stdout, stderr = proc.communicate()
    if proc.returncode != 0:
        raise subprocess.CalledProcessError("Couldn't git describe: %d"
                                            % proc.returncode)
    git_describe = stdout

    proc = subprocess.Popen('git rev-parse --abbrev-ref HEAD'.split(' '),
            cwd=repo_dir, stdout=subprocess.PIPE)
    stdout, stderr = proc.communicate()
    if proc.returncode != 0:
        raise subprocess.CalledProcessError("Couldn't git rev-parse: %d"
                                            % proc.returncode)
    git_branch = stdout

    release = "g-%s-%s" % (git_branch, git_describe)
    if re.search('[0-9]+(\.[0-9]+)+', git_describe) is None:
        # There is no correct tag, so let's use the VERSION file
        verfilever = 1.0
        try:
            with open(version_file, 'rt') as verfile:
                verfilever = verfile.read().replace('\n', '')
        except (OSError, IOError) as e:
            print("Couldn't open version file for git! Using 1.0")
        release = verfilever + "-" + release
else:
    try:
        with open(version_file, 'rt') as verfile:
            release = verfile.read().replace('\n', '')
    except (OSError, IOError) as e:
        print("Couldn't open version file! Using 1.0")

version = re.search('[0-9]+(\.[0-9]+)+', release)
if version is not None:
    version = version.group(0)
else:
    print("Couldn't find version from '%s'! Using 1.0" & (release,))
    version = 1.0


# -- Options for HTML output ----------------------------------------------

html_theme = 'theme'
#html_theme_options = {}
html_theme_path = ["."] # Custom theme paths
#html_title = None # Default: "<project> v<release> documentation"
#html_short_title = None # Default: html_title
#html_logo = None
#html_favicon = None
#html_static_path = ['_static']
#html_extra_path = []
#html_last_updated_fmt = '%b %d, %Y'
#html_use_smartypants = True
#html_sidebars = {}
#html_additional_pages = {}
#html_domain_indices = True
#html_use_index = True
#html_split_index = False
#html_show_sourcelink = True
#html_show_sphinx = True
#html_show_copyright = True
#html_use_opensearch = ''
#html_file_suffix = None
htmlhelp_basename = 'wla-dx'


# -- Options for LaTeX output ---------------------------------------------

latex_elements = {
#'papersize': 'letterpaper', # The paper size ('letterpaper' or 'a4paper').
#'pointsize': '10pt', # The font size ('10pt', '11pt' or '12pt').
#'preamble': '', # Additional stuff for the LaTeX preamble.
}

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title,
#  author, documentclass [howto, manual, or own class]).
latex_documents = [
  ('wla-dx', 'wla-dx.tex', u'wla-dx Documentation',
   u'vhelin', 'manual'),
]
#latex_logo = None
#latex_use_parts = False
#latex_show_pagerefs = False
#latex_show_urls = False
#latex_appendices = []
#latex_domain_indices = True


# -- Options for manual page output ---------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [
    ('wla-dx', 'wla-dx', u'wla-dx Documentation', [u'vhelin'], 7),
    ('man/wlalink', 'wlalink', u'WLA Linker', [u'vhelin'], 1),
    ('man/wlab', 'wlab', u'WLA Binary to DB converter', [u'vhelin'], 1)
]

# Autogenerate man-files for the CPUs
for cpu in ['gb', '65c02', '6502', '6510', '65816', 'huc6280', 'spc700', 'z80']:
    man_pages.append(('man/wla-cpu', 'wla-%s' % cpu, \
                      u'WLA assembler for %s' % cpu, [u'vhelin'], 1))

#man_show_urls = False # If true, show URL addresses after external links.


# -- Options for Texinfo output -------------------------------------------

# Grouping the document tree into Texinfo files. List of tuples
# (source start file, target name, title, author,
#  dir menu entry, description, category)
texinfo_documents = [
  ('wla-dx', 'wla-dx', u'wla-dx Documentation',
   u'vhelin', 'wla-dx', '',
   'Miscellaneous'),
]
#texinfo_appendices = [] # Documents to append as an appendix to all manuals
#texinfo_domain_indices = True # If false, no module index is generated
#texinfo_show_urls = 'footnote'
#texinfo_no_detailmenu = False

if os.environ.get('READTHEDOCS') == 'True':
    html_theme = 'default'
