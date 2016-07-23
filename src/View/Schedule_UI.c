/*
* File name:  Schedule_UI.c
* File ID:	  TTMS_SSF_Schedule_UI_Sour
* Author:     Huang Ru
* Version:    V1.0
* Date: 	  2015-04-25
*/


#include "Schedule_UI.h"
#include "Play_UI.h"

#include "../Common/list.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"

#include <stdio.h>
#include <stdlib.h>

	/*
	 * Function:    Schedule_UI_ListAll
	 * Function ID:	TTMS_SCU_Schedule_UI_List
	 * Description: ��ʾ���е��ݳ��ƻ���Ϣ,���в�ѯ��
	 * Input:       ��
	 * Output:      ���о�Ŀ����Ϣ
	 * Return:      ��
	 */
void Schedule_UI_ListAll(void) {
	system("cls");
	schedule_list_t     list;
    schedule_node_t     *p;
	int                 Snumber,i,pageSize = 10;
	Pagination_t        paging;
	char                choice;
    int                 id;
	
    List_Init(list,schedule_node_t);
    
    printf("Please input Play ID:");
    scanf("%d",&id);
    
    Snumber = Schedule_Srv_FetchByPlay(list,id);
    
    paging.totalRecords = Snumber;
    paging.pageSize = pageSize;
    Paging_Locate_FirstPage(list,paging);           //��λ����һҳ
    
    do {
    	 system("cls");
        printf("================================ Schedule List ===================================\n");
        printf("ID\tPlay_ID\t\tStudio_ID\tData\tTime\tSeat_Count\n");
        printf("----------------------------------------------------------------------------------\n");
        for (i = 0, p = (schedule_node_t *) (paging.curPos); //��ǰҳ�����ʼλ��
             p != list && i < paging.pageSize; i++) {
            printf("%d\t%d\t%d\t%d.%d.%d\t%d:%d:%d\t%d\n", p->data.id, p->data.play_id,
                   p->data.studio_id,
                   p->data.date.year,p->data.date.month,p->data.date.day,
                   p->data.time.hour,p->data.time.minute,p->data.time.second,
                   p->data.seat_count);
            p=p->next;
        }
        printf("--Total Records:%2d --------------------------------------------------- Page %2d/%2d --\n",
               paging.totalRecords, Pageing_CurPage(paging),
               Pageing_TotalPages(paging));
        printf("[P]revPage | [N]extPage | [R]eturn\n");
        printf("=====================================================================================\n");
        printf("Your Choice:");
        while(getchar() != '\n');
        scanf("%c", &choice);
        if(choice=='r'||choice=='R')
        return ;
        switch (choice) {
            case 'p':
            case 'P':
                if (1 < Pageing_CurPage(paging)) {
                    Paging_Locate_OffsetPage(list, paging, -1, schedule_node_t); //ǰ��
                }
                break;
            case 'n':
            case 'N':
                if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
                    Paging_Locate_OffsetPage(list, paging, 1, schedule_node_t);  //����
                }
                break;
        }
    } while (choice != 'r' && choice != 'R');
    
    List_Destroy(list,schedule_node_t);
}

/*
 * Function:    Schedule_UI_Add
 * Function ID:	TTMS_SCU_Schedule_UI_Add
 * Description: ���һ���ݳ��ƻ���Ϣ
 * Input:       ��ؾ�Ŀ��ID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ��ӵļ�¼��
 */
int Schedule_UI_Add(int play_id) {
	system("cls");
int         newCount = 0;
    int         key;
    char        choice;
    schedule_t  pnew;
    studio_t    data;
    printf("=================================== Schedule Add ==================================\n");
    do{
        
        key = EntKey_Srv_CompNewKey("schedule"); //��������
        pnew.id = key;
        pnew.play_id = play_id;
        printf("Please input Studio_ID:\n");
       
        scanf("%d",&pnew.studio_id);
        printf("Please input Date (like 2015 06 29):\n");
        scanf("%d %d %d",&pnew.date.year, &pnew.date.month, &pnew.date.day);
        printf("Please input Time (like 12(hour) 30(minute) 00(second)):\n");
        scanf("%d %d %d",&pnew.time.hour, &pnew.time.minute, &pnew.time.second);
        if (Studio_Srv_FetchByID(pnew.studio_id, &data)) {
            pnew.seat_count = data.seatsCount;
            if (Schedule_Srv_Add(&pnew)) {
                    printf("Add succeed!!\n");
                    newCount++;
                    printf("Continue? (y/n):");
                    getchar();
                    scanf("%c",&choice);
            }
            else
            {
                printf("Schedule Add fail!\n");
                sleep(1);
                return newCount;
            }
        }
        else{
            printf("Cannot Found Studio!\n");
            sleep(1);
            return newCount;
        }
    }while (choice == 'y' || choice == 'Y');
    return newCount;
}

/*
 * Function:    Schedule_UI_Modify
 * Function ID:	TTMS_SCU_Schedule_UI_Mod
 * Description: ����ID�Ÿ����ݳ��ƻ���Ϣ
 * Input:       �����µľ�ĿID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ���µľ�Ŀ��Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Schedule_UI_Modify(int id){
	system("cls");
	int         rtn = 0;
    schedule_t  temp;
    //schedule_t  pnew;
    studio_t    data;
    if (Schedule_Srv_FetchByID( id, &temp)) {
        printf("================================= Schedule Update ================================\n");
        printf("------------------------------------ Old Data ----------------------------------\n");
        printf("ID: \t%d\n",temp.id);
        printf("Play_id: \t%d\n",temp.play_id);
        printf("Studio_id: \t%d\n",temp.studio_id);
        printf("Date: \t%d %d %d\n", temp.date.year, temp.date.month, temp.date.day);
        printf("Time: \t%d %d %d\n",temp.time.hour, temp.time.minute, temp.time.second);
        printf("SeatCount: \t%d\n",temp.seat_count);
        printf("\n------------------------------------ New Data ----------------------------------\n");
        printf("Please input Studio_ID:\n");
        scanf("%d",&temp.studio_id);
        if(Studio_Srv_FetchByID(temp.studio_id, &data))
        {
            printf("Please input Date (like 2015 06 29):\n");
            scanf("%d %d %d",&temp.date.year, &temp.date.month, &temp.date.day);
            printf("Please input Time (like 12(hour) 30(minute) 00(second)):\n");
            scanf("%d %d %d",&temp.time.hour, &temp.time.minute, &temp.time.second);
            temp.seat_count = data.seatsCount;
            if (Schedule_Srv_Modify(&temp))
            {
                rtn = 1;
                printf("Update successfully!\n");
            }
            else{
                printf("Update failly!\n");
            }
        }
        else
        {
            printf("Cannot Found Studio!\n[E]nter to return \n");
            getch();
            return 0;
        }
    }
    else
    {
        printf("Schedule Not Found!\n");
        sleep(1);
        return 0;
    }
    sleep(1);
    return rtn;
}

/*
 * Function:    Schedule_UI_Delete
 * Function ID:	TTMS_SCU_Schedule_UI_Del
 * Description: ����ID��ɾ���ݳ��ƻ���Ϣ
 * Input:       ��ɾ���ľ�ĿID��
 * Output:      ��ʾɾ���Ƿ�ɹ�
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Schedule_UI_Delete(int id){
system("cls");
	int rtn = 0;
    printf("========================= Schedule Delete =========================\n");
    if (Schedule_Srv_DeleteByID(id)) {
        rtn = 1;
        printf("Delete successfully!\n");
    }
    else
    {
        printf("Delete failly!\n");
    }
    sleep(1);
    return rtn;
}

/*
 * Function:    Schedule_UI_Query
 * Function ID:	TTMS_SCU_Schedule_UI_Qry
 * Description: ����ID�Ų�ѯ�ݳ��ƻ���Ϣ
 * Input:       �����ҵľ�ĿID��
 * Output:      ���ҵ��ľ�Ŀ��Ϣ
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Schedule_UI_Query(int id){
	system("cls");
	int         rtn = 0;
    schedule_t  buf;
    if (Schedule_Srv_FetchByID( id, &buf)) {
        rtn = 1;
        printf("Found this schedule!!\n");
        printf("=========================== Schedule Information ===========================\n");
        printf("ID: \t%d",buf.id);
        printf("Play_id: \t%d",buf.play_id);
        printf("Studio_id: \t%d",buf.studio_id);
        printf("Date: \t%d %d %d", buf.date.year, buf.date.month, buf.date.day);
        printf("Time: \t%d %d %d",buf.time.hour, buf.time.minute, buf.time.second);
        printf("SeatCount: \t%d",buf.seat_count);
        printf("-----------------------------------------------------------------------\n");
        printf("[Enter] to return !!");
        while(getchar() != '\n');
    }
    else
    {
        printf("Not found this schedule!!");
        sleep(1);
    }
    return rtn;


}

/*
 * Function:    Schedule_UI_ListByPlay
 * Function ID:	TTMS_SCU_Schedule_UI_ListByPlay
 * Description: ���б�ģʽ��ʾ������Ŀ���ݳ��ƻ���Ϣ
 * Input:       listΪ���ҵ����ݳ��ƻ���Ϣ�����ͷָ�룬playΪ��Ŀ��Ϣ��pagingΪ��ʾ��ҳ���ò���
 * Output:      ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
void Schedule_UI_ListByPlay(const play_t *play, schedule_list_t list, Pagination_t paging){

}

/*
 * Function:    Schedule_UI_MgtEntry
 * Function ID:	TTMS_SCU_Schedule_UI_MgtEnt
 * Description: �ݳ��ƻ�������棬�ݳ��ƻ����վ�Ŀ���й����Ծ�ĿID��Ϊ����
 * Input:       ��ؾ�ĿID��
 * Output:      �ݳ��ƻ���Ϣ
 * Return:      ��
 */
void Schedule_UI_MgtEntry(int play_id){
	system("cls");
	schedule_list_t     list;
    schedule_node_t     *p;
    int                 Snumber,i,pageSize = 10;
    Pagination_t        paging;
    char                choice;
    int                 id;
    //schedule_t          buf;
    
    List_Init(list,schedule_node_t);
    Snumber = Schedule_Srv_FetchByPlay(list, play_id);
    paging.totalRecords = Snumber;
    paging.pageSize = pageSize;
    Paging_Locate_FirstPage(list,paging);           //��λ����һҳ
    
    do {
        system("cls");
        printf("================================ Schedule List ===================================\n");
        printf("ID\tPlay_ID\t\tStudio_ID\tData\t\tTime\tSeat_Count\n");
        printf("----------------------------------------------------------------------------------\n");
        for (i = 0, p = (schedule_node_t *) (paging.curPos); //��ǰҳ�����ʼλ��
             p != list && i < paging.pageSize; i++) {
            printf("%d%10d%20d%10d.%d.%d%10d:%d:%d%10d\n", p->data.id, p->data.play_id,
                   p->data.studio_id,
                   p->data.date.year,p->data.date.month,p->data.date.day,
                   p->data.time.hour,p->data.time.minute,p->data.time.second,
                   p->data.seat_count);
            p=p->next;
        }
        printf("--Total Records:%2d --------------------------------------------------- Page %2d/%2d --\n",
               paging.totalRecords, Pageing_CurPage(paging),
               Pageing_TotalPages(paging));
        printf("[A]dd  |  [D]elete  |  [U]pdate\n");
        printf("[P]revPage | [N]extPage | [R]eturn\n");
        printf("=====================================================================================\n");
        printf("Your Choice:");
        while(getchar() != '\n');
        scanf("%c", &choice);
        switch (choice) {
            case 'a':
            case 'A':
                if (Schedule_UI_Add(play_id)) {
                    paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
                    Paging_Locate_LastPage(list, paging, schedule_node_t);    //��λ�����һҳ
                }
                break;
            case 'd':
            case 'D':
                printf("Please input Schedule id:");
                scanf("%d",&id);
                if (Schedule_UI_Delete(id)) {
                    paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
                    List_Paging(list, paging, schedule_node_t);
                }
                break;
            case 'u':
            case 'U':
                printf("Please input Schedule id:");
                scanf("%d",&id);
                if (Schedule_UI_Modify(id)) {
                    paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
                    List_Paging(list, paging, schedule_node_t);
                }
                break;
            case 'p':
            case 'P':
                if (1 < Pageing_CurPage(paging)) {
                    Paging_Locate_OffsetPage(list, paging, -1, schedule_node_t); //ǰ��
                }
                break;
            case 'n':
            case 'N':
                if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
                    Paging_Locate_OffsetPage(list, paging, 1, schedule_node_t);  //����
                }
                break;
        }
    } while (choice != 'r' && choice != 'R');
    
    List_Destroy(list,schedule_node_t);

}

