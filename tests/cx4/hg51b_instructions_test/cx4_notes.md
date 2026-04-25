# cx4_notes.md — Things that will bite Cx4 / HG51B programmers on SNES

Field notes collected while writing this test. Every item is something we
hit on real hardware (ares 147 with the real HG51B interpreter) and lost
time on. Listed in roughly decreasing nastiness.

---

## 1. Never put anything a CPU routine might touch at WRAM `$7E:0000..$7E:001F`

Direct Page defaults to `$0000` on reset. Every `STZ $00..$1F`, `STA $0x`,
`LDY $00,`, etc. the init code does aliases to `$7E:0000..$7E:001F`.

So if you copy a small kick / polling stub into `$7E:0000` (a common idiom
because `JSL $7E0000` is tiny), any pre-kick rendering code that happens
to use direct-page scratch at `$00..$0F` will silently corrupt the first
bytes of your stub. The Cx4 then never starts, your poll times out, and
from the outside it looks like every GPR came back zero.

**Fix:** stage the WRAM blob at `$7E:0200` or any address beyond your DP
scratch area. `JSL $7E0200` works just as well.

Symptom to recognize: a ~10 second pause during boot (your WRAM poll's
timeout), followed by all GPRs except the seeded ones reading `$00`.
Dump `$7E:0000..` in the emulator's memory viewer — if the first opcodes
don't match your source, this is the bug.

---

## 2. The kick / poll stub MUST run from WRAM, not ROM

While the HG51B is "busy" (cache loading or running), ares' HitachiDSP
read path returns IO-window bytes for bank-00 cart-ROM fetches. If your
polling loop is executing from ROM, it reads garbage into the opcode
stream and wanders off.

**Fix:** copy the poll stub into WRAM and `JSL` into it. Any WRAM address
works (subject to rule #1).

---

## 3. `VMADDL` / `VMADDH` (`$2116`/`$2117`) are WRITE-ONLY

Do not try to read-modify-write them. If you need to know the current
VRAM address, track it in a DP byte. Reading returns open-bus / junk and
will silently give you wrong tiles in hex-dump routines.

---

## 4. `LDY dp` in 16-bit index mode reads TWO bytes

After `REP #$10`, `LDY _tmp` is a 16-bit load. The high byte of Y comes
from the DP byte *immediately after* `_tmp`. If that neighbouring byte
isn't zero, your index is silently polluted — and it is easy for it to
be nonzero when that same DP address also serves as 8-bit scratch for
something else earlier.

**Fix:** reserve an explicit `_tmp_hi` byte that stays zero. Either
`STZ _tmp_hi` once at boot, or declare it as the very next DP symbol and
never write to it. This test does the former.

Symptom: some table rows show garbage bytes where a previous row's
readback value has leaked into the index for the next row's fetch.

---

## 5. `A` on the HG51B is 24-bit, not 8-bit

Every ALU op — `MOV A,imm`, `ADD`, `AND`, shifts, etc. — operates on the
full 24-bit accumulator. `MOV A,$80` puts `$00_00_80`, not `$80`, in A.
Consequences:

- `SHR A,n` on `$80` with n>=8 does NOT produce `$00` by virtue of being
  narrow — the result is `$00_00_00` because the whole 24-bit value is
  shifted.
- `SXB A` sign-extends bit 7 to bits 8..23. Start with `$00_00_80`, end
  with `$FF_FF_80`. That's why `R12` in this test is the only row where
  byte-1 and byte-2 are nonzero — it proves the full 24-bit path, not
  just the low byte.
- `ROR`/`ROTR` rotate across all 24 bits. A single-step `ROR A,1` on
  `$00_00_01` ends up as `$80_00_00` — byte-0 goes to zero!

When you want byte-0 to do the intuitive thing, either use SHL/SHR by
small amounts or mask explicitly with `AND A<<0,$FF`.

---

## 6. `CMP` and `CMPR` only set flags. `SUB` and `SUBR` write A.

- `CMP A,r` → flags of (A - r). A is unchanged.
- `CMPR A,r` → flags of (r - A). A is unchanged.
- `SUB A,r` → A := A - r.
- `SUBR A,r` → A := r - A.

Easy to reach for `CMP` when you meant `SUB`, or vice versa. If your
arithmetic "doesn't do anything" check that you didn't pick a CMP
variant.

---

## 7. `SKIPxx` skips ONE instruction, not one "statement"

`SKIPEQ` / `SKIPNE` / `SKIPGE` / `SKIPLT` / `SKIPMI` / `SKIPPL` /
`SKIPVS` / `SKIPVC` all conditionally skip exactly the next 2-byte
opcode word. If you try to skip a branch, remember the branch target is
encoded in the opcode after the skip, not two instructions ahead.

The test in `cx4_prog.s` validates SKIPEQ and SKIPNE with arithmetic
chosen so that a buggy "skip didn't skip" / "skip skipped when it
shouldn't" produces a DIFFERENT final byte than the correct behaviour.
Always design skip tests this way; if both paths give the same result,
you aren't actually testing the skip.

---

## 8. Cx4 microcode writes to GPRs take a beat to become visible via MMIO

Don't read-back GPRs the instant you issue `JSL` into the kick stub.
Wait on a sentinel — the last GPR your microcode writes — to change to a
known value. In this test, R15 byte-0 going to `$AB` is the "done"
signal. Only after that is every earlier GPR (`R2..R14`) guaranteed to
reflect the microcode's final state.

---

## 9. `ADD A,imm` needs the shift specifier; `ADD A,rN` does not

Register form: `ADD A,r3` assembles fine (shift defaults to 0).
Immediate form: `ADD A,$55` is NOT accepted by wla-cx4. You must write
`ADD A<<0,$55`. Same rule applies to `SUB`, `SUBR`, `AND`, `OR`, `XOR`,
`XNOR`, `CMP`, `CMPR`. Possible shift counts are 0, 1, 8, 16 (see
icx4.c).

---

## 9b. WLA-DX emits BYTE addresses for Cx4 branch operands (HG51B wants WORDS)

The HG51B program counter counts WORDS — every instruction is exactly
2 bytes and `pc` advances by 1 per instruction. So a branch operand is
a word-index into the cache page, not a byte offset.

wla-cx4, however, resolves label-relative operands as BYTE addresses and
stuffs them straight into the instruction. So `BRA label` where `label`
is two words away emits `BRA $04` (byte distance) instead of `BRA $02`
(word distance). On ares the Cx4 silently jumps to the wrong location,
executes garbage, and usually never writes your "done" sentinel — the
CPU kick-poll times out and every GPR past the last successful one
reads back as its cleared initial value.

**Observed symptoms:** your R15 sentinel never fires, the CPU pauses for
the full RamKickBlob timeout, and the screen shows several GPRs stuck at
$00 starting at the slot that contained the branch.

**Workaround:** avoid `BRA`, `BSR`, `RTS`, `JMP`, `JSR`, `BEQ`, `BNE`,
`BGE`, `BMI`, `BVS`, `BSR*`, `JSR*` forms with label operands. If you
absolutely need a jump, compute the word-address yourself as a numeric
literal and pass it directly, e.g. `JMP $05` (jump to word 5). This is
fragile because any edit that moves instructions around will break it,
but it's the only way until wla-cx4 divides branch operands by 2.

**What you CAN do without branches:** `SKIPxx` instructions are 1-word
skips that don't take an operand, so they assemble correctly and work
fine for conditional tests. Use them freely.

---

## 10. `SHL A,n` etc. take the shift count as an IMMEDIATE (0..15)

Different from the "A<<n" shift-in prefix used by the ALU-ops above.
`SHL A,4`, `SHR A,4`, `ASR A,3`, `ROR A,1` all take a simple small
immediate count. `SHLL`, `SHLR`, `SHAR`, `ROTR` are just aliases (see
icx4.c — same opcodes).

---

## 11. WLA-DX assembles Cx4 words in operand-then-opcode order

Every Cx4 instruction is 2 bytes, and in ROM the order is
`[operand byte] [opcode byte]` (little-endian as a 16-bit word). So the
annotation `; @BT 12 85` on `ADD A<<1,$12` means ROM contains `12 85`,
which reads back as the 16-bit word `$8512`. Useful when disassembling
`cx4_prog.bin` by eye.

---

## 12. Minimal Cx4 register map for driving microcode from CPU

```
$7F48   cache.page       ; write 0 then write pc/base to start loading
$7F49   cache.base low   ; SNES-bus address of microcode (24-bit)
$7F4A   cache.base mid
$7F4B   cache.base high
$7F4D   cache.pb low     ; program bank (usually 0)
$7F4E   cache.pb high
$7F4F   cache.pc         ; writing clears halt and starts execution
$7F53   soft reset       ; STZ to re-arm
$7F5F   status
$7F80+3*N+k              ; GPR N byte k (k=0..2, 24-bit wide)
```

Canonical kick sequence (from WRAM, rule #2):

```
STZ $7F48            ; cache.page = 0
STZ $7F4F            ; cache.pc = 0  -> begin execution
poll:  LDA $7FAD     ; R15 byte-0
       CMP #$AB
       BNE poll      ; (with a timeout counter in X/Y)
```

Before the kick, the CPU should have already written `cache.base`,
`cache.pb`, and seeded any input GPRs (R0, R1, ...).
