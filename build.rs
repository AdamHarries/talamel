extern crate bindgen;
extern crate cmake;

use cmake::Config;

use std::env;
use std::path::PathBuf;

fn main() {
    // tell cargo to build our taglib branch
    let dst = Config::new("talamel")
        .define("CMAKE_C_FLAGS", "-fPIC -Wall -O3")
        .define("CMAKE_CXX_FLAGS", "-fPIC -Wall -O3")
        .build();
    // tell cargo to look for it when trying to link
    println!("cargo:rustc-link-search={}", dst.display());
    println!(
        "cargo:rustc-link-search={}/build/taglib/install/lib",
        dst.display()
    );
    // link libc++, as the static linker doesn't, and we need it for the tag internals
    // and tell cargo to link the static library that it finds there!
    if cfg!(target_os = "macos") {
        println!("cargo:rustc-link-lib=dylib=c++");
        println!("cargo:rustc-flags=-l talamel -l tag -l z");
    } else if cfg!(target_os = "windows") {
        println!("cargo:rustc-flags=-l talamel -l tag_c -l tag -l stdc++");
        println!("cargo:rustc-link-lib=static=zlib")
    } else if cfg!(target_os = "linux") {
        println!("cargo:rustc-flags=-l talamel -l tag_c -l tag -l z -l stdc++");
    }
    // create bindings for the static c library
    let header = dst.join("talamel.h");
    let bindings = bindgen::Builder::default()
        // use the header from the dst, where cmake has writen the headers
        .header(header.to_str().unwrap())
        // Finish the builder and generate the bindings.
        .generate()
        // Unwrap the Result and panic on failure.
        .expect("Unable to generate bindings");
    // Write the bindings to the $OUT_DIR/bindings.rs file.
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
