#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>


#define HEADER_LEN	44
#define FIRST_POS	4
#define SECOND_POS	40


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


int CutWave(const char *szInputFileName, FILE *pstFileInput, const int iSegTime, const char *szPath)
{
	int i = 0;
	int iWFIndex = 0;
	unsigned int uiFileReadLen = 0;
	unsigned int uiSegSecondLen = iSegTime * 60 * 8000 * 2;
	unsigned int uiSegTotalLen = uiSegSecondLen + HEADER_LEN;
	unsigned int uiSegFirstLen = uiSegTotalLen - 8;
	char *pcSegData = NULL;
	char *pcLenBytes = NULL;
	char szHeader[HEADER_LEN + 1];
	char szWFIndex[8];
	char szRFName[256];
	char szWFName[256];

	pcSegData = (char *)malloc(uiSegSecondLen + 1);
	if (NULL == pcSegData)
	{
		printf("malloc memory error\n");
		return -1;
	}

	/* XX.wav to path/XX_ */
	strncpy(szRFName, szPath, sizeof(szRFName)-1);
	szRFName[sizeof(szRFName)-1] = 0;
	strcat(szRFName, szInputFileName);
	for (i = strlen(szRFName)-1; i >=0; i--)
	{
		if ('.' == szRFName[i])
		{
			szRFName[i] = '_';
			szRFName[i+1] = 0;
		}
	}

	/* WF header */
	fread(szHeader, HEADER_LEN, 1, pstFileInput);
	uiFileReadLen += HEADER_LEN;

	pcLenBytes = (char *)szHeader + FIRST_POS;
	*(unsigned int *)pcLenBytes = uiSegFirstLen;
	pcLenBytes = (char *)szHeader + SECOND_POS;
	*(unsigned int *)pcLenBytes = uiSegSecondLen;

	/* read file and write */
	iWFIndex = 1;
	while (0 != fread(pcSegData, uiSegSecondLen, 1, pstFileInput))
	{
		/* create write file */
		sprintf(szWFIndex, "%d", iWFIndex);
		strcpy(szWFName, szRFName);
		strcat(szWFName, szWFIndex);
		strcat(szWFName, ".wav");

		FILE *pstFileOutput = fopen(szWFName, "wb");
		if (NULL == pstFileOutput)
		{
			printf("can't open file %s\n", szWFName);
			return -1;
		}

		fwrite(szHeader, HEADER_LEN, 1, pstFileOutput);
		fwrite(pcSegData, uiSegSecondLen, 1, pstFileOutput);
		fclose(pstFileOutput);

		uiFileReadLen += uiSegSecondLen;
		iWFIndex++;
	}

	/* check last fread */
	unsigned int uiLastReadLen = ftell(pstFileInput) - uiFileReadLen;
	if (0 != uiLastReadLen)
	{
		fseek(pstFileInput, -uiLastReadLen, SEEK_END);
		fread(pcSegData, uiLastReadLen, 1, pstFileInput);

		/* modify header */
		pcLenBytes = (char *)szHeader + FIRST_POS;
		*(unsigned int *)pcLenBytes = uiLastReadLen + HEADER_LEN - 8;
		pcLenBytes = (char *)szHeader + SECOND_POS;
		*(unsigned int *)pcLenBytes = uiLastReadLen;

		/* write file */
		sprintf(szWFIndex, "%d", iWFIndex);
		strcpy(szWFName, szRFName);
		strcat(szWFName, szWFIndex);
		strcat(szWFName, ".wav");

		FILE *pstFileOutput = fopen(szWFName, "wb");
		if (NULL == pstFileOutput)
		{
			printf("can't open file %s\n", szWFName);
			return -1;
		}

		fwrite(szHeader, HEADER_LEN, 1, pstFileOutput);
		fwrite(pcSegData, uiSegSecondLen, 1, pstFileOutput);
		fclose(pstFileOutput);
	}

	free(pcSegData);

	return 0;
}


int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("USAGE:%s input_file segtime(min) output_dir\n", argv[0]);
		return -1;
	}

	/* check input file is OK */
	FILE *pstFileInput = fopen(argv[1], "rb");
	if (NULL == pstFileInput)
	{
		printf("can't open file %s\n", argv[1]);
		return -1;
	}

	/* check segment time is OK */
	int iSegTime = atoi(argv[2]);
	if (iSegTime <= 0)
	{
		printf("segtime is invalid\n");
		fclose(pstFileInput);
		return -1;
	}
	
	/* make output dir */
	char szPath[256];   //linux max path length is 255
	strcpy(szPath, argv[3]);
	if (0 != CreateDir(szPath))
	{
		printf("can't create dir %s\n", argv[3]);
		fclose(pstFileInput);
		return -1;
	}

	CutWave(argv[1], pstFileInput, iSegTime, szPath);
	fclose(pstFileInput);

	return 0;
}
