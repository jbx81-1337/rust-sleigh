#include <sleigh/libsleigh.hh>

typedef struct InternalSleighInstruction {
    uint8_t opcode;
    std::string mnemonic;
    std::string body;
} InternalSleighInstruction;

#if defined(_MSC_VER)
    //  Microsoft 
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
    //  GCC
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#else
    //  do nothing and hope for the best?
    #define EXPORT
    #define IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

class AssemblyPrinter : public ghidra::AssemblyEmit {
    public:
        std::string *mnemonic;
        std::string *body;
        void dump(const ghidra::Address &addr, 
                    const std::string &mnemonic,
                    const std::string &body) override {
                        printf("AssemblyPrinter> %p %s %s\n", addr.getOffset(), mnemonic.c_str(), body.c_str());
                        this->mnemonic = new std::string(mnemonic);
                        this->body =new std::string(body);
        }
};

class InMemoryLoadImage : public ghidra::LoadImage {
    public:
        explicit InMemoryLoadImage(uint64_t base_addr)
            : LoadImage("nofile"), base_addr(base_addr) {}

        void loadFill(unsigned char *ptr, int size,
                        const ghidra::Address &addr) override {
            
            for(int i = 0; i < size; i++) {    
                if(i < this->size) {
                    ptr[i] = this->bytes[addr.getOffset() - baddr + i];
                }else{
                    ptr[i] = 0;
                }
            }
        }
        std::string getArchType(void) const override { return "memory"; }
        void set(uint64_t baddr, u_int8_t *bytes, size_t size) {
            this->bytes = bytes;
            this->size = size;
            this->baddr = baddr;
        }
        void adjustVma(long) override {}

    private:
        const uint64_t base_addr;
        u_int8_t *bytes;
        uint32_t size;
        uint64_t baddr;
};

class SleighDecompiler {
    private:
        InMemoryLoadImage *img;
        ghidra::ContextInternal *ctx;
        ghidra::Sleigh *engine;
        ghidra::DocumentStorage *storage;
        ghidra::Element *slaRoot;
        ghidra::Element *pspecRoot;
        const ghidra::Element *el;
        ghidra::XmlDecode *decoder;
    
    public:
        SleighDecompiler(std::string sleigh_path, std::string sla_file, std::string pspec_file);
        std::vector<InternalSleighInstruction> *disas(uint8_t *bytes, uint64_t addr, size_t size);
};