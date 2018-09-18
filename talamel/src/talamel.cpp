#include <stdlib.h>
#include <iostream> 
#include <string> 

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

#include <talamel.h>

/*
  Internal functions
*/
char *stringToCharArray(const TagLib::String &s) {
  const std::string str = s.to8Bit(true);

#ifdef HAVE_ISO_STRDUP
  return ::_strdup(str.c_str());
#else
  return ::strdup(str.c_str());
#endif
}

TagLib::String charArrayToString(const char *s) {
  return TagLib::String(s, TagLib::String::UTF8);
}

TagLib::PropertyMap get_properties(TalamelFile *tf) {
  return reinterpret_cast<TagLib::File *>(tf)->properties();
}

/*
  Library interface
*/
void print_properties(const char *filename) {
  TagLib::FileRef f(filename);

  TagLib::PropertyMap properties = f.file()->properties();

  for (auto pair = properties.begin(); pair != properties.end(); pair++) {
    std::cout << "Key: " << pair->first << ", value(s): " << pair->second
              << std::endl;
  }
}

TalamelFile *tml_open_file(const char *filename) {
  return reinterpret_cast<TalamelFile *>(TagLib::FileRef::create(filename));
}

void tml_free_file(TalamelFile *f) {
  delete reinterpret_cast<TagLib::File *>(f);
}

void tml_print_properties(TalamelFile *tf) {
  // Reinterpret as a File
  TagLib::File *f = reinterpret_cast<TagLib::File *>(tf);
  TagLib::PropertyMap properties = f->properties();

  for (auto pair = properties.begin(); pair != properties.end(); pair++) {
    std::cout << "Key: " << pair->first << ", value(s): " << pair->second
              << std::endl;
  }
}

char *tml_read_title(TalamelFile *tf) {
  TagLib::PropertyMap properties = get_properties(tf);
  auto sl = properties["TITLE"];
  if(!(sl.size() > 0)) { 
    return nullptr;
  }
  return stringToCharArray(sl[0]);
}

char *tml_read_artist(TalamelFile *tf){
TagLib::PropertyMap properties = get_properties(tf);
  auto sl = properties["ARTIST"];
  if(!(sl.size() > 0)) { 
    return nullptr;
  }
  return stringToCharArray(sl[0]);
}

int tml_read_bpm(TalamelFile *tf){
TagLib::PropertyMap properties = get_properties(tf);
  auto sl = properties["BPM"];
  if(!(sl.size() > 0)) { 
    return -1;
  }else {
    return stoi(sl[0].to8Bit(true));
  }
}

int tml_count_comments(TalamelFile *tf){
  TagLib::PropertyMap properties = get_properties(tf);
  auto sl = properties["COMMENT"];
  return sl.size();
}

char *tml_get_comment(TalamelFile *tf, int comment){
  TagLib::PropertyMap properties = get_properties(tf);
  auto sl = properties["COMMENT"];
  if(comment >= sl.size()){
    return nullptr;
  }else{
    return stringToCharArray(sl[comment]);
  }
}
