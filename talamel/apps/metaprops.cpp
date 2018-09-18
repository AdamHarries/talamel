#include <iostream>

#include <string.h>
#include <taglib/asffile.h>
#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/id3v2framefactory.h>
#include <taglib/mp4file.h>
#include <taglib/mpcfile.h>
#include <taglib/mpegfile.h>
#include <taglib/oggflacfile.h>
#include <taglib/speexfile.h>
#include <taglib/tag.h>
#include <taglib/tfile.h>
#include <taglib/tpropertymap.h>
#include <taglib/trueaudiofile.h>
#include <taglib/vorbisfile.h>
#include <taglib/wavpackfile.h>

int main(int argc, char const *argv[]) {
  TagLib::FileRef mp3file("/home/adam/Music/Archive.org/Jumpin' at the "
                          "Woodside - Count Basie And His Orchestra.mp3");
  TagLib::FileRef m4afile(
      "/home/adam/Music/iTunes/iTunes Media/Music/Michael Gamble/Michael "
      "Gamble & the Rhythm Serenaders/19 Airmail Special.m4a");

  auto files = std::vector<TagLib::FileRef>({mp3file, m4afile});

  for (auto file : files) {
    TagLib::String artist = file.tag()->artist();
    std::cout << artist << std::endl;

    TagLib::PropertyMap properties = file.file()->properties();

    // try and look up the bpm

    for (auto pair = properties.begin(); pair != properties.end(); pair++) {
      std::cout << "Key: " << pair->first << ", value(s): " << pair->second
                << std::endl;
    }
  }

  return 0;
}
