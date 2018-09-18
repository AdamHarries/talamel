// The C talamel interface - heavily "inspired" by the taglib c bindings

#ifndef TALAMEL
#define TALAMEL

#ifdef __cplusplus
extern "C" {
#endif

// Define static library export stuff

#define TALAMEL_STATIC
#if defined(TALAMEL_STATIC)
#define TALAMEL_C_EXPORT
#elif defined(_WIN32) || defined(_WIN64)
#ifdef MAKE_TALAMEL_C_LIB
#define TALAMEL_C_EXPORT __declspec(dllexport)
#else
#define TALAMEL_C_EXPORT __declspec(dllimport)
#endif
#elif defined(__GNUC__) && (__GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ >= 1)
#define TALAMEL_C_EXPORT __attribute__ ((visibility("default")))
#else
#define TALAMEL_C_EXPORT
#endif

#ifndef BOOL
#define BOOL int
#endif

typedef struct { int empty; } TalamelFile;

void print_properties(const char* filename);

TALAMEL_C_EXPORT TalamelFile* tml_open_file(const char* filename); 

void tml_free_file(TalamelFile* f);

void tml_print_properties(TalamelFile* tf);

TALAMEL_C_EXPORT char * tml_read_title(TalamelFile* tf);

TALAMEL_C_EXPORT char * tml_read_artist(TalamelFile* tf); 

TALAMEL_C_EXPORT int tml_read_bpm(TalamelFile* tf);

TALAMEL_C_EXPORT int tml_count_comments(TalamelFile* tf); 

TALAMEL_C_EXPORT char * tml_get_comment(TalamelFile* tf, int comment);

#ifdef __cplusplus
}
#endif
#endif // DEFINE TALAMEL
