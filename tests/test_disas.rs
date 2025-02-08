use rust_sleigh::SleighDecompiler;

#[test]
fn test_disas() {
    /* 
             undefined         d2:1           <RETURN>
                             IfxCan_Node_getTxBufferElementAddress           XREF[5]:     Entry Point(*), 
                                                                                          IfxCan_Can_sendMessage:8000182e(
                                                                                          .debug_frame::00000db8(*), 
                                                                                          _elfProgramHeaders::00000308(*), 
                                                                                          _elfSectionHeaders::000008f4(*)  
        8000040a 0b 45 10 88     mov        d8/d9,d5,d4
        8000040e 02 6a           mov        d10,d6
        80000410 da 08           mov        d15,#0x8
        80000412 6d ff ee ff     call       IfxCan_Node_getTxBufferDataFieldSize             undefined IfxCan_Node_getTxBuffe
        80000416 42 2f           add        d15,d2
        80000418 e2 af           mul        d15,d10
        8000041a 42 98           add        d8,d9
        8000041c 42 f8           add        d8,d15
        8000041e 60 82           mov.a      a2,d8
        80000420 3c 01           j          LAB_80000422

    */
    let sleigh_path = String::from("../rust-sleigh/vendor/share/sleigh/specfiles/Ghidra/Processors");
    let sla_file = String::from("/tricore/data/languages/tricore.sla");
    let pspec_file = String::from("/tricore/data/languages/tricore.pspec");
    let dec = SleighDecompiler::new(sleigh_path, sla_file, pspec_file);
    let tricore_code = b"\x0b\x45\x10\x88\x02\x6a\xda\x08\x6d\xff\xee\xff\x42\x2f\xe2\xaf\x42\x98\x42\xf8\x60\x82\x3c\x01".to_vec();
    let size: u32 = tricore_code.len().try_into().unwrap();
    let disassembly = dec.disas(tricore_code, 0x8000040a, size);
    return assert_eq!(disassembly.len(), 10)
}