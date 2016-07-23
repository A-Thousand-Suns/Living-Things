/*
  * Account_UI.c
 *
 *  Created on: 2015��5��8��
 *      Author: Administrator
 */

#include "Account_UI.h"
#include "../Common/list.h"
#include "../Service/Account.h"
#include "../Service/EntityKey.h"
#include "Account_UI.h"
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

//��¼��������ʾ�û������û��������룬��¼�ɹ�return 1��������ʾ���µ�¼������3�Σ���¼ʧ��
int SysLogin() {
	char name[30];
	char password[30];
	int all=0;
	do{
	fflush(stdin);
	system("cls");
	printf("==================Login=========================\n");
	printf("name:\t\t");
	scanf("%s",name);
	fflush(stdin);
	printf("password:\t\t");
	scanf("%s",password);

	if(strcmp("1",name)==0&&strcmp("1",password)==0)
	return 1;
	else
	
		all++;
	
}while(all<=2);
	printf("more than 3 times\n ");
	return 0;

}

//����ϵͳ�û�������ں�������ʾ�û��˺Ź���˵�
void Account_UI_MgtEntry() {
	if(!SysLogin)
	{
		return ;
	}
	char choice;
	account_list_t head;
	account_node_t * pos;
	Pagination_t paging;
	List_Init(head, account_node_t);
	if(Account_Srv_FetchAll(head))
	{
		printf("no record !\n");
		return ;
	}
		


do{

		system("cls");
		printf("\n==================================================================\n");
		printf("********************** Account **********************\n");
		//��ʾ����
	/*	while(head->next!=NULL)
		{
			printf("ID\t\tName\t\t\n");
			printf("%d\t\t%s",head->data.id ,head->data.username );
			head=head->next ;
			
		}*/
        
        
	
		printf(
				"******************************************************************\n");
		printf(
				"  [A]dd   |   [M]odify |  [D]elete  | [Q]uery  | [R]eturn");
		printf(
				"\n==================================================================\n");
		printf("Your Choice:");
		fflush(stdin);
		scanf("%c", &choice);
		
		
		
		switch(choice)
		{
			case'A':
			case'a':
				
				if(Account_UI_Add( head))
				{
					printf("can not add\n");
					break;
				}
				printf("add successfully\n");
				break;
				
				
				
			
		}
	}while(choice != 'r' && choice != 'R');
	
	
	

}


//���һ���û��˺���Ϣ������˺������ڣ���ʾ������Ϣ
int Account_UI_Add(account_list_t list ) {
	account_t rec;
	int newRecCount = 0;
	char choice;

	do {
		system("cls");
		printf("\n=======================================================\n");
		printf("****************  Add New Account  ****************\n");
		printf("-------------------------------------------------------\n");
		printf("Name:\n");
		fflush(stdin);
		gets(rec.username );
		fflush(stdin);
		printf("password:\n");
		gets(rec.password );
		fflush(stdin);
		printf("type\n");
		scanf("%d",&rec.type );
		rec.id =EntKey_Srv_CompNewKey("Account");
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	
	
	
	
	

		
	

	return 1;

}

//�����û��˺����޸��û��˺����룬����������û��˺�������ʾ������Ϣ
int Account_UI_Modify(account_list_t list ,char usrName[]) {

	return 1;
}

//�����û��˺���ɾ��һ���Ѿ����ڵ��û��˺���Ϣ���������������û��˺�������ʾ������Ϣ
int Account_UI_Delete(account_list_t list ,char usrName[]) {

	return 1;
}

//�����û��˺������Ҹ��û��˺��Ƿ���ڣ����ڷ���1�����򷵻�0������ʾ������Ϣ
int Account_UI_Query(account_list_t  list ,char usrName[]) {


	return 1;
}


