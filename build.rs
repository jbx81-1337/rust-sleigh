use std::path::Path;
fn main() {
    let sleigh_include = Path::new("vendor/include/sleigh");
    let rsleigh_include = Path::new("include");
    cxx_build::bridge("src/lib.rs")
        .file("lib/rsleigh/rsleigh.cpp")
        .file("lib/rsleigh/sleigh.cpp")
        .include(sleigh_include)
        .include(rsleigh_include)
        .include(sleigh_include)
        .flag("-lsla")
        .flag("-ldecomp")
        .flag("-lslaSupport")
        .compile("rsleigh");

    println!("cargo:rustc-link-search=native=vendor/lib");
    println!("cargo:rustc-link-lib=static=decomp");
    println!("cargo:rustc-link-lib=static=sla");
    println!("cargo:rustc-link-lib=static=slaSupport");

    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=src/instruction.rs");
    println!("cargo:rerun-if-changed=lib/rsleigh/rsleigh.cpp");
    println!("cargo:rerun-if-changed=include/rsleigh.h");
    println!("cargo:rerun-if-changed=lib/rsleigh/sleigh.cpp");
    println!("cargo:rerun-if-changed=lib/rsleigh/sleigh.h");
    
}
