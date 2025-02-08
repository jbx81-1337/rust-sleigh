use std::path::Path;
use std::process::Command;

fn main() {
    let mut build_sleigh = Command::new("bash");
    build_sleigh.arg("./build_sleigh.sh");
    let _ = build_sleigh.output();
    let sleigh_include = Path::new("vendor/include/sleigh");
    let rsleigh_include = Path::new("include");
    cxx_build::bridge("src/lib.rs")
        .file("lib/rsleigh/rsleigh.cpp")
        .file("lib/rsleigh/sleigh.cpp")
        .include(sleigh_include)
        .include(rsleigh_include)
        .include(sleigh_include)
        .flag("-lz")
        .flag("-lsla")
        .flag("-ldecomp")
        .flag("-lslaSupport")
        .compile("rsleigh");

    println!("cargo:rustc-link-search=native=vendor/lib");
    println!("cargo:rustc-link-lib=static=decomp");
    println!("cargo:rustc-link-lib=static=sla");
    println!("cargo:rustc-link-lib=static=slaSupport");

    println!("cargo:rustc-link-lib=z");
    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=src/instruction.rs");
    println!("cargo:rerun-if-changed=lib/rsleigh/rsleigh.cpp");
    println!("cargo:rerun-if-changed=include/rsleigh.h");
    println!("cargo:rerun-if-changed=lib/rsleigh/sleigh.cpp");
    println!("cargo:rerun-if-changed=lib/rsleigh/sleigh.h");
    
}
