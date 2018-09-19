extern crate talamel;

use std::path::PathBuf;

use talamel::*;

use std::env;

fn main() {
    for arg in env::args().skip(1) {
        print_metadata(arg);
    }
}

fn print_metadata<P: Into<PathBuf>>(filename: P) -> () {
    let t: TalamelFile = TalamelFile::new(filename).unwrap();

    println!("Title: {:?}", t.title());
    println!("Artist: {:?}", t.artist());
}
