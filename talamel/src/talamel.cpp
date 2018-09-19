#include <stdlib.h>


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

TagLib::PropertyMap get_properties(tml_TalamelFile *tf) {
  return reinterpret_cast<TagLib::File *>(tf)->properties();
}

/*
  Library interface
*/
extern "C" tml_TalamelFile *tml_open_file(const char *filename) {
  return reinterpret_cast<tml_TalamelFile *>(TagLib::FileRef::create(filename));
}

extern "C" void tml_free_file(tml_TalamelFile *f) {
  delete reinterpret_cast<TagLib::File *>(f);
}

extern "C" void tml_free_str(char* str) { 
  delete str;
}

extern "C" void tml_print_properties(tml_TalamelFile *tf) {
  // Reinterpret as a File
  TagLib::File *f = reinterpret_cast<TagLib::File *>(tf);
  TagLib::PropertyMap properties = f->properties();

  for (auto pair = properties.begin(); pair != properties.end(); pair++) {
    std::cout << "Key: " << pair->first << ", value(s): " << pair->second
              << std::endl;
  }
}

extern "C" char *tml_read_title(tml_TalamelFile *tf) {
  TagLib::PropertyMap properties = get_properties(tf);
  TagLib::StringList sl = properties["TITLE"];
  if(!(sl.size() > 0)) { 
    return nullptr;
  }
  return stringToCharArray(sl[0]);
}

extern "C" char *tml_read_artist(tml_TalamelFile *tf){
TagLib::PropertyMap properties = get_properties(tf);
  TagLib::StringList sl = properties["ARTIST"];
  if(!(sl.size() > 0)) { 
    return nullptr;
  }
  return stringToCharArray(sl[0]);
}

extern "C" unsigned int tml_read_bpm(tml_TalamelFile *tf){
TagLib::PropertyMap properties = get_properties(tf);
  TagLib::StringList sl = properties["BPM"];
  if(!(sl.size() > 0)) { 
    return 0;
  }else {
    return stoi(sl[0].to8Bit(true));
  }
}

extern "C" unsigned int tml_count_comments(tml_TalamelFile *tf){
  TagLib::PropertyMap properties = get_properties(tf);
  TagLib::StringList sl = properties["COMMENT"];
  return sl.size();
}

extern "C" char *tml_get_comment(tml_TalamelFile *tf, int comment){
  TagLib::PropertyMap properties = get_properties(tf);
  TagLib::StringList sl = properties["COMMENT"];
  if(comment >= sl.size()){
    return nullptr;
  }else{
    return stringToCharArray(sl[comment]);
  }
}
