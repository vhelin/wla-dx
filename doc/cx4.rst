Cx4 Notes
=========

WLA Cx4 targets the Capcom Cx4 instruction format used by the Hitachi HG51B169.
The backend is usable as a normal WLA target, but control flow deserves a short
CPU-specific note because jump, branch, and call operands are page-local.


Program Page Model
------------------

Cx4 control flow is split into two parts:

- the opcode operand supplies the low 8 bits of the target program counter
- the current program page stays unchanged unless the instruction uses the
  far form

In WLA syntax, the far form is written with ``.F``. For example:

- ``JMP <label`` keeps the current program page and loads the low target byte
- ``JMP.F <label`` loads the low target byte and also transfers the page
  selector from ``P``
- the same rule applies to ``BRA``, ``BSR``, ``JSR``, and the conditional forms

WLA does not infer the page setup from a label. You must manage the page
selector explicitly with ``MOV P,x``, ``LD P,x``, ``MOV PH,x``, and
``MOV PL,x``.


Labels And Operands
-------------------

For Cx4 control-flow operands, prefer explicit byte selection with
``<label`` or ``lobyte(label)``.

Example::

    MOV P, >RenderLoop
    JSR.F <RenderLoop

This keeps the intent obvious:

- ``>RenderLoop`` selects the page byte used to set up ``P``
- ``<RenderLoop`` selects the low 8-bit PC value encoded in the jump or call

Plain label operands such as ``JMP RenderLoop`` only work when the final label
value already fits in one byte. That is a linker-level 8-bit relocation rule in
WLA, not a full page-aware Cx4 relocation.


Recommended Pattern
-------------------

For calls or jumps to code outside the current 256-byte page, keep the page load
and the far control-flow instruction next to each other::

    MOV P, >MathInit
    JSR.F <MathInit

For local control flow inside the current page, use the non-far form::

    JSR <LocalHelper
    BRA <LoopTop


Practical Notes
---------------

- The Cx4 backend in WLA emits 16-bit instruction words as low byte first,
  matching the rest of the existing tests.
- The dedicated regression tests under ``tests/cx4`` cover mnemonic encoding,
  arithmetic expressions, and linker-style external symbol resolution for
  page-local targets.
- If you want to keep source readable, use ``<label`` and ``>label`` directly
  instead of baking page and offset constants by hand.