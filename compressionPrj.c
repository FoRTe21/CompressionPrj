#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 256
#define TRUE 1
#define FALSE 0

void compression(FILE *fpInput, FILE *fpOutput);
void deCompression(FILE *fpInput, FILE *fpOutput);


void main(int argc, char* argv[])
{
	if(argc != 4)										// 1. 압축 또는 풀기
	{													// 2. target 파일이름.
		printf("Usage : command(c or d) target output\n");		// 3. output 파일이름.
		exit(1);
	}

	char cmd[BUFSIZE] = {0,}; 		// output파일의 권한 설정용 버퍼.
	FILE *fpInput = NULL;
	FILE *fpOutput = NULL;

	fpInput = fopen(argv[2], "rb");

	if(fpInput == NULL)
	{
		printf("There's not a file : %s\n", argv[1]);
		exit(1);
	}
	fpOutput = fopen(argv[3], "wb");	
	if(strcmp(argv[1],"c") == 0)
	{
		compression(fpInput, fpOutput);	// 압축
	}
	else if(strcmp(argv[1], "d") == 0)
	{
		deCompression(fpInput, fpOutput); // 풀기
	}
	else 
	{
		printf("There's not a command such as %s\ncommand : c or d\n", argv[1]); // c,d 이외의 명령어 경우 프로그램 종료.

		fclose(fpInput);
		fclose(fpOutput);
		exit(1);
	}

	fclose(fpInput);
	fclose(fpOutput);

	sprintf(cmd, "chmod 775 %s",argv[3]);		// output파일의 권한 설정.
	system(cmd);
}

//=========================================================
// target 파일을 읽어가면서 바로 output 파일에 출력.
// 데이터값이 0인 경우 바로 output파일에 출력하지 않고 
// count를 증가시킴(이 때 한계는 255)
// 0이 끝나면 count값을 output파일에 저장.
//=========================================================
void compression(FILE *fpInput, FILE *fpOutput)
{
	int ch;
	int flag = FALSE;
	int count = 0;
	while((ch = fgetc(fpInput)) != EOF)		// 1byte씩 파일 읽기.
	{
		if(ch == 0 && flag == FALSE)		// 데이터값이 0이면 
		{									// count 증가하기 시작.
			fputc(ch, fpOutput);
			count++;
			flag = TRUE;	
			continue;
		}
		else if(ch == 0 && flag == TRUE)	// 0이 계속되면 count 증가,
		{									// 1byte 한계선인 255까지만 증가
			count++;
			if(count == 255)				// 255 이상이면 count를 다시 증가.
			{
				fputc(count, fpOutput); 
				count = 0;
				flag = FALSE;
			}
			continue;
		}
		else if(ch != 0 && flag == TRUE)	// 0에서 다른 값으로 됐을때 count를 output 파일에 출력.
		{
			fputc(count, fpOutput);
			fputc(ch, fpOutput);
			count = 0;
			flag = FALSE;
			continue;
		}
		else
		{
			fputc(ch, fpOutput);
		}
	}
}
//====================================================
//기본적으로 target 파일의 데이터를 target 파일로 출력.
//0이 나오면 뒤의 데이터값을 0의 개수로 인식하여
//그 수만큼 0을 출력.
//===================================================
void deCompression(FILE *fpInput, FILE *fpOutput)
{
	int i = 0;
	int ch = 0;
	int flag = FALSE;
	int count = 0;
	while((ch = fgetc(fpInput)) != EOF)
	{
		if(ch == 0)
		{
			count = fgetc(fpInput);	
			for(i = 0; i < count; i++)
			{
				fputc(0,fpOutput);
			}
		}	
		else
		{
			fputc(ch, fpOutput);
		}
	}
	fputc(0, fpOutput);
}
