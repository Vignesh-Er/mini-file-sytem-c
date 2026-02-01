#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_CONTENT 1024
#define DISK_LIMIT 10000

int used_space = 0;

// -------- FILE --------
typedef struct File {
    char name[MAX_NAME];
    char content[MAX_CONTENT];
    int size;
    int readable;
    int writable;
    struct File *next;
} File;

// -------- DIRECTORY --------
typedef struct Directory {
    char name[MAX_NAME];
    struct Directory *parent;
    struct Directory *subdirs;
    struct Directory *next;
    File *files;
} Directory;

Directory *root;
Directory *current;

// -------- CREATE DIR --------
Directory* createDir(char *name, Directory *parent){
    Directory *d=malloc(sizeof(Directory));
    strcpy(d->name,name);
    d->parent=parent;
    d->subdirs=NULL;
    d->next=NULL;
    d->files=NULL;
    return d;
}

// -------- MKDIR --------
void mkdir_cmd(char *name){
    Directory *d=createDir(name,current);
    d->next=current->subdirs;
    current->subdirs=d;
    printf("Directory created\n");
}

// -------- CD --------
void cd_cmd(char *name){
    if(strcmp(name,"..")==0){
        if(current->parent) current=current->parent;
        return;
    }

    Directory *d=current->subdirs;
    while(d){
        if(strcmp(d->name,name)==0){
            current=d;
            return;
        }
        d=d->next;
    }
    printf("Not found\n");
}

// -------- PWD -------
void pwd(){
    Directory *t = current;

    char stack[50][MAX_NAME]; 
    int top = 0;

    // Collect names
    while(t){
        strcpy(stack[top++], t->name);
        t = t->parent;
    }

    // Print in reverse
    for(int i = top-1; i >= 0; i--){
        printf("/%s", stack[i]);
    }

    printf("\n");
}


// -------- CREATE FILE --------
void createFile(char *name){
    File *f=malloc(sizeof(File));
    strcpy(f->name,name);
    f->content[0]='\0';
    f->size=0;
    f->readable=1;
    f->writable=1;
    f->next=current->files;
    current->files=f;
    printf("File created\n");
}

// -------- WRITE --------
void writeFile(char *name,char *content){
    File *f=current->files;
    while(f){
        if(strcmp(f->name,name)==0){

            if(!f->writable){
                printf("No write permission\n");
                return;
            }

            int newsize=strlen(content);

            if(used_space - f->size + newsize > DISK_LIMIT){
                printf("Disk full!\n");
                return;
            }

            used_space = used_space - f->size + newsize;

            strcpy(f->content,content);
            f->size=newsize;

            printf("Written\n");
            return;
        }
        f=f->next;
    }
    printf("File not found\n");
}

// -------- READ --------
void readFile(char *name){
    File *f=current->files;
    while(f){
        if(strcmp(f->name,name)==0){

            if(!f->readable){
                printf("No read permission\n");
                return;
            }

            printf("Content: %s\n",f->content);
            return;
        }
        f=f->next;
    }
    printf("Not found\n");
}

// -------- DELETE --------
void deleteFile(char *name){
    File *c=current->files,*p=NULL;
    while(c){
        if(strcmp(c->name,name)==0){
            if(p) p->next=c->next;
            else current->files=c->next;

            used_space -= c->size;
            free(c);
            printf("Deleted\n");
            return;
        }
        p=c; c=c->next;
    }
}

// -------- PERMISSION --------
void chmod_cmd(char *name,int r,int w){
    File *f=current->files;
    while(f){
        if(strcmp(f->name,name)==0){
            f->readable=r;
            f->writable=w;
            printf("Permissions updated\n");
            return;
        }
        f=f->next;
    }
}

// -------- LS --------
void ls(){
    Directory *d=current->subdirs;
    File *f=current->files;

    printf("Directories:\n");
    while(d){
        printf("[DIR] %s\n",d->name);
        d=d->next;
    }

    printf("Files:\n");
    while(f){
        printf("%s (r:%d w:%d size:%d)\n",
        f->name,f->readable,f->writable,f->size);
        f=f->next;
    }

    printf("Disk: %d/%d\n",used_space,DISK_LIMIT);
}

// -------- TREE --------
void tree(Directory *d,int level){
    for(int i=0;i<level;i++) printf("  ");
    printf("[%s]\n",d->name);

    File *f=d->files;
    while(f){
        for(int i=0;i<level+1;i++) printf("  ");
        printf("- %s\n",f->name);
        f=f->next;
    }

    Directory *sub=d->subdirs;
    while(sub){
        tree(sub,level+1);
        sub=sub->next;
    }
}

// -------- SAVE --------
void save(){
    FILE *fp=fopen("fs_save.txt","w");
    fprintf(fp,"%d\n",used_space);
    fclose(fp);
    printf("Saved (basic)\n");
}

// -------- LOAD --------
void load(){
    FILE *fp=fopen("fs_save.txt","r");
    if(!fp) return;
    fscanf(fp,"%d",&used_space);
    fclose(fp);
    printf("Loaded disk usage\n");
}

// -------- MAIN --------
int main(){
    char cmd[20],name[50],content[200];
    int r,w;

    root=createDir("root",NULL);
    current=root;
    load();

    printf("Mini FS MAX\n");

    while(1){
        printf("\ncmd> ");
        scanf("%s",cmd);

        if(strcmp(cmd,"mkdir")==0){scanf("%s",name); mkdir_cmd(name);}
        else if(strcmp(cmd,"cd")==0){scanf("%s",name); cd_cmd(name);}
        else if(strcmp(cmd,"pwd")==0){pwd();}
        else if(strcmp(cmd,"ls")==0){ls();}
        else if(strcmp(cmd,"tree")==0){tree(root,0);}
        else if(strcmp(cmd,"create")==0){scanf("%s",name); createFile(name);}
        else if(strcmp(cmd,"write")==0){
            scanf("%s",name);
            scanf(" %[^\n]",content);
            writeFile(name,content);
        }
        else if(strcmp(cmd,"read")==0){scanf("%s",name); readFile(name);}
        else if(strcmp(cmd,"delete")==0){scanf("%s",name); deleteFile(name);}
        else if(strcmp(cmd,"chmod")==0){
            scanf("%s %d %d",name,&r,&w);
            chmod_cmd(name,r,w);
        }
        else if(strcmp(cmd,"save")==0){save();}
        else if(strcmp(cmd,"exit")==0){save(); break;}
        else printf("Unknown\n");
    }
}
