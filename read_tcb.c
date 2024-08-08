#include <read_tcb.h>

int
get_cnt_object_in_folder (char *path)
{
    DIR *dir;
    struct dirent *ent;
    int cnt = 0;

    dir = opendir(path);
    if (dir){
        while((ent = readdir(dir)) != NULL){
            cnt += 1;
        }
    }
    return cnt;
}

void
show_array (char **arr, int size)
{
    for (int i = 0; i < size; i++){
        printf("%s\n", arr[i]);
    }
}

char **
create_array (int size){
    char **ptr;

    ptr = (char**) malloc(size * sizeof (char*));
    for (int i = 0; i < size; i++){
        ptr[i] = (char*) malloc(101 * sizeof (char));
    }

    return ptr;
}

void
free_array (char **ptr, int size){
    for (int i = 0; i < size; i++){
        free(ptr[i]);
    }

    free(ptr);
}

struct spwd *
get_keys (char *path)
{
    struct spwd *sp;
    FILE *file;

    file = fopen(path, "r");
    if (file != NULL){
        sp = fgetspent(file);
        fclose(file);
        if (sp){
            return sp;
        }
        return NULL;
    }
}

char *
concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

bool
compare_arrays (char **arr1, char **arr2, int size)
{
    for (int i = 0; i < size; i++) {
        char *ent1 = arr1[i];

        if (!strcmp(ent1, "")) {
            continue;
        }

        bool is_comp = false;
        for (int j = 0; j < size; j++) {
            char *ent2 = arr2[j];
            if (!strcmp (ent1, ent2)) {
                is_comp = !is_comp;
                break;
            }
        }
        if (!is_comp){
            printf("No account %s\n", ent1);
            return false;
        }
    }
    return true;
}

// получение записей БЕЗ "прибитого гвоздями" /etc/tcb/
char **
get_tcb_entries_without_path ()
{
    struct spwd *sp;
    char **ptr;
    int size;

    size = get_cnt_object_in_folder(PATH_TCB);
    ptr = create_array(size);

    int  i = 0;
    setspent ();
    while ((sp = getspent()) != NULL) {
        ptr[i] = strdup(sp->sp_namp);
        i += 1;
    }
    endspent();

    return ptr;
}

// получение записей C "прибитым гвоздями" /etc/tcb/
char **
get_tcb_entries_with_path (char *path)
{
    DIR *dir;
    struct dirent *ent;
    char *shadow_path;

    dir = opendir(path);
    if (dir){
        char **ptr;
        int size;

        size = get_cnt_object_in_folder(path);
        ptr = create_array(size);

        int i = 0;
        struct spwd *sp;
        while((ent = readdir(dir)) != NULL){
            if (!strcmp (ent->d_name, ".") || !strcmp (ent->d_name, "..")){
                continue;
            }

            shadow_path = concat(path, ent->d_name);
            shadow_path = concat(shadow_path, "/shadow");

            sp = get_keys(shadow_path);
            ptr[i] = strdup(sp->sp_namp);
            i += 1;
        }
        free(shadow_path);

        return ptr;
    }
}

int main(){
    char **array_with_tcb;
    char **array_without_tcb;
    int size;

    size = get_cnt_object_in_folder(PATH_TCB);

    array_with_tcb = get_tcb_entries_with_path(PATH_TCB);
    show_array(array_with_tcb, size);
    array_without_tcb = get_tcb_entries_without_path();
    show_array(array_without_tcb, size);

    bool compare = compare_arrays(array_with_tcb, array_without_tcb, size);
    if (compare) {
        printf ("The entries all match.\n");
    }

    free_array(array_with_tcb, size);
    free_array(array_without_tcb, size);

    return 0;
}
