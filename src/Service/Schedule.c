/*
* File name:  Schedule.c
* File ID:	  TTMS_SSF_Schedule_Srv_Sour
* Author:     Huang Ru
* Version:    V1.0
* Date: 	  2015-04-25
*/


#include "Schedule.h"
#include "../Common/list.h"
#include "../Persistence/Schedule_Persist.h"
#include "Ticket.h"
#include <stdio.h>

/*
 * Function:    Schedule_Srv_Add
 * Function ID:	TTMS_SCU_Schedule_Srv_Add
 * Description: ���һ���ݳ��ƻ���Ϣ
 * Input:       ����ӵ��ݳ��ƻ���Ϣ����
 * Output:      ��
 * Return:      ��ӵļ�¼��
 */
inline int Schedule_Srv_Add(const schedule_t *data) {
	int rtn;
	rtn = Schedule_Perst_Insert(data);
	
    if(Ticket_Srv_AddBatch( data->id, data->studio_id)){
		
		return rtn;
    }
    else{

		return 2;
    }
}

/*
 * Function:    Schedule_Srv_Modify
 * Function ID:	TTMS_SCU_Schedule_ Srv _Mod
 * Description: ����һ���ݳ��ƻ���Ϣ
 * Input:       �����µ��ݳ��ƻ���Ϣ����
 * Output:      ��
 * Return:      ���µ��ݳ��ƻ���Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
inline int Schedule_Srv_Modify(const schedule_t *data) {
	int rtn;
	rtn = Schedule_Perst_Update(data);
	Ticket_Srv_DeleteBatch(data->id);
	Ticket_Srv_AddBatch( data->id, data->studio_id);
    return rtn;
}





/*
 * Function:    Schedule_Srv_DeleteByID
 * Function ID:	TTMS_SCU_Schedule_Srv_DelByID
 * Description: ����ID��ɾ���ݳ��ƻ���Ϣ
 * Input:       ��ɾ�����ݳ��ƻ�ID��
 * Output:      ��
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
inline int Schedule_Srv_DeleteByID(int ID) {
 int rtn;
	rtn = Schedule_Perst_DeleteByID(ID);
	Ticket_Srv_DeleteBatch(ID);
    return rtn;
}

/*
 * Function:    Schedule_Srv_FetchByID
 * Function ID:	TTMS_SCU_Schedule_Srv_FetchByID
 * Description: ����ID�Ų���һ���ݳ��ƻ���Ϣ������buf��ָ��Ԫ
 * Input:       �����ҵ��ݳ��ƻ�ID�ţ������ҵ�����Ϣ���浽buf��
 * Output:      ��
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
inline int Schedule_Srv_FetchByID(int ID, schedule_t *buf) {
int newcount = 0;
    if(Schedule_Perst_SelectByID( ID, buf)){
        newcount++;
    }
    return newcount;

}

/*
 * Function:    Schedule_Srv_FetchAll
 * Function ID:	TTMS_SCU_Schedule_Srv_FetchAll
 * Description: ȡ�������ݳ��ƻ���Ϣ������һ������
 * Input:       list�ݳ��ƻ���Ϣ�����ͷָ��
 * Output:      ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
inline int Schedule_Srv_FetchAll(schedule_list_t list) {
int newcount = 0;
    if(Schedule_Perst_SelectAll(list)){
        newcount++;
    }
    return newcount;
}

/*
 * Function:    Schedule_Srv_FetchByPlay
 * Function ID:	TTMS_SCU_Schedule_Srv_FetchByPlay
 * Description: ���վ�ĿID�ţ�������ݳ��ƻ���Ϣȡ��������һ������
 * Input:       listΪ���ҵ����ݳ��ƻ���Ϣ�����ͷָ�룬play_idΪ��Ŀ��ID��
 * Output:      ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
inline int Schedule_Srv_FetchByPlay(schedule_list_t list,int play_id) {
 return Schedule_Perst_SelectByPlay( list, play_id);
}




