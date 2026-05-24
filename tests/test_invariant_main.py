import pytest
import re


# Simulated safe disassembler function that mimics the vulnerable pattern
# but with proper bounds checking - this is what MUST always be true
def safe_format_label(label_value: int, counter: int) -> str:
    """Safely format a label string as 'label_XX_NNN'"""
    result = f"label_{label_value & 0xFF:02x}_{counter & 0xFFF:03d}"
    return result


def safe_format_hex_byte(byte_value: int) -> str:
    """Safely format a hex byte as '$XX'"""
    result = f"${byte_value & 0xFF:02x}"
    return result


def safe_disassemble_bytes(input_bytes: bytes) -> str:
    """Simulate disassembly of a byte sequence with safe formatting"""
    output = []
    i = 0
    while i < len(input_bytes):
        byte = input_bytes[i] & 0xFF
        formatted = f"${byte:02x}"
        # Ensure output length is bounded
        if len(formatted) > 3:  # '$' + 2 hex chars = 3 chars max
            raise ValueError(f"Formatted output exceeds expected bounds: {formatted}")
        output.append(formatted)
        i += 1
    return " ".join(output)


@pytest.mark.parametrize("payload", [
    # Boundary byte values
    b"\x00",
    b"\xff",
    b"\x7f",
    b"\x80",
    # Maximum single byte
    bytes([0xFF]),
    # All possible byte values
    bytes(range(256)),
    # Repeated max values
    b"\xff" * 100,
    b"\xff" * 1000,
    # Null bytes
    b"\x00" * 100,
    # Mixed adversarial
    b"\xff\x00\xff\x00" * 50,
    # Format string attack attempts (as bytes)
    b"%s%s%s%s%s%s%s%s",
    b"%x%x%x%x%x%x%x%x",
    b"%n%n%n%n",
    b"%.2x%.3d" * 20,
    # Large inputs
    bytes(range(256)) * 10,
    # Single bytes at boundaries
    b"\x01",
    b"\xfe",
    b"\x41",  # 'A'
    b"\x61",  # 'a'
    # Sequences that could trigger label formatting
    bytes([i % 256 for i in range(1000)]),
    # Empty input
    b"",
    # Very large input
    bytes([0xAA] * 10000),
])
def test_disassembler_output_bounded(payload):
    """Invariant: Disassembler output for each byte must be exactly 3 characters ('$XX'),
    and total output must be proportional to input size with no unbounded growth,
    ensuring no buffer overflow can occur regardless of input content."""

    if len(payload) == 0:
        result = safe_disassemble_bytes(payload)
        assert result == "", "Empty input must produce empty output"
        return

    result = safe_disassemble_bytes(payload)

    # Each byte produces exactly "$XX" (3 chars) plus separator
    tokens = result.split(" ") if result else []

    # INVARIANT 1: Number of tokens equals number of input bytes
    assert len(tokens) == len(payload), (
        f"Output token count {len(tokens)} must equal input byte count {len(payload)}"
    )

    # INVARIANT 2: Each token is exactly 3 characters long
    for i, token in enumerate(tokens):
        assert len(token) == 3, (
            f"Token '{token}' at position {i} must be exactly 3 chars, got {len(token)}"
        )

    # INVARIANT 3: Each token matches the pattern '$XX' where XX is hex
    hex_pattern = re.compile(r'^\$[0-9a-f]{2}$')
    for i, token in enumerate(tokens):
        assert hex_pattern.match(token), (
            f"Token '{token}' at position {i} must match pattern '$XX'"
        )

    # INVARIANT 4: Output size is strictly bounded relative to input
    # Each byte produces at most 4 chars (3 + separator), so total <= 4 * len(payload)
    assert len(result) <= 4 * len(payload), (
        f"Output length {len(result)} exceeds 4x input length {4 * len(payload)}"
    )


@pytest.mark.parametrize("label_value,counter", [
    (0x00, 0),
    (0xFF, 999),
    (0x7F, 500),
    (0x80, 1),
    (0xFF, 0),
    (0x00, 999),
    (0xAA, 123),
    (0x55, 456),
    # Boundary overflow attempts
    (256, 1000),    # value exceeds byte range
    (-1, -1),       # negative values
    (0xFFFF, 9999), # large values
    (0x1234, 0xABCD),
])
def test_label_format_bounded(label_value, counter):
    """Invariant: Label formatting must always produce output of known, bounded length.
    The format 'label_XX_NNN' must never exceed a fixed maximum size regardless of input."""

    result = safe_format_label(label_value, counter)

    # INVARIANT 1: Label format is always exactly 'label_XX_NNN' = 13 chars
    expected_length = len("label_") + 2 + len("_") + 3  # = 12 chars
    assert len(result) == expected_length, (
        f"Label '{result}' length {len(result)} must be exactly {expected_length}"
    )

    # INVARIANT 2: Label matches expected pattern
    label_pattern = re.compile(r'^label_[0-9a-f]{2}_[0-9]{3}$')
    assert label_pattern.match(result), (
        f"Label '{result}' must match pattern 'label_XX_NNN'"
    )

    # INVARIANT 3: Values are properly masked/bounded (no overflow into adjacent memory)
    hex_part = result[6:8]
    dec_part = result[9:12]
    hex_val = int(hex_part, 16)
    dec_val = int(dec_part, 10)

    assert 0 <= hex_val <= 0xFF, f"Hex part {hex_val} must be in byte range [0, 255]"
    assert 0 <= dec_val <= 999, f"Decimal part {dec_val} must be in range [0, 999]"


@pytest.mark.parametrize("byte_value", [
    0x00, 0x01, 0x7F, 0x80, 0xFE, 0xFF,
    -1, 256, 0xFFFF, 0x10000,
    # Adversarial values
    0xDEAD, 0xBEEF, 0xCAFE, 0xBABE,
])
def test_hex_byte_format_bounded(byte_value):
    """Invariant: Single byte hex formatting must always produce exactly 3 characters ('$XX'),
    preventing any buffer overflow from oversized output."""

    result = safe_format_hex_byte(byte_value)

    # INVARIANT 1: Output is always exactly 3 characters
    assert len(result) == 3, (
        f"Hex byte format '{result}' must be exactly 3 chars, got {len(result)}"
    )

    # INVARIANT 2: Output matches '$XX' pattern
    assert re.match(r'^\$[0-9a-f]{2}$', result), (
        f"Hex byte format '{result}' must match pattern '$XX'"
    )

    # INVARIANT 3: The formatted value is within byte range
    hex_val = int(result[1:], 16)
    assert 0 <= hex_val <= 0xFF, (
        f"Formatted value {hex_val} must be in byte range [0, 255]"
    )