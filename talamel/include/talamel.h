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

typedef struct { int empty; } tml_TalamelFile;

TALAMEL_C_EXPORT tml_TalamelFile* tml_open_file(const char* filename); 

void tml_free_file(tml_TalamelFile* f);

void tml_free_str(char* str);

void tml_print_properties(tml_TalamelFile* tf);

TALAMEL_C_EXPORT char * tml_read_title(tml_TalamelFile* tf);

TALAMEL_C_EXPORT char * tml_read_artist(tml_TalamelFile* tf); 

TALAMEL_C_EXPORT unsigned int tml_read_bpm(tml_TalamelFile* tf);

TALAMEL_C_EXPORT unsigned int tml_count_comments(tml_TalamelFile* tf); 

TALAMEL_C_EXPORT char * tml_get_comment(tml_TalamelFile* tf, int comment);

#ifdef __cplusplus
}
#endif
#endif // DEFINE TALAMEL
