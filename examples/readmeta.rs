extern crate talamel;
use talamel::*;

use std::env;
/*
    Dump metadata from an audio file using talamel. Equivalent to "talamel/apps/readmeta.c"
    Example usage:
        readmeta <audiofile>
*/
fn main() {
    let args: Vec<String> = env::args().collect();
    let filename = args[1].clone();

    println!("\nReading from file: {}", filename);

    let tf = TalamelFile::new(filename).expect("Could not open file with talamel!");

    println!("Title: {}", tf.title().expect("Could not read title"));

    println!("Artist: {}", tf.artist().expect("Could not read artist"));

    if let Some(b) = tf.bpm() {
        println!("Bpm: {}", b);
    }

    tf.comments()
        .expect("Could not read comments!")
        .iter()
        .for_each(|c| {
            println!("Comment: {}", c);
        });
}
