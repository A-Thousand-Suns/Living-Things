/*
* File name:  Play_Persist.c
* File ID:	  TTMS_SSF_Play_Perst_Sour
* Author:     Huang Ru
* Version:    V1.0
* Date: 	  2015-04-25
*/


#include "Play_Persist.h"
#include "../Service/play.h"
#include "../Common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

static const char PLAY_DATA_FILE[] = "Play.dat";//�����Ŀ��Ϣ
static const char PLAY_DATA_TEMP_FILE[] = "PlayTmp.dat";//ɾ�������ʱ����ʱ�����Ŀ��Ϣ

/*
 * Function:    Play_Perst_Insert
 * Function ID:	TTMS_SCU_Play_Perst_Insert
 * Description: �ھ�Ŀ��Ϣ�ļ�ĩβд��һ����Ŀ��Ϣ
 * Input:       �������ļ��ľ�Ŀ��Ϣ����
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      д���ļ��ļ�¼��
 */
int Play_Perst_Insert(const play_t *data) {
  int     rtn = 0;
    FILE    *fp;
    fp = fopen(PLAY_DATA_FILE,"ab");
    if(fp == NULL)
    {
        printf("Failed to open file %s!\n[Enter] to return!",PLAY_DATA_FILE);
        getchar();
        return 0;
    }
    rtn = fwrite( data, sizeof(play_t), 1, fp);
    
    fclose(fp);
    
    return rtn;
}

/*
 * Function:    Play_Perst_Update
 * Function ID:	TTMS_SCU_Play_Perst_Mod
 * Description: ���վ�ĿID�Ÿ����ļ��еľ�Ŀ��Ϣ
 * Input:       �����ļ��и��µľ�Ŀ��Ϣ����
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      ���µľ�Ŀ��Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Play_Perst_Update(const play_t *data) {
   int     found = 0;
    play_t  buf;
    FILE    *fp;
    fp = fopen(PLAY_DATA_FILE,"rb+");
    if(fp == NULL)
    {
        printf("Failed to open file %s!\n[Enter] to return!",PLAY_DATA_FILE);
        getchar();
        return 0;
    }
    
    while(!feof(fp))
    {
        if(fread( &buf, sizeof(play_t), 1, fp))
        {
            if(buf.id == data->id)
            {
                fseek(fp,-sizeof(play_t),SEEK_CUR);     
                fwrite( data, sizeof(play_t), 1, fp);
                found = 1;
                break;
            }
        }
    }
    fclose(fp);
    return found;
}

/*
 * Function:    Play_Perst_DeleteByID
 * Function ID:	TTMS_SCU_Play_Perst_DelByID
 * Description: ���վ�ĿID��ɾ����Ŀ����Ϣ
 * Input:       ��ɾ���ľ�ĿID��
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 int c;
    f1 = fopen("in.txt", "rb");
    f2 = fopen("out.txt", "wb");//��in.txt����Ϊout.txt;
    while((c = fgetc(f1)) != EOF)
        fputc(c,f2);
    fcloseall();
    
    return 0;    
 */
int Play_Perst_DeleteByID(int ID) {
	if(rename(PLAY_DATA_FILE, PLAY_DATA_TEMP_FILE)<0){
		printf("Cannot open file %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(PLAY_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("Cannot open file %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(PLAY_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("Cannot open file %s!\n", PLAY_DATA_TEMP_FILE);
		return 0;
	}


	play_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(play_t), 1, fpSour)) {
			if (ID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(play_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//ɾ����ʱ�ļ�
	remove(PLAY_DATA_TEMP_FILE);
	return found;
}

/*
 * Function:    Play_Perst_SelectByID
 * Function ID:	TTMS_SCU_Play_Perst_SelByID
 * Description: ���վ�ĿID�Ų��Ҿ�Ŀ����Ϣ
 * Input:       �����ҵľ�ĿID�ţ�������ҽ�����ڴ�ĵ�ַ
 * Output:      ��
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Play_Perst_SelectByID(int ID, play_t *buf) {
   int     found = 0;
    FILE    *fp;
    play_t  data;
    fp = fopen(PLAY_DATA_FILE,"rb");
    if(fp == NULL)
    {
        printf("Failed to open file %s!\n[Enter] to return!",PLAY_DATA_FILE);
        getchar();
        return 0;
    }
    
    while(!feof(fp))
    {
        if(fread( &data, sizeof(play_t), 1, fp))
        {
            if(ID== data.id)
            {
                *buf = data;
                found = 1;
                break;
            }
        }
    }
    fclose(fp);
    return found;
}


/*
 * Function:    Play_Perst_SelectAll
 * Function ID:	TTMS_SCU_Play_Perst_SelAll
 * Description: �����о�Ŀ��Ϣ������һ������
 * Input:       list��Ŀ��Ϣ�����ͷָ��
 * Output:      ��ʾ��������ʱ������ռ�ʧ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
int Play_Perst_SelectAll(play_list_t list) {
   play_t          data;
    play_node_t     *newNode;
    int             recCount = 0;
    FILE            *fp;
    fp = fopen(PLAY_DATA_FILE,"rb");
    if(fp == NULL)
    {
        //printf("Failed to open file %s!\n[Enter] to return!",STUDIO_FILE);
        //getchar();
        return 0;
    }
    
    List_Free(list,play_node_t);
    
    while(!feof(fp))
    {
        if(fread( &data, sizeof(play_t), 1, fp))
        {
            newNode = (play_node_t *)malloc(sizeof(play_node_t));
            if(newNode)
            {
                newNode->data = data;
                List_AddTail(list,newNode);
                recCount++;
            }
            else
            {
                printf("Memory application failure!!\n[Enter] to continue!");
                getchar();
                break;
            }
        }
    }
    fclose(fp);
    return recCount;
}


/*
 * Function:    Play_Perst_SelectByName
 * Function ID:	TTMS_SCU_Play_Perst_SelByName
 * Description: ���վ�Ŀ���Ʋ��Ҿ�Ŀ����Ϣ
 * Input:       listΪ���ҵ��ľ�Ŀ��Ϣ����condtΪģ����ѯ�Ĺؼ���
 * Output:      ��ʾ��������ʱ������ռ�ʧ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
int Play_Perst_SelectByName(play_list_t list, char condt[]) {
    play_t      data;
    play_node_t *newNode;
    int         iCount = 0;
    FILE        *fp;
    fp = fopen(PLAY_DATA_FILE,"rb");
    if(fp == NULL)
    {
        printf("Failed to open file %s!\n[Enter] to return!",PLAY_DATA_FILE);
        getchar();
        return 0;
    }
    
    List_Free(list,play_node_t);
    
    while (!feof(fp))
    {
        if (fread( &data, sizeof(play_t), 1, fp))
        {
            if (strcmp(condt,data.name) == 0)
            {
                newNode->data = data;
                List_AddTail(list,newNode);
                iCount++;
            }
        }
    }
    return iCount;
}


