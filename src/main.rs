extern crate talamel;

use std::path::PathBuf;

use talamel::*;

use std::env;

use std::fmt::Debug;

fn main() {
    for arg in env::args().skip(1) {
        print_metadata(arg);
    }
}

fn print_metadata<P: Into<PathBuf> + Debug>(filename: P) -> () {
    println!("Reading from : {:?}", filename);

    let f: TalamelFile = TalamelFile::new(filename).unwrap();

    match f.title() {
        Ok(t) => println!("Title: {}", t),
        _ => println!("Could not read title!"),
    }

    match f.artist() {
        Ok(a) => println!("Artist: {}", a),
        _ => println!("Could not read artist!"),
    }

    match f.comments() {
        Ok(cs) => {
            for comm in cs {
                println!("Comment: {:?}", comm);
            }
        }
        _ => println!("Could not read comments!"),
    }
}
