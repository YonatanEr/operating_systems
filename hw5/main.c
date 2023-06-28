#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "name_parsing.h"
#include "msdos_fs.h"

#define MAX_INT_LENGTH 16


int count_digits(char* str){
    // return the digits amount in str
    for (int i=0; i<MAX_INT_LENGTH; i++){
        if (str[i] == '\0'){
            return i;
        }
    }
    return -1;
}


void print_size(int size){
    // prints the size of the file with , on each 3 digits
    char* str = (char*) calloc (MAX_INT_LENGTH+1, sizeof(char));
    assert(str);
    sprintf(str, "%d", size);
    int digits = count_digits(str);
    for (int i=0; i<=11-digits; i++){
        printf(" ");
    }
    for (int i=0; i<digits; i++){
        if (i>0 && (digits-i)%3 == 0){
            printf(",");
        }
        printf("%c", str[i]);
    }
    free(str);
    str = NULL;
    printf(" ");
}



void print_entry(struct msdos_dir_entry* entry) {
    // prints the entry in the windows dir format
    int year = ((entry->cdate & 0xFE00) >> 9) + 1980;
    int month = (entry->cdate & 0x01E0) >> 5;
    int day = entry->cdate & 0x001F;
    int hour = (entry->ctime & 0xF800) >> 11;
    int minute = (entry->ctime & 0x07E0) >> 5;
    const char *ampm = (hour >= 12) ? "PM":"AM";
    if (hour > 12){
        hour -= 12;
    }
    if (entry->attr & ATTR_DIR) {
        printf("%02d/%02d/%04d  %02d:%02d %s    <DIR>              ", month, day, year, hour, minute, ampm);
    } 
    else {
        printf("%02d/%02d/%04d  %02d:%02d %s         ", month, day, year, hour, minute, ampm);
        print_size(entry->size);
    }

    unsigned char* user_name = (unsigned char*) calloc (13, sizeof(unsigned char));
    assert(user_name);
    unsigned char* name = (unsigned char*) calloc (8, sizeof(unsigned char));
    assert(name);
    unsigned char* ext = (unsigned char*) calloc (3, sizeof(unsigned char));
    assert(ext);

    split_stored(entry->name, name, ext);
    stored2username(name, ext, user_name);
    
    printf("%s\n", user_name);
    
    free(user_name);    
    user_name = NULL;
    free(name);         
    name = NULL;
    free(ext);          
    ext = NULL;
}


bool is_valid_entry(struct msdos_dir_entry* entry)
{
    // return True iff entry is valid
    if ((entry->name)[0] <= 32 || (entry->name)[0] == 0xE5){
        return false;
    }
    if (entry->attr==ATTR_VOLUME){
        return false;
    }
    return true;
}



struct msdos_dir_entry** read_root_directory(FILE* fp, int RootDirStart)
{
    // reads the root dir and returns a list o the root directory entries
    fseek(fp, RootDirStart+0, SEEK_SET);
    struct msdos_dir_entry** entries = (struct msdos_dir_entry**) calloc (16*14, sizeof(struct msdos_dir_entry*));
    assert(entries);
    for (int i=0; i<14*16; i++){
        struct msdos_dir_entry* entry = (struct msdos_dir_entry*) calloc (1, sizeof(struct msdos_dir_entry));
        assert(entry);
        entries[i] = NULL;
        fread(entry, sizeof(struct msdos_dir_entry), 1, fp);
        if (is_valid_entry(entry)){
            entries[i] = entry;
        }
        else{
            free(entry);
        }
    }
    return entries;
}


int get_first_not_null_index(struct msdos_dir_entry** entries){
    // returns the first index of entry which is not NULL
    for (int i=0; i<14*16; i++){
        if (entries[i] != NULL){
            return i;
        }
    }
    return -1;
}


int find_min(struct msdos_dir_entry** entries){
    // returns the index of the minimum (lex sorted) entry name
    unsigned char* user_name = (unsigned char*) calloc (13, sizeof(unsigned char));
    assert(user_name);
    unsigned char* name = (unsigned char*) calloc (8, sizeof(unsigned char));
    assert(name);
    unsigned char* ext = (unsigned char*) calloc (3, sizeof(unsigned char));
    assert(ext);

    unsigned char* other_user_name = (unsigned char*) calloc (13, sizeof(unsigned char));
    assert(other_user_name);
    unsigned char* other_name = (unsigned char*) calloc (8, sizeof(unsigned char));
    assert(other_name);
    unsigned char* other_ext = (unsigned char*) calloc (3, sizeof(unsigned char));
    assert(other_ext);

    int min_ind = get_first_not_null_index(entries);
    split_stored((entries[min_ind])->name, name, ext);
    stored2username(name, ext, user_name);

    for (int i=0; i<14*16; i++){
        if (entries[i] == NULL){
            continue;
        }
        split_stored((entries[i])->name, other_name, other_ext);
        stored2username(other_name, other_ext, other_user_name);
        if (strcmp(user_name, other_user_name) > 0){
            min_ind = i;
        }
        strcpy(user_name, other_user_name);
    }

    free(other_user_name);
    other_user_name = NULL;
    free(other_name);
    other_name = NULL;
    free(other_ext);
    other_ext = NULL;

    free(user_name);
    user_name = NULL;
    free(name);
    name = NULL;
    free(ext);
    ext = NULL;

    return min_ind;
}


void print_root_dir(struct msdos_dir_entry** entries) {
    // goes over the root dir and prints each entry
    int i;
    int count_entries = 0;
    for (i=0; i<14*16; i++){
        if (entries[i] != NULL){
            count_entries++;
        }
    }
    for (i=0; i<count_entries; i++){
        int min_ind = find_min(entries);
        print_entry(entries[min_ind]);
        free(entries[min_ind]);
        entries[min_ind] = NULL;
    }
}


int find(struct msdos_dir_entry** entries, char* filename){
    // returns the index of entry named filename, if it does not exitst returns -1
    int ind = -1;
    char* filename_copy = (char*) calloc (1+strlen(filename), sizeof(char));
    assert(filename_copy);
    strcpy(filename_copy, filename);
    for (int i=0; i<(int) strlen(filename); i++){
        filename_copy[i] = ToUpperCase(filename[i]);
    }
    unsigned char* user_name = (unsigned char*) calloc (13, sizeof(unsigned char));
    assert(user_name);
    unsigned char* name = (unsigned char*) calloc (8, sizeof(unsigned char));
    assert(name);
    unsigned char* ext = (unsigned char*) calloc (3, sizeof(unsigned char));
    assert(ext);
    for (int i=0; i<14*16; i++){
        if (entries[i] == NULL || (entries[i])->attr & ATTR_DIR){
            continue;
        }
        split_stored((entries[i])->name, name, ext);
        stored2username(name, ext, user_name);
        if (strcmp(user_name, filename_copy)==0){
            ind = i;
            break;
        }
    }
    free(filename_copy);
    filename_copy = NULL;
    free(user_name);
    user_name = NULL;
    free(name);
    name = NULL;
    free(ext);
    ext = NULL;

    return ind;
}


void extract_from_root_dir(FILE* fp, struct fat_boot_sector bs, struct msdos_dir_entry** entries, char* filename, char* dest){
    // extract the content of file named filename from the root directory into a file named dest
    int ind = find(entries, filename);
    if (ind == -1){
        printf("File %s does not exist in the root directory\n", filename);
        return;
    }
    struct msdos_dir_entry* entry = entries[ind];
    unsigned char* user_name = (unsigned char*) calloc (13, sizeof(unsigned char));
    assert(user_name);
    unsigned char* name = (unsigned char*) calloc (8, sizeof(unsigned char));
    assert(name);
    unsigned char* ext = (unsigned char*) calloc (3, sizeof(unsigned char));
    assert(ext);
    split_stored(entry->name, name, ext);
    stored2username(name, ext, user_name);

    unsigned int bytesRead = 0;
    unsigned int data_area = 33*SECTOR_SIZE;
    unsigned int file_data_area = data_area + (entry->start - 2) * bs.sec_per_clus * SECTOR_SIZE;
    unsigned char* content = (unsigned char*) calloc (entry->size, sizeof(char));
    assert(content);

    while (bytesRead < entry->size) {
        fseek(fp, file_data_area + bytesRead, SEEK_SET);
        unsigned int remainingBytes = entry->size - bytesRead;
        unsigned int bytesToRead = (remainingBytes < SECTOR_SIZE) ? remainingBytes : SECTOR_SIZE;
        fread(content + bytesRead, bytesToRead, 1, fp);
        bytesRead += bytesToRead;
    }
    FILE* df = fopen(dest, "wb");
    assert(df);
    for (unsigned int i=0; i<entry->size; i++){
        fprintf(df, "%c", content[i]);
    }
    fclose(df);
    free(content);
    free(user_name);
    free(name);
    free(ext);
}


int main(int argc, char *argv[]) {
    // runs the whole sequece
    if (argc != 3 && argc != 5) {
        return 1;
    }
    const char* command = argv[2];

    FILE* fp = fopen(argv[1], "rb");
    assert(fp);
    struct fat_boot_sector bs;
    fread(&bs, sizeof(struct fat_boot_sector), 1, fp);
    int FirstRootDirSecNum = bs.reserved + (bs.fats * bs.fat_length);
    int RootDirStart = FirstRootDirSecNum * SECTOR_SIZE;
    struct msdos_dir_entry** entries = read_root_directory(fp, RootDirStart);
    if (strcmp(command, "dir") == 0){   
        // dir was chosen
        print_root_dir(entries);
    }
    if (strcmp(command, "cp") == 0){
        // cp was chosen
        char *filename = argv[3];
        char *dest = argv[4];
        extract_from_root_dir(fp, bs, entries, filename, dest);
    }
    fclose(fp);
    for (int i=0; i<14*16; i++){
        free(entries[i]);
        entries[i] = NULL;
    }
    free(entries);
    entries = NULL;
    return 0;
}