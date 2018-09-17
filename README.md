# TALAMEL: TA(g) L(ib) A(bstract) M(etadata) E(xtraction) L(ibrary)

TALAMEL is designed to be a simple abstract interface to the C++ "taglib" library, primarily accessed through the Rust bindings. TALAMEL is built as a C wrapper library around taglib, with the aim of providing an abstract interface to audio metadata. In particular, TALAMEL aims to provide metadata not supplied by taglib's existing C api. 

TALAMEL, at present, is not designed to write audio metadata. It is unlikely that it will ever do so. Taglib has a number of bugs that cause issues in some file formats when writing tags, so I would advise anyone wanting to write audio file metadata script existing, well tested, application such as iTunes rather than using a tag library. 