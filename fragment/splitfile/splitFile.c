#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>


#define HEADER_LEN	56
#define WAV_LEN		4
#define FILENAME_LEN	512
#define BUFFER_LEN	1024


int SplitFile(char* szFileName, char* szWavDir, char* szTxtDir)
{
	FILE* fin = fopen(szFileName, "rb");
	if (NULL == fin)
	{
		printf("can't open %s\n", szFileName);
		return -1;
	}

	/* get length from data */
	unsigned int wavLen = 0;
	char szWavLen[WAV_LEN + 1];

	if (0 != fseek(fin, HEADER_LEN, SEEK_SET))
	{
		printf("file length is no enough");
		fclose(fin);
		return -1;
	}
	if (NULL == fgets(szWavLen, sizeof(szWavLen), fin))
	{
		printf("can't get wav data length");
		fclose(fin);
		return -1;
	}
	
	wavLen = *(unsigned int*)szWavLen;
	wavLen += HEADER_LEN + WAV_LEN;

	/* open write files */
	char szWavFileName[FILENAME_LEN + 1];
	char szTxtFileName[FILENAME_LEN + 1];
	szWavFileName[FILENAME_LEN] = 0;
	szTxtFileName[FILENAME_LEN] = 0;
	
	strncpy(szWavFileName, szWavDir, sizeof(szWavFileName)-1);
	strcat(szWavFileName, szFileName);
	strcat(szWavFileName, ".wav");

	strncpy(szTxtFileName, szTxtDir, sizeof(szTxtFileName)-1);
	strcat(szTxtFileName, szFileName);
	strcat(szTxtFileName, ".txt");

	FILE* fwav = fopen(szWavFileName, "wb");
	if (NULL == fwav)
	{
		printf("can't open %s\n", szWavFileName);
		fclose(fin);
		return -1;
	}

	FILE* ftxt = fopen(szTxtFileName, "w");
	if (NULL == ftxt)
	{
       	printf("can't open %s\n", szTxtFileName);
		fclose(fin);
		fclose(ftxt);
		return -1;
	}

	/* get file length */
	int fileLen = 0;
	int txtLen = 0;
	char szBuf[10240];
	while(fread(szBuf, 10240, 1, fin));
	fileLen = ftell(fin);
	txtLen = fileLen - wavLen - 8;
	
	/* read input file and write to output files */
	char *pcWavBuf = (char *)malloc(wavLen);
	char *pcTxtBuf = (char *)malloc(txtLen);

	fseek(fin, 0, SEEK_SET);
	fread(pcWavBuf, wavLen, 1, fin);
	fwrite(pcWavBuf, wavLen, 1, fwav);

	fseek(fin, wavLen + 8, SEEK_SET);
	fread(pcTxtBuf, txtLen, 1, fin);
	fwrite(pcTxtBuf, txtLen, 1, ftxt);

	free(pcWavBuf);
	free(pcTxtBuf);
	fclose(fin);
	fclose(fwav);
	fclose(ftxt);

	return 0;
}


int CreateDir(char *szPath)
{
	int iPathLen = strlen(szPath);

	/* add '/' to the path tail */
	if ('/' != szPath[iPathLen - 1])
	{
		strcat(szPath, "/");
		iPathLen++;
	}

	/* create dir */
	int i = 0;
	for (i = 1; i < iPathLen; i++)
	{
		if ('/' == szPath[i])
		{
			szPath[i] = 0;

			if (0 != access(szPath, 0))
			{
				if (0 != mkdir(szPath, 0755))
				{
					printf("can't create dir %s\n", szPath);
					return -1;
				}
			}

			szPath[i] = '/';
		}
	}

	return 0;
}


int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("USAGE:%s filename wavdir txtdir\n", argv[0]);
		return -1;
	}

	char szWavPath[256];   //linux max path length is 255
	strcpy(szWavPath, argv[2]);
	if (0 != CreateDir(szWavPath))
	{
		return -1;
	}

	char szTxtPath[256];   //linux max path length is 255
	strcpy(szTxtPath, argv[3]);
	if (0 != CreateDir(szTxtPath))
	{
		return -1;
	}

	return SplitFile(argv[1], szWavPath, szTxtPath);
}
