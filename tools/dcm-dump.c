#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dicom.h>


static const char usage[] = "usage: dcm-dump [-v] [-V] [-h] FILE_PATH\n";


int main(int argc, char *argv[]) {

    int i;
    const char *file_path = NULL;
    DcmDataSet *metadata = NULL;
    DcmDataSet *file_meta = NULL;
    DcmFile *file = NULL;

    dcm_log_level = DCM_LOG_ERROR;


    for (i = 1; i < argc && argv[i][0] == '-'; i++) {
        switch (argv[i][1]) {
            case 'h':
                printf("%s\n", usage);
                return EXIT_SUCCESS;
            case 'V':
                printf("%s\n", dcm_get_version());
                return EXIT_SUCCESS;
            case 'v':
                dcm_log_level = DCM_LOG_INFO;
                break;
            default:
                fprintf(stderr, "%s\n", usage);
                return EXIT_FAILURE;
        }
    }

    if ((i + 1) != argc) {
        fprintf(stderr, "%s\n", usage);
        return EXIT_FAILURE;
    }
    file_path = argv[i];

    dcm_log_info("Read file '%s'", file_path);
    file = dcm_file_create(file_path, 'r');
    if (file == NULL) {
        dcm_log_error("Reading file '%s' failed.", file_path);
        return EXIT_FAILURE;
    }
    dcm_log_info("Read File Meta Information");
    file_meta = dcm_file_read_file_meta(file);

    printf("===File Meta Information===\n");
    dcm_dataset_print(file_meta, 0);

    dcm_log_info("Read metadata");
    metadata = dcm_file_read_metadata(file);
    if (metadata == NULL) {
        dcm_log_error("Reading file '%s' failed. "
                      "Could not read Data Set.", file_path);
        dcm_file_destroy(file);
        dcm_dataset_destroy(file_meta);
        return EXIT_FAILURE;
    }

    printf("===Dataset===\n");
    dcm_dataset_print(metadata, 0);

    dcm_file_destroy(file);
    dcm_dataset_destroy(file_meta);
    dcm_dataset_destroy(metadata);

    return EXIT_SUCCESS;
}
