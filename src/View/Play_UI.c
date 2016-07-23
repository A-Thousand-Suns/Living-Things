/*
 * 
 *
 * Created on: 2015年6月29日
 *  Author on: King
 */

#include "Play_UI.h"
#include "Schedule_UI.h"

#include "../Common/List.h"
#include "../Service/Play.h"
#include "../Service/EntityKey.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//主界面函数
void Play_UI_MgtEntry(int flag) {
	system("cls"); 

    //函数实现部分
    play_list_t     list;
    play_node_t     *p;
    int             Snumber,i,pageSize=4545;
    Pagination_t    paging;
    char            choice;
    int             id;
    play_t          buf;
    
    List_Init(list,play_node_t);
    
    Snumber = Play_Srv_FetchAll(list);
    
   
    
    paging.totalRecords = Snumber;
    paging.pageSize = pageSize;
    Paging_Locate_FirstPage(list,paging);           //定位到第一页
    
    do {
    	system("cls");
        printf("==================================== Play List ========================================\n");
        printf("ID\tName\tType\tArea\tRating\tDuration\tStart_Date\tEnd_Date\tPrice\n");
        printf("---------------------------------------------------------------------------------------\n");
        for (i = 0, p = (play_node_t *) (paging.curPos); //当前页面的起始位置
             p != list && i < paging.pageSize; i++) {
            printf("%d\t%s\t%s\t%s\t%s\t%d\t\t%d.%d.%d\t\t%d.%d.%d\t\t%d\n", p->data.id, p->data.name,
                   (p->data.type == 1)?"film":(p->data.type == 2?"opear":"concert"),
                   p->data.area,
                   (p->data.rating == 1)?"child":(p->data.rating == 2?"teenage":"adult"),
                   p->data.duration,
                   p->data.start_date.year,p->data.start_date.month,p->data.start_date.day,
                   p->data.end_date.year,p->data.end_date.month,p->data.end_date.day,
                   p->data.price);
            p=p->next;
        }
        printf("--Total Records:%2d --------- Page %2d/%2d --\n",
               paging.totalRecords, Pageing_CurPage(paging),
               Pageing_TotalPages(paging));
        if(flag == 0)
        {
            printf("[A]dd  |  [D]elete  |  [U]pdate | [S]chedule \n");
            printf("[P]revPage | [N]extPage | [R]eturn\n");
            printf("===================================================================================\n");
            do{
                printf("Your Choice:");
		fflush(stdin);
                scanf("%c", &choice);
            }while(choice != 'a' && choice != 'A' && choice != 'd' && choice != 'D' &&
                   choice != 'u' && choice != 'U' && choice != 'r' && choice != 'R'&& choice != 'S' && choice != 's');
        }
        else if(flag == 1)
        {
            printf("[S]chedule  |  [Q]uery\n");
            printf("[P]revPage | [N]extPage | [R]eturn\n");
            printf("=================================================================================\n");
            do{
                printf("Your Choice:");
		fflush(stdin);
                scanf("%c", &choice);
            }while(choice != 'q' && choice != 'Q' && choice != 'S' && choice != 's' &&
                   choice != 'r' && choice != 'R');
        }
        
        switch (choice) {
            case 's':
            case 'S':
                printf("Please input id:");
                scanf("%d",&id);
                if (Play_Perst_SelectByID(id,&buf)) {
                    Schedule_UI_MgtEntry(id);
                }
                else
                {
                    printf("The play does not exist！！\n[E]nter to return \n");
                    getch();
                }
                break;
            case 'q':
            case 'Q':
                printf("Please input id:");
                scanf("%d",&id);
              //  Play_UI_Query(id);
                break;
            case 'a':
            case 'A':
                if (Play_UI_Add()) {
                    paging.totalRecords = Play_Srv_FetchAll(list);
                    Paging_Locate_LastPage(list, paging, play_node_t);    //定位到最后一页
                }
                break;
            case 'd':
            case 'D':
                printf("Please input id:");
                scanf("%d",&id);
                if (Play_UI_Delete(id)) {
                    paging.totalRecords = Play_Srv_FetchAll(list);
                    List_Paging(list, paging, play_node_t);
                }
                break;
            case 'u':
            case 'U':
                printf("Please input id:");
                scanf("%d",&id);
                if (Play_UI_Modify(id)) {
                    paging.totalRecords = Play_Srv_FetchAll(list);
                    List_Paging(list, paging, play_node_t);
                }
                break;
            case 'p':
            case 'P':
                if (1 < Pageing_CurPage(paging)) {
                    Paging_Locate_OffsetPage(list, paging, -1, play_node_t); //前移
                }
                break;
            case 'n':
            case 'N':
                if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
                    Paging_Locate_OffsetPage(list, paging, 1, play_node_t);  //后移
                }
                break;
        }
        
    } while (choice != 'r' && choice != 'R');
    
    List_Destroy(list,play_node_t);
}

//添加剧目界面
int Play_UI_Add(void) {

    //函数实现部分
    int     newCount = 0;
    int     key;
    char    choice;
    play_t  pnew;
    printf("======================= Play Add ========================\n");
    do{
	
		system("cls");
        //pnew = (studio_t *)malloc(sizeof(studio_t));
        printf("Please input Play Name:\n");
        //fflush(stdin);
        scanf("%s",pnew.name);
        printf("(1.PLAY_TYPE_FILM  2.PLAY_TYPE_OPEAR  3.PLAY_TYPE_CONCERT)\n");
        printf("Please input play_type:");
        scanf("%d",&pnew.type);
        printf("Please input area:\n");
        scanf("%s",pnew.area);
        printf("(1.PLAY_RATE_CHILD  2.PLAY_RATE_TEENAGE  3.PLAY_RATE_ADULT)\n");
        printf("Please input rating:\n");
        scanf("%d",&pnew.rating);
        printf("Please input duration (minute):");
        scanf("%d",&pnew.duration);
        printf("Please input start_date (like 2015 06 29):\n");
        scanf("%d %d %d",&pnew.start_date.year, &pnew.start_date.month, &pnew.start_date.day);
        printf("Please input end_date (like 2015 06 29):\n");
        scanf("%d %d %d",&pnew.end_date.year, &pnew.end_date.month, &pnew.end_date.day);
        printf("Please input price:");
        scanf("%d",&pnew.price);
        key = EntKey_Srv_CompNewKey("play"); 
        pnew.id = key;
        if (Play_Srv_Add(&pnew)) {
            printf("Add succeed!!\n");
            newCount++;
            printf("Continue? (y/n):");
            getchar();
		fflush(stdin);
            scanf("%c",&choice);
        }
        else
        {
            printf("Add fail!\n");
            sleep(1);
            return newCount;
        }
    }while (choice == 'y' || choice == 'Y');
    return newCount;
}

//修改剧目界面
int Play_UI_Modify(int id) {
	system("cls");

    int     rtn = 0;
    play_t  temp;
    printf("========================= Play Update =========================\n");
    if (Play_Srv_FetchByID(id,&temp)) {
        printf("--------------------------- Old Data ----------------------------\n");
        printf("ID: \t%d",temp.id);
        printf("Name: \t%s",temp.name);
        printf("Play_Type: \t%s",(temp.type == 1)?"film":(temp.type == 2?"opear":"concert"));
        printf("Area: \t%s",temp.area);
        printf("Play_Dating: \t%s",(temp.rating == 1)?"child":(temp.rating == 2?"teenage":"adult"));
        printf("Duration: \t%d",temp.duration);
        printf("Start_Date: \t%d %d %d", temp.start_date.year, temp.start_date.month, temp.start_date.day);
        printf("End_Date: \t%d %d %d",temp.end_date.year, temp.end_date.month, temp.end_date.day);
        printf("Price: \t%d",temp.price);
        printf("--------------------------- New Data ----------------------------\n");
        printf("Please input New N ame:\n");
        //fflush(stdin);
        scanf("%s",temp.name);
        printf("(1.PLAY_TYPE_FILE  2.PLAY_TYPE_OPEAR  3.PLAY_TYPE_CONCERT)\n");
        printf("Please input New play_type:");
        scanf("%d",&temp.type);
        printf("Please input New area:\n");
        scanf("%s",temp.area);
        printf("(1.PLAY_RATE_CHILD  2.PLAY_RATE_TEENAGE  3.PLAY_RATE_ADULT)\n");
        printf("Please input New rating:\n");
        scanf("%d",&temp.rating);
        printf("Please input New duration (minute):");
        scanf("%d",&temp.duration);
        printf("Please input New start_date (like 2015 06 29):\n");
        scanf("%d %d %d",&temp.start_date.year, &temp.start_date.month, &temp.start_date.day);
        printf("Please input New end_date (like 2015 06 29):\n");
        scanf("%d %d %d",&temp.end_date.year, &temp.end_date.month, &temp.end_date.day);
        printf("Please input New price:");
        scanf("%d",&temp.price);
        if (Play_Srv_Modify(&temp)) {
            rtn = 1;
            printf("Update successfully!\n");
        }
        else
        {
            printf("Update failly!\n");
        }
    }
    else
    {
        printf("Play Not Found!\n[Enter] to return...");
        getchar();
        return 0;
    }
    sleep(1);
    return rtn;

}

//删除剧目界面
int Play_UI_Delete(int id) {

    int rtn = 0;
    printf("========================= Play Delete =========================\n");
    if (Play_Srv_DeleteByID(id)) {
        rtn = 1;
        printf("Delete successful\n");
    }
    else
    {
        printf("Delete failly!\n");
    }
    sleep(1);
    return rtn;
}

//查询剧目界面
int Play_UI_Query(int id) {

    int     rtn = 0;
    play_t  buf;
    if (Play_Srv_FetchByID(id,&buf)) {
        rtn = 1;
        printf("Found this play!!\n");
        printf("=========================== Play Information ===========================\n");
        printf("ID\tName\tType\tArea\tRating\tDuration\tStart_Date\tEnd_Date\tPrice\n");
        printf("-----------------------------------------------------------------------\n");
        printf("%d\t%s\t%s\t%s\t%s\t%d\t%d.%d.%d\t%d.%d.%d\t%d\n", buf.id, buf.name,
               (buf.type == 1)?"film":(buf.type == 2?"opear":"concert"),
               buf.area,
               (buf.rating == 1)?"child":(buf.rating == 2?"teenage":"adult"),
               buf.duration,
               buf.start_date.year, buf.start_date.month, buf.start_date.day,
               buf.end_date.year, buf.end_date.month, buf.end_date.day,
               buf.price);
        printf("-----------------------------------------------------------------------\n");
        //printf("Automatically jump to the performance plan management layer!!");
        printf("[Enter] to return !!");
        while(getchar() != '\n');
    }
    else
    {
        printf("Not found this play.\n");
        sleep(1);
        
    }
    return rtn;
}

/*
//以列表形式显示剧目信息
void Play_UI_ShowList(play_list_t list, Pagination_t paging) {

    //函数实现部分
}
*/

