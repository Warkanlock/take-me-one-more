#include "../src/file.h"
#include "../unity/unity.h"

void test_cast_file_type(void) {
    TEST_ASSERT_EQUAL_STRING("Directory", cast_file_type(DT_DIR));
}

void test_get_parent_dir(void) {
    char* cwd;
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        char* parent_dir = get_parent_dir(cwd);
        TEST_ASSERT_EQUAL_STRING(cwd, parent_dir);
        free(parent_dir);
    }
}

void test_read_dir(void) {
    FilesContainer fc = read_dir(".", false);
    TEST_ASSERT_NOT_NULL(fc.files);
    free_files_container(&fc);
}

void test_free_files_container(void) {
    FilesContainer* fc = (FilesContainer*)malloc(sizeof(FilesContainer));
    free_files_container(fc);
    TEST_ASSERT_NULL(fc->files);
    free(fc);
}

void test_create_directory(void) {
    create_directory("/path/to/new_directory");
}

void test_get_inference_path(void) {
    char* path = get_inference_path("/path/to/file", true);
    TEST_ASSERT_NOT_NULL(path);
    free(path);
}

void test_extract_path_components(void) {
    char* components = extract_path_components("/path/to/directory");
    TEST_ASSERT_NOT_NULL(components);
    free(components);
}

void test_validate_path(void) {
    char* valid_path = validate_path(".");
    TEST_ASSERT_NOT_NULL(valid_path);
}
