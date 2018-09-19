// The C talamel interface - heavily "inspired" by the taglib c bindings

#ifndef TALAMEL
#define TALAMEL

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BOOL
#define BOOL int
#endif

typedef void* tml_TalamelFile;

// typedef struct {
//   int empty;
// } tml_TalamelFile;

tml_TalamelFile *tml_open_file(const char *filename);

void tml_free_file(tml_TalamelFile *f);

void tml_free_str(char *str);

unsigned int tml_count_property_values(tml_TalamelFile *tf,
                                                        const char *key);

char *
tml_read_property_value(tml_TalamelFile *tf, const char *key, unsigned int ix);

char *tml_read_title(tml_TalamelFile *tf);

char *tml_read_artist(tml_TalamelFile *tf);

unsigned int tml_read_bpm(tml_TalamelFile *tf);

unsigned int tml_count_comments(tml_TalamelFile *tf);

char *tml_get_comment(tml_TalamelFile *tf,
                                       unsigned int comment);

#ifdef __cplusplus
}
#endif
#endif // DEFINE TALAMEL
