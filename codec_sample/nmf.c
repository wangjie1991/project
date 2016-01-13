#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("USAGE:%s nmf_input g729_output.\n", argv[0]);
        fflush(stdout);
        return -1;
    }

    FILE *pf_nmf = fopen(argv[1], "rb");
    if (NULL == pf_nmf)
    {
        printf("nmf file %s can't open.\n", argv[1]);
        fflush(stdout);
        return -1;
    }

    FILE *pf_g729 = fopen(argv[2], "wb");
    if (NULL == pf_g729)
    {
        fclose(pf_nmf);
        printf("g729 file %s can't open.\n", argv[2]);
        fflush(stdout);
        return -1;
    }

    int file_len = 0;
    int label = 0;
    int all_len = 0;
    int head_len = 0;
    int data_len = 0;

    fseek(pf_nmf, 0, SEEK_END);
    file_len = (int)ftell(pf_nmf);
    printf("%d\n", file_len);
    fflush(stdout);
    char *pcBuf = (char *)malloc(file_len);

    fseek(pf_nmf, 166, SEEK_SET);
    fseek(pf_g729, 0, SEEK_SET);
    while(1)
    {
        fread(&label, 4, 1, pf_nmf);
        if (label != 4)
        {
            break;
        }

        fseek(pf_nmf, 16, SEEK_CUR);
        fread(&all_len, 4, 1, pf_nmf);
        fread(&head_len, 4, 1, pf_nmf);
        fseek(pf_nmf, head_len, SEEK_CUR);

        data_len = all_len - head_len;
        fread(pcBuf, data_len, 1, pf_nmf);
        fwrite(pcBuf, data_len, 1, pf_g729);
    }

    free(pcBuf);
    fclose(pf_nmf);
    fclose(pf_g729);

    return 0;
}

