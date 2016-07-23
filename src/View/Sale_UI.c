#include "Play_UI.h"
#include "Schedule_UI.h"
#include "Sale_UI.h"
#include "Ticket_UI.h"
#include "../Service/ticket.h"
#include "../Service/seat.h"
#include "../Service/studio.h"
#include "../Service/EntityKey.h"
#include "../Service/Sale.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//引用登陆用户的全局数据结构
extern account_t gl_CurUser;

//根据计划ID，显示演出票
void Sale_UI_ShowTicket(int schID){
	
	
	
}

inline int Sale_UI_SellTicket(ticket_list_t tickList, seat_list_t seatList){
	system("cls");
	int i;
	char choice;
	seat_node_t *p=NULL;
	int row,column;
	printf("input row  &&  cloumn\n");
	scanf("%d%d",&row,&column);
	p=Seat_Srv_FindByRowCol( seatList,  row,  column);
	if(NULL==p)
	{
		printf("not find\n[E]nter tot return\n");
		getch();
		return 0;
	}
	
	if(Ticket_Srv_FetchByID(p->data.id ,&(tickList->data ) )) 
	{
		if(p->data.status !=0)
		{
			printf("y/n\n");
			scanf("%c",&choice);
			if(choice=='N'||choice!='n')
			{
				printf("give up to sell\n");
				return 0;
			}
			if(Ticket_Srv_Modify(&(tickList->data )))
			{
				sale_t data;
				data.id =1;
				printf("input saler id\n");
				scanf("%d",&data.user_id );
				data.ticket_id =p->data.id ;
				printf("input price\n");
				scanf("%d",&data.value );
				data.type =1;
				printf("input date\n");
				scanf("%d%d%d",&data.date.year , &data.date.month, &data.date.day);
				printf("input time\n");
				scanf("%d%d%d",&data.time.hour ,&data.time.minute ,&data.time.second );
				i=Sale_Srv_Add(&data);
				printf("successful\n [E]nter to return\n ");
				getch();
				return 1;
				
			}
			printf("wrong\n[E]nter to return\n");
			getch();
			return 0;
			
		}
	}
	printf("no ticket!\n");
	return 0;
	

	
	


}



//根据剧目ID显示演出计划
void Sale_UI_ShowScheduler(int playID) {
	
	
	system("cls");
	schedule_list_t     list;
    schedule_node_t     *p;
	int                 Snumber,i,pageSize = 10;
	Pagination_t        paging;
  
	char choice;
    List_Init(list,schedule_node_t);
    
   
    
    Snumber = Schedule_Srv_FetchByPlay(list,playID);
    
    paging.totalRecords = Snumber;
    paging.pageSize = pageSize;
    Paging_Locate_FirstPage(list,paging);   
	
	  	 system("cls");
        printf("================================ Schedule List ===================================\n");
        printf("ID\tPlay_ID\t\tStudio_ID\tData\tTime\tSeat_Count\n");
        printf("----------------------------------------------------------------------------------\n");
        for (i = 0, p = (schedule_node_t *) (paging.curPos); //当前页面的起始位置
             p != list && i < paging.pageSize; i++) {
            printf("%d\t%d\t%d\t%d.%d.%d\t\t%d:%d:%d\t\t%d\n", p->data.id, p->data.play_id,
                   p->data.studio_id,
                   p->data.date.year,p->data.date.month,p->data.date.day,
                   p->data.time.hour,p->data.time.minute,p->data.time.second,
                   p->data.seat_count);
            p=p->next;
        }
        printf("--Total Records:%2d --------------------------------------------------- Page %2d/%2d --\n",
               paging.totalRecords, Pageing_CurPage(paging),
               Pageing_TotalPages(paging));
        printf("=====================================================================================\n"); 
        printf("[E]nter to Return \n");
        getch();
		
	

}

void Sale_UI_MgtEntry() {
	ticket_list_t tickList;
	seat_list_t seatList;
		int an;
	int i, id;
	char choice;
	int playID;
	play_list_t head;
	play_node_t *pos;
	Pagination_t paging;

	List_Init(head, play_node_t);
	paging.offset = 0;
	paging.pageSize = PLAY_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("********************** Ticket **********************\n");
		printf("------------------------------------------------------------------\n\n\n");
		//显示数据
	printf("+++++++++++++++++++++++++++++++Play List ++++++++++++++++++++++++++++++\n");
        printf("ID\tName\tType\tArea\tRating\tDuration\tStart_Date\tEnd_Date\tPrice\n");
        printf("---------------------------------------------------------------------------------------\n");
        for (i = 0, pos = (play_node_t *) (paging.curPos); 
             pos!= head && i < paging.pageSize; i++) {
            printf("%d\t%s\t%s\t%s\t%s\t%d\t\t%d.%d.%d\t\t%d.%d.%d\t\t%d\n", pos->data.id, pos->data.name,
                   (pos->data.type == 1)?"film":(pos->data.type == 2?"opear":"concert"),
                   pos->data.area,
                   (pos->data.rating == 1)?"child":(pos->data.rating == 2?"teenage":"adult"),
                   pos->data.duration,
                   pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day,
                   pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day,
                   pos->data.price);
                   
                   printf("\n");
            pos=pos->next;
        }
		printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n\n\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]revPage|    [N]extPage |   S[c]hedule   |   [S]ell Ticket | [R]eturn");
		printf(
				"\n==================================================================\n");
		printf("Your Choice:");
		fflush(stdin);
		scanf("%c", &choice);
		if(choice=='r'||choice=='R')
		return ;
		fflush(stdin);

		switch (choice) {
		case 'c':
		case 'C':
			printf("input playID\n");
			scanf("%d",&playID);
			Sale_UI_ShowScheduler(playID );
			break;
		
		
		
		case 's':
		case 'S':
			
	 	
	 		List_Init(tickList, ticket_node_t);
	 		List_Init(seatList, seat_node_t);
	 		 an=Sale_UI_SellTicket( tickList,  seatList);
	 		
	 		
			
			break;
			
			
		case 'p':
        case 'P':
                if (1 < Pageing_CurPage(paging)) {
                    Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t); //前移
                }
                break;
            case 'n':
            case 'N':
                if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
                    Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);  //后移
                }
                break;
	
		
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, studio_node_t);

	
	
	
	
}

//退票
void Sale_UI_ReturnTicket(){
	int id;
	sale_t data;
	ticket_t buf;
	printf("Please input ticket id:\n");
	scanf("%d",&id);
	if(Ticket_Srv_FetchByID(id,&buf))
	{
		if(buf.status==0) 		//查看票的状态
		{
			return 0;	
		}				
		buf.status=0;
		if(Ticket_Srv_Modify(&buf))
		{

			printf("seccessful");
			Sale_Srv_Add(&buf);
			return 1;
		}		
	}
	else
		printf("Don't have the ticket\n");
		sleep(2);
		
	
	
}

