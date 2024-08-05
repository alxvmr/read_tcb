#include <read_tcb.h>

static void
print_shadow (struct spwd *sp)
{
  if (putspent (sp, stdout) != 0)
    fprintf (stderr, "error writing shadow entry: %m\n");
}

void
shadow_keys (char *path)
{
    struct spwd *sp;
    FILE *file;

    file = fopen(path, "r");
    if (file != NULL){
        sp = fgetspent(file);
        if (sp){
            print_shadow(sp);
        }
        fclose(file);
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

void
print_shadow_tcb (char *path)
{
    DIR *dir;
    struct dirent *ent;
    char *shadow_path;

    dir = opendir(path);
    if (dir){
        while((ent = readdir(dir)) != NULL){
            shadow_path = concat(path, ent->d_name);
            shadow_path = concat(shadow_path, "/shadow");
            shadow_keys(shadow_path);
        }
        free(shadow_path);
    }
}

int main(){
    print_shadow_tcb(PATH_TCB);
    return 0;
}
