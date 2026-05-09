# include/neogeo/neogeo.mk
#
# Optional GNU-make fragment that packages WLA DX Neo Geo build outputs
# into a MAME-style ROM-set ZIP. Include this file from your game
# makefile after your own compile/link rules, set the variables listed
# below, and invoke the 'neogeo-romset' target.
#
#   include $(WLA_DX_PREFIX)/include/neogeo/neogeo.mk
#
# Required variables:
#   NG_GAME_ID     - ROM-set name (no extension), e.g. "hello".
#   NG_P1          - Path to the P1 68000 ROM image (.bin).
#
# Optional variables (set any that apply to your cartridge layout):
#   NG_M1          - M1 Z80 program ROM (.bin).
#   NG_S1          - S1 fix-layer tile ROM (.bin).
#   NG_C1, NG_C2   - C1/C2 sprite ROM pair (.bin each).
#   NG_C3, NG_C4   - optional second C-ROM pair.
#   NG_V1          - V1 ADPCM ROM (.bin).
#   NG_V2          - optional second V-ROM.
#   NG_EXTRA_FILES - space-separated list of extra files to include in
#                    the ZIP, each entry formatted as "source=target";
#                    source is copied in and renamed to target inside
#                    the ZIP. Useful for crom.1, srom.1 etc.
#   NG_ZIP         - output ZIP path (default: $(NG_GAME_ID).zip).
#   NG_ZIP_DIR     - staging directory (default: $(NG_GAME_ID)-romset).
#   NG_ZIP_CMD     - override the zip command invocation. If unset, the
#                    rule tries 'zip -q -9' first, then 'python -m
#                    zipfile -c' / 'python3 -m zipfile -c'.
#
# Provided targets:
#   neogeo-romset   - builds $(NG_ZIP) with MAME-style filenames, such
#                     as $(NG_GAME_ID)-p1.bin, -m1.bin, -c1.bin...
#   neogeo-stage    - populates $(NG_ZIP_DIR) with the renamed files
#                     only (useful when no zip tool is available).
#   neogeo-clean    - removes $(NG_ZIP) and $(NG_ZIP_DIR).
#   neogeo-softlist - generates a local MAME neogeo.xml softlist entry.
#   neogeo-run      - launches MAME with -hashpath/-rompath/-cart1.
#
# Note: this fragment only packages files; it does not assemble or link
# anything. Your game makefile is expected to produce each referenced
# .bin via wla-68000 / wla-z80 / wlalink as usual.

ifndef NG_GAME_ID
$(error neogeo.mk: NG_GAME_ID must be set before including this file)
endif

ifndef NG_P1
$(error neogeo.mk: NG_P1 must point at the P1 ROM image)
endif

NG_ZIP     ?= $(NG_GAME_ID).zip
NG_ZIP_DIR ?= $(NG_GAME_ID)-romset
NG_SOFTLIST_SCRIPT ?= ../../../include/neogeo/gen_softlist.py
NG_SOFTLIST_DESC ?= WLA DX $(NG_GAME_ID) homebrew
NG_SOFTLIST_YEAR ?= 2026
NG_SOFTLIST_PUBLISHER ?= WLA DX
NG_MAME_HASHPATH_DIR ?= mame_hashpath
NG_MAME_ROMPATH_DIR ?= mame_rompath
NG_SOFTLIST_XML ?= $(NG_MAME_HASHPATH_DIR)/neogeo.xml
NG_MAME_ROMPATH_ZIP ?= $(NG_MAME_ROMPATH_DIR)/$(notdir $(NG_ZIP))
MAME ?= /cygdrive/c/Users/ville/Downloads/Mame/mame.exe
MAME_BIOS_ROMPATH ?= C:/Users/ville/Downloads/Mame/roms
MAME_FLAGS ?= -window

# Build the list of "source:rom_name" pairs that should be staged.
NG_ROMSET_PAIRS := $(NG_P1):$(NG_GAME_ID)-p1.bin
ifdef NG_M1
NG_ROMSET_PAIRS += $(NG_M1):$(NG_GAME_ID)-m1.bin
endif
ifdef NG_S1
NG_ROMSET_PAIRS += $(NG_S1):$(NG_GAME_ID)-s1.bin
endif
ifdef NG_C1
NG_ROMSET_PAIRS += $(NG_C1):$(NG_GAME_ID)-c1.bin
endif
ifdef NG_C2
NG_ROMSET_PAIRS += $(NG_C2):$(NG_GAME_ID)-c2.bin
endif
ifdef NG_C3
NG_ROMSET_PAIRS += $(NG_C3):$(NG_GAME_ID)-c3.bin
endif
ifdef NG_C4
NG_ROMSET_PAIRS += $(NG_C4):$(NG_GAME_ID)-c4.bin
endif
ifdef NG_V1
NG_ROMSET_PAIRS += $(NG_V1):$(NG_GAME_ID)-v1.bin
endif
ifdef NG_V2
NG_ROMSET_PAIRS += $(NG_V2):$(NG_GAME_ID)-v2.bin
endif
ifdef NG_EXTRA_FILES
NG_ROMSET_PAIRS += $(subst =,:,$(NG_EXTRA_FILES))
endif

# Inputs the ZIP depends on (source paths only).
NG_ROMSET_SOURCES := $(foreach pair,$(NG_ROMSET_PAIRS),$(firstword $(subst :, ,$(pair))))

NG_SOFTLIST_ARGS := --out "$(NG_SOFTLIST_XML)" --game-id "$(NG_GAME_ID)" --description "$(NG_SOFTLIST_DESC)" --year "$(NG_SOFTLIST_YEAR)" --publisher "$(NG_SOFTLIST_PUBLISHER)" --p1 "$(NG_P1)"
ifdef NG_M1
NG_SOFTLIST_ARGS += --m1 "$(NG_M1)"
endif
ifdef NG_S1
NG_SOFTLIST_ARGS += --s1 "$(NG_S1)"
endif
ifdef NG_C1
NG_SOFTLIST_ARGS += --c1 "$(NG_C1)"
endif
ifdef NG_C2
NG_SOFTLIST_ARGS += --c2 "$(NG_C2)"
endif
ifdef NG_V1
NG_SOFTLIST_ARGS += --v1 "$(NG_V1)"
endif
ifdef NG_V2
NG_SOFTLIST_ARGS += --v2 "$(NG_V2)"
endif

.PHONY: neogeo-romset neogeo-stage neogeo-clean neogeo-softlist neogeo-mame-stage neogeo-run

neogeo-stage: $(NG_ROMSET_SOURCES)
	@rm -rf "$(NG_ZIP_DIR)"
	@mkdir -p "$(NG_ZIP_DIR)"
	@$(foreach pair,$(NG_ROMSET_PAIRS), \
	    cp "$(firstword $(subst :, ,$(pair)))" "$(NG_ZIP_DIR)/$(lastword $(subst :, ,$(pair)))"; )
	@echo "neogeo.mk: staged ROM-set files in $(NG_ZIP_DIR)"

neogeo-romset: $(NG_ZIP)

$(NG_ZIP): neogeo-stage
	@rm -f "$(NG_ZIP)"
	@if [ -n "$(NG_ZIP_CMD)" ]; then \
	  cd "$(NG_ZIP_DIR)" && $(NG_ZIP_CMD) "../$(NG_ZIP)" *; \
	elif command -v zip >/dev/null 2>&1; then \
	  cd "$(NG_ZIP_DIR)" && zip -q -9 "../$(NG_ZIP)" *; \
	elif command -v python >/dev/null 2>&1; then \
	  cd "$(NG_ZIP_DIR)" && python -m zipfile -c "../$(NG_ZIP)" *; \
	elif command -v python3 >/dev/null 2>&1; then \
	  cd "$(NG_ZIP_DIR)" && python3 -m zipfile -c "../$(NG_ZIP)" *; \
	else \
	  echo "neogeo.mk: no zip or python available; set NG_ZIP_CMD to override" >&2; \
	  exit 1; \
	fi
	@echo "neogeo.mk: wrote $(NG_ZIP)"

neogeo-softlist: $(NG_SOFTLIST_XML)

$(NG_SOFTLIST_XML): $(NG_ROMSET_SOURCES) $(NG_SOFTLIST_SCRIPT)
	@mkdir -p "$(NG_MAME_HASHPATH_DIR)" "$(NG_MAME_ROMPATH_DIR)"
	@if command -v python >/dev/null 2>&1; then PY=python; \
	 elif command -v python3 >/dev/null 2>&1; then PY=python3; \
	 else echo "neogeo.mk: python or python3 is required for softlist generation" >&2; exit 1; fi; \
	 $$PY "$(NG_SOFTLIST_SCRIPT)" $(NG_SOFTLIST_ARGS)
	@echo "neogeo.mk: wrote $(NG_SOFTLIST_XML)"

$(NG_MAME_ROMPATH_ZIP): $(NG_ZIP)
	@mkdir -p "$(NG_MAME_ROMPATH_DIR)"
	cp "$(NG_ZIP)" "$@"

neogeo-mame-stage: $(NG_SOFTLIST_XML) $(NG_MAME_ROMPATH_ZIP)

neogeo-run: neogeo-romset neogeo-mame-stage
	"$(MAME)" aes \
	  -hashpath ./$(NG_MAME_HASHPATH_DIR) \
	  -rompath "./$(NG_MAME_ROMPATH_DIR);$(MAME_BIOS_ROMPATH)" \
	  -cart1 $(NG_GAME_ID) \
	  $(MAME_FLAGS)

neogeo-clean:
	@rm -rf "$(NG_ZIP_DIR)" "$(NG_ZIP)" "$(NG_MAME_HASHPATH_DIR)" "$(NG_MAME_ROMPATH_DIR)"
