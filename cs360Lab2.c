#include <stdio.h>             // for I/O
#include <stdlib.h>            // for I/O
#include <libgen.h>            // for dirname()/basename()
#include <string.h>

typedef struct node{
         char  name[64];       // node's name string
         char  type;           // 'D' for DIR; 'F' for file
   struct node *child, *sibling, *parent;
}NODE;


NODE *root, *cwd, *start;
char line[128];
char command[16], pathname[64];

//               0       1      2    
char *cmd[] = {"mkdir", "ls", "quit", 0};

int findCmd(char *command)
{
    int i = 0;
    while(cmd[i]){
      if (strcmp(command, cmd[i])==0)
          return i;
      i++;
    }
    return -1;
}

NODE *search_child(NODE *parent, char *name)
{
    NODE *p;
    printf("search for %s in parent DIR\n", name);
    p = parent->child;
    if (p==0)
      return 0;
    while(p){
      if (strcmp(p->name, name)==0)
        return p;
      p = p->sibling;
      }
    return 0;
  }

  int insert_child(NODE *parent, NODE *q)
  {
      NODE *p;
      printf("insert NODE %s into END of parent child list\n", q->name);
      p = parent->child;
      if (p==0)
        parent->child = q;
      else{
        while(p->sibling)
          p = p->sibling;
        p->sibling = q;
      }
      q->parent = parent;
      q->child = 0;
      q->sibling = 0;
}

/***************************************************************
 This mkdir(char *name) makes a DIR in the current directory
 You MUST improve it to mkdir(char *pathname) for ANY pathname
****************************************************************/

int mkdir(char *pathname)
{
    NODE *p, *q;
    printf("mkdir: name=%s\n", pathname);

    if (!strcmp(pathname, "/") || !strcmp(pathname, ".") || !strcmp(pathname, "..")){
      printf("can't mkdir with %s\n", pathname);
      return -1;
    }
    if (pathname[0]=='/')
      start = root;
    else
      start = cwd;

    printf("check whether %s already exists\n", pathname);
    p = search_child(start, pathname);
    if (p){
      printf("name %s already exists, mkdir FAILED\n", pathname);
      return -1;
    }
    printf("--------------------------------------\n");
    printf("ready to mkdir %s\n", pathname);
    q = (NODE *)malloc(sizeof(NODE));
    q->type = 'D';
    strcpy(q->name, pathname);
    insert_child(start, q);
    printf("mkdir %s OK\n", pathname);
    printf("--------------------------------------\n");
      
    return 0;
}

// This ls() list CWD. You MUST improve it to ls(char *pathname)
int ls(char *pathname)
{
  if(cwd->type =='D'){
    NODE *p = cwd->child;
    printf("cwd contents = ");
    for(int i=0; p; i++){
      while(true){
        if(i!=4){
          printf("[%c %s] ",p->type,p->name);}
        else{
          printf("[%c %s] ",p->type,p->name);
        }
      }
    }
  }
    while(p){
      printf("[%c %s] ", p->type, p->name);
      p = p->sibling;
    }
    printf("\n");
}

int quit()
{
    FILE *file = fopen(pathname,"w");
    int a=0;
    char *buf = "";
    for(int a =0, path1[a], a++){
      char path1[a];
      strcpy(path1, buf);
      strcat(path1,"/");
      strcat(path1,root->name);
      fprintf(file,root->type,path1);
      if(root->child){
        fprintf(file,root->child->type,path1);
      }
      if (root->sibling){
        fprintf(file,root->sibling->type,path1);
      }
    
    }
    printf("Program exit\n");
    exit(0);
  // improve quit() to SAVE the current tree as a Linux file
  // for reload the file to reconstruct the original tree
}

int initialize()
{
    root = (NODE *)malloc(sizeof(NODE));
    strcpy(root->name, "/");
    root->parent = root;
    root->sibling = 0;
    root->child = 0;
    root->type = 'D';
    cwd = root;
    printf("Root initialized OK\n");
}

int main()
{
    int index;

    initialize();

    printf("NOTE: commands = [mkdir|ls|quit]\n");

    while(1){
        printf("Enter command line : ");
        fgets(line, 128, stdin);
        line[strlen(line)-1] = 0;

        command[0] = pathname[0] = 0;
        sscanf(line, "%s %s", command, pathname);
        printf("command=%s pathname=%s\n", command, pathname);
        
        if (command[0]==0) 
          continue;

        index = findCmd(command);

        switch (index){
          case 0: mkdir(pathname); break;
          case 1: ls();            break;
          case 2: quit();          break;
      }
  }
}
