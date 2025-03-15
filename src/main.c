#define NOB_STRIP_PREFIX
#define NOB_IMPLEMENTATION
#include "../nob.h"

String_View get_last_component_of_filepath(String_View sv) {
    nob_sv_chop_by_delim(&sv, '/');
    return sv;
}

bool compare_last_component_of_filepath(char *filepath, char *component) {
    String_View last = get_last_component_of_filepath(nob_sv_from_cstr(filepath));
    return nob_sv_eq(last, nob_sv_from_cstr(component));
}

void cmd_cat(int argc, char **argv) {
    while (argc) {
        char *input_filepath = nob_shift(argv, argc);
        String_Builder input_sb = {0};
        if (!nob_read_entire_file(input_filepath, &input_sb)) {
            abort();
        }
        String_View input_sv = nob_sb_to_sv(input_sb);
        printf(SV_Fmt, SV_Arg(input_sv));
    }
}

int main(int argc, char **argv) {
    char *program_name = nob_shift(argv, argc);
    if (compare_last_component_of_filepath(program_name, "cat")) {
//    if (0 == strcmp(program_name, "./cat") || 0==strcmp(program_name, "cat")) {
        cmd_cat(argc, argv);
    }
}
