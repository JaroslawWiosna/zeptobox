#define NOB_IMPLEMENTATION
#include "nob.h"

#define EXE_NAME     "zeptobox"
#define BUILD_FOLDER "_build/"
#define SRC_FOLDER   "src/"

char *subcommands[] = {
    "cat",
    "echo",
    "wc",
};

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    const char *program_name = nob_shift(argv, argc);

    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd,
        "rm", "-rf", BUILD_FOLDER);
    if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    nob_cmd_append(&cmd,
        "cc", "-Wall", "-Wextra", "-ggdb",
        "-o", BUILD_FOLDER EXE_NAME,
        SRC_FOLDER"main.c");
    if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;

    for (int i=0; i<NOB_ARRAY_LEN(subcommands); ++i) {
        char exe_absolute_path[PATH_MAX];
        realpath("./" BUILD_FOLDER EXE_NAME, exe_absolute_path);
        char link_absolute_path[PATH_MAX];
        realpath(
                nob_temp_sprintf("./%s%s", BUILD_FOLDER, subcommands[i]),
                link_absolute_path);

        nob_cmd_append(&cmd,
            "ln", "-fs", exe_absolute_path, link_absolute_path);
        if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;
    }

    if (argc && 0 == strcmp("tests", nob_shift(argv, argc))) {
        nob_log(NOB_INFO, "tests...");
    }

    return 0;
}
