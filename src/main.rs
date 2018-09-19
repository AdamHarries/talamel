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
        e => println!("Could not read title, got error: {:?}", e),
    }

    match f.artist() {
        Ok(a) => println!("Artist: {}", a),
        e => println!("Could not read artist, got error: {:?}", e),
    }

    match f.read_property_values("ALBUM") {
        Ok(vs) => {
            for v in vs {
                println!("Album: {}", v);
            }
        }
        e => println!("Could not read album, got error: {:?}", e),
    }

    match f.comments() {
        Ok(cs) => {
            for comm in cs {
                println!("Comment: {:?}", comm);
            }
        }
        e => println!("Could not read comments, got error: {:?}", e),
    }
}
