#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gbv.h"

#define MAX_NAME 256
#define BUFFER_SIZE 512

static char current_library[256];

int find_doc(const Library *lib, const char *docname)
{
  int i;

  if (!lib || !docname)
    return -1;

  for (i = 0; i < lib->count; i++) 
  {
    if (strcmp(lib->docs[i].name, docname) == 0)
      return i;
  }

  return -1;
}

void free_library(Library *lib)
{
  if(!lib)
    return;

  free(lib->docs);
  lib->docs = NULL;
  lib->count = 0;
}

int cmp_name(const void *a, const void *b)
{
    const Document *x = a;
    const Document *y = b;
    return strcmp(x->name, y->name);
}

int cmp_size(const void *a, const void *b)
{
    const Document *x = a;
    const Document *y = b;

    if (x->size < y->size) return -1;
    if (x->size > y->size) return 1;
    return 0;
}

int cmp_date(const void *a, const void *b)
{
    const Document *x = a;
    const Document *y = b;

    if (x->date < y->date) return -1;
    if (x->date > y->date) return 1;
    return 0;
}

int gbv_create(const char *filename, const char *password)
{
  FILE *fp;
  SuperBloco sb;

  if(!filename)
    return 1;

  fp = fopen(filename, "wb");
  if(!fp)
    return 1;

  sb.count = 0;
  sb.dir_offset = sizeof(SuperBloco);

  strncpy(sb.password, password, 63);
  sb.password[63] = '\0';

  if(fwrite(&sb, sizeof(SuperBloco), 1, fp) != 1) 
  {
    fclose(fp);
    return 1;
  }

  fclose(fp);
  return 0;
}

int gbv_open(Library *lib, const char *filename, const char *password)
{
  if(strcmp(sb.password, password) != 0)
  {
    fclose(fp);
    return 1;
  }
  
  FILE *fp;
  SuperBloco sb;

  if(!filename || !lib)
    return 1;

  fp = fopen(filename, "rb");
  if(!fp)
    return 1;

  if(fread(&sb, sizeof(SuperBloco), 1, fp) != 1)
  {
    fclose(fp);
    return 1;
  }


  lib->count = sb.count;

  if (lib->count > 0) 
  {
    lib->docs = malloc(lib->count * sizeof(Document));
    if(!lib->docs) 
    {
      fclose(fp);
      return 1;
    }
    
    fseek(fp, sb.dir_offset, SEEK_SET);
    if(fread(lib->docs, sizeof(Document), lib->count, fp) != lib->count) 
    {
      free_library(lib);
      fclose(fp);
      return 1;
    } 
  } 
  else 
  {
    lib->docs = NULL;
  }
  strncpy(current_library, filename, sizeof(current_library) - 1);
  current_library[sizeof(current_library) - 1] = '\0';

  fclose(fp);
  return 0;
}

int gbv_add(Library *lib, const char *archive, const char *docname)
{
  FILE *fp, *doc;
  SuperBloco sb;
  long size, new_offset;
  int pos, i;
  size_t nread;
  char buffer[BUFFER_SIZE];
  Document *tmp;

  if(!lib || !archive || !docname)
    return 1;


  fp = fopen(archive, "rb+");
  if(!fp)
    return 1;

  if(fread(&sb, sizeof(SuperBloco), 1, fp) != 1) 
  {
    fclose(fp);
    return 1;
  }


  doc = fopen(docname, "rb");
  if(!doc) 
  {
    fclose(fp);
    return 1;
  }


  fseek(doc, 0, SEEK_END);
  size = ftell(doc);
  rewind(doc);


  pos = find_doc(lib, docname);
  if(pos != -1) 
  {
    for (i = pos; i < lib->count - 1; i++)
      lib->docs[i] = lib->docs[i + 1];

    lib->count--;

    if(lib->count == 0) 
      free_library(lib);
    else 
    {
      tmp = realloc(lib->docs, lib->count * sizeof(Document));
      if (tmp)
        lib->docs = tmp;
    }
  }

  
  new_offset = sb.dir_offset;
  fseek(fp, new_offset, SEEK_SET);

  
  while ((nread = fread(buffer, 1, BUFFER_SIZE, doc)) > 0) 
  {
    if(fwrite(buffer, 1, nread, fp) != nread) 
    {
      fclose(doc);
      fclose(fp);
      return 1;
    }
  }

  fclose(doc);


  tmp = realloc(lib->docs, (lib->count + 1) * sizeof(Document));
  if(!tmp) 
  {
    fclose(fp);
    return 1;
  }
  
  lib->docs = tmp;

  
  strncpy(lib->docs[lib->count].name, docname, MAX_NAME - 1);
  lib->docs[lib->count].name[MAX_NAME - 1] = '\0';
  lib->docs[lib->count].size = size;
  lib->docs[lib->count].date = time(NULL);
  lib->docs[lib->count].offset = new_offset;

  lib->count++;

  
  sb.count = lib->count;
  sb.dir_offset = new_offset + size;

  
  fseek(fp, sb.dir_offset, SEEK_SET);
  if (fwrite(lib->docs, sizeof(Document), lib->count, fp) != (size_t)lib->count) 
  {
    fclose(fp);
    return 1;
  }

    
  fseek(fp, 0, SEEK_SET);
  if (fwrite(&sb, sizeof(SuperBloco), 1, fp) != 1) 
  {
    fclose(fp);
    return 1;
  }

  fclose(fp);
  return 0;
}

  


int gbv_remove(Library *lib, const char *docname)
{
  FILE *fp;
  SuperBloco sb;
  int pos, i;
  Document *tmp;

  if(!lib || !docname)
    return 1;

  pos = find_doc(lib, docname);
  if(pos == -1)
    return 1;

  for(i = pos; i < lib->count - 1; i++)
    lib->docs[i] = lib->docs[i + 1];

  lib->count--;

  if(lib->count == 0) 
    free_library(lib);
   
  else 
  {
    tmp = realloc(lib->docs, lib->count * sizeof(Document));
    if(tmp)
      lib->docs = tmp;
  }

  fp = fopen(current_library, "rb+");
  if(!fp)
    return 1;

  fseek(fp, 0, SEEK_END);
  sb.count = lib->count;
  sb.dir_offset = ftell(fp);

  if(lib->count > 0) 
  {
    if(fwrite(lib->docs, sizeof(Document), lib->count, fp) != (size_t)lib->count) 
    {
      fclose(fp);
      return 1;
    }
  }

  fseek(fp, 0, SEEK_SET);
  if(fwrite(&sb, sizeof(SuperBloco), 1, fp) != 1) 
  {
    fclose(fp);
    return 1;
  }

  fclose(fp);
  return 0;
}

int gbv_list(const Library *lib)
{
  int i;
  char data_str[64];
  struct tm *tm_info;

  if(!lib)
    return 1;

  for(i = 0; i < lib->count; i++) 
  {
    tm_info = localtime(&lib->docs[i].date);
    strftime(data_str, sizeof(data_str), "%d/%m/%Y %H:%M:%S", tm_info);

    printf("Nome: %s\n", lib->docs[i].name);
    printf("Tamanho: %ld bytes\n", lib->docs[i].size);
    printf("Data: %s\n", data_str);
    printf("Offset: %ld\n", lib->docs[i].offset);
    printf("\n");
  }

  return 0;
}

int gbv_view(const Library *lib, const char *docname)
{
  FILE *fp;
  int pos;
  long block = 0, total_blocks, remaining, to_read;
  char buffer[BUFFER_SIZE + 1];
  char cmd;

  if(!lib || !docname)
    return 1;

  pos = find_doc(lib, docname);
  if(pos == -1)
    return 1;

  fp = fopen(current_library, "rb");
  if(!fp)
    return 1;

  total_blocks = (lib->docs[pos].size + BUFFER_SIZE - 1) / BUFFER_SIZE;

  while(1) 
  {
    remaining = lib->docs[pos].size - block * BUFFER_SIZE;
    to_read = remaining < BUFFER_SIZE ? remaining : BUFFER_SIZE;

    fseek(fp, lib->docs[pos].offset + block * BUFFER_SIZE, SEEK_SET);
    fread(buffer, 1, to_read, fp);
    buffer[to_read] = '\0';

    printf("\n--- Bloco %ld/%ld ---\n", block + 1, total_blocks);
    printf("%s\n", buffer);

    printf("[n] próximo  [p] anterior  [q] sair: ");
    scanf(" %c", &cmd);

    if(cmd == 'q')
      break;
    else if(cmd == 'n' && block < total_blocks - 1)
      block++;
    else if(cmd == 'p' && block > 0)
      block--;
  }

  fclose(fp);
  return 0;
}

int gbv_order(Library *lib, const char *archive, const char *criteria)
{
  FILE *fp;
  SuperBloco sb;

  (void)archive;

  if(!lib || !criteria)
    return 1;

  if( strcmp(criteria, "nome") == 0)
    qsort(lib->docs, lib->count, sizeof(Document), cmp_name);
  else if(strcmp(criteria, "data") == 0)
    qsort(lib->docs, lib->count, sizeof(Document), cmp_date);
  else if( strcmp(criteria, "tamanho") == 0)
    qsort(lib->docs, lib->count, sizeof(Document), cmp_size);
  else
    return 1;

  fp = fopen(current_library, "rb+");
  if(!fp)
    return 1;

  fseek(fp, 0, SEEK_END);
  sb.count = lib->count;
  sb.dir_offset = ftell(fp);

  if(lib->count > 0) 
  {
    if(fwrite(lib->docs, sizeof(Document), lib->count, fp) != (size_t)lib->count) 
    {
      fclose(fp);
      return 1;
    }
  }

  fseek(fp, 0, SEEK_SET);
  if(fwrite(&sb, sizeof(SuperBloco), 1, fp) != 1) 
  {
    fclose(fp);
    return 1;
  }

  fclose(fp);
  return 0;
}

int gbv_rename(Library *lib, const char *oldname, const char *newname)
{
  FILE *fp;
  SuperBloco sb;
  int pos, pos2;

  if(!lib || !oldname || !newname)
    return 1;

  pos = find_doc(lib, oldname);
  if(pos == -1)
    return 1;

  pos2 = find_doc(lib, newname);
  if(pos2 != -1)
    return 1;

  strncpy(lib->docs[pos].name, newname, MAX_NAME-1);
  lib->docs[pos].name[MAX_NAME - 1] = '\0';

  fp = fopen(current_library, "rb+");
  if(!fp)
    return 1;

  fseek(fp, 0, SEEK_END);
  sb.count = lib->count;
  sb.dir_offset = ftell(fp);

  if(lib->count > 0) 
  {
    if(fwrite(lib->docs, sizeof(Document), lib->count, fp) != (size_t)lib->count) 
    {
      fclose(fp);
      return 1;
    }
  }

  fseek(fp, 0, SEEK_SET);
  if(fwrite(&sb, sizeof(SuperBloco), 1, fp) != 1) 
  {
    fclose(fp);
    return 1;
  }

  fclose(fp);
  return 0;
}

int gbv_extract(const Library *lib, const char *docname, const char *output)
{
  FILE *fp, exit;
  SuperBloco sb;
  int pos, size;

  if(!lib || !docname || !output)
    return 1;

  pos = find_doc(lib, docname);
  if(pos == -1)
    return 1;

  fp = fopen(current_library, "rb");
  if(!fp)
    return 1;

  exit = fopen(output, "wb");
  if(!exit)
  {
    fclose(fp);
    return 1;
  }

  fseek(fp, lib->docs[pos].offset, SEEK_SET);
  size = ftell(fp);

  while ((nread = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) 
  {
    if(fwrite(buffer, 1, nread, exit) != nread) 
    {
      fclose(fp);
      fclose(exit);
      return 1;
    }
  }

  fclose(fp);
  fclose(exit);
  return 0;

  
}