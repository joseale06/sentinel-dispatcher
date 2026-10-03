#include <stdio.h>
#include <stdlib.h>
#include "../../include/config.h"
#include "../../include/core/bootstrap.h"

int main(int argc, char *argv[]) {
    int status = init(argc, argv);
    if (status == EXIT_FAILURE) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}