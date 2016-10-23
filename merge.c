#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

int readaline_and_out(FILE *fin, FILE *fout);

int
main(int argc, char *argv[])
{
    //입력파일2개, 출력파일1개
    FILE *file1, *file2, *fout;
    
    //file끝
    int eof1 = 0, eof2 = 0;
    
    //file1 line + file2 line => lineout
    long line1 = 0, line2 = 0, lineout = 0;
    struct timeval before, after; //time
    int duration;
    int ret = 1;

    //input error
    if (argc != 4) {
        fprintf(stderr, "usage: %s file1 file2 fout\n", argv[0]);
        goto leave0;
    }
    if ((file1 = fopen(argv[1], "rt")) == NULL) {
        perror(argv[1]);
        goto leave0;
    }
    if ((file2 = fopen(argv[2], "rt")) == NULL) {
        perror(argv[2]);
        goto leave1;
    }
    if ((fout = fopen(argv[3], "wt")) == NULL) {
        perror(argv[3]);
        goto leave2;
    }
    
    gettimeofday(&before, NULL);
    do {
        if (!eof1) {
            if (!readaline_and_out(file1, fout)) {
                line1++; lineout++;
            } else
                eof1 = 1;
        }
        if (!eof2) {
            if (!readaline_and_out(file2, fout)) {
                line2++; lineout++;
            } else
                eof2 = 1;
        }
    } while (!eof1 || !eof2);
    gettimeofday(&after, NULL);
    
    duration = (after.tv_sec - before.tv_sec) * 1000000 + (after.tv_usec - before.tv_usec);
    printf("Processing time = %d.%06d sec\n", duration / 1000000, duration % 1000000);
    printf("File1 = %ld, File2= %ld, Total = %ld Lines\n", line1, line2, lineout);
    ret = 0;
    
leave3:
    fclose(fout);
leave2:
    fclose(file2);
leave1:
    fclose(file1);
leave0:
    return ret; 
}

/* Read a line from fin and write it to fout */
/* return 1 if fin meets end of file */
int
readaline_and_out(FILE *fin, FILE *fout)
{    
 
    int ch, count = 0;      //char ch
    char arr_[300];          //reverse array
    char temp;              //swap value
    int count_2=0;          //string length
    int i;
    int j=0;

    do {
        if ((ch = fgetc(fin)) == EOF) {
            if (!count)
                return 1;
            else {
                fputc(0x0a, fout);
                break;
            }
        }
        arr_[j]=ch;
        count++;
        count_2++;
        j++;
    } while (ch != 0x0a);
    
    for(i = 0; i < count_2 /2; i++){ //swap
        temp = arr_[i];
        arr_[i] = arr_[(count_2-1)-i];
        arr_[(count_2-1)-i] = temp;
    }
    
    for(i = 0; i < count_2; i++){ //fout file->write
        fputc(arr_[i], fout);
    }
    return 0;
}

