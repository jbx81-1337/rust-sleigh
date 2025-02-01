#include <cassert>
#include <iostream>
#include <string>
#include <stdio.h>
#include "rsleigh.h"

EXPORT std::shared_ptr<SleighDecompiler> new_sleigh_decompiler(rust::String sleigh_path, rust::String sla_file, rust::String pspec_file) {
  std::string path = std::string(sleigh_path.c_str());
  std::string sla  = std::string(sla_file.c_str());
  std::string pspec = std::string(pspec_file.c_str());
  
  return std::shared_ptr<SleighDecompiler>(new SleighDecompiler(path, sla, pspec));
}

EXPORT rust::Vec<struct SleighInstruction> disas(std::shared_ptr<SleighDecompiler> decompiler, rust::Vec<uint8_t> bytes, uint64_t addr, uint32_t size){
  rust::Vec<struct SleighInstruction> output;
  auto instructions = decompiler.get()->disas(bytes.data(), addr, size);
  for(auto ins = instructions->begin(); ins != instructions->end(); ++ins) {
    struct SleighInstruction tmp;

    tmp.opcode = ins->opcode;
    tmp.mnemonic = rust::String(ins->mnemonic);
    tmp.body = rust::String(ins->body);

    output.push_back(tmp);
  }
  return output;
}