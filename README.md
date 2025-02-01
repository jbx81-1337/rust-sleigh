# Ghidra Decompiler for Rust

This project aims to provide a basic funtionality of the Ghidra Decompiler usig the [lifting-bits's Sleigh project](https://github.com/lifting-bits/sleigh) and exposing some high-level functions with [cxx.rs](https://cxx.rs) bridge.

## Make it work
1. Clone and build the lifting-bit's sleigh repository
2. When you are going to execute the command ```cmake --install build --prefix ./install``` specify the prefix to point to ./vendor folder
inside the rust-sleigh (you can also just copy the ./install content later...)
3. now that you have the sleigh installed you should be able to run ```cargo build```
   
*NOTE*: This process will be automated at some point, idially inside the build.rs (?)
## TODO
- [ ] PCode Printer
