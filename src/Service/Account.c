/*
 * account.c
 *
 *  Created on: 2015��6��12��
 *      Author: Administrator
 */
#include "Account.h"
#include "EntityKey.h"
#include "../Common/list.h"
#include "../Persistence/Account_Persist.h"
#include <string.h>
#include <stdlib.h>

extern account_t gl_CurUser;

//����ϵͳ��ʼ���˺�admin
void Account_Srv_InitSys(){
	if(Account_Perst_CheckAccFile())
	{
		return;
	}
	account_t admin;
	admin.id =1;
	strcpy("1",admin.password );
	strcpy("1",admin.username );
	admin.type=9;


}


//��֤��¼�˺��Ƿ��Ѵ��ڣ����ڣ������¼�û���Ϣ��ȫ�ֱ���gl_CurUser��return 1������return 0
inline int Account_Srv_Verify(char usrName[], char pwd[]){
		return 1;
}


//��֤�û����Ƿ���ڣ�����list�������ڣ����ض�Ӧ���ָ�룻���򣬷���null
account_node_t * Account_Srv_FindByUsrName(account_list_t list, char usrName[]) {

	return NULL ;
}

//����һ���û��˺ţ�ͨ������Account_Perst_Insert(data)����ʵ��
inline int Account_Srv_Add(const account_t *data){
	 return Account_Perst_Insert(data);
}

//�޸�һ���û��˺ţ�ͨ������Account_Perst_Update(data)����ʵ��
inline int Account_Srv_Modify(const account_t *data){
	return Account_Perst_Update( data);
}

//ɾ��һ���û��˺ţ�ͨ������Account_Perst_DeleteByID(usrID)����ʵ��
inline int Account_Srv_DeleteByID(int usrID){
	return Account_Perst_DeleteByID( usrID);
}

//��ȡusrID��Ӧ���û��˺���Ϣ��ͨ������Account_Perst_SelectByID(usrID, buf)����ʵ��
inline int Account_Srv_FetchByID(int usrID, account_t *buf){
	return Account_Perst_SelectByID(usrID, buf);
}

//��ȡusrName��Ӧ���û��˺���Ϣ��ͨ������Account_Perst_SelByName(usrName, buf)����ʵ��
inline int Account_Srv_FetchByName(char usrName[], account_t *buf){
	return 1;
}

//��ȡ�����û��˺���Ϣ�����浽list�����У�ͨ������Account_Perst_SelectAll(list)����ʵ��
inline int Account_Srv_FetchAll(account_list_t list){
		return Account_Perst_SelectAll(list);
}

