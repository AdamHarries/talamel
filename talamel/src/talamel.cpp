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
extern "C" char *stringToCharArray(const TagLib::String &s) {
  const std::string str = s.to8Bit(true);

#ifdef HAVE_ISO_STRDUP
  return ::_strdup(str.c_str());
#else
  return ::strdup(str.c_str());
#endif
}

extern "C" TagLib::String charArrayToString(const char *s) {
  return TagLib::String(s, TagLib::String::UTF8);
}

extern "C" TagLib::PropertyMap get_properties(tml_TalamelFile *tf) {
  return reinterpret_cast<TagLib::File *>(tf)->properties();
}

/*
  Library interface
*/
extern "C" tml_TalamelFile *tml_open_file(const char *filename) {
  TagLib::File * f = TagLib::FileRef::create(filename);
  if (!f->isValid()) 
    return nullptr; 
  return reinterpret_cast<tml_TalamelFile *>(f);
}

extern "C" void tml_free_file(tml_TalamelFile *f) {
  delete reinterpret_cast<TagLib::File *>(f);
}

extern "C" void tml_free_str(char *str) { delete str; }

extern "C" unsigned int tml_count_property_values(tml_TalamelFile *tf,
                                                  const char *key) {
  TagLib::PropertyMap properties = get_properties(tf);
  TagLib::StringList sl = properties[key];
  return sl.size();
}

extern "C" char *tml_read_property_value(tml_TalamelFile *tf, const char *key,
                                         unsigned int ix) {
  TagLib::PropertyMap properties = get_properties(tf);
  TagLib::StringList sl = properties[key];
  if (!(sl.size() > 0) || ix >= sl.size()) {
    return nullptr;
  }
  return stringToCharArray(sl[ix]);
}

extern "C" char *tml_read_title(tml_TalamelFile *tf) {
  TagLib::PropertyMap properties = get_properties(tf);
  TagLib::StringList sl = properties["TITLE"];
  if (!(sl.size() > 0)) {
    return nullptr;
  }
  return stringToCharArray(sl[0]);
}

extern "C" char *tml_read_artist(tml_TalamelFile *tf) {
  TagLib::PropertyMap properties = get_properties(tf);
  TagLib::StringList sl = properties["ARTIST"];
  if (!(sl.size() > 0)) {
    return nullptr;
  }
  return stringToCharArray(sl[0]);
}

extern "C" unsigned int tml_read_bpm(tml_TalamelFile *tf) {
  TagLib::PropertyMap properties = get_properties(tf);
  TagLib::StringList sl = properties["BPM"];
  if (!(sl.size() > 0)) {
    return 0;
  } else {
    return stoi(sl[0].to8Bit(true));
  }
}

extern "C" unsigned int tml_count_comments(tml_TalamelFile *tf) {
  TagLib::PropertyMap properties = get_properties(tf);
  TagLib::StringList sl = properties["COMMENT"];
  return sl.size();
}

extern "C" char *tml_get_comment(tml_TalamelFile *tf, unsigned int comment) {
  TagLib::PropertyMap properties = get_properties(tf);
  TagLib::StringList sl = properties["COMMENT"];
  if (comment >= sl.size()) {
    return nullptr;
  } else {
    return stringToCharArray(sl[comment]);
  }
}
