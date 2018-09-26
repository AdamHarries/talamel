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
    if cfg!(target_os = "windows") {
        println!(
            "cargo:rustc-link-search={}\\build\\taglib\\install\\lib",
            dst.display()
        );
        println!(
            "cargo:rustc-link-search={}\\build\\zlib\\install\\lib",
            dst.display()
        );
    } else {
        println!(
            "cargo:rustc-link-search={}/build/taglib/install/lib",
            dst.display()
        );
        println!(
            "cargo:rustc-link-search={}/build/zlib/install/lib",
            dst.display()
        );
    }

    println!("cargo:rustc-link-lib=static=talamel");
    println!("cargo:rustc-link-lib=static=tag_c");
    println!("cargo:rustc-link-lib=static=tag");
    println!("cargo:rustc-link-lib=static=z");
    if cfg!(target_os = "macos") {
        println!("cargo:rustc-link-lib=c++");
    } else if cfg!(target_os = "windows") {
    } else {
        println!("cargo:rustc-link-lib=stdc++");
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
