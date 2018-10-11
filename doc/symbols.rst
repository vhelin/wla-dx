WLA Symbols
===========



Information For Emulator Developers
===================================

The intent for the symbol generation is intended for emulators to provide extra information for developers of ROM patches or programs, especially when trying to debug a program, or otherwise help understand how it operates.

In order to properly support loading of WLA symbol files, it is recommended to follow this specification below, especially so as to gracefully support future changes and additions to the symbol files.

- The file should be read line-by-line
- Any text following a ``;`` should be ignored
- Lines matching ``\[\S+\]`` in regex or ``[%s]`` in scanf code are section headers, and represent a new section. Note that no section data will start with ``[``.
- Lines following the section header are the data for that section. If you're acknowledging the section, utilize that section's specific formatting. Read lines that match until a new section header is encountered.
- Unless otherwise specified, none of the sections contain data that is sorted in any way.

The following are the list of currently supported sections, what they mean, and how their data should be interpreted.

[labels]
--------
This is a list of all Labels to sections of the ROM, such as subroutine locations, or data locations. Each line lists an address in hexadecimal (bank and offset) and a string associated with that address. This data could be used, for example, to identify what section a given target address is in, by searching for the label with the closest address less than the target address.

- Regex match: ``[0-9a-fA-F]{2}:[0-9a-fA-F]{4} .*``
- Format specifier: ``%2x:%4x %s``

[definitions]
This is a list of various definitions provided in code - or automatically during WLA's processing - and values associated with them. Most prominently, WLA outputs the size of each section of the ROM. Each line lists an integer value in hexadecimal, and a string (name) associated with that value.

- Regex match: ``[0-9a-fA-F]{8} .*``
- Format specifier: ``%8x %s``

[source files]
These are used to identify what files were used during the assembly process, especially to map generated assembly back to source file contents. Each line lists a hexadecimal file index, a hexadecimal CRC32 checksum of the file, and a file path relative to the generated ROM's root. This could be used to load in the contents of one of the input files when running the ROM and verifying the file is up-to-date by checking its CRC32 checksum against the one generated during assembly.

- Regex match: ``[0-9a-fA-F]{4} [0-9a-fA-F]{8} .*``
- Format specifier: ``%4x %8x %s``

[rom checksum]
This is just a single line identifying what the hexadecimal CRC32 checksum of the ROM file was when the symbol file was generated. This could be used to verify that the symbol file itself is up-to-date with the ROM in question. This checksum is calculated by reading the ROM file's entire binary, and not by reading any platform-specific checksum value embedded in the ROM itself.

- Regex match:  ``[0-9a-fA-F]{8}``
- Format specifier: ``%8x``

[addr-to-line mapping]
This is a listing of hexadecimal ROM addresses (bank and offset) each mapped to a hexadecimal file index and hexadecimal line index. The file index refers back to the file indices specified in the ``source files`` section, so that the source file name can be discovered. This information can be used to, for example, display source file information in line with disassembled code, or to communicate with an external text editor the location of the current Program Counter by specifying a source file and line instead of some address in the binary ROM file. 

- Regex match: ``[0-9a-fA-F]{2}:[0-9a-fA-F]{4} [0-9a-fA-F]{4}:[0-9a-fA-F]{8}``
- Format specifier: ``%2x:%4x %4x:%8x``
