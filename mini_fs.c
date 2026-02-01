#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_CONTENT 1024

// ---------- FILE STRUCT ----------
typedef struct File {
    char name[MAX_NAME];
    char content[MAX_CONTENT];
    struct File *next;
} File;

// ---------- DIRECTORY STRUCT ----------
typedef struct Directory {
    char name[MAX_NAME];
    struct Directory *subdirs;
    struct Directory *next;
    File *files;
} Directory;

Directory *root;

// ---------- CREATE DIRECTORY ----------
Directory* createDirectory(char *name) {
    Directory *dir = (Directory*)malloc(sizeof(Directory));
    strcpy(dir->name, name);
    dir->subdirs = NULL;
    dir->next = NULL;
    dir->files = NULL;
    return dir;
}

// ---------- CREATE FILE ----------
void createFile(Directory *dir, char *name) {
    File *newFile = (File*)malloc(sizeof(File));
    strcpy(newFile->name, name);
    newFile->content[0] = '\0';
    newFile->next = dir->files;
    dir->files = newFile;

    printf("File '%s' created!\n", name);
}

// ---------- WRITE FILE ----------
void writeFile(Directory *dir, char *name, char *content) {
    File *f = dir->files;

    while(f) {
        if(strcmp(f->name, name)==0) {
            strcpy(f->content, content);
            printf("Written successfully!\n");
            return;
        }
        f = f->next;
    }

    printf("File not found!\n");
}

// ---------- READ FILE ----------
void readFile(Directory *dir, char *name) {
    File *f = dir->files;

    while(f) {
        if(strcmp(f->name, name)==0) {
            printf("Content: %s\n", f->content);
            return;
        }
        f = f->next;
    }

    printf("File not found!\n");
}

// ---------- DELETE FILE ----------
void deleteFile(Directory *dir, char *name) {
    File *curr = dir->files;
    File *prev = NULL;

    while(curr) {
        if(strcmp(curr->name,name)==0) {
            if(prev)
                prev->next = curr->next;
            else
                dir->files = curr->next;

            free(curr);
            printf("Deleted!\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    printf("File not found!\n");
}

// ---------- LIST FILES ----------
void listFiles(Directory *dir) {
    File *f = dir->files;

    if(!f) {
        printf("No files present.\n");
        return;
    }

    printf("Files:\n");
    while(f) {
        printf("- %s\n", f->name);
        f = f->next;
    }
}

// ---------- MAIN ----------
int main() {
    root = createDirectory("root");

    char cmd[20], name[50], content[200];

    printf("Mini File System Simulator\n");

    while(1) {
        printf("\nCommands: create write read delete ls exit\n");
        printf("Enter command: ");
        scanf("%s", cmd);

        if(strcmp(cmd,"create")==0){
            scanf("%s", name);
            createFile(root,name);
        }

        else if(strcmp(cmd,"write")==0){
            scanf("%s", name);
            scanf(" %[^\n]", content);
            writeFile(root,name,content);
        }

        else if(strcmp(cmd,"read")==0){
            scanf("%s", name);
            readFile(root,name);
        }

        else if(strcmp(cmd,"delete")==0){
            scanf("%s", name);
            deleteFile(root,name);
        }

        else if(strcmp(cmd,"ls")==0){
            listFiles(root);
        }

        else if(strcmp(cmd,"exit")==0){
            printf("Exiting...\n");
            break;
        }

        else{
            printf("Unknown command!\n");
        }
    }

    return 0;
}
