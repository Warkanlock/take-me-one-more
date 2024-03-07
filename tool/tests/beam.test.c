#include "../src/beam.h"
#include "../unity/unity.h"

void read_fixtures(void)
{
    DIR* d;
    struct dirent* dir;
    d = opendir("./fixtures");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
            {
                char path[1024];
                snprintf(path, sizeof(path), "./fixtures/%s", dir->d_name);
                printf("Processing image: %s\n", path);
                PixelImage* img = process_image(path);
                free_image(img);
            }
        }
        closedir(d);
    }
}

void test_process_image(void) { read_fixtures(); }

void test_create_image(void)
{
    unsigned int width = 100, height = 100, color_depth = 24;
    PixelImage* img = create_image(width, height, color_depth);
    TEST_ASSERT_NOT_NULL(img);
    free_image(img);
}

void test_free_image(void)
{
    unsigned int width = 100, height = 100, color_depth = 24;
    PixelImage* img = create_image(width, height, color_depth);
    TEST_ASSERT_NOT_NULL(img);
    free_image(img);
    TEST_ASSERT_NULL(img->pixels);
}

void test_process_difference(void)
{
    char* image_path_fixture_beam1 = "tests/fixtures/beam_1.ppm";
    char* image_path_fixture_beam2 = "tests/fixtures/beam_2.ppm";

    PixelImage* img1 = process_image(image_path_fixture_beam1);
    PixelImage* img2 = process_image(image_path_fixture_beam2);

    PixelImage* diff = process_difference(img1, img2);

    TEST_ASSERT_NOT_NULL(diff->pixels);
    free_image(img1);
    free_image(img2);
}

void test_store_difference(void)
{
    char* image_path_fixture_beam1 = "tests/fixtures/beam_1.ppm";
    char* image_path_fixture_beam2 = "tests/fixtures/beam_2.ppm";

    PixelImage* img1 = process_image(image_path_fixture_beam1);
    PixelImage* img2 = process_image(image_path_fixture_beam2);

    PixelImage* diff = process_difference(img1, img2);

    // this will go to the .inferece directory
    char* output_path = "tests/fixtures/diff.ppm";

    // create a File from path
    File file;
    file.name = "diff.ppm";
    file.path = output_path;
    file.type = DT_REG;

    int index_diff = 0;

    store_difference(diff, &file, index_diff);
    char* inference_path = get_inference_path(output_path, false, index_diff);

    TEST_ASSERT_EQUAL_STRING(".inference/tests/fixtures/diff.ppm/diff_0.dat", inference_path);

    free_image(img1);
    free_image(img2);
    free_image(diff);
}

void test_read_difference(void)
{
    char* image_path_fixture_beam1 = "tests/fixtures/inception.ppm";
    char* image_path_fixture_beam2 = "tests/fixtures/beam_1.ppm";

    PixelImage* img1 = process_image(image_path_fixture_beam1);
    PixelImage* img2 = process_image(image_path_fixture_beam2);

    PixelImage* diff = process_difference(img1, img2);

    // call read_difference(diff_path, difference, index)
    char* diff_path = "tests/fixtures/inception.ppm";

    // inception path, difference file and index
    read_difference(diff_path, diff, 0); // given that we only store 1 difference for now

    TEST_ASSERT_NOT_NULL(diff->pixels);

    free_image(img1);
    free_image(img2);
    free_image(diff);
}

void test_process_image_single(void)
{
    char* image_path_fixture_beam1 = "tests/fixtures/beam_1.ppm";

    PixelImage* img = process_image(image_path_fixture_beam1);

    TEST_ASSERT_NOT_NULL(img);
}
