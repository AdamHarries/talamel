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
    let t : TalamelFile = TalamelFile::new(filename).unwrap(); 

    println!("Title: {:?}", t.title());
    println!("Artist: {:?}", t.artist());
}



// void print_metadata(const char* filename) { 
//   tml_TalamelFile* f = tml_open_file(filename); 

//   char* title = tml_read_title(f); 
//   printf("Title: %s\n", title); 
//   free(title); 

//   char* artist = tml_read_artist(f); 
//   printf("Artist: %s\n", artist); 
//   free(artist); 

//   int bpm = tml_read_bpm(f); 
//   printf("Bpm: %d\n", bpm);

//   int comments = tml_count_comments(f); 
//   if(comments == 0)
//     printf("No comments found in file.\n");
//   for(int i = 0; i< comments; i++) { 
//     char * comm = tml_get_comment(f, i); 
//     printf("Comment [%d]: %s\n", i, comm); 
//     free(comm);
//   }

//   tml_free_file(f); 
// }

// int main(int argc, char *argv[]) {
//   print_metadata("/home/adam/Music/Archive.org/Jumpin' at the "
//                     "Woodside - Count Basie And His Orchestra.mp3");
//   print_metadata(
//       "/home/adam/Music/iTunes/iTunes Media/Music/Michael Gamble/Michael "
//       "Gamble & the Rhythm Serenaders/19 Airmail Special.m4a");

//   print_metadata(
//     "/home/adam/Music/Dani Sicari & The Easy Rollers - Dani Sicari & The Easy Rollers/01 - Dani Sicari & The Easy Rollers - Some of these days.flac"
//   );

//   return 0;
// }
