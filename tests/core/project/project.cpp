#include <gtest/gtest.h>
#include <mycroft/mycroft.h>

/**
 * Ensure that we actually can allocate and free a project.  Hopefully this
 * test works!
 */
TEST(project_new, create_project) {
    mc_project_t* project = mc_project_create("name");
    ASSERT_TRUE(project != NULL);
    mc_project_free(project);
}

/**
 * Make sure that internal values are set properly.
 */
TEST(project_new, verify_default_values) {
    mc_project_t* project = mc_project_create("name");
    ASSERT_TRUE(project != NULL);
    ASSERT_TRUE(mc_project_get_focused_file(project) == NULL);
    ASSERT_TRUE(mc_project_get_file_amount(project) == 0);
    mc_project_free(project);
}

/**
 * Test to see if we can add files to the project.  Also bonus test to make
 * sure we actually record the number of files!
 */
TEST(project_new, add_file) {
    mc_project_t* project = mc_project_create("name");
    ASSERT_TRUE(project != NULL);

    mc_file_t* files[3] = {NULL};
    ASSERT_TRUE((files[0] = mc_file_open("res/testfile1", NULL)) != NULL);
    ASSERT_TRUE((files[1] = mc_file_open("res/testfile2", NULL)) != NULL);
    ASSERT_TRUE((files[2] = mc_file_open("res/testfile3", NULL)) != NULL);

    ASSERT_TRUE(mc_project_add_file(project, files[0]) == MC_OK);
    ASSERT_TRUE(mc_project_add_file(project, files[1]) == MC_OK);
    ASSERT_TRUE(mc_project_add_file(project, files[2]) == MC_OK);

    ASSERT_TRUE(mc_project_get_file_amount(project) == 3);

    mc_project_free(project);
}

/**
 * Make sure that we can't add duplicate files.
 */
TEST(project_new, add_duplicate_file) {
    mc_project_t* project = mc_project_create("name");
    ASSERT_TRUE(project != NULL);

    mc_file_t* files[2] = {NULL};
    ASSERT_TRUE((files[0] = mc_file_open("res/testfile1", NULL)) != NULL);
    ASSERT_TRUE((files[1] = mc_file_open("res/testfile1", NULL)) != NULL);

    ASSERT_TRUE(mc_project_add_file(project, files[0]) == MC_OK);
    ASSERT_TRUE(mc_project_add_file(project, files[1]) != MC_OK);

    ASSERT_TRUE(mc_project_get_file_amount(project) == 1);

    mc_project_free(project);
}

/**
 * Make sure that we can't add duplicate files.  A second test for
 * thoroughness.
 */
TEST(project_new, add_duplicate_file2) {
    mc_project_t* project = mc_project_create("name");
    ASSERT_TRUE(project != NULL);

    mc_file_t* file = NULL;
    ASSERT_TRUE((file = mc_file_open("res/testfile1", NULL)) != NULL);

    ASSERT_TRUE(mc_project_add_file(project, file) == MC_OK);
    ASSERT_TRUE(mc_project_add_file(project, file) != MC_OK);

    ASSERT_TRUE(mc_project_get_file_amount(project) == 1);

    mc_project_free(project);
}

/**
 * Test to make adding a NULL file fails.
 */
TEST(project_new, add_null_file) {
    mc_project_t* project = mc_project_create("name");
    ASSERT_TRUE(project != NULL);

    ASSERT_TRUE(mc_project_add_file(project, NULL) != MC_OK);

    mc_project_free(project);
}


/**
 * Check to make sure we can retrieve files we add to the project.
 */
TEST(project_new, get_file) {
    mc_project_t* project = mc_project_create("name");
    ASSERT_TRUE(project != NULL);

    mc_file_t* files[3] = {NULL};
    ASSERT_TRUE((files[0] = mc_file_open("res/testfile1", NULL)) != NULL);
    ASSERT_TRUE((files[1] = mc_file_open("res/testfile2", NULL)) != NULL);
    ASSERT_TRUE((files[2] = mc_file_open("res/testfile3", NULL)) != NULL);

    ASSERT_TRUE(mc_project_add_file(project, files[0]) == MC_OK);
    ASSERT_TRUE(mc_project_add_file(project, files[1]) == MC_OK);
    ASSERT_TRUE(mc_project_add_file(project, files[2]) == MC_OK);

    ASSERT_TRUE(files[0] == mc_project_get_file(project, 0));
    ASSERT_TRUE(files[1] == mc_project_get_file(project, 1));
    ASSERT_TRUE(files[2] == mc_project_get_file(project, 2));

    mc_project_free(project);
}

/**
 * Check to make sure we can't retrieve files we didn't add to the project.
 */
TEST(project_new, get_null_file) {
    mc_project_t* project = mc_project_create("name");
    ASSERT_TRUE(project != NULL);
    ASSERT_TRUE(mc_project_get_file(project, 0) == NULL);
    ASSERT_TRUE(mc_project_get_file(project, 1) == NULL);
    ASSERT_TRUE(mc_project_get_file(project, 123) == NULL);
    mc_project_free(project);
}

/**
 * Check to make sure we can remove files we add to the project.
 */
TEST(project_new, remove_file) {
    mc_project_t* project = mc_project_create("name");
    ASSERT_TRUE(project != NULL);

    mc_file_t* files[3] = {NULL};
    ASSERT_TRUE((files[0] = mc_file_open("res/testfile1", NULL)) != NULL);
    ASSERT_TRUE((files[1] = mc_file_open("res/testfile2", NULL)) != NULL);
    ASSERT_TRUE((files[2] = mc_file_open("res/testfile3", NULL)) != NULL);

    ASSERT_TRUE(mc_project_add_file(project, files[0]) == MC_OK);
    ASSERT_TRUE(mc_project_add_file(project, files[1]) == MC_OK);
    ASSERT_TRUE(mc_project_add_file(project, files[2]) == MC_OK);

    ASSERT_TRUE(mc_project_remove_file(project, 2) == MC_OK);
    ASSERT_TRUE(mc_project_remove_file(project, 1) == MC_OK);
    ASSERT_TRUE(mc_project_remove_file(project, 0) == MC_OK);

    ASSERT_TRUE(mc_project_get_file_amount(project) == 0);

    mc_project_free(project);
}

/**
 * Check in a different way if we can remove files we add to the project.
 */
TEST(project_new, remove_file_2) {
    mc_project_t* project = mc_project_create("name");
    ASSERT_TRUE(project != NULL);

    mc_file_t* files[3] = {NULL};
    ASSERT_TRUE((files[0] = mc_file_open("res/testfile1", NULL)) != NULL);
    ASSERT_TRUE((files[1] = mc_file_open("res/testfile2", NULL)) != NULL);
    ASSERT_TRUE((files[2] = mc_file_open("res/testfile3", NULL)) != NULL);

    ASSERT_TRUE(mc_project_add_file(project, files[0]) == MC_OK);
    ASSERT_TRUE(mc_project_add_file(project, files[1]) == MC_OK);
    ASSERT_TRUE(mc_project_add_file(project, files[2]) == MC_OK);

    ASSERT_TRUE(mc_project_remove_file(project, 0) == MC_OK);
    ASSERT_TRUE(mc_project_remove_file(project, 0) == MC_OK);
    ASSERT_TRUE(mc_project_remove_file(project, 0) == MC_OK);

    ASSERT_TRUE(mc_project_get_file_amount(project) == 0);

    mc_project_free(project);
}

/**
 * Check to make sure that the remove function shifts over elements
 */
TEST(project_new, remove_shiftover) {
    mc_project_t* project = mc_project_create("name");
    ASSERT_TRUE(project != NULL);

    mc_file_t* files[3] = {NULL};
    ASSERT_TRUE((files[0] = mc_file_open("res/testfile1", NULL)) != NULL);
    ASSERT_TRUE((files[1] = mc_file_open("res/testfile2", NULL)) != NULL);
    ASSERT_TRUE((files[2] = mc_file_open("res/testfile3", NULL)) != NULL);

    ASSERT_TRUE(mc_project_add_file(project, files[0]) == MC_OK);
    ASSERT_TRUE(mc_project_add_file(project, files[1]) == MC_OK);
    ASSERT_TRUE(mc_project_add_file(project, files[2]) == MC_OK);

    ASSERT_TRUE(files[1] == mc_project_get_file(project, 1));
    ASSERT_TRUE(mc_project_remove_file(project, 1) == MC_OK);
    ASSERT_TRUE(files[2] == mc_project_get_file(project, 1));

    mc_project_free(project);
}

/**
 * Test focused file functionality.
 */
TEST(project_new, focused_file) {
    mc_project_t* project = mc_project_create("name");
    ASSERT_TRUE(project != NULL);

    mc_file_t* file = NULL;
    ASSERT_TRUE((file = mc_file_create()) != NULL);

    ASSERT_TRUE(mc_project_add_file(project, file) == MC_OK);

    mc_project_set_focused_file(project, file);
    ASSERT_TRUE(file == mc_project_get_focused_file(project));

    ASSERT_TRUE(mc_project_remove_file(project, 0) == MC_OK);

    mc_project_free(project);
}

/**
 * Check to make sure we can't set an unadded file as focused file.
 */
TEST(project_new, focused_file_unadded) {
    mc_project_t* project = mc_project_create("name");
    ASSERT_TRUE(project != NULL);

    mc_file_t* file = NULL;
    ASSERT_TRUE((file = mc_file_create()) != NULL);

    mc_project_set_focused_file(project, file);

    ASSERT_TRUE(mc_project_get_focused_file(project) == NULL);

    mc_project_free(project);
}
/**
 * Check to make sure we set the focused file to null if we remove it.
 */
TEST(project_new, focused_file_remove_null) {
    mc_project_t* project = mc_project_create("name");
    ASSERT_TRUE(project != NULL);

    mc_file_t* file = NULL;
    ASSERT_TRUE((file = mc_file_create()) != NULL);
    ASSERT_TRUE(mc_project_add_file(project, file) == MC_OK);

    mc_project_set_focused_file(project, file);

    ASSERT_TRUE(mc_project_remove_file(project, 0) == MC_OK);

    ASSERT_TRUE(mc_project_get_focused_file(project) == NULL);

    mc_project_free(project);
}

/**
 * See if we can save the database to a file successfully.
 */
TEST(project_save, add_and_save) {
    mc_project_t* project = mc_project_create("name");
    ASSERT_TRUE(project != NULL);

    mc_file_t* files[3] = {NULL};
    ASSERT_TRUE((files[0] = mc_file_open("res/testfile1", NULL)) != NULL);
    ASSERT_TRUE((files[1] = mc_file_open("res/testfile2", NULL)) != NULL);
    ASSERT_TRUE((files[2] = mc_file_open("res/testfile3", NULL)) != NULL);

    ASSERT_TRUE(mc_project_add_file(project, files[0]) == MC_OK);
    ASSERT_TRUE(mc_project_add_file(project, files[1]) == MC_OK);
    ASSERT_TRUE(mc_project_add_file(project, files[2]) == MC_OK);

    ASSERT_TRUE(mc_project_get_file_amount(project) == 3);

    ASSERT_TRUE(mc_project_save(project, "testprojectdb1.mdb") == MC_OK);

    mc_project_free(project);
}
