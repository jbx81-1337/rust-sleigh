#include "sleigh.h"

SleighDecompiler::SleighDecompiler(std::string sleigh_path, std::string sla_file, std::string pspec_file) {
  
  auto sla_file_path = std::string(sleigh_path).append(sla_file);
  auto pspec_file_path = std::string(sleigh_path).append(pspec_file);
  
  std::cout << sleigh_path << std::endl;

  ghidra::AttributeId::initialize();
  ghidra::ElementId::initialize();

  this->img = new InMemoryLoadImage(0);
  this->ctx = new ghidra::ContextInternal();
  this->engine = new ghidra::Sleigh(this->img, this->ctx);
  this->storage = new ghidra::DocumentStorage();
  std::istringstream sla("<sleigh>" + sla_file_path + "</sleigh>");
  this->slaRoot = 
        this->storage->parseDocument(
            sla
          )->getRoot();

  this->storage->registerTag(this->slaRoot);
  this->pspecRoot = 
        this->storage->openDocument(
            pspec_file_path
          )->getRoot();

  this->storage->registerTag(this->pspecRoot);
  this->engine->initialize(*(this->storage));
  this->engine->allowContextSet(false);
  this->el = storage->getTag("processor_spec");

  if (el) {
    this->decoder = new ghidra::XmlDecode(this->engine, el);
    ghidra::uint4 elemId = this->decoder->openElement(ghidra::ELEM_PROCESSOR_SPEC);
    for (;;) {
      ghidra::uint4 subId = decoder->peekElement();
      if (subId == 0)
          break;
      else if (subId == ghidra::ELEM_CONTEXT_DATA) {
          this->ctx->decodeFromSpec(*decoder);
          break;
      } else {
          this->decoder->openElement();
          this->decoder->closeElementSkipping(subId);
      }
    }
    decoder->closeElement(elemId);
  }

}

std::vector<InternalSleighInstruction> *GetInstructions(ghidra::Sleigh &engine, uint64_t addr, size_t len) {
  std::vector<InternalSleighInstruction> *out = new std::vector<struct InternalSleighInstruction>();
  ghidra::Address cur_addr = ghidra::Address(engine.getDefaultCodeSpace(), addr);
  ghidra::Address last_addr(engine.getDefaultCodeSpace(), addr + len);
  AssemblyPrinter asm_emit;
  while (cur_addr < last_addr) {
    InternalSleighInstruction ins;
    int32_t instr_len = engine.printAssembly(asm_emit, cur_addr);
    ins.mnemonic = *asm_emit.mnemonic;
    ins.body = *asm_emit.body;
    out->push_back(ins);
    cur_addr = cur_addr + instr_len;
  }
  return out;
}
std::vector<InternalSleighInstruction>* SleighDecompiler::disas(uint8_t *bytes, uint64_t addr, size_t size){
  auto codeSpace = this->engine->getDefaultCodeSpace();
  ghidra::Address cur_addr(codeSpace, addr);
  this->img->set(addr, bytes, size);
  auto ret = GetInstructions(*(this->engine), addr, size);
  return ret;
}