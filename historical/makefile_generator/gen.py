from string import Template
import os

# Config
output_folder = '../makefiles'
compile_command = '$(CC) $(CFLAGS)'
template_delimiter = '@'

warning_message = [
    '###################################################',
    '# WARNING: This file was automatically generated. #',
    '###################################################',
]

cpus = [
    {'name': '6502', 'define': 'MCS6502'},
    {'name': '65816', 'define': 'W65816'},
    {'name': '65c02', 'define': 'WDC65C02'},
    {'name': '65ce02', 'define': 'CSG65CE02'},
    {'name': '68000', 'define': 'MC68000'},
    {'name': '6800', 'define': 'MC6800'},
    {'name': '6801', 'define': 'MC6801'},
    {'name': '6809', 'define': 'MC6809'},
    {'name': '8008', 'define': 'I8008'},
    {'name': '8080', 'define': 'I8080'},
    {'name': 'gb', 'define': 'GB'},
    {'name': 'superfx', 'define': 'SUPERFX'},
    {'name': 'huc6280', 'define': 'HUC6280'},
    {'name': 'spc700', 'define': 'SPC700'},
    {'name': 'z80', 'define': 'Z80'},
    {'name': 'z80n', 'define': 'Z80N'}
]

def main():
    class MyTemplate(Template):
        delimiter = template_delimiter

    # Here we instantiate a new Template for each platform.
    # For now, only MS-DOS and Amiga are generated.
    msdos_stub = open('makefile.msdos.template')
    msdos_stub_contents = msdos_stub.read()
    msdos_stub.close()
    msdos_template = MyTemplate(msdos_stub_contents)

    amiga_stub = open('makefile.amiga.template')
    amiga_stub_contents = amiga_stub.read()
    amiga_stub.close()
    amiga_template = MyTemplate(amiga_stub_contents)

    smake_stub = open('makefile.smake.template')
    smake_stub_contents = smake_stub.read()
    smake_stub.close()
    smake_template = MyTemplate(smake_stub_contents)

    # Create output directory if necessary
    if not os.path.exists(output_folder):
        print('Created output directory: ' + output_folder)
        os.mkdir(output_folder)

    for cpu in cpus:
        msdos = generate(cpu, '\r\n', msdos_template)
        amiga = generate(cpu, '\n', amiga_template)
        smake = generate(cpu, '\n', smake_template)

        f = open(output_folder + '/makefile.msdos.' + cpu['name'], "w")
        f.write(msdos)
        f.close()

        f = open(output_folder + '/makefile.amiga.' + cpu['name'], "w")
        f.write(amiga)
        f.close()

        f = open(output_folder + '/smake.' + cpu['name'], "w")
        f.write(smake)
        f.close()
    
    print('Makefiles successfully generated!')


def generate(cpu, new_line, template):
    # First we define a dictionary where each item represents a C source file.
    # The item key is the file name and the value is an array containing its
    # includes. Note that only direct includes are added. Indirect includes
    # are handled next.
    sources = {
        'main.c': [
            'defines.h',
            'hashmap.h',
            'include.h',
            'listfile.h',
            'mersenne.h',
            'parse.h',
            'phase_1.h',
            'phase_2.h',
            'phase_3.h',
            'phase_4.h',
            'printf.h',
        ],
        'crc32.c': [
            'crc32.h'
        ],
        'decode.c': [
            'decode.h',
            'parse.h',
            'phase_1.h',
            'printf.h',
            'stack.h'
        ],
        'hashmap.c': [
            'hashmap.h',
            'crc32.h',
        ],
        'include.c': [
            'include.h',
            'crc32.h',
            'defines.h',
            'parse.h',
            'phase_1.h',
            'printf.h',
        ],
        'listfile.c': [
            'listfile.h',
            'defines.h',
            'include.h',
        ],
        'mersenne.c': [
            'mersenne.h'
        ],
        'parse.c': [
            'parse.h',
            'defines.h',
            'phase_1.h',
            'stack.h',
            'include.h',
            'printf.h',
        ],
        'phase_1.c': [
            'phase_1.h',
            'decode.h',
            'defines.h',
            'include.h',
            'parse.h',
            'phase_2.h',
            'phase_3.h',
            'stack.h',
            'hashmap.h',
            'printf.h',
            'mersenne.h',
        ],
        'phase_2.c': [
            'phase_2.h',
            'defines.h',
            'phase_1.h',
            'phase_4.h',
            'hashmap.h',
            'printf.h',
        ],
        'phase_3.c': [
            'phase_3.h',
            'defines.h',
            'include.h',
            'printf.h',
        ],
        'phase_4.c': [
            'defines.h',
            'include.h',
            'listfile.h',
            'phase_3.h',
            'phase_4.h',
            'parse.h',
            'stack.h',
            'printf.h',
        ],
        'printf.c': [
            'printf.h'
        ],
        'stack.c': [
            'stack.h',
            'defines.h',
            'hashmap.h',
            'parse.h',
            'phase_1.h',
            'include.h',
            'printf.h',
        ],
        't' + cpu['name'] + '.c': [],
        'i' + cpu['name'] + '.c': [],
    }

    # It's possible that an header such as defines.h includes other headers.
    # So we use this dictionary to automatically add those as well.
    headers = {
        'defines.h': [
            'hashmap.h',
            'shared.h',
        ]
    }

    # Now build the rules list by iterating the source files
    # and adding their direct and indirect dependencies. 
    rules = []

    for source, deps in sources.items():
        obj = source[:-2] + '.o'
        rule = obj + ': ' + source + ' '
        rule_deps = []
        for dep in deps:
            rule_deps.append(dep)
            if dep in headers:
                rule_deps += headers[dep]

        rule += '{}{}\t{} {}'.format(
            ' '.join(rule_deps),
            new_line,
            compile_command,
            source
        )

        rules.append(rule)
    
    rules.sort()
    
    # After all rules for this CPU are created, we prepare the variables to
    # substitute in the templates, apply them and return the results
    substitutions = {
        'CPU': cpu['name'],
        'CPU_DEFINE': cpu['define'],
        'OFILES': ' '.join([s[:-2] + '.o' for s in sources]),
        'RULES': (new_line*2).join(rules),
    }

    result = template.safe_substitute(substitutions)
    
    if not os.path.exists(output_folder):
        os.mkdir(output_folder)

    return new_line.join(warning_message) + new_line*2 + result


main()
