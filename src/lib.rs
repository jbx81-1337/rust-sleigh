#[cxx::bridge]
pub mod ffi {
    pub struct SleighInstruction {
        opcode: u8,
        mnemonic: String,
        body: String
    }

    unsafe extern "C++" {
        include!("rust-sleigh/include/rsleigh.h");
        type SleighDecompiler;
        fn new_sleigh_decompiler(sleighPath: String, slaFile: String, pspecFile: String) -> SharedPtr<SleighDecompiler>;
        fn disas(decompiler: SharedPtr<SleighDecompiler>, bytes: Vec<u8>, addr: u64, size: u32) -> Vec<SleighInstruction>;
    }
}

#[derive(Clone)]
pub struct SleighDecompiler {
    inner_decompiler: cxx::SharedPtr<ffi::SleighDecompiler>
}

impl SleighDecompiler {

    pub fn disas(&self, code: Vec<u8>, addr: u64, size: u32) -> Vec<ffi::SleighInstruction> {
        let ins: Vec<ffi::SleighInstruction> = ffi::disas(self.inner_decompiler.clone(), code, addr, size);
        return ins;
    }

    pub fn new(sleigh_path: String, sla_file: String, pspec_file: String) -> SleighDecompiler {
        return SleighDecompiler { inner_decompiler: ffi::new_sleigh_decompiler(sleigh_path, 
                                                                            sla_file, 
                                                                            pspec_file)};
    }
}