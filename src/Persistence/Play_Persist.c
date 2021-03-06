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

static const char PLAY_DATA_FILE[] = "Play.dat";//保存剧目信息
static const char PLAY_DATA_TEMP_FILE[] = "PlayTmp.dat";//删除或更新时，暂时保存剧目信息

/*
 * Function:    Play_Perst_Insert
 * Function ID:	TTMS_SCU_Play_Perst_Insert
 * Description: 在剧目信息文件末尾写入一条剧目信息
 * Input:       待加入文件的剧目信息数据
 * Output:      打开文件失败的信息
 * Return:      写入文件的记录数
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
 * Description: 按照剧目ID号更新文件中的剧目信息
 * Input:       待在文件中更新的剧目信息数据
 * Output:      打开文件失败的信息
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
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
 * Description: 按照剧目ID号删除剧目的信息
 * Input:       待删除的剧目ID号
 * Output:      打开文件失败的信息
 * Return:      0表示删除失败，1表示删除成功
 int c;
    f1 = fopen("in.txt", "rb");
    f2 = fopen("out.txt", "wb");//将in.txt复制为out.txt;
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

	//删除临时文件
	remove(PLAY_DATA_TEMP_FILE);
	return found;
}

/*
 * Function:    Play_Perst_SelectByID
 * Function ID:	TTMS_SCU_Play_Perst_SelByID
 * Description: 按照剧目ID号查找剧目的信息
 * Input:       待查找的剧目ID号，保存查找结果的内存的地址
 * Output:      无
 * Return:      0表示未找到，1表示找到了
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
 * Description: 将所有剧目信息建立成一条链表
 * Input:       list剧目信息链表的头指针
 * Output:      提示建立链表时，申请空间失败
 * Return:      返回查找到的记录数目
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
 * Description: 按照剧目名称查找剧目的信息
 * Input:       list为查找到的剧目信息链表，condt为模糊查询的关键字
 * Output:      提示建立链表时，申请空间失败
 * Return:      返回查找到的记录数目
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


