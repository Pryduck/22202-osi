typedef struct action {
    char* line;
    int number;
} action;

enum ind {
    create_directory,
    list_directory_contents,
    delete_directory,
    create_file,
    display_file_contents,
    delete_file,
    create_symbolic_link,
    display_symbolic_link_contents,
    display_linked_file_contents,
    delete_symbolic_link,
    create_hard_link,
    delete_hard_link,
    display_file_permissions_and_links,
    change_file_permissions
};

action ind[] = {
    {"create_directory", create_directory},
    {"list_directory_contents", list_directory_contents},
    {"delete_directory", delete_directory},
    {"create_file", create_file},
    {"display_file_contents", display_file_contents},
    {"delete_file", delete_file},
    {"create_symbolic_link", create_symbolic_link},
    {"display_symbolic_link_contents", display_symbolic_link_contents},
    {"display_linked_file_contents", display_linked_file_contents},
    {"delete_symbolic_link", delete_symbolic_link},
    {"create_hard_link", create_hard_link},
    {"delete_hard_link", delete_hard_link},
    {"display_file_permissions_and_links", display_file_permissions_and_links},
    {"change_file_permissions", change_file_permissions}
};
