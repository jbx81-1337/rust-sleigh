#pragma once
#include <memory>
#include <rust/cxx.h>
#include "sleigh.h"
#include "rust-sleigh/src/lib.rs.h"

EXPORT std::shared_ptr<SleighDecompiler> new_sleigh_decompiler(rust::String sleigh_path, rust::String sla_file, rust::String pspec_file);
EXPORT rust::Vec<struct SleighInstruction> disas(std::shared_ptr<SleighDecompiler> decompiler, rust::Vec<uint8_t> bytes, uint64_t addr, uint32_t size);