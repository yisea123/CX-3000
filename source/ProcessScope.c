// ProcessScope.c

#ifndef    __PROCESS_SCOPE_C__
#define    __PROCESS_SCOPE_C__

#include "KernelHeader.h"
#include "stdlib.h"

// ADC1
IO_ ADC_Status_InitTypeDef ADC1_Status = {0};
UINT16 g_ADC1_Buffer[ADC1_BUFFER_LEN_HALF] = {0};

// ADC2
IO_ ADC_Status_InitTypeDef ADC2_Status = {0};
UINT16 g_ADC2_Buffer[ADC2_BUFFER_LEN_HALF] = {0};

/* const UINT8 softver_edtion[] = "CX2100_SLA_V004_20180521_1000"; */  /* 统一版本号命名 */

IO_ UINT32  g_Udp_Count, g_Frame_Count, g_Send_Fail;
IO_ UINT8 g_AirLight_Flag;
const UINT8 softver_edtion[] = "0.0.1";

IO_ struct CRP_DataType g_CRP_Data;

//
IO_ UINT8 g_Test_Mode = 0xFF;
IO_ UINT8 g_Micro_Switch = 0xFF;

#ifdef DEBUG_INFO_UP_LOAD
//	_STA_ IO_ UINT16 XRAM_ pDInfo[DEBUG_INFO_UP_LEN];
//	_STA_ IO_ UINT16 XRAM_ nDILen = 0;
#endif



    
/* 采集报告处理 */
void collect_return_hdl(UINT16 stat)
{
    UINT8  dat[16], n;
    UINT16 tim;
	UINT32 udp_count;

    n = 0;
    tim = IT_LIST_GetTicks();
    dat[n++] = tim >> 8;
	//the time value change to large-end, data still use little-end ,yaolan_20190418
    dat[n++] = tim;  /* 为了保持与数据包一致，采用小段模式 */
    tim = IT_ADC_GetTicks();
    dat[n++] = tim >> 8;
    dat[n++] = tim;  /* 为了保持与数据包一致，采用小段模式 */

    dat[n++] = stat >> 8;
    dat[n++] = stat;      /* 大小端模式转换，该处采用大端模式 */

	udp_count = Get_Udp_Count();
	dat[n++]  = udp_count >> 24;
	dat[n++]  = udp_count >> 16;
	dat[n++]  = udp_count >> 8;
	dat[n++]  = udp_count;
	
    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, 0x100000, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, 0x100000, dat, n);  /* 重发一次数据包 */
    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, 0x100000, dat, n);  /* 重发一次数据包 */

}

void Msg_Return_Handle_0(EN_MSG_TYPE eType, UINT32 nCmd)
{
    UINT8  dat[2] = {0}, n;
    UINT32 tim;

    n = 0;
 //   dat[n++] = nResult;      /* 大小端模式转换，该处采用大端模式 */
	
	tim = IT_SYS_GetTicks();
	if(eType == e_Msg_Status)
	{
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
	}else if(eType == e_Msg_Data){
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
	}
}

void Msg_Return_Handle_8(EN_MSG_TYPE eType, UINT32 nCmd, INT8 nResult)
{
    UINT8  dat[2] = {0}, n;
    UINT32 tim;

    n = 0;
    dat[n++] = nResult;      /* 大小端模式转换，该处采用大端模式 */
	
	tim = IT_SYS_GetTicks();
	if(eType == e_Msg_Status)
	{
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
	}else if(eType == e_Msg_Data){
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
	}
}

void Msg_Return_Handle_16(EN_MSG_TYPE eType, UINT32 nCmd, INT16 nResult)
{
    UINT8  dat[3], n;
    UINT32 tim;

    n = 0;
    dat[n++] = nResult >> 8;
    dat[n++] = nResult;      /* 大小端模式转换，该处采用大端模式 */
	
	tim = IT_SYS_GetTicks();
	if(eType == e_Msg_Status)
	{
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
	}else if(eType == e_Msg_Data){
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
	}
}

void Msg_Return_Handle_32(EN_MSG_TYPE eType, UINT32 nCmd, INT32 nResult)
{
    UINT8  dat[5]={0}, n;
    UINT32 tim;

    n = 0;
    dat[n++] = nResult >> 24;
    dat[n++] = nResult >> 16;      /* 大小端模式转换，该处采用大端模式 */
	dat[n++] = nResult >> 8;
	dat[n++] = nResult;
	
	tim = IT_SYS_GetTicks();
	if(eType == e_Msg_Status)
	{
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
	}else if(eType == e_Msg_Data){
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
	}
}

void Msg_Return_Handle_String(EN_MSG_TYPE eType, UINT32 nCmd, UINT8 *pRst, UINT8 nLen)
{
    UINT32 tim;
	UINT8 n = nLen, *dat = pRst;
		
	tim = IT_SYS_GetTicks();
	if(eType == e_Msg_Status)
	{
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
//		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
//		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
//		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
//		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
	}else if(eType == e_Msg_Data){
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
//		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
//		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
//		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
//		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
	}

}


//void Status_Return_Handle_8(UINT32 nCmd, UINT8 nResult)
//{
//    UINT8  dat[4], n;
//    UINT16 tim;

//    n = 0;
//    dat[n++] = nResult;      /* 大小端模式转换，该处采用大端模式 */

//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
//}

//void Status_Return_Handle_16(UINT32 nCmd, UINT16 nResult)
//{
//    UINT8  dat[4], n;
//    UINT16 tim;

//    n = 0;
//    dat[n++] = nResult >> 8;
//    dat[n++] = nResult;      /* 大小端模式转换，该处采用大端模式 */

//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
//	
//}

//void Status_Return_Handle_32(UINT32 nCmd, UINT32 nResult)
//{
//    UINT8  dat[8], n;
//    UINT16 tim;

//    n = 0;
//    dat[n++] = nResult >> 24;
//    dat[n++] = nResult >> 16;      /* 大小端模式转换，该处采用大端模式 */
//	dat[n++] = nResult >> 8;
//	dat[n++] = nResult;
//	
//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
//}

//void Data_Return_Handle_8(UINT32 nCmd, UINT8 nResult)
//{
//    UINT8  dat[4], n;
//    UINT16 tim;

//    n = 0;
//    dat[n++] = nResult;      /* 大小端模式转换，该处采用大端模式 */

//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
//}

//void Data_Return_Handle_16(UINT32 nCmd, UINT16 nResult)
//{
//	UINT8  dat[4], n;
//    UINT16 tim;

//    n = 0;
//    dat[n++] = nResult >> 8;
//    dat[n++] = nResult;      /* 大小端模式转换，该处采用大端模式 */

//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */

//}

//void Data_Return_Handle_32(UINT32 nCmd, UINT32 nResult)
//{
//    UINT8  dat[8], n;
//    UINT16 tim;

//    n = 0;
//    dat[n++] = nResult >> 24;
//    dat[n++] = nResult >> 16;      /* 大小端模式转换，该处采用大端模式 */
//	dat[n++] = nResult >> 8;
//	dat[n++] = nResult;
//	
//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
//    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
//    PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
//}


void moto_work_stat(UINT8 mot_num, MOTO_WORK_STAT_E stat)
{
    UINT8  dat[16], n;
    UINT16 tim;

    n = 0;
    dat[n++] = mot_num;
    dat[n++] = stat;

    tim = IT_ADC_GetTicks();
    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, CMD_STATUS_MOT, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 10); /* 延时10毫秒后再发送结果 */
    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, CMD_STATUS_MOT, dat, n);  /* 重发一次数据包 */
    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 20); /* 延时10毫秒后再发送结果 */
    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, CMD_STATUS_MOT, dat, n);  /* 重发一次数据包 */
}

void moto_work_stat_2(UINT8 mot_num, MOTO_WORK_STAT_E stat, BUILD_PRESS_E stat2)
{
    UINT8  dat[16], n;
    UINT16 tim;

    n = 0;
    dat[n++] = mot_num;
    dat[n++] = stat;
	dat[n++] = stat2;

    tim = IT_ADC_GetTicks();
    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, CMD_STATUS_MOT_WORK, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 10); /* 延时10毫秒后再发送结果 */
    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, CMD_STATUS_MOT_WORK, dat, n);  /* 重发一次数据包 */
    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 20); /* 延时10毫秒后再发送结果 */
    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, CMD_STATUS_MOT_WORK, dat, n);  /* 重发一次数据包 */
}


void Moto_Work_Time(MOTO_WORK_DIR eDir, UINT16 nStatus)
{
    UINT8  dat[16], n;
    UINT16 tim;

    n = 0;
    dat[n++] = nStatus >> 8;
    dat[n++] = nStatus;

    tim = IT_ADC_GetTicks();
	if(eDir == e_Moto_Out)
	{
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, CMD_STATUS_MOT_OUT_CHECK, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_ADC_GetTicks() - tim)) < 10); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, CMD_STATUS_MOT_OUT_CHECK, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_ADC_GetTicks() - tim)) < 20); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, CMD_STATUS_MOT_OUT_CHECK, dat, n);  /* 重发一次数据包 */

	}else if(eDir == e_Moto_In){
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, CMD_STATUS_MOT_IN_CHECK, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_ADC_GetTicks() - tim)) < 10); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, CMD_STATUS_MOT_IN_CHECK, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_ADC_GetTicks() - tim)) < 20); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, CMD_STATUS_MOT_IN_CHECK, dat, n);  /* 重发一次数据包 */
	}
}

void Add_Udp_Count(void)
{
	g_Udp_Count++;
}

UINT32 Get_Udp_Count(void)
{
	return g_Udp_Count;
}

void Reset_Udp_Count(UINT32 nVal)
{
	g_Udp_Count   = nVal;
	g_Frame_Count = nVal;
	g_Send_Fail   = nVal;
}

_EXT_ UINT8 MSG_Handling_MsgHandle(UINT8* pchCmdBuf, UINT8* pchFbkBuf)
{
	UINT32 nCommand = 0;
	UINT8  nReturn = e_Feedback_Success, nType;
	
	
	printf("\r\nMSG Handling:%02X%02X%02X%02X ", pchCmdBuf[0],pchCmdBuf[1],pchCmdBuf[2],pchCmdBuf[3]);
    printf("%02X%02X%02X%02X ", pchCmdBuf[4],pchCmdBuf[5],pchCmdBuf[6],pchCmdBuf[7]);
	printf("%02X%02X%02X%02X\r\n", pchCmdBuf[8],pchCmdBuf[9],pchCmdBuf[10],pchCmdBuf[11]);
	   
	nCommand = PL_UnionFourBytes(*(pchCmdBuf + 4),
                                 *(pchCmdBuf + 5),
                                 *(pchCmdBuf + 6),
                                 *(pchCmdBuf + 7));
	
	switch(nCommand)
	{
		case CMD_CTRL_PART_TEST:
		{
			nType = *(pchCmdBuf + 8);
			if(nType == 0xFF) return e_Feedback_Fail;
		}
		break;
		default:break;
	
	}
	
	return nReturn;
	
	
}

eTestMode GetTestMode(UINT32 nCmd)
{
	eTestMode eMode = EN_MODE_END;
	if(nCmd == CMD_CTRL_TEST_WBC){
		eMode =  EN_WBC_TEST;
	}else if(nCmd == CMD_CTRL_TEST_RBC){
		eMode =  EN_RBC_TEST;
	}else if(nCmd == CMD_CTRL_TEST_PLT){
		eMode =  EN_PLT_TEST;
	}else if(nCmd == CMD_CTRL_TEST_RBC_PLT){
		eMode =  EN_RBC_PLT_TEST;
	}
	return eMode;
}

//===================================================
//
UINT8 MSG_Handling(UINT8 * pchCmdBuf, UINT8 * pchFbkBuf)
{
    UINT8  chType              = 0;
	UINT8  nStatus             = 0;
//	UINT8  nChannel            = 0;
    UINT32 nCommand            = 0;
    UINT16 nParaLen            = 0;
    //
    enum eFlag  bSendBack      = e_False;
	eTestMode 	eMode		   = EN_MODE_END;
    //
	INT16  nShort2 = 0;
    UINT16 nShort  = 0, nAdd = 0, nParam1 = 0, nParam2 = 0;
	UINT32 nWord   = 0;
    INT32  nWord2  = 0;
	UINT8  nVal    = 0;
	
    //
    chType   = *(pchCmdBuf + 3);
    nCommand = PL_UnionFourBytes(*(pchCmdBuf + 4),
                                 *(pchCmdBuf + 5),
                                 *(pchCmdBuf + 6),
                                 *(pchCmdBuf + 7));

    //===================================================
    // action
	printf("\r\nMSG Handling:%02X%02X%02X%02X ", pchCmdBuf[0],pchCmdBuf[1],pchCmdBuf[2],pchCmdBuf[3]);
    printf("%02X%02X%02X%02X ", pchCmdBuf[4],pchCmdBuf[5],pchCmdBuf[6],pchCmdBuf[7]);
	printf("%02X%02X%02X%02X\r\n", pchCmdBuf[8],pchCmdBuf[9],pchCmdBuf[10],pchCmdBuf[11]);
	// 1.
    if (PROTOCOL_HEAD_RECV_WR == chType) // cntrol cmd
    {
        switch (nCommand)
        {
			//---------------------TEST CMD------------------------
			case CMD_CTRL_TEST_RBC: 
			case CMD_CTRL_TEST_PLT:
			case CMD_CTRL_TEST_RBC_PLT:
            case CMD_CTRL_TEST_WBC: // count func
            {
				Micro_OC_Exit_Disable();
				eMode  = GetTestMode(nCommand);// which cmd was be exec
				//g_Test_Mode = eMode;
#if SIMUATION_TEST
				nParaLen = 0;
				Simulation_Data((UINT8*)g_achFbkSdLogBuf, &nParaLen, eMode);
#else
				if(e_Feedback_Success != MSG_TestingFunc((UINT8*)g_achFbkSdLogBuf, &nParaLen, eMode)) //if(e_Feedback_Success != MSG_TestingFunc())
				{
					HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);
					HW_Valve_Off(EN_VALVE_AIR);
					HW_Valve_Off(EN_VALVE_LIQUID);
				}
				printf("adc1 end: id=%d, sendid=%d\r\n", \
						(int)ADC1_Status.nID, (int)ADC1_Status.nSendID);
				printf("adc2 end: id=%d, sendid=%d\r\n", \
						(int)ADC2_Status.nID, (int)ADC2_Status.nSendID);
				// send debug info for count
				g_achFbkSdLogBuf[8]  = (nParaLen - 10) >> 8;
				g_achFbkSdLogBuf[9] = (nParaLen - 10);
				if(e_Feedback_Fail == udp_echoserver_senddata((UINT8 *)g_achFbkSdLogBuf, nParaLen))
				{
					IT_SYS_DlyMs(1);
				}
#endif				
				printf("debug msg len: %d\r\n", nParaLen);
				nParaLen = 0;
				Reset_Udp_Count(0);	
				Reset_ADC_InitDataType();
				collect_return_hdl(COLLECT_RET_SUCESS); 
				Micro_OC_Init();
			}
			break;
			case CMD_CTRL_TEST_HGB:
			{
				HGB_Test_Exec(EN_HGB_TEST);
			}
			break;
			case CMD_CTRL_TEST_CRP:
			{
				printf("CRP Param Set: nTime=%d,nHz=%d, nTotal=%d\r\n", g_Record_Param.nTime,\
												g_Record_Param.nHZ, g_Record_Param.nTotal_Num);
				CRP_Test_Exec(EN_CRP_TEST);
			}
			break;
			case CMD_CTRL_CALIBRATE_HGB:
			{
				HGB_Test_Exec(EN_HGB_CALIBRATE);
			}
			break;
			case CMD_CTRL_CALIBTATE_CRP:
			{
				CRP_Test_Exec(EN_CRP_CALIBRATE);
			}
			break;
			//---------------------Ctrol CMD------------------------			
            case CMD_CTRL_VALVE: //
            {
				//Valve_Exec(*(pchCmdBuf + 8), *(pchCmdBuf + 9));
                if (0 == (*(pchCmdBuf + 9)))
				{
					HW_Valve_Off(*(pchCmdBuf + 8));
				}
				else
				{
					HW_Valve_On(*(pchCmdBuf + 8));
				}
            }
			break;
			case CMD_CTRL_PUMP:
			{
				
				nWord = PL_UnionFourBytes(*(pchCmdBuf +  8),
                                          *(pchCmdBuf +  9),
                                          *(pchCmdBuf + 10),
                                          *(pchCmdBuf + 11));
                //
                HW_PUMP_Pulse(nWord, e_Dir_Pos);
				
			}
			break;
			case CMD_CTRL_MOT_IN:
            {
                MT_X_Home(e_NormalCheck_Call); // go home
            }
			break;
            //
            case CMD_CTRL_MOT_OUT:
            {
                MT_X_MoveToPosRel(e_NormalCheck_Call); // out
                //
            }
			break;
            case CMD_CTRL_MOT_RELEASE:  /* 释放芯片 */
			{
               // MT_Y_Home(e_NormalCheck_Call);
			}
            break;
            case CMD_CTRL_MOT_LOCK:     /* 锁定芯片 */
			{
             //   MT_Y_MoveToPosRel(e_NormalCheck_Call);
			}
            break;
            case CMD_CTRL_MOT_IN_ONLY:  /* 单独驱动大电机进仓 */
			{
                MT_X_Home_only();
			}
            break;
            case CMD_CTRL_MOT_OUT_ONLY: /* 单独驱动大电机出仓 */
			{
                MT_X_MoveToPosRel_only();
			}
			break;
			case CMD_CTRL_TURN_MOTOR_RESET:
			{
				Turn_Motor_Reset();
				Msg_Return_Handle_8(e_Msg_Status, CMD_STATUS_TURN_MOTOR_RESET, 0);	
			}
			break;
			case CMD_CTRL_MIXING_MOTOR:
			{
				Mixing_Motor_Ctr(*(pchCmdBuf + 8));
			}
			break;
			case CMD_CTRL_LED:
			{
				nShort = 0;
				printf("LED=%d, Opt=%d\r\n", *(pchCmdBuf + 8), *(pchCmdBuf + 9));
				LED_All_Reset();
				if(*(pchCmdBuf + 9) == EN_OPEN)
				{
					LED_Exec(*(pchCmdBuf + 8), EN_OPEN); 	 	 		 // open led
					//Turn_Motor_Select_LED(nLED); 			 // led go to test positon 
					LED_Cur_ADC_Check_Channel(*(pchCmdBuf + 8));		 // CD4051 open the channel, and then start to adjust
					LED_Cur_DAC_Set(LED_840_DEFUALT_CUR_VALUE);
					LED_Cur_Switch(EN_OPEN);	//led cur open				
				}
				nShort = (*(pchCmdBuf + 8)) << 8;
				nShort |= 0x00; 
				Msg_Return_Handle_16(e_Msg_Status, CMD_STATUS_LED, nShort);
			}
			break;
			case CMD_CTRL_MOTOR_SEL_LED:
			{
				nShort = 0;
				Turn_Motor_Select_LED(*(pchCmdBuf + 8));
				nShort = (*(pchCmdBuf + 8)) << 8;
				nShort |= 0x00; 
				Msg_Return_Handle_16(e_Msg_Status, CMD_STATUS_MOTOR_SEL_LED, nShort);
			}
			break;
			//---------------------Self Check CMD----------------------------
			case CMD_CTRL_WBC_48V_CHECK:
			{		
				WBC_48V_Self_Check();
			}
			break;
			case CMD_CTRL_BUILD_PRESS_CHECK:
			{
				Build_Press_Self_Check();
			}
			break;
			case CMD_CTRL_AIRLIGHT_CHECK:
			{
				AirLight_Self_Check(e_SelfCheck_Call);
			}
			break;
			case CMD_CTRL_GPUMP_CHECK:
			{
				Pump_Self_Check();
			}
			break;
			case CMD_CTRL_VALVE1_CHECK:
			{
				Valve1_Self_Check();
			}
			break;
			case CMD_CTRL_VALVE2_CHECK:
			{
				Valve2_Self_Check();
			}
			break;
			case CMD_CTRL_MOT_OUT_CHECK:
			{
				MT_X_OUT_Self_Check(e_SelfCheck_Call);
			}
			break;
			case CMD_CTRL_MOT_IN_CHECK:
			{
				MT_X_IN_Self_Check(e_SelfCheck_Call);
			}
			break;
			case CMD_CTRL_PART_TEST: // aging test
			{
				nWord = PL_UnionFourBytes(*(pchCmdBuf +  9),
						  *(pchCmdBuf +  10),
						  *(pchCmdBuf + 11),
						  *(pchCmdBuf + 12));
				Part_Test_Exec(*(pchCmdBuf + 8), nWord);			
			}
			break;	
			case CMD_CTRL_MIXING_MOTOR_CHECK:
			{
				Mixing_Motor_Go_On(MIXING_OVER_TIME);
			}
			break;
			case CMD_CTRL_TURN_MOTOR_CHECK:
			{
				Turn_Motor_Goto_Postion(e_Dir_Pos, 10000);
			}
			break;
			case CMD_CTRL_DEBUG_TEST:
			{
				printf("index: %d\r\n", *(pchCmdBuf +  8));
				Driver_Debug(*(pchCmdBuf +  8));
			}
			break;			
			//---------------------SET CMD------------------------
			case CMD_CTRL_TEST_MODE_SET:
			{
				printf("Test Mode = %d\r\n", *(pchCmdBuf + 8));
				LED_Mode_Set(*(pchCmdBuf + 8),  *(pchCmdBuf + 9));
			}
			break;
			case CMD_CTRL_PRESS_ADD: // 
			{
				nParam1 = PL_UnionTwoBytes_2(*(pchCmdBuf + 8),
						  *(pchCmdBuf + 9));
				Set_Press_Add(nParam1);
			}
			break;
			case CMD_CTRL_MOT_X_IN_ADD:
			{
				// 
				nShort = PL_UnionTwoBytes(*(pchCmdBuf + 8), *(pchCmdBuf + 9));
				g_Record_Param.nXAddStep = nShort;
				g_Record_Param.nFlag = FLASH_INIT_FLAG;
				printf("Set Moto X Add Step%d\r\n", g_Record_Param.nXAddStep);
				Flash_Write_Param(&g_Record_Param, RECORD_PARAM_LEN);
				Msg_Return_Handle_8(e_Msg_Status, CMD_STATUS_MOTO_X_IN_ADD, e_Feedback_Success);
			}
			break;
			case CMD_CTRL_REGISTER: 
            {
				Set_Register_Param(*(pchCmdBuf + 8), *(pchCmdBuf + 9));
            }
			break;
/*			case CMD_CTRL_CRP_PARAM_SET:
			{
				nParam1 = PL_UnionTwoBytes_2(*(pchCmdBuf + 8), *(pchCmdBuf + 9));
				nParam2 = PL_UnionTwoBytes_2(*(pchCmdBuf + 10), *(pchCmdBuf + 11));
				Set_CRP_Param(nParam1, nParam2);
			}
			break;
*/
			//---------------------Update CMD------------------------
			case CMD_MCU_UPDATE:
			{
				printf("%02X%02X%02X%02X ", pchCmdBuf[12],pchCmdBuf[13],pchCmdBuf[14],pchCmdBuf[15]);
				printf("%02X%02X%02X%02X\r\n", pchCmdBuf[16],pchCmdBuf[17],pchCmdBuf[18],pchCmdBuf[19]);
				
				Update_Start_Msg(&pchCmdBuf[8]);
			}
			break;
			case CMD_UPDATE_DATA_PACKET:
			{
				Update_Packet_Handler(&pchCmdBuf[8]);
			}
			break;
			case CMD_QUERY_UPDATE_FLAG:
			{
				printf("update flag=%d\r\n",  g_Record_Param.nUpdate_Flag);
				Msg_Return_Handle_8(e_Msg_Status, CMD_STATUS_QUARY_UPDATE_FLAG, g_Record_Param.nUpdate_Flag);
			}
			break;
			case CMD_CTRL_UPDATE_FLAG:
			{
				
				printf("set updata flag value=%d\r\n",  pchCmdBuf[8]);
				g_Record_Param.nUpdate_Flag = pchCmdBuf[8];
				nStatus = Flash_Write_Param(&g_Record_Param, RECORD_PARAM_LEN);
				if(nStatus == e_Feedback_Fail)
				{
					Msg_Return_Handle_8(e_Msg_Status, CMD_STATUS_SET_UPDATE_FLAG, e_Feedback_Fail);
					printf("set updata flag value=%d error\r\n",  pchCmdBuf[8]);
				}else{
					Msg_Return_Handle_8(e_Msg_Status, CMD_STATUS_SET_UPDATE_FLAG, e_Feedback_Success);
					printf("set updata flag value=%d, successe\r\n",  pchCmdBuf[8]);
				}
			
			}
			break;					
            default:
            {
                break;
            }

        } // end control cmd
    }

    //---------------------Status Query CMD------------------------
    if (PROTOCOL_HEAD_RECV_RD == chType)
    {
        switch (nCommand)
        {
            case CMD_QUERY_PRESSURE:
            {
                nCommand  = CMD_STATUS_PRESSURE;
                bSendBack = e_True;
                //
                nWord2 = Get_Press_Value(GET_PRESS_NUM_FIVE);
                *(pchFbkBuf + 0) = (UINT8)((nWord2 >> 24) & 0xff);
                *(pchFbkBuf + 1) = (UINT8)((nWord2 >> 16) & 0xff);
				*(pchFbkBuf + 2) = (UINT8)((nWord2 >> 8) & 0xff);
				*(pchFbkBuf + 3) = (UINT8)((nWord2 >> 0) & 0xff);
                nParaLen         = 4;
            }
			break;
			case CMD_QUERY_PUMP_SPEED:
			{
                bSendBack = e_True;
                nCommand  = CMD_STATUS_PUMP_SPEED;

                nShort = 12500; // todo...
                *(pchFbkBuf + 0) = (nShort >> 8);
                *(pchFbkBuf + 1) = (nShort & 0xff);
                nParaLen = 2;
			}
            break;
			case CMD_QUERY_MOTO_IN_X_ADD: // moto x go home add
			{
				printf("Query Moto X Add Step=%d\r\n", (int)g_Record_Param.nXAddStep);
				Msg_Return_Handle_16(e_Msg_Data, CMD_DATA_MOTO_IN_X_ADD, g_Record_Param.nXAddStep);
			}
			break;	
			case CMD_QUERY_REGISTER_VALUE: // get wbc value
			{
				bSendBack = e_True;
                nCommand  = CMD_STATUS_REGISTER_VALUE;
				nVal = Get_DRegister_Value(*(pchCmdBuf + 8)); // todo...
				 *(pchFbkBuf + 0) = *(pchCmdBuf + 8);
				 *(pchFbkBuf + 1) = nVal;
				printf("channel = %d, Dregister = %d\r\n", *(pchCmdBuf + 8), nVal);		
			}
			break;
			case CMD_QUERY_PRESS_ADD: // get press add
			{
				nShort2 = (int)g_Record_Param.nAddPress/PRESS_PRECISION_FACTOR;
				printf("add press=%d, %d\r\n", (int)g_Record_Param.nAddPress, nShort2);
				Msg_Return_Handle_16(e_Msg_Data, CMD_STATUS_PRESS_ADD, nShort2);				
			}
			break;	
			case CMD_QUERY_TMEPERATURE:
			{
				// todo...
			}
			break;	
			case CMD_QUERY_CUR_V_XK:
			{
				nShort = Get_XK_V();
				printf("Get XK V=%d\r\n", (int)nShort);
				Msg_Return_Handle_16(e_Msg_Data, CMD_STATUS_CUR_V_XK, nShort);	
			}	
			break;
			case CMD_QUERY_CUR_V_56V:
			{
				nShort = Get_56V_Cur_V();
				printf("Query Add Press=%d\r\n", (int)nAdd);
				Msg_Return_Handle_16(e_Msg_Data, CMD_STATUS_CUR_V_56V, nShort);	
			}	
			break;
			case CMD_QUERY_LIGHT_PATH_V:
			{
				nParam1 = *(pchCmdBuf + 8);
				
                nCommand  = CMD_STATUS_LIGHT_PATH_V;
                bSendBack = e_True;
                //
                nWord = Get_Light_Path_V((UINT8)nParam1);
                *(pchFbkBuf + 0) = (UINT8)((nWord >> 24) & 0xff);
                *(pchFbkBuf + 1) = (UINT8)((nWord >> 16) & 0xff);
                *(pchFbkBuf + 2) = (UINT8)((nWord >>  8) & 0xff);
                *(pchFbkBuf + 3) = (UINT8)((nWord >>  0) & 0xff);
                nParaLen         = 4;
			}
			break;
			case CMD_QUERY_MICRO_SWITCH:
			{ 
				nVal = Get_Micro_OC_Status(); // 0: in, 1: out
				printf("micro switch status = %d\r\n", (int)nVal);
				Msg_Return_Handle_8(e_Msg_Data, CMD_STATUS_MICRO_SWITCH, nVal);	
			}
			break;
			case CMD_QUERY_OC: // todo...
            {
                nCommand  = CMD_STATUS_OC;
                bSendBack = e_True;
                //
                *(pchFbkBuf + 0) = HW_LEVEL_GetOC(0); // in oc
                *(pchFbkBuf + 1) = HW_LEVEL_GetOC(1); // out oc
                *(pchFbkBuf + 2) = HW_LEVEL_GetOC(2); // 
                *(pchFbkBuf + 3) = HW_LEVEL_GetOC(3);
				*(pchFbkBuf + 4) = 0;
				*(pchFbkBuf + 5) = 0;
                nParaLen         = 6;
            }
			break;
            case CMD_QUERY_ELECTRODE:
            {
                nCommand  = CMD_STATUS_ELECTRODE;
                bSendBack = e_True;
                nShort = Get_Elec_V();
				
                *(pchFbkBuf + 0) = Get_Elec_Status();
                *(pchFbkBuf + 1) = (nShort >> 8);
                *(pchFbkBuf + 2) = nShort | 0xFF;
                nParaLen         = 3;
            }
			break;
			case CMD_QUERY_BIO_MODE:
			{
				nVal = g_Test_Mode; // 0: HGB, 1: CRP
				printf("bio mode = %d\r\n", (int)nVal);
				Msg_Return_Handle_8(e_Msg_Data, CMD_STATUS_BIO_MODE, nVal);	
			}
			break;
            case CMD_QUERY_EDTION:
            {
                nCommand  = CMD_STATUS_EDTION;
                bSendBack = e_True;
                //
                *(pchFbkBuf + 0) = '2';        /* FPGA版本 V1.0 ASCII格式 */
                *(pchFbkBuf + 1) = '0';        /* FPGA版本 V1.0 ASCII格式 */
                memset((pchFbkBuf + 2), 0, 32);
                memcpy((pchFbkBuf + 2), softver_edtion, (sizeof(softver_edtion)));/* MCU版本 ASCII格式 32个字节 */
                nParaLen         = (2 + 32);           
            }
			break;
			/*
            case CMD_QUERY_MOT_STAT:    // 
			{
                bSendBack = e_True;
                nCommand  = CMD_STATUS_MOT;

                if (0 == *(pchCmdBuf +  8))  // 进出仓电机 
                {
                    *(pchFbkBuf + 0) = 0;
                    *(pchFbkBuf + 1) = MT_X_get_posi();
                }
                else
                {
                    *(pchFbkBuf + 0) = 1;
                    *(pchFbkBuf + 1) = MT_Y_get_posi();
                }
                nParaLen = 2;
			}
            break;
			*/
            default:
            {
                break;
            }

        } // end query cmd
    }

    //-----------------------------------------------
    // result back

    if (e_True == SYS_HasError())
    {
        SYS_ErrorAnalyze(pchFbkBuf, &nShort);
        PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCommand, pchFbkBuf, nShort);
    }
    else if (e_True == bSendBack)
    {
        PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCommand, pchFbkBuf, nParaLen);
    }
    else                           // never happen
    {
        // do nothing
    }
    //
    return e_Feedback_Success;

} // end of "MSG_Handling()"

/*
UINT8 MSG_Testing(void)
{
    IO_ UINT32 nCurTicks = 0;
    IO_ UINT32 nLstTicks = 0;
    //
    IO_ UINT16 nPress1   = 0;
    IO_ UINT16 nPress2   = 0;
    IO_ UINT16 nHgb      = 0;
#if 1
    //==========================
    // reset the ticks
    //IT_SYS_SetTicks(0);
    //
    HW_ADC_SpiCtrl(e_True);
    //
    //--------- 1 --------------
    nCurTicks = IT_SYS_GetTicks();
    printf("\r\n\r\n === timing 1: %0.8d === \r\n", (int)nCurTicks);
    //
    HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);    // off
    HW_Valve_On(EN_VALVE_AIR);  // all the air way
    HW_Valve_Off(EN_VALVE_LIQUID);  // WBC
    HW_Valve_Off(2); // RBC
    // wait some time
    nCurTicks = IT_SYS_GetTicks();
    nLstTicks = nCurTicks;
    while (nCurTicks <= (nLstTicks + TIMING_1))
    {
        nCurTicks = IT_SYS_GetTicks();
        //HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_DROP_FPGA_DATA);
    }
    //--------- 2 --------------
    nCurTicks = IT_SYS_GetTicks();
    printf("\r\n\r\n === timing 2: %0.8d === \r\n", (int)nCurTicks);
    //
    HW_PUMP_Pulse(PUMP_PRESS_FREQ, e_Dir_Pos);  // on, 25000 ticks per ms
    HW_Valve_On(EN_VALVE_AIR);    // all the air way
    HW_Valve_Off(EN_VALVE_LIQUID);    // WBC
    HW_Valve_Off(2); // RBC
    // reset the LIST-TICK
    IT_LIST_SetTicks(0);  //===================
    // detect the press 1 and press 2 all the time
    nCurTicks = IT_SYS_GetTicks();
    nLstTicks = nCurTicks;
    //
    while (nCurTicks <= (nLstTicks + TIMING_2 - TIMING_1))
    {
        nCurTicks = IT_SYS_GetTicks();
        if (0 == (nCurTicks % 10))
        {
            nPress1   = HW_ADC_SpiGetADC(INDEX_PRESS);  // 0: HGB, 1: press1
            nPress2   = HW_ADC_SlaveGetADC();
            printf("\r\n %0.8d: %0.5d, %0.5d", (int)nCurTicks, nPress1, nPress2);
        }
        //HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_DROP_FPGA_DATA);
    }
    //--------- 3 --------------
    nCurTicks = IT_SYS_GetTicks();
    printf("\r\n\r\n === timing 3: %0.8d === \r\n", (int)nCurTicks);
    //
    HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);     // off
    HW_Valve_Off(EN_VALVE_AIR);  // all the air way
    HW_Valve_On(EN_VALVE_LIQUID);    // WBC
    HW_Valve_On(2); // RBC
    // detect the press 1 and press 2 all the time
    nCurTicks = IT_SYS_GetTicks();
    nLstTicks = nCurTicks;
    while (nCurTicks <= (nLstTicks + TIMING_3 - TIMING_2))
    {
        nCurTicks = IT_SYS_GetTicks();
        if (0 == (nCurTicks % 10))
        {
            nPress1   = HW_ADC_SpiGetADC(INDEX_PRESS);  // 0: HGB, 1: press1
            nPress2   = HW_ADC_SlaveGetADC();
            printf("\r\n %0.8d: %0.5d, %0.5d", (int)nCurTicks, nPress1, nPress2);
        }
        //HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_DROP_FPGA_DATA);
    }
    //--------- 4 --------------
    nCurTicks = IT_SYS_GetTicks();
    printf("\r\n\r\n === timing 4: %0.8d === \r\n", (int)nCurTicks);
    //
    HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);     // off
    HW_Valve_Off(EN_VALVE_AIR);  // all the air way
    HW_Valve_On(EN_VALVE_LIQUID);    // WBC
    HW_Valve_On(2); // RBC
    //==========================
    // switch on : WBC and RBC
    HW_SW_AdcWBC(e_True);
    HW_SW_AdcRBC(e_True);
    //++++++++++++++++++++++++++
    // get the ADC data
    HW_EN_WBC(e_True);
    // reset the ADC-TICK
    IT_ADC_SetTicks(0);   //===================
    //
    // detect the press 1 and press 2 all the time
    nCurTicks = IT_SYS_GetTicks();
    nLstTicks = nCurTicks;
    while (nCurTicks <= (nLstTicks + TIMING_4 - TIMING_3))
    {
        nCurTicks = IT_SYS_GetTicks();
        if (0 == (nCurTicks % 10))
        {
            nPress1   = HW_ADC_SpiGetADC(INDEX_PRESS);  // 1: press1
            nPress2   = HW_ADC_SlaveGetADC();
            nHgb	  = HW_ADC_SpiGetADC(0);  // 0: HGB, get the sample of the HGB channel
            printf("\r\n %0.8d: %0.5d, %0.5d, HGB = %0.5d", (int)nCurTicks, nPress1, nPress2, nHgb);
        }
        //HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_DROP_FPGA_DATA);
    }
    //==========================
    // switch off : WBC and RBC
    HW_SW_AdcWBC(e_False);
    HW_SW_AdcRBC(e_False);
    //++++++++++++++++++++++++++
    // close the ADC channel
    HW_EN_WBC(e_False);
    //
    //--------- 5 --------------
    nCurTicks = IT_SYS_GetTicks();
    printf("\r\n\r\n === timing 5: %0.8d === \r\n", (int)nCurTicks);
    //
    HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);     // off
    HW_Valve_Off(EN_VALVE_AIR);  // all the air way
    //HW_Valve_Off(EN_VALVE_LIQUID); // WBC
    //HW_Valve_Off(2); // RBC
    // detect the press 1 and press 2 all the time
    nCurTicks = IT_SYS_GetTicks();
    nLstTicks = nCurTicks;
    while (nCurTicks <= (nLstTicks + TIMING_5 - TIMING_4))
    {
        nCurTicks = IT_SYS_GetTicks();
        if (0 == (nCurTicks % 10))
        {
            nPress1   = HW_ADC_SpiGetADC(INDEX_PRESS);  // 0: HGB, 1: press1
            nPress2   = HW_ADC_SlaveGetADC();
            printf("\r\n %0.8d: %0.5d, %0.5d", (int)nCurTicks, nPress1, nPress2);
        }
        //HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_DROP_FPGA_DATA);
    }
    //--------- 6 --------------
    nCurTicks = IT_SYS_GetTicks();
    printf("\r\n\r\n === timing 6: %0.8d === \r\n", (int)nCurTicks);
    //
    HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);     // off
    HW_Valve_Off(EN_VALVE_AIR);  // all the air way
    //HW_Valve_Off(EN_VALVE_LIQUID); // WBC
    //HW_Valve_Off(2); // RBC
    // detect the press 1 and press 2 all the time
    nCurTicks = IT_SYS_GetTicks();
    nLstTicks = nCurTicks;
    while (nCurTicks <= (nLstTicks + TIMING_6 - TIMING_5))
    {
        nCurTicks = IT_SYS_GetTicks();
        // do not get press 1
        // do not get press 2
        //HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_DROP_FPGA_DATA);
    }
    //--------- 7 --------------
    nCurTicks = IT_SYS_GetTicks();
    printf("\r\n\r\n === timing 7: %0.8d === \r\n", (int)nCurTicks);
    nCurTicks = IT_SYS_GetTicks();
    nLstTicks = nCurTicks;
    while (nCurTicks <= (nLstTicks + TIMING_7 - TIMING_6))
    {
        nCurTicks = IT_SYS_GetTicks();
        // do nothing
        //HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_DROP_FPGA_DATA);
	}
    //
    HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);    // off
    HW_Valve_Off(EN_VALVE_AIR); // all the air way
    HW_Valve_Off(EN_VALVE_LIQUID);  // WBC
    HW_Valve_Off(2); // RBC
    //
    // off
    HW_SW_AdcWBC(e_True);
    //
#if 0
    while (1)	 // attention: it must be stoped here !
    {
        nPress1   = HW_ADC_SpiGetADC(INDEX_PRESS);  // 0: HGB, 1: press1
        nPress2   = HW_ADC_SlaveGetADC();
        printf("%0.8d: %0.5d, %0.5d\r\n", nCurTicks, nPress1, nPress2);
        //
        IT_SYS_DlyMs(1000);
    }
#endif
    //
    return e_Feedback_Success;
#endif
}
*/

#ifdef  DEBUG_INFO_UP_LOAD
void Append_Debug_Info(INT8 *pInfo, INT8 *pTemp, UINT16 *pInfoLen)
{
	INT16 nLen = 0;

	if(*pInfoLen >= PROTOCOL_SEND_BUF_LENGTH) return;
	while(*(pTemp) != '\0')
	{
		*(pInfo++) = *(pTemp++);
		nLen++;
		if(nLen >= DEBUG_INFO_TEMP_LEN){
			nLen = DEBUG_INFO_TEMP_LEN;
			printf("temp buf is over flow\r\n");
			break;
		}
	}
	*pInfoLen += nLen;	
}
#endif



// motox part test
void Part_Test_Moto_X(UINT8 nNo, UINT32 nNum)
{
	UINT32 i = 1;
	UINT8 nLen = 0, strReturn[8] = {0};
	
	// at first not sure where is moto, go out
	if(g_tAxisPosStatus.eAxisX == E_AXIS_X_POS_UNSURE) 
	{
		if(e_Feedback_Success == MT_X_MoveToPosRel(e_SelfCheck_Call)){ // out
			printf("Part_Test_Moto_X: moto out success");
		}else{
			strReturn[nLen++] = MOTO_OUT_IN_TEST;
			strReturn[nLen++] = 0x01;
			strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
			strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
			Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
			nLen = 0;
			printf("Part_Test_Moto_X: moto out error, i=%d\r\n", (int)((i + 1)/2));
			return;
		}
	}
	//
	for(i = 1; i <= nNum*2; i++)
	{
		if(g_tAxisPosStatus.eAxisX == E_AXIS_X_POS_HOME) // go out 
		{
			if(e_Feedback_Success == MT_X_MoveToPosRel(e_SelfCheck_Call)) // out
			{	// send success once time
				strReturn[nLen++] = MOTO_OUT_IN_TEST;
				strReturn[nLen++] = 0x00;
				strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
				strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
				Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
				nLen = 0;
				printf("Part_Test_Moto_X: moto out success, i=%d\r\n", (int)((i + 1)/2));
			}else{
				strReturn[nLen++] = MOTO_OUT_IN_TEST;
				strReturn[nLen++] = 0x01;
				strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
				strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
				Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
				nLen = 0;
				printf("Part_Test_Moto_X: moto out error, i=%d\r\n", (int)((i + 1)/2));
				return;
			}	
			if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
		}else if(g_tAxisPosStatus.eAxisX == E_AXIS_X_POS_CTRL){ // get in
			if(e_Feedback_Success == MT_X_Home(e_SelfCheck_Call))
			{
				printf("Part_Test_Moto_X: moto in success, i=%d\r\n", (int)((i + 1)/2));
			}else{
				strReturn[nLen++] = MOTO_OUT_IN_TEST;
				strReturn[nLen++] = 0x01;
				strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
				strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
				Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
				nLen = 0;
				printf("Part_Test_Moto_X: moto in error, i=%d\r\n", (int)((i + 1)/2));
				return;
			}
			if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
		}else{ // moto position error
			strReturn[nLen++] = MOTO_OUT_IN_TEST;
			strReturn[nLen++] = 0x01;
			strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
			strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
			Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
			nLen = 0;
			printf("Part_Test_Moto_X: position error, i=%d\r\n", (int)((i + 1)/2));
			return;
		}
		if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
	}
	// success
//	strReturn[nLen++] = MOTO_OUT_IN_TEST;
//	strReturn[nLen++] = 0x00;
//	strReturn[nLen++] = ((i)/2) >> 24; strReturn[nLen++] = ((i)/2) >> 16;
//	strReturn[nLen++] = ((i)/2) >> 8;  strReturn[nLen++] = ((i)/2);
//	Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
//	nLen = 0;
	printf("Part_Test_Moto_X: Test Finished, i=%d\r\n", (int)((i)/2));
	return;
}



// wbc elec test
void Part_Test_WBC_Elec(UINT8 nNo, UINT32 nNum)
{
	UINT32 i = 1;
	UINT8 Temp = 0xFF, Flag = 0xFE, nLen = 0, strReturn[8] = {0};
	
	// at first not sure where is moto, go out
	if(g_tAxisPosStatus.eAxisX == E_AXIS_X_POS_UNSURE) 
	{
		if(e_Feedback_Success == MT_X_MoveToPosRel(e_SelfCheck_Call)){ // out
			Flag = Get_WBC_V_Status(COUNT_WBC_TOUCH_CHECK_V);	
			printf("Part_Test_WBC_Elec: moto out success\r\n");
		}else{
			strReturn[nLen++] = WBC_ELEC_TEST;
			strReturn[nLen++] = 0x02; // moto error
			strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
			strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
			Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
			nLen = 0;
			printf("Part_Test_WBC_Elec: moto out error, i=%d\r\n", (int)((i)/2));
			return;
		}
	}
	//
	for(i = 1; i <= nNum*2; i++)
	{
		if(g_tAxisPosStatus.eAxisX == E_AXIS_X_POS_HOME) // go out 
		{
			if(e_Feedback_Success == MT_X_MoveToPosRel(e_SelfCheck_Call)) // out
			{
				Temp = Get_WBC_V_Status(COUNT_WBC_TOUCH_CHECK_V);		
//				if(Flag == Temp) 
//				{
//					strReturn[nLen++] = WBC_ELEC_TEST;
//					strReturn[nLen++] = 0x01;
//					strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
//					strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
//					Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
//					nLen = 0;
//					printf("Part_Test_WBC_Elec: moto out, check error,Temp=%d, Flag=%d, i=%d\r\n", Temp, Flag, (int)((i + 1)/2));
//					return;
//				}else{
//					// send success once time
//					strReturn[nLen++] = WBC_ELEC_TEST;
//					strReturn[nLen++] = 0x00;
//					strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
//					strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
//					Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
//					nLen = 0;
//					printf("Part_Test_WBC_Elec: moto out,Temp=%d, Flag=%d, i=%d\r\n", Temp, Flag, (int)((i + 1)/2));
//				}
				printf("Part_Test_WBC_Elec: moto out,Temp=%d, Flag=%d, i=%d\r\n", Temp, Flag, (int)((i + 1)/2));
				Flag = Temp;
			}else{
				strReturn[nLen++] = WBC_ELEC_TEST;
				strReturn[nLen++] = 0x02; //moto error
				strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
				strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
				Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
				nLen = 0;
				printf("Part_Test_WBC_Elec: moto out error, i=%d\r\n", (int)((i + 1)/2));
				return;
			}
			if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
		}else if(g_tAxisPosStatus.eAxisX == E_AXIS_X_POS_CTRL){ // get in
			if(e_Feedback_Success == MT_X_Home(e_SelfCheck_Call))
			{
				IT_SYS_DlyMs(PART_TEST_CHECK_DELAY);
				Temp = Get_WBC_V_Status(COUNT_WBC_TOUCH_CHECK_V);
//				printf("Part_Test_WBC_Elec: moto in,Temp=%d, Flag=%d, i=%d\r\n", Temp, Flag, (int)((i + 1)/2));
				if(Flag == Temp) 
				{
					strReturn[nLen++] = WBC_ELEC_TEST;
					strReturn[nLen++] = 0x01; // wbc check error
					strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
					strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
					Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
					nLen = 0;
					printf("Part_Test_WBC_Elec: moto in, check error,Temp=%d, Flag=%d, i=%d\r\n", Temp, Flag, (int)((i + 1)/2));
				}else{
					strReturn[nLen++] = WBC_ELEC_TEST;
					strReturn[nLen++] = 0x00; // wbc check success
					strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
					strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
					Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
					nLen = 0;
					printf("Part_Test_WBC_Elec: moto in, check success,Temp=%d, Flag=%d, i=%d\r\n", Temp, Flag, (int)((i + 1)/2));	
				}
				Flag = Temp;
			}else{
				strReturn[nLen++] = WBC_ELEC_TEST;
				strReturn[nLen++] = 0x02; //moto error
				strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
				strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
				Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
				nLen = 0;
				printf("Part_Test_WBC_Elec: moto in error, i=%d\r\n", (int)((i + 1)/2));
				return;
			}
			if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
		}else{ // moto position error //moto error
			strReturn[nLen++] = WBC_ELEC_TEST;
			strReturn[nLen++] = 0x02; //moto error
			strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
			strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
			Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
			nLen = 0;
			printf("Part_Test_WBC_Elec: position error, i=%d\r\n", (int)((i + 1)/2));
			return;
		}
		if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
	}
	// success
//	strReturn[nLen++] = WBC_ELEC_TEST;
//	strReturn[nLen++] = 0x00;
//	strReturn[nLen++] = ((i)/2) >> 24; strReturn[nLen++] = ((i)/2) >> 16;
//	strReturn[nLen++] = ((i)/2) >> 8;  strReturn[nLen++] = ((i)/2);
//	Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
//	nLen = 0;
	printf("Part_Test_WBC_Elec: Test Finished, i=%d\r\n", (int)((i)/2));
	return;
}

// part test check elec
void Part_Test_Check_Elec(UINT8 nNo, UINT32 nNum)
{
	UINT32 i = 1;
	UINT8 Temp = 0xFF, Flag = 0xFE, nLen = 0, strReturn[8] = {0};
	
	// at first not sure where is moto, go out
	if(g_tAxisPosStatus.eAxisX == E_AXIS_X_POS_UNSURE) 
	{
		if(e_Feedback_Success == MT_X_MoveToPosRel(e_SelfCheck_Call)){ // out
			Flag = hw_filter_get_electrode(INDEX_ELECTRODE);
			printf("Part_Test_Check_Elec: moto out success, i=%d\r\n", (int)((i + 1)/2));
		}else{
			strReturn[nLen++] = CHECK_ELEC_TEST;
			strReturn[nLen++] = 0x02; //moto error
			strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
			strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
			Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
			nLen = 0;
			printf("Part_Test_Check_Elec: moto out error, i=%d\r\n", (int)((i + 1)/2));
			return;
		}
	}
	//
	for(i = 1; i <= nNum*2; i++)
	{
		if(g_tAxisPosStatus.eAxisX == E_AXIS_X_POS_HOME) // go out 
		{
			if(e_Feedback_Success == MT_X_MoveToPosRel(e_SelfCheck_Call)) // out
			{
				Temp = hw_filter_get_electrode(INDEX_ELECTRODE);
//				if(Flag == Temp) 
//				{
//					strReturn[nLen++] = CHECK_ELEC_TEST;
//					strReturn[nLen++] = 0x01;
//					strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
//					strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
//					Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
//					nLen = 0;
//					printf("Part_Test_Check_Elec: moto out, check error,Temp=%d, Flag=%d, i=%d\r\n", Temp, Flag, (int)((i + 1)/2));
//					return;
//				}else{
//					// send success once time
//					strReturn[nLen++] = CHECK_ELEC_TEST;
//					strReturn[nLen++] = 0x00;
//					strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
//					strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
//					Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
//					nLen = 0;
//					printf("Part_Test_Check_Elec: moto out,Temp=%d, Flag=%d, i=%d\r\n", Temp, Flag, (int)((i + 1)/2));
//				}
				printf("Part_Test_Check_Elec: moto out,Temp=%d, Flag=%d, i=%d\r\n", Temp, Flag, (int)((i + 1)/2));
				Flag = Temp;
			}else{ //error
				strReturn[nLen++] = CHECK_ELEC_TEST;
				strReturn[nLen++] = 0x02; //moto error
				strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
				strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
				Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
				nLen = 0;
				printf("Part_Test_Check_Elec: moto out error, i=%d\r\n", (int)((i + 1)/2));
				return;
			}	
			if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
		}else if(g_tAxisPosStatus.eAxisX == E_AXIS_X_POS_CTRL){ // get in
			if(e_Feedback_Success == MT_X_Home(e_SelfCheck_Call))
			{
				Temp = hw_filter_get_electrode(INDEX_ELECTRODE);
				//printf("Part_Test_Check_Elec: moto in,Temp=%d, Flag=%d, i=%d\r\n", Temp, Flag, (int)((i + 1)/2));
				if(Flag == Temp) 
				{
					strReturn[nLen++] = CHECK_ELEC_TEST;
					strReturn[nLen++] = 0x01; // check error
					strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
					strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
					Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
					nLen = 0;
					printf("Part_Test_Check_Elec: moto in, check error,Temp=%d, Flag=%d, i=%d\r\n", Temp, Flag, (int)((i + 1)/2));
				}else{
					// send success once time
					strReturn[nLen++] = CHECK_ELEC_TEST;
					strReturn[nLen++] = 0x00; //success
					strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
					strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
					Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
					nLen = 0;
					printf("Part_Test_Check_Elec: moto in,Temp=%d, Flag=%d, i=%d\r\n", Temp, Flag, (int)((i + 1)/2));
				}
				Flag = Temp;
			}else{
				strReturn[nLen++] = CHECK_ELEC_TEST;
				strReturn[nLen++] = 0x02; // moto position error
				strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
				strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
				Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
				nLen = 0;
				printf("Part_Test_Check_Elec: moto in error, i=%d\r\n", (int)((i + 1)/2));
				return;
			}
			if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
		}else{ // moto position error
			strReturn[nLen++] = CHECK_ELEC_TEST;
			strReturn[nLen++] = 0x02; //moto error 
			strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
			strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
			Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
			nLen = 0;
			printf("Part_Test_Check_Elec: position error, i=%d\r\n", (int)((i + 1)/2));
			return;
		}
		if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
	}
	// success
//	strReturn[nLen++] = CHECK_ELEC_TEST;
//	strReturn[nLen++] = 0x00;
//	strReturn[nLen++] = ((i)/2) >> 24; strReturn[nLen++] = ((i)/2) >> 16;
//	strReturn[nLen++] = ((i)/2) >> 8;  strReturn[nLen++] = ((i)/2);
//	Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
//	nLen = 0;
	printf("Part_Test_Check_Elec: Test Finished, i=%d\r\n", (int)((i)/2));
	return;
}	



// part test wbc and check elec
void Part_Test_WBC_And_Check_Elec(UINT8 nNo, UINT32 nNum)
{
	UINT32 i = 1;
	IO_ UINT8 Temp_wbc = 0xFF, Flag_wbc = 0xFE, Temp_Check = 0xFF, Flag_Check = 0xFE, Result = 0;
	UINT8 nLen = 0, strReturn[8] = {0};
	
	// at first not sure where is moto, go out
	if(g_tAxisPosStatus.eAxisX == E_AXIS_X_POS_UNSURE) 
	{
		if(e_Feedback_Success == MT_X_MoveToPosRel(e_SelfCheck_Call)){ // out
			Flag_Check = hw_filter_get_electrode(INDEX_ELECTRODE);
			Flag_wbc = Get_WBC_V_Status(COUNT_WBC_TOUCH_CHECK_V);
			printf("Part_Test_WBC_And_Check_Elec: moto out success\r\n");
		}else{
			strReturn[nLen++] = WBC_AND_CHECK_ELEC_TEST;
			strReturn[nLen++] = 0x02;
			strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
			strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
			Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
			nLen = 0;
			printf("Part_Test_WBC_And_Check_Elec: moto out error");
			return;
		}
	}
	//
	for(i = 1; i <= nNum*2; i++)
	{
		if(g_tAxisPosStatus.eAxisX == E_AXIS_X_POS_HOME) // go out 
		{
			if(e_Feedback_Success == MT_X_MoveToPosRel(e_SelfCheck_Call)) // out
			{
				Temp_Check = hw_filter_get_electrode(INDEX_ELECTRODE);
				Temp_wbc = Get_WBC_V_Status(COUNT_WBC_TOUCH_CHECK_V);
				printf("Part_Test_WBC_And_Check_Elec: moto out,i=%d, Temp_wbc=%d, Flag_wbc=%d, Temp_check=%d, Flag_Check=%d\r\n",\
						(int)((i + 1)/2), Temp_wbc, Flag_wbc, Temp_Check, Flag_Check);
//				Result = 0;
//				if(Flag_Check == Temp_Check) 
//				{
//					Result = 3;
//				}
//				if(Flag_wbc == Temp_wbc)
//				{
//					if(Result == 3) Result = 5;
//					else Result = 4;
//				}	
//				//
//				if(Result == 0){
//					// send success once time
//					strReturn[nLen++] = WBC_AND_CHECK_ELEC_TEST;
//					strReturn[nLen++] = Result;
//					strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
//					strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
//					Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
//					nLen = 0;
//					printf("Part_Test_WBC_And_Check_Elec: moto out,i=%d, Temp_wbc=%d, Flag_wbc=%d, Temp_check=%d, Flag_Check=%d\r\n",\
						(int)((i + 1)/2), Temp_wbc, Flag_wbc, Temp_Check, Flag_Check);
//				}else{
//					
//					strReturn[nLen++] = WBC_AND_CHECK_ELEC_TEST;
//					strReturn[nLen++] = Result;
//					strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
//					strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
//					Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
//					nLen = 0;
//					printf("Part_Test_WBC_And_Check_Elec: moto out,i=%d, Temp_wbc=%d, Flag_wbc=%d, Temp_check=%d, Flag_Check=%d\r\n",\
						(int)((i + 1)/2), Temp_wbc, Flag_wbc, Temp_Check, Flag_Check);
//				}
				Flag_Check = Temp_Check;
				Flag_wbc = Temp_wbc;
				
			}else{
				strReturn[nLen++] = WBC_AND_CHECK_ELEC_TEST;
				strReturn[nLen++] = 0x02;
				strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
				strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
				Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
				nLen = 0;
				printf("Part_Test_WBC_And_Check_Elec: moto error,i=%d, Temp_wbc=%d, Flag_wbc=%d, Temp_check=%d, Flag_Check=%d\r\n",\
						(int)((i + 1)/2), Temp_wbc, Flag_wbc, Temp_Check, Flag_Check);
				return;
			}	
			if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
		}else if(g_tAxisPosStatus.eAxisX == E_AXIS_X_POS_CTRL){ // get in
			if(e_Feedback_Success == MT_X_Home(e_SelfCheck_Call))
			{
				IT_SYS_DlyMs(PART_TEST_CHECK_DELAY);
				Temp_Check = hw_filter_get_electrode(INDEX_ELECTRODE);
				Temp_wbc = Get_WBC_V_Status(COUNT_WBC_TOUCH_CHECK_V);
				
				Result = 0;
				if(Flag_Check == Temp_Check) 
				{
					Result = 3;
					printf("Part_Test_WBC_And_Check_Elec: moto in, check elec fail,i=%d, Temp_wbc=%d, Flag_wbc=%d, Temp_check=%d, Flag_Check=%d\r\n",\
						(int)((i + 1)/2), Temp_wbc, Flag_wbc, Temp_Check, Flag_Check);
				}
				if(Flag_wbc == Temp_wbc)
				{
					if(Result == 3)
					{
						Result = 5;
						printf("Part_Test_WBC_And_Check_Elec: moto in,wbc and check elec fail,i=%d, Temp_wbc=%d, Flag_wbc=%d, Temp_check=%d, Flag_Check=%d\r\n",\
						(int)((i + 1)/2), Temp_wbc, Flag_wbc, Temp_Check, Flag_Check);
					}else{
						Result = 4;
						printf("Part_Test_WBC_And_Check_Elec: moto in, wbc elec fail,i=%d, Temp_wbc=%d, Flag_wbc=%d, Temp_check=%d, Flag_Check=%d\r\n",\
						(int)((i + 1)/2), Temp_wbc, Flag_wbc, Temp_Check, Flag_Check);
					}
				}	
				//				
				if(Result == 0) 
				{
					// send success once time
					strReturn[nLen++] = WBC_AND_CHECK_ELEC_TEST;
					strReturn[nLen++] = Result;
					strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
					strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
					Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
					nLen = 0;
					printf("Part_Test_WBC_And_Check_Elec: moto in,check success,i=%d, Temp_wbc=%d, Flag_wbc=%d, Temp_check=%d, Flag_Check=%d\r\n",\
						(int)((i + 1)/2), Temp_wbc, Flag_wbc, Temp_Check, Flag_Check);
				}else{
					strReturn[nLen++] = WBC_AND_CHECK_ELEC_TEST;
					strReturn[nLen++] = Result;
					strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
					strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
					Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
					nLen = 0;
					printf("Part_Test_WBC_And_Check_Elec: moto in, check fail,i=%d, Temp_wbc=%d, Flag_wbc=%d, Temp_check=%d, Flag_Check=%d\r\n",\
						(int)((i + 1)/2), Temp_wbc, Flag_wbc, Temp_Check, Flag_Check);
				}
				Flag_Check = Temp_Check;
				Flag_wbc = Temp_wbc;
		
			}else{ // moto position error
				strReturn[nLen++] = WBC_AND_CHECK_ELEC_TEST;
				strReturn[nLen++] = 0x02;
				strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
				strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
				Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
				nLen = 0;
				printf("Part_Test_WBC_And_Check_Elec: moto error,i=%d, Temp_wbc=%d, Flag_wbc=%d, Temp_check=%d, Flag_Check=%d\r\n",\
						(int)((i + 1)/2), Temp_wbc, Flag_wbc, Temp_Check, Flag_Check);
				return;
			}
			if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
		}
		if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
	}
	// success
//	strReturn[nLen++] = WBC_AND_CHECK_ELEC_TEST;
//	strReturn[nLen++] = 0x00;
//	strReturn[nLen++] = ((i)/2) >> 24; strReturn[nLen++] = ((i)/2) >> 16;
//	strReturn[nLen++] = ((i)/2) >> 8;  strReturn[nLen++] = ((i)/2);
//	Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
//	nLen = 0;
	printf("Part_Test_WBC_And_Check_Elec: Test Finished, i=%d\r\n", (int)((i)/2));
	return;
}	



// part airlight test,air faucet
void Part_Test_AirFaucet(UINT8 nNo, UINT32 nNum)
{
	UINT32 i = 1;
	UINT8 nLen = 0, strReturn[8] = {0};
	// at first not sure where is moto, go out
	if(g_tAxisPosStatus.eAxisX == E_AXIS_X_POS_UNSURE) 
	{
		if(e_Feedback_Success == MT_X_MoveToPosRel(e_SelfCheck_Call)){ // out
			printf("Part_Test_AirFaucet: moto out success, i=%d\r\n", (int)((i + 1)/2));
		}else{
			strReturn[nLen++] = AIRFAUCET_TEST;
			strReturn[nLen++] = 0x02;
			strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
			strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
			Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
			nLen = 0;
			printf("Part_Test_AirFaucet: moto out error\r\n");
			return;
		}
	}	
	//
	for(i = 1; i <= nNum*2; i++)
	{
		if(g_tAxisPosStatus.eAxisX == E_AXIS_X_POS_HOME) // go out 
		{
			if(e_Feedback_Success == MT_X_MoveToPosRel(e_SelfCheck_Call)){ // out
//				// send success once time
//				strReturn[nLen++] = AIRFAUCET_TEST;
//				strReturn[nLen++] = 0x00;
//				strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
//				strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
//				Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
//				nLen = 0;
				printf("Part_Test_AirFaucet: moto out success, i=%d\r\n", (int)((i + 1)/2));
			}else{
				strReturn[nLen++] = AIRFAUCET_TEST;
				strReturn[nLen++] = 0x02;
				strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
				strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
				Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
				nLen = 0;
				printf("Part_Test_AirFaucet: moto out error, i=%d\r\n", (int)((i + 1)/2));
				return;
			}
			if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
		}else if(g_tAxisPosStatus.eAxisX == E_AXIS_X_POS_CTRL){ // get in
			if(e_Feedback_Success == MT_X_Home(e_SelfCheck_Call))
			{		
				printf("Part_Test_AirFaucet: moto in, i=%d\r\n", (int)((i + 1)/2));
				if(e_Feedback_Fail == AirLight_Self_Check(e_PartTest_Call)){
					strReturn[nLen++] = AIRFAUCET_TEST;
					strReturn[nLen++] = 0x01;
					strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
					strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
					Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
					nLen = 0;
					printf("Part_Test_AirFaucet: failure, i=%d\r\n", (int)((i)/2));
//					return;
				}else{
					// success
					strReturn[nLen++] = AIRFAUCET_TEST;
					strReturn[nLen++] = 0x00;
					strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
					strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
					Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
					nLen = 0;
					printf("Part_Test_AirFaucet: success, i=%d\r\n", (int)((i)/2));
				}
			}else{
				strReturn[nLen++] = AIRFAUCET_TEST;
				strReturn[nLen++] = 0x02;
				strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
				strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
				Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
				nLen = 0;
				printf("Part_Test_AirFaucet: moto in error, i=%d\r\n", (int)((i + 1)/2));
				return;
				
			}
			printf("Part_Test_AirFaucet: after airlight check, i=%d\r\n", (int)((i + 1)/2));
			if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
		}else{ // moto position error
			printf("Part_Test_AirFaucet: moto position error, i=%d\r\n", (int)((i + 1)/2));
			strReturn[nLen++] = AIRFAUCET_TEST;
			strReturn[nLen++] = 0x01;
			strReturn[nLen++] = ((i + 1)/2) >> 24; strReturn[nLen++] = ((i + 1)/2) >> 16;
			strReturn[nLen++] = ((i + 1)/2) >> 8;  strReturn[nLen++] = ((i + 1)/2);
			nLen = 0;
			Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
			return;
		}
		if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
	}
	// success
//	strReturn[nLen++] = AIRFAUCET_TEST;
//	strReturn[nLen++] = 0x00;
//	strReturn[nLen++] = ((i)/2) >> 24; strReturn[nLen++] = ((i)/2) >> 16;
//	strReturn[nLen++] = ((i)/2) >> 8;  strReturn[nLen++] = ((i)/2);
//	Msg_Return_Handle_String(e_Msg_Status, CMD_STATUS_PART_TEST, strReturn, nLen);
//	nLen = 0;
	printf("Part_Test_AirFaucet: Test Finished, i=%d\r\n", (int)((i)/2));
	return;

}

void Part_Test_Valve(UINT8 nNo, UINT32 nNum)
{
	UINT32 i = 1;
	UINT8 nLen = 0, strReturn[8] = {0};
	
	for(i = 1; i <= nNum; i++)
	{
		HW_Valve_On(EN_VALVE_LIQUID);
		IT_SYS_DlyMs(750);
		HW_Valve_Off(EN_VALVE_LIQUID);
		IT_SYS_DlyMs(750);
		
		strReturn[nLen++] = VALVE_TEST;
		strReturn[nLen++] = 0x00;
		strReturn[nLen++] = i >> 24; strReturn[nLen++] = i >> 16;
		strReturn[nLen++] = i >> 8;  strReturn[nLen++] = i;
		Msg_Return_Handle_String(e_Msg_Status, VALVE_TEST, strReturn, nLen);
		nLen = 0;
		printf("Part_Test_Valve: success, i=%d\r\n", (int)i);
		if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
	}
}


// part test func
void Part_Test_Exec(UINT8 nNo, UINT32 nNum)
{
	printf("Part_Test_Exec: No=%d, Num = %d\r\n", nNo, (int)nNum);
	if(nNo == 0xFF && nNum==0) printf("exit\r\n");
	switch(nNo)
	{
		case MOTO_OUT_IN_TEST:
		{
			Part_Test_Moto_X(nNo, nNum);
		}
		break;
		case WBC_ELEC_TEST:
		{
			Part_Test_WBC_Elec(nNo, nNum);
		}
		break;				
		case CHECK_ELEC_TEST:
		{
			Part_Test_Check_Elec(nNo, nNum);
		}
		break;
		case AIRFAUCET_TEST:
		{
			Part_Test_AirFaucet(nNo, nNum);
		}
		break;
		case WBC_AND_CHECK_ELEC_TEST:
		{
			Part_Test_WBC_And_Check_Elec(nNo, nNum);
		}
		case VALVE_TEST:
		{
			Part_Test_Valve(nNo, nNum);
		}
		break;
		default: break;
	}
	MT_X_Home(e_SelfCheck_Call);
}

void Send_Packets_Test(UINT16 Time, UINT32 Num)
{
		UINT32 nCurTicks = 0, nHoldTimcks = 0, i,j; //nTempTicks = 0;
		INT32  us = 0;
	
		g_Udp_Count  = 0;
		g_Send_Fail  = 0;
		HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks());
		if(Num != 0 || Time != 0)
		{
			//us = Time*1000/Num;
			//us = us*2/3;
			us = Time*600/Num;
			if(us < 0)
			{
				printf("us < 0, exit\r\n");
				g_Udp_Count  = 0;
				g_Send_Fail  = 0;
				Num    = 0;
				Time   = 0;
			}
			printf("us=%d\r\n", (int)us);
		}
		
		if(Num == 0 || Time == 0)
		{
			//printf("Nothing is send to App, ticks =%d\r\n", (int)IT_ADC_GetTicks());
			IT_SYS_DlyMs(10);
		}else{
			IT_ADC_SetTicks(0);
			IT_LIST_SetTicks(0);
			nCurTicks   = IT_ADC_GetTicks();
//			nTempTicks  = nCurTicks;
			nHoldTimcks = nCurTicks;
			while((nCurTicks < nHoldTimcks + Time))
			{				
				for(i = 0; i < 4; i++)
				{
					Send_Packet_Test();
//					LwIP_Periodic_Handle(nCurTicks);
				}
				//
				nCurTicks  = IT_ADC_GetTicks();
				LwIP_Periodic_Handle(nCurTicks);
//				if(nCurTicks - nTempTicks >= 1)
//				{
//				//	printf("t=%d, s=%d\r\n", (int)nCurTicks, (int)g_Udp_Count);
//					nTempTicks = nCurTicks;
//				}
				//
				
				for(i = 0; i <us; i++)
				{
					for(j = 0; j < 168; j++) //168 1us
					{
	
					}
				}
				LwIP_Periodic_Handle(nCurTicks);
			}
			collect_return_hdl(COLLECT_RET_SUCESS); 
//			IT_SYS_DlyMs(500);
			printf("\r\nt=%d, s=%d, f=%d\r\n", (int)nCurTicks, (int)g_Udp_Count, (int)g_Send_Fail);
			g_Udp_Count  = 0;
			g_Send_Fail  = 0;
			printf("Send Over\r\n");
		}
}

// CRP 
void Micro_Switch_Check(void)
{
	if(EN_CRP_TEST == g_Test_Mode)
	{
		if(g_Micro_Switch == EN_CLOSE) // had checked cuvette put in
		{
			CRP_Test_Exec(EN_CRP_CALIBRATE);
			g_Micro_Switch = EN_OPEN;
		}
	}
}


UINT8 LED_Mode_Set(UINT8 nIndex, UINT8 nLED)
{
	UINT32 nRet = 0;
	
	LED_All_Reset();
	switch(nIndex)
	{
		case EN_HGB_TEST: // HGB LED adjust LED Cur
		{
			nRet = (EN_HGB_TEST << 24) | nRet;
//			//LED_Exec(nLED, EN_OPEN); 	  		// open led
//			LED_Exec(HGB_LED_NUM, EN_OPEN); 	  		// open led
//			//Turn_Motor_Select_LED(nLED); 		// led go to test positon 
//			LED_Cur_ADC_Check_Channel(nLED); 	// CD4051 open the channel, and then start to adjust	
//			LED_Cur_DAC_Set(LED_525_DEFUALT_CUR_VALUE);
//			LED_Cur_Switch(EN_OPEN);	//led cur open
			
			//LED_Exec(nLED, EN_OPEN); 	  		// open led
			LED_Exec(0, EN_OPEN); 	  		// open led
			//Turn_Motor_Select_LED(nLED); 		// led go to test positon 
			LED_Cur_ADC_Check_Channel(0); 	// CD4051 open the channel, and then start to adjust	
			LED_Cur_DAC_Set(1480);
			LED_Cur_Switch(EN_OPEN);	//led cur open
			g_Test_Mode = EN_HGB_TEST;
			printf("HGB Mode Set Finished M=%d\r\n", g_Test_Mode);	
		}
		break;
		case EN_CRP_TEST: // CRP need select LED and adjust LED Cur
		{
			nRet = (EN_CRP_TEST << 24) | nRet;
			LED_Exec(nLED, EN_OPEN); 	 	 		 // open led
			Turn_Motor_Select_LED(nLED); 			 // led go to test positon 
			LED_Cur_ADC_Check_Channel(nLED);		 // CD4051 open the channel, and then start to adjust
			LED_Cur_DAC_Set(LED_840_DEFUALT_CUR_VALUE);
			LED_Cur_Switch(EN_OPEN);	//led cur open
			
			//LED_Cur_Auto_Adjust(CRP_LED_CUR_ADJUST_VALUE); 	 // adjust cur		
			g_Test_Mode = EN_CRP_TEST;
			printf("CRP Mode Set Finished, M=%d\r\n", g_Test_Mode);
		}
		break;
		default:
		{
			LED_Exec(nLED, EN_OPEN); 	 	 		 // open led
			Turn_Motor_Select_LED(nLED); 			 // led go to test positon 
			LED_Cur_ADC_Check_Channel(nLED);		 // CD4051 open the channel, and then start to adjust
			LED_Cur_DAC_Set(LED_840_DEFUALT_CUR_VALUE);
			LED_Cur_Switch(EN_OPEN);	//led cur open
			
			//LED_Cur_Auto_Adjust(CRP_LED_CUR_ADJUST_VALUE); 	 // adjust cur		
			//g_Test_Mode = EN_CRP_TEST;
			printf("CRP Mode Set Finished, M=%d\r\n", g_Test_Mode);
		}
		break;	
	}
	nRet = (nLED << 16) | nRet;
	nRet = (0 << 8) | nRet;
	//Turn_Motor_Power(EN_CLOSE);
	Turn_Motor_Disable();
	Msg_Return_Handle_32(e_Msg_Status, CMD_STATUS_TEST_MODE_SET, nRet);
	return 0;
}

//
UINT8 LED_Test_Exec(UINT8 Index, UINT8 nFlag)
{
	if(nFlag == LED_STATUS_OPEN) // HGB LED
	{
		if(Index == LED_HGB_INDEX || Index == LED_CRP_INDEX)
		{
			HW_LED_On(Index);
		}else{
			return e_Feedback_Fail;
		}
	
	}else if(nFlag == LED_STATUS_CLOSE){ // CRP LED
		if(Index == LED_HGB_INDEX || Index == LED_CRP_INDEX)
		{
			HW_LED_Off(Index);	
		}else{
			return e_Feedback_Fail;
		}
	}
	return e_Feedback_Success;
}


UINT8 HGB_Test_Exec(eTestMode eMode)
{
	UINT16 i;
    UINT32 buffer[HGB_CALIBRATE_DATA_NUM] = {0}, nSum = 0;
	
	// check postion 
//	if (E_AXIS_POS_HOME != HW_LEVEL_GetOC(OC_AXIS_POS_INDEX))
//    {
//		printf("Count Error: MT x not at home position\r\n");
//        collect_return_hdl(COLLECT_RET_FAIL_NONE_HOME);  /* 未执行进仓操作 */
//        return e_Feedback_Error;
//    }
//#if 0
//	// check HGB current
//	if(Get_V_HGB_LED() < HGB_MIX_THRESHOLD_V)
//	{
//		printf("Count Error: HGB V is wrong\r\n");
//        collect_return_hdl(COLLECT_RET_FAIL_HGB_V);  /* 未执行进仓操作 */
//        return e_Feedback_Error;
//	}
//#endif
	// open LED
//	HW_LED_On(LED_HGB_INDEX);
	// enable LED ADC Channel
//	HW_EN_ADC_HGB(e_True);
//	IT_SYS_DlyMs(100);
	//
	AD7799_SetChannel(AD7799_CH_AIN3P_AIN3M);
	if(eMode ==  EN_HGB_TEST)
	{
		// get HGB adc data
		printf("HGB_Test_Exec Start\r\n");
#if SIMUATION_TEST
		srand(IT_SYS_GetTicks());
		nVal = rand()%5000;
		printf("\r\nHGB_V: %d=0d\r\n", nVal);
#else				
		for(i = 0; i < HGB_CALIBRATE_DATA_NUM; i++)
		{
//			if(Get_Micro_OC_Status() == EN_OPEN){ // cuvette out
//				printf(" cuvette out error\r\n");
//				collect_return_hdl(COLLECT_RET_FAIL_CUVETTE_OUT);
//				return 0;
//			}
			buffer[i] = HW_Get_ADC_HGB();     
			printf("ADC=%d, V=%6.2f\r\n", (int)buffer[i], (float)buffer[i]*ADC_V_REF_VALUE_5/ADC_RESOLUTION_24);
			IT_SYS_DlyMs(100);
		}
#endif
		// send HGB data
		//Send_Data_HGB(CMD_DATA_TEST_HGB, &nVal, 1);
		Send_Data_HGB(CMD_DATA_TEST_HGB, buffer, HGB_CALIBRATE_DATA_NUM);
		collect_return_hdl(COLLECT_RET_SUCESS);
		Beep(5, 300);
	
		printf("HGB_Test_Exec End\r\n");

	}else if(eMode == EN_HGB_CALIBRATE){
		printf("HGB Calibrate\r\n");
		for(i = 0; i < HGB_CALIBRATE_DATA_NUM; i++)
		{
			buffer[i] = HW_Get_ADC_HGB();
			nSum += buffer[i];
			printf("HGB ADC=%d, 5V=%6.2f\r\n", (int)buffer[i], (float)buffer[i]*ADC_V_REF_VALUE_5/ADC_RESOLUTION_24);
			IT_SYS_DlyMs(100);
		}
		printf("AVG HGB ADC=%08d, 5V=%6.2f\r\n", (int)nSum/10, (float)((nSum/10)*ADC_V_REF_VALUE_5/ADC_RESOLUTION_24));
		// send HGB data
		Send_Data_HGB(CMD_DATA_CALIBRATE_HGB, buffer, HGB_CALIBRATE_DATA_NUM);
	}
	// enable LED ADC Channel
//	HW_EN_ADC_HGB(e_False);
	// close LED
//	HW_LED_Off(LED_HGB_INDEX);
	// send finished flag
//	 collect_return_hdl(COLLECT_RET_SUCESS);
	
	return 0;
}



UINT8 CRP_Test_Exec(eTestMode eMode)
{
	UINT16 i;
	UINT32 buffer[CRP_CALIBRATE_DATA_NUM] = {0};
	UINT32 nCurTicks, nTempTicks, nSum = 0;
//	printf("CRP_Test_Exec Start\r\n");
	// check postion 
//    if (E_AXIS_POS_HOME != HW_LEVEL_GetOC(OC_AXIS_POS_INDEX))
//    {
//		printf("Count Error: MT x not at home position\r\n");
//        collect_return_hdl(COLLECT_RET_FAIL_NONE_HOME);  /* 未执行进仓操作 */
//        return e_Feedback_Error;
//    }
//#if 0
//	// check CRP current
//	if(Get_V_CRP_LED() < CRP_MIX_THRESHOLD_V)
//	{
//		printf("Count Error: CRP V is wrong\r\n");
//        collect_return_hdl(COLLECT_RET_FAIL_CRP_V);  /* 未执行进仓操作 */
//        return e_Feedback_Error;
//	}
//#endif
	// enable CRP adc channel
	// open LED
//	HW_LED_On(LED_CRP_INDEX);
	// enable LED ADC Channel
//	HW_EN_ADC_CRP(e_True);
//	IT_SYS_DlyMs(50);
	// get CRP adc data
//	printf("CRP_Test_Exec:");
	
	AD7799_SetChannel(AD7799_CH_AIN2P_AIN2M);
	if(eMode ==  EN_CRP_TEST)
	{
		printf("CRP_Test_Exec Start\r\n");
#if SIMUATION_TEST
		memset((void*)&g_CRP_Data, 0, sizeof(g_CRP_Data));
		g_CRP_Data.eEnable = e_False;
		srand(IT_SYS_GetTicks());
		for(i = 0; i < 5; i++ )
		{
			for(j = 0; j < DATA_FRAME_NUM_4BYTE; j++)
			{
				g_CRP_Data.crpBuffer[j] = rand()%ADC_RESOLUTION_24;
			}
			Send_Data_CRP(CMD_DATA_TEST_CRP, &g_CRP_Data.crpBuffer[0], DATA_FRAME_NUM_4BYTE);
		}		
#else
		memset((void*)&g_CRP_Data, 0, sizeof(struct CRP_DataType));
		g_CRP_Data.eEnable = 0x55;
		IT_SYS_SetTicks(0);
		while(g_CRP_Data.nTotal < g_Record_Param.nTotal_Num)
		{
//			if(Get_Micro_OC_Status() == EN_OPEN){ // cuvette out
//				collect_return_hdl(COLLECT_RET_FAIL_CUVETTE_OUT);
//			}
			if(g_CRP_Data.eSend == e_True)
			{
				if((g_CRP_Data.nTotal/DATA_FRAME_NUM_4BYTE)%2 == 1) //crpBuffer[0-511]
				{
					printf("Send one Frame: ticks=%d,total=%d, index=%d\r\n", (int)IT_SYS_GetTicks(), g_CRP_Data.nTotal, g_CRP_Data.nIndex);
					Send_Data_CRP(CMD_DATA_TEST_CRP, &g_CRP_Data.crpBuffer[0], DATA_FRAME_NUM_4BYTE);
					memset((void*)&g_CRP_Data.crpBuffer[0], 0, DATA_FRAME_NUM_4BYTE*4);
				}else if((g_CRP_Data.nTotal/DATA_FRAME_NUM_4BYTE)%2 == 0){ //crpBuffer[512-1024]
					printf("Send one Frame: ticks=%d,total=%d, index=%d\r\n", (int)IT_SYS_GetTicks(), g_CRP_Data.nTotal, g_CRP_Data.nIndex);
					Send_Data_CRP(CMD_DATA_TEST_CRP, &g_CRP_Data.crpBuffer[DATA_FRAME_NUM_4BYTE], DATA_FRAME_NUM_4BYTE);
					memset((void*)&g_CRP_Data.crpBuffer[DATA_FRAME_NUM_4BYTE], 0, DATA_FRAME_NUM_4BYTE*4);
				}
				g_CRP_Data.eSend = e_False;
				//AD7799_Reset();
				//printf("send end\r\n");
			}
			IT_SYS_DlyMs(5);
		}
		if(g_CRP_Data.nTotal == INVAIL_VALUE) // check bi se ming error
		{
			memset((void*)&g_CRP_Data, 0, sizeof(struct CRP_DataType));
			printf(" cuvette out error\r\n");
			collect_return_hdl(COLLECT_RET_FAIL_CUVETTE_OUT);
			return 0;
		}
		// send the last data
		if(g_CRP_Data.nIndex != 0)
		{
			if((g_CRP_Data.nTotal/DATA_FRAME_NUM_4BYTE)%2 == 0) //crpBuffer[0-511]
			{
				Send_Data_CRP(CMD_DATA_TEST_CRP, &g_CRP_Data.crpBuffer[0], g_CRP_Data.nIndex);
				printf("Send one Frame: ticks=%d,total=%d, index=%d", (int)IT_SYS_GetTicks(), g_CRP_Data.nTotal, g_CRP_Data.nIndex);
			}else{ //crpBuffer[512-1024]
				Send_Data_CRP(CMD_DATA_TEST_CRP, &g_CRP_Data.crpBuffer[DATA_FRAME_NUM_4BYTE], g_CRP_Data.nIndex);
				printf("Send one Frame: ticks=%d,total=%d, index=%d", (int)IT_SYS_GetTicks(), g_CRP_Data.nTotal, g_CRP_Data.nIndex);
			}
		}
#endif	
		memset((void*)&g_CRP_Data, 0, sizeof(g_CRP_Data));
		g_CRP_Data.eEnable = e_False;	
		collect_return_hdl(COLLECT_RET_SUCESS);
		Beep(5, 300);
		printf("CRP_Test_Exec End\r\n");
	}else if(eMode == EN_CRP_CALIBRATE){
		printf("CRP Calibrate Start\r\n");
		// send crp calibrate start msg to app
		Msg_Return_Handle_0(e_Msg_Status, CMD_STATUS_CRP_TEST_START);
		// check micro switch status
		if(Get_Micro_OC_Status() == EN_OPEN) // cuvette out 
		{
			printf(" cuvette out error\r\n");
			collect_return_hdl(COLLECT_RET_FAIL_CUVETTE_OUT);
			return 0;
		}
		// mixing
		nCurTicks = IT_SYS_GetTicks();
		nTempTicks = nCurTicks;
		Mixing_Motor_Run();
		while(nCurTicks < nTempTicks + MIXING_OVER_TIME){ // 2s
			if(Get_Micro_OC_Status() == EN_OPEN) // cuvette out 
			{
				Mixing_Motor_Stop();
				printf(" cuvette out error\r\n");
				collect_return_hdl(COLLECT_RET_FAIL_CUVETTE_OUT);
				return 0;
			}
			IT_SYS_DlyMs(10);
			nCurTicks = IT_SYS_GetTicks();
		}
		Mixing_Motor_Stop();
		// get data
		IT_SYS_DlyMs(500);
		for(i = 0; i < CRP_CALIBRATE_DATA_NUM; i++)
		{
			if(Get_Micro_OC_Status() == EN_OPEN){ // cuvette out
				printf(" cuvette out error\r\n");
				collect_return_hdl(COLLECT_RET_FAIL_CUVETTE_OUT);
				return 0;
			}
			buffer[i] = HW_Get_ADC_CRP();
			nSum += buffer[i];
			printf("CRP ADC=%08d, 5V=%6.2f\r\n", (int)buffer[i], (float)(buffer[i])*ADC_V_REF_VALUE_5/ADC_RESOLUTION_24);
			IT_SYS_DlyMs(100);
		}
		printf("AVG CRP ADC=%08d, 5V=%6.2f\r\n", (int)nSum/10, (float)((nSum/10)*ADC_V_REF_VALUE_5/ADC_RESOLUTION_24));
		// send CRP data
		Send_Data_CRP(CMD_DATA_CALIBRATE_CRP, buffer, CRP_CALIBRATE_DATA_NUM);	
		collect_return_hdl(COLLECT_RET_SUCESS);
		printf("CRP Calibrate End\r\n");
	}
	
	// enable LED ADC Channel
//	HW_EN_ADC_CRP(e_False);
	// close LED
//	HW_LED_Off(LED_CRP_INDEX);
	// send finished flag
//	collect_return_hdl(COLLECT_RET_SUCESS);
	//printf("CRP_Test_Exec End\r\n");
	return 0;
}

void Simulation_Data(UINT8 *pDInfo, UINT16 *pDILen,eTestMode eMode)
{
	UINT16 nDILen;
	UINT32 nCurTicks, nTempTicks, i, j;
    nCurTicks = IT_SYS_GetTicks();
	nTempTicks = nCurTicks;
	IT_ADC_SetTicks(0);
	IT_LIST_SetTicks(4500);
	
	Reset_Udp_Count(0);
	for(i = 0; i < 40000; i++)
	{
		//Add_Udp_Count();
		//----------getting data and send ------------------------------
		//HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_SEND_FPGA_DATA);
		Data_Circle_Handle(eMode);
	    //nCurTicks = IT_SYS_GetTicks();
		for(j = 0; j < 4500; j++);
	}	
	collect_return_hdl(COLLECT_RET_SUCESS);
	
	nDILen = 0;
	pDInfo[nDILen++] = 0x44; pDInfo[nDILen++] = 0x53; pDInfo[nDILen++] = 0x57; 
	pDInfo[nDILen++] = 0x44; pDInfo[nDILen++] = 0x30; pDInfo[nDILen++] = 0x00;
	pDInfo[nDILen++] = 0x02; pDInfo[nDILen++] = 0x01;
	pDInfo[nDILen++] = 0x00; pDInfo[nDILen++] = 0x00;
	
	nDILen = 30;
	strncpy((char*)&pDInfo[10],"simulation test!!!\r\n", nDILen);
	pDInfo[8]  = (nDILen - 10) >> 8;
	pDInfo[9] = (nDILen - 10);
	if(e_Feedback_Fail == udp_echoserver_senddata((UINT8 *)pDInfo, nDILen))
	{
		IT_SYS_DlyMs(1);
	}
	printf("simulation msg len: %d\r\n", nDILen);
}



void MSG_DataTesting(UINT8 *pDInfo, UINT16 *pDILen, eTestMode eMode)
{
	IO_ INT8 sTempInfo[100] = {0};
	IO_ UINT16 nDILen = 0;
	IO_ UINT32 nStartTicks = 0, nCurTicks = 0;	
	
	*pDILen = nDILen;
	
	HW_Enable_Data_Channel(eMode);
	nStartTicks = IT_SYS_GetTicks();
	nCurTicks = nStartTicks;
	printf("start\r\n");
	while(nCurTicks < nStartTicks + 8000)
	{
		Data_Circle_Handle(eMode);
		nCurTicks = IT_SYS_GetTicks();	
	}
	HW_Disable_Data_Channel(eMode);
	printf("Data Test: id=%d, sendid=%d, ticks=%d\r\n", (int)ADC1_Status.nID, (int)ADC1_Status.nSendID, (int)(nCurTicks-nStartTicks));
	collect_return_hdl(COLLECT_RET_SUCESS);
	
	nDILen = 0;
	pDInfo[nDILen++] = 0x44; pDInfo[nDILen++] = 0x53; pDInfo[nDILen++] = 0x57; 
	pDInfo[nDILen++] = 0x44; pDInfo[nDILen++] = 0x30; pDInfo[nDILen++] = 0x00;
	pDInfo[nDILen++] = 0x02; pDInfo[nDILen++] = 0x01;
	pDInfo[nDILen++] = 0x00; pDInfo[nDILen++] = 0x00;
	#ifdef DEBUG_INFO_UP_LOAD
		sprintf((char*)sTempInfo, "Data Test: id=%d, sendid=%d\r\n", (int)ADC1_Status.nID, (int)ADC1_Status.nSendID);
		Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
		memset((char*)sTempInfo, 0, 100);
		*pDILen = nDILen;
#endif
	
	memset((void*)&ADC1_Status, 0, sizeof(ADC_Status_InitTypeDef));
}
//
#ifdef DEBUG_INFO_UP_LOAD
UINT8 MSG_TestingFunc(UINT8 *pDInfo, UINT16 *pDILen, eTestMode eMode)
{
#else
UINT8 MSG_TestingFunc(void)
{
#endif
	
    IO_ INT32 nPress = 0;
    IO_ UINT32 nCurTicks = 0, nLstTicks = 0,  nTempTicks = 0, nTempTicks1 = 0;
	IO_ UINT32 nPreTicks1 = 0, nPreTicks = 0, nPreTimeOutTicks = 0, nTestTimeOutTicks = 0;
	IO_ UINT32 nStartTicks = 0, nEndTicks = 0, wbc_v = 0;
	IO_ UINT8 nWBC_Count = 0, nWBC_Val = 0, i = 0, nTemp = 0xFF, flag = 0;
	IO_ _STA_  UINT32 Num = 1;
#ifdef DEBUG_INFO_UP_LOAD
	IO_ INT8 sTempInfo[DEBUG_INFO_TEMP_LEN] = {0};
	IO_ UINT16 nDILen = 0;
	*pDILen = nDILen;
#endif
	
	Reset_Udp_Count(0);
	Reset_ADC_InitDataType();
	nStartTicks = IT_SYS_GetTicks();
	printf("\r\n------------------Start Count: num=%d----------\r\n", (int)Num);
#ifdef DEBUG_INFO_UP_LOAD
	nDILen = 0;
	pDInfo[nDILen++] = 0x44; pDInfo[nDILen++] = 0x53; pDInfo[nDILen++] = 0x57; 
	pDInfo[nDILen++] = 0x44; pDInfo[nDILen++] = 0x30; pDInfo[nDILen++] = 0x00;
	pDInfo[nDILen++] = 0x02; pDInfo[nDILen++] = 0x01;
	pDInfo[nDILen++] = 0x00; pDInfo[nDILen++] = 0x00;
//	sprintf((char*)sTempInfo, "\r\n-----------------Start Count: num=%d----------\r\n", (int)Num);
//	Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
//	memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
#endif
	Num++;

//	// check MT x position
//    if (E_AXIS_POS_HOME != HW_LEVEL_GetOC(OC_AXIS_POS_INDEX))
//    {
//		printf("Count Error: MT x not at home position\r\n");
//#ifdef DEBUG_INFO_UP_LOAD
//		sprintf((char*)sTempInfo,"Count Error: MT x not at home position\r\n");
//		Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
//		memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
//		*pDILen = nDILen;
//#endif
//        collect_return_hdl(COLLECT_RET_FAIL_NONE_HOME);  /* 未执行进仓操作 */
//        return e_Feedback_Error;
//    }
	
	// check the ELECTRODE status
	nTemp = hw_filter_get_electrode(INDEX_ELECTRODE);  // 1
    if (ELECTRODE_WASTE == nTemp)  // 1
    {
        printf("Count Error: the ELECTRODE error = %d(before build press)\r\n", nTemp);
#ifdef DEBUG_INFO_UP_LOAD
		sprintf((char*)sTempInfo, "Count Error: the ELECTRODE error = %d(before build press)\r\n", nTemp);
		Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
		memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
		*pDILen = nDILen;
#endif
		collect_return_hdl(COLLECT_RET_FAIL_ELECTRODE);  /* 采集异常 */
        return e_Feedback_Error;
    }
    //
    HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);     // off
    HW_Valve_Off(EN_VALVE_AIR);  /* all the air way  CKP CHANGED 20171219*/
    HW_Valve_Off(EN_VALVE_LIQUID);   // WBC

    // 1. build the pressure
	nPress = 0;
	nPress =  Get_Press_Value(GET_PRESS_NUM_FIVE);
    nCurTicks = IT_SYS_GetTicks();  /* 获取系统时钟 */
	printf("Count Status: before build press, ticks=%05d, press=%010d\r\n", \
						(int)(nCurTicks - nStartTicks), (int)nPress);
#ifdef DEBUG_INFO_UP_LOAD
	sprintf((char*)sTempInfo, "Count Status: before build press, ticks=%05d, press=%010d\r\n", \
						(int)(nCurTicks - nStartTicks), (int)nPress);
	Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
	memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
	*pDILen = nDILen;
#endif
    if (nPress < PRESS_BUILD)
    {
        HW_PUMP_Pulse(PUMP_PRESS_FREQ, e_Dir_Pos);  // on, 25000 ticks per ms
        HW_Valve_On(EN_VALVE_AIR);    // all the air way
        HW_Valve_Off(EN_VALVE_LIQUID);    // WBC
        //detect the press 1 and press 2 all the time
        nCurTicks = IT_SYS_GetTicks();
        nTempTicks = nCurTicks;
		nLstTicks = nCurTicks;
        while ((nCurTicks - nTempTicks) <= TIME_OVER_TS_BUILD_PRESS) // 15000ms
        {
            nCurTicks = IT_SYS_GetTicks();
            if (nCurTicks - nLstTicks >= 100)    // 10 ms
            {
				nLstTicks = nCurTicks;
				nPress = 0;
				nPress =  Get_Press_Value(GET_PRESS_NUM_FIVE);
                printf("Count Status(building press): ticks=%05d, press=%010d\r\n", \
							(int)(nCurTicks - nStartTicks), (int)nPress);
                //
#ifdef DEBUG_INFO_UP_LOAD
	// this process not need up load
#endif
                if (nPress >= PRESS_BUILD)
                {
                    break;
                }
            }
		   //HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_DROP_FPGA_DATA);
        }
        // turn off
        HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);    // off
        HW_Valve_Off(EN_VALVE_AIR); // all the air way
        HW_Valve_Off(EN_VALVE_LIQUID);  // WBC
		printf("Count Status(after build press): ticks=%05d, press=%010d\r\n", \
							(int)(nCurTicks - nTempTicks), (int)nPress);
        //-----------
#ifdef DEBUG_INFO_UP_LOAD
		sprintf((char*)sTempInfo, "Count Status(after build press): after build press, ticks=%08d, press=%010d\r\n", \
						(int)(nCurTicks - nTempTicks), (int)nPress);
		Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
		memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
		*pDILen = nDILen;
#endif
        // to check the preeure
        //nPress = HW_ADC_SpiGetPress();
        if (nCurTicks > (nLstTicks + TIME_OVER_TS_BUILD_PRESS))  // 15000ms
        {
            nCurTicks = IT_SYS_GetTicks();
            printf("Count Error: build press timeout,ticks=%05d, press=%010d\r\n", \
						(int)(nCurTicks -  nTempTicks), (int)nPress);
            
#ifdef DEBUG_INFO_UP_LOAD
			sprintf((char*)sTempInfo, "Count Error: build press timeout,ticks=%05d, press=%010d\r\n", \
						(int)(nCurTicks -  nTempTicks), (int)nPress);
			Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
			memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
			*pDILen = nDILen;
#endif
            collect_return_hdl(COLLECT_RET_FAIL_PUMP);  /* 采集异常 */
            return e_Feedback_Error;     
        }
    }
    else
    {
        // do nothing
        nCurTicks = IT_SYS_GetTicks();
        printf("Count Status(press enough): ticks=%05d, press=%010d\r\n",\
				(int)(nCurTicks - nTempTicks), (int)nPress);
#ifdef DEBUG_INFO_UP_LOAD
		sprintf((char*)sTempInfo, "Count Status(press enough): ticks=%05d, press=%010d\r\n",\
				(int)(nCurTicks - nTempTicks), (int)nPress);
		Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
		memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
		*pDILen = nDILen;
#endif
    }

    //==================
    // 2. check the ELECTRODE
	nTemp = hw_filter_get_electrode(INDEX_ELECTRODE);
    if (ELECTRODE_WASTE == nTemp)  // 1
    {
        //*****************************
        // Exception 2
        //*****************************
        nCurTicks = IT_SYS_GetTicks();
        printf("Count Error: the ELECTRODE=%d error (after build press)\r\n", nTemp);
#ifdef DEBUG_INFO_UP_LOAD
		sprintf((char*)sTempInfo, "Count Error: the ELECTRODE=%d error(after build press)\r\n", nTemp);
		Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
		memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
		*pDILen = nDILen;
#endif
        collect_return_hdl(COLLECT_RET_FAIL_ELECTRODE);  /* 采集异常 */
        return e_Feedback_Error;
    }

	// first
    // 3. 数据预采集
	if(eMode == EN_WBC_TEST)
	{
		nPreTimeOutTicks = TIME_TS_ACTION_PRE_TIME_1; // 4500ms
		
	}else{
		nPreTimeOutTicks = TIME_TS_ACTION_PRE_TIME_2; // 7000ms
	}
	
    HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);     // off
    HW_Valve_Off(EN_VALVE_AIR);  // all the air way
    HW_Valve_On(EN_VALVE_LIQUID);    // WBC

    // reset the LIST-TICK
    IT_LIST_SetTicks(0);            /* 先清零本次测试流程的计时器 */
    nCurTicks = IT_SYS_GetTicks();
    //nLstTicks = nCurTicks;
	nTempTicks = nCurTicks;  
	nPreTicks = 0;
	printf("Count Status(before %d.%dS): ticks=%05d, preTicks=%05d, press=%010d, udp=%d, wbc_v=%d\r\n", (int)(nPreTimeOutTicks/1000), (int)(nPreTimeOutTicks%1000/100),\
					(int)IT_LIST_GetTicks(), (int)nPreTicks, (int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)Get_Udp_Count(), (int)Get_XK_V_Value());
#ifdef DEBUG_INFO_UP_LOAD
	sprintf((char*)sTempInfo, "Count Status(before %d.%dS): ticks=%05d, preTicks=%05d,press=%010d, udp=%d, wbc_v=%d\r\n", (int)(nPreTimeOutTicks/1000), (int)(nPreTimeOutTicks%1000/100),\
					(int)IT_LIST_GetTicks(),(int)nPreTicks, (int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)Get_Udp_Count(), (int)Get_XK_V_Value());
	Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
	memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
	*pDILen = nDILen;
#endif
	//------------pre-handle 4.5s---------
	IT_LIST_SetTicks(0);            /* 先清零本次测试流程的计时器 */
	nCurTicks = IT_SYS_GetTicks();
	nTempTicks = nCurTicks;
	nTempTicks1 = nCurTicks;
	nPreTicks = 0; 
	flag = 0;
	
	printf("WBC_V: ");
#ifdef DEBUG_INFO_UP_LOAD
	sprintf((char*)sTempInfo,"WBC_V: ");
	Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
	*pDILen = nDILen;
#endif

	while((nCurTicks - nTempTicks) <= nPreTimeOutTicks) 
    {
		if((nCurTicks - nTempTicks1) >= 500) // 1ms per time
		{
			nTempTicks1 = nCurTicks;
			wbc_v = Get_XK_V_Value();
			printf("%d,", (int)wbc_v);
#ifdef DEBUG_INFO_UP_LOAD
			if((nTempTicks1 + 200) > nTempTicks + nPreTimeOutTicks){
				sprintf((char*)sTempInfo,"%d", (int)wbc_v);
			}else{
				sprintf((char*)sTempInfo,"%d,", (int)wbc_v);
			}
			Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
			memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
			*pDILen = nDILen;
#endif			
		}	

        // to check the ELECTRODE
		nTemp = hw_filter_get_electrode(INDEX_ELECTRODE);
        if (ELECTRODE_WASTE == nTemp)
        {
			printf("\r\nCount Error: the ELECTRODE=%d error(%d.%dS), ticks=%05d, press=%010d, udp=%d, q=%d\r\n", nTemp, (int)(nPreTimeOutTicks/1000), (int)(nPreTimeOutTicks%1000/100),\
									(int)IT_LIST_GetTicks(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)Get_Udp_Count(), (int)g_Frame_Count);
#ifdef DEBUG_INFO_UP_LOAD
			sprintf((char*)sTempInfo, "\r\nCount Error: the ELECTRODE=%d error(%d.%dS), ticks=%05d, press=%010d, udp=%d, q=%d\r\n", nTemp, (int)(nPreTimeOutTicks/1000), (int)(nPreTimeOutTicks%1000/100),\
										(int)IT_LIST_GetTicks(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)Get_Udp_Count(), (int)g_Frame_Count);
			Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
			memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
			*pDILen = nDILen;
#endif
            collect_return_hdl(COLLECT_RET_FAIL_ELECTRODE);  /* 采集异常 */
            return e_Feedback_Error;
        }
		// to check press
		nPress = 0;
		nPress =  Get_Press_Value(GET_PRESS_NUM_FIVE);
		if(nPress <= COUNT_MIN_PRESS)
		{
			printf("\r\nCount Error: the press error(%d.%dS),ticks=%05d, press=%010d, udp=%d, q=%d\r\n", (int)(nPreTimeOutTicks/1000), (int)(nPreTimeOutTicks%1000/100),\
								(int)IT_LIST_GetTicks(), (int)nPress, (int)Get_Udp_Count(), (int)g_Frame_Count);
#ifdef DEBUG_INFO_UP_LOAD
			sprintf((char*)sTempInfo, "\r\nCount Error: the press error(%d.%dS), ticks=%05d, press=%010d, udp=%d, q=%d\r\n", (int)(nPreTimeOutTicks/1000), (int)(nPreTimeOutTicks%1000/100),\
												(int)IT_LIST_GetTicks(), (int)nPress, (int)Get_Udp_Count(), (int)g_Frame_Count);
			Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
			memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
			*pDILen = nDILen;
#endif
            collect_return_hdl(COLLECT_RET_FAIL_AIR_COKE);
            return e_Feedback_Error;
		}
		//HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_DROP_FPGA_DATA);
		nCurTicks = IT_SYS_GetTicks();
		nPreTicks = nCurTicks - nTempTicks;
//		if(nPreTicks <= 1500 && flag == 0){ // 0-1500ms
//			if(EN_WBC_V_LOW == Get_WBC_V_Status(COUNT_WBC_XK_CHECK_V)){ // wbc_v lower than COUNT_WBC_XK_CHECK_V(1.5v)
//				flag = 1;
//				printf("\r\nCount Error: (0-1.5s)xiao_kong error, ticks=%08d, preticks=%08d, press=%010d, udp=%d, preticks=%d, wbc_v=%d\r\n", \
//						(int)IT_LIST_GetTicks(), (int)nPreTicks, (int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)Get_Udp_Count(), (int)nPreTicks, (int)Get_XK_V_Value());
//#ifdef DEBUG_INFO_UP_LOAD
//				sprintf((char*)sTempInfo, "\r\nCount Error: (0-1.5s)xiao_kong error: ticks=%08d, preticks=%08d, press=%010d, udp=%d, preticks=%d,wbc_v=%d\r\n", \
//							(int)IT_LIST_GetTicks(), (int)nPreTicks,(int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)Get_Udp_Count(), (int)nPreTicks, (int)Get_XK_V_Value());
//				Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
//				memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
//				*pDILen = nDILen;
//#endif			
//////				collect_return_hdl(COLLECT_RET_FAIL_WBC_BSK);
//////				return e_Feedback_Error;
//			}
//		}
    }
	printf("\r\nCount Status(after %d.%dS): ticks=%05d, preTicks=%05d, press=%010d, udp=%d, preticks=%d, wbc_v=%d\r\n", (int)(nPreTimeOutTicks/1000), (int)(nPreTimeOutTicks%1000/100),\
						(int)IT_LIST_GetTicks(), (int)nPreTicks, (int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)Get_Udp_Count(), (int)nPreTicks, (int)Get_XK_V_Value());
#ifdef DEBUG_INFO_UP_LOAD
	sprintf((char*)sTempInfo, "\r\nCount Status(after %d.%dS): ticks=%05d, preTicks=%05d,press=%010d, udp=%d, preticks=%d,wbc_v=%d\r\n", (int)nPreTimeOutTicks/1000, (int)nPreTimeOutTicks%1000/100,\
							(int)IT_LIST_GetTicks(), (int)nPreTicks,(int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)Get_Udp_Count(), (int)nPreTicks, (int)Get_XK_V_Value());
	Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
	memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
	*pDILen = nDILen;
#endif

	// -----check wbc elec touch before count----------
	if(EN_WBC_V_LOW != Get_WBC_V_Status(COUNT_WBC_START_V)) // the wbc_v not changed
	{
		printf("Count Error: WBC Elec Touch Error, ticks=%05d, udp=%d, q=%d, elec=%d, wbc_v=%d, press=%09d\r\n",\
		(int)IT_LIST_GetTicks(), (int)Get_Udp_Count(), (int)g_Frame_Count,\
		(int)hw_filter_get_electrode(INDEX_ELECTRODE),(int)Get_XK_V_Value(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE));
#ifdef DEBUG_INFO_UP_LOAD	
		sprintf((char*)sTempInfo, "Count Error: WBC Elec Touch Error, wbc_v error, ticks=%05d, udp=%d, q=%d, elec=%d, wbc_v=%d, press=%09d\r\n",\
		(int)IT_LIST_GetTicks(), (int)Get_Udp_Count(), (int)g_Frame_Count, \
		(int)hw_filter_get_electrode(INDEX_ELECTRODE),(int)Get_XK_V_Value(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE));
		Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
		*pDILen = nDILen;
#endif
//		collect_return_hdl(COLLECT_RET_FAIL_WBC_TOUCH);
//		HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_DROP_FPGA_DATA);
//		return e_Feedback_Error;	
	}
	// -----check wbc elec v before count-----------
//	if(EN_WBC_V_LOW != Get_WBC_V_Status(COUNT_WBC_TOUCH_CHECK_V)) // the wbc_v is not low than COUNT_WBC_MIN_V(1.8v)
//	{
//		printf("Count Error: WBC V Error, ticks=%05d, udp=%d, q=%d, elec=%d, wbc_v=%d, press=%09d\r\n",\
//		(int)IT_LIST_GetTicks(), (int)Get_Udp_Count(), (int)g_Frame_Count,\
//		(int)hw_filter_get_electrode(INDEX_ELECTRODE),(int)Get_XK_V_Value(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE));
//#ifdef DEBUG_INFO_UP_LOAD	
//		sprintf((char*)sTempInfo, "Count Error: WBC V Error, ticks=%05d, udp=%d, q=%d, elec=%d, wbc_v=%d, press=%09d\r\n",\
//		(int)IT_LIST_GetTicks(), (int)Get_Udp_Count(), (int)g_Frame_Count,\
//		(int)hw_filter_get_electrode(INDEX_ELECTRODE),(int)Get_XK_V_Value(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE));
//		Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
//		*pDILen = nDILen;
//#endif
///////		collect_return_hdl(COLLECT_RET_FAIL_WBC_ELECTRODE);
///////		HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_DROP_FPGA_DATA);
////		return e_Feedback_Error;
//	}

	//-------second 3s stop-----------
    HW_Valve_Off(EN_VALVE_LIQUID);  /* 关阀 */
    nCurTicks = IT_SYS_GetTicks();
    nTempTicks = nCurTicks;
	printf("Count Status(before 3S): ticks=%05d, press=%010d, udp=%d, wbc_v=%d\r\n", \
				(int)IT_LIST_GetTicks(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)Get_Udp_Count(), (int)Get_XK_V_Value());
#ifdef DEBUG_INFO_UP_LOAD
	sprintf((char*)sTempInfo, "Count Status((before 3S)): ticks=%05d, press=%010d, udp=%d, wbc_v=%d\r\n", \
				(int)IT_LIST_GetTicks(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)Get_Udp_Count(), (int)Get_XK_V_Value());
	Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
	memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
	*pDILen = nDILen;
#endif

	while((nCurTicks - nTempTicks) <= 3000)
    {
        nCurTicks = IT_SYS_GetTicks();
//		IT_SYS_DlyMs(1);
		//HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_DROP_FPGA_DATA);
        // to check the ELECTRODE
		nTemp = hw_filter_get_electrode(INDEX_ELECTRODE);
        if (ELECTRODE_WASTE == nTemp)  /* 流程正常结束 */
		{
			printf("Count Error: the ELECTRODE=%d tigger(3S stop), ticks=%05d, press=%010d, udp=%d, wbc_v=%d\r\n", nTemp,\
							(int)IT_LIST_GetTicks(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)Get_Udp_Count(), (int)Get_XK_V_Value());
#ifdef DEBUG_INFO_UP_LOAD
			sprintf((char*)sTempInfo, "Count Error: the ELECTRODE=%d tigger(3S stop), ticks=%05d, press=%010d, udp=%d, wbc_v=%d\r\n", nTemp,\
								(int)IT_LIST_GetTicks(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)Get_Udp_Count(), (int)Get_XK_V_Value());
			Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
			memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
			*pDILen = nDILen;
#endif
            break;
        }
		// to check press
		nPress = 0;
		nPress =  Get_Press_Value(GET_PRESS_NUM_FIVE);
		if(nPress <= COUNT_MIN_PRESS)
		{
			printf("Count Error: the press error(3S stop), ticks=%05d, press=%010d, udp=%d, wbc_v=%d\r\n", \
							(int)IT_LIST_GetTicks(), (int)nPress, (int)Get_Udp_Count(), (int)Get_XK_V_Value());
#ifdef DEBUG_INFO_UP_LOAD
			sprintf((char*)sTempInfo, "Count Error: the press error(3S stop), ticks=%05d, press=%010d, udp=%d, wbc_v=%d\r\n", \
								(int)IT_LIST_GetTicks(), (int)nPress, (int)Get_Udp_Count(), (int)Get_XK_V_Value());
			Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
			memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
			*pDILen = nDILen;
#endif
            collect_return_hdl(COLLECT_RET_FAIL_AIR_COKE);
            return e_Feedback_Error;
		}
		//HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_DROP_FPGA_DATA);
    }
	printf("Count Status(after 3S): ticks=%05d, press=%010d, udp=%d, wbc_v=%d\r\n", \
				(int)IT_LIST_GetTicks(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)Get_Udp_Count(), (int)Get_XK_V_Value());
#ifdef DEBUG_INFO_UP_LOAD
	sprintf((char*)sTempInfo, "Count Status(after 3S): ticks=%05d, press=%010d, udp=%d, wbc_v=%d\r\n", \
							(int)IT_LIST_GetTicks(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)Get_Udp_Count(), (int)Get_XK_V_Value());
	Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
	memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
	*pDILen = nDILen;
#endif
	
	//-------third, counting start-------
	if(eMode == EN_WBC_TEST)
	{
		nTestTimeOutTicks = TIME_TS_ACTION_TIMEOUT_1; // 24000ms
		
	}else{
		nTestTimeOutTicks = TIME_TS_ACTION_TIMEOUT_2; // 34000ms
	}
	IT_ADC_SetTicks(0); 
	IT_LIST_SetTicks(nPreTicks);
	g_Frame_Count = 0;
	printf("Count Status(before count): ticks=%05d, adc_ticks=%05d, press=%010d, udp=%d, q=%d, f=%d, wbc_v=%d, maxt=%d\r\nWBC_V: ", \
		(int)IT_LIST_GetTicks(), (int)IT_ADC_GetTicks(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)Get_Udp_Count(), (int)g_Frame_Count, (int)g_Send_Fail, (int)Get_XK_V_Value(), (int)nTestTimeOutTicks);
#ifdef DEBUG_INFO_UP_LOAD
		sprintf((char*)sTempInfo, "Count Status(before count): ticks=%05d, adc_ticks=%05d, press=%010d, udp=%d, q=%d, f=%d, wbc_v=%d, maxt=%d\r\nWBC_V: ", \
		(int)IT_LIST_GetTicks(), (int)IT_ADC_GetTicks(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)Get_Udp_Count(), (int)g_Frame_Count, (int)g_Send_Fail, (int)Get_XK_V_Value(),(int)nTestTimeOutTicks);
	Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
	memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
	*pDILen = nDILen;
#endif
	HW_Valve_On(EN_VALVE_LIQUID);  /* 开阀准备检测 */
//    HW_SW_AdcWBC(e_True);          /* switch on : WBC and RBC */
//    HW_SW_AdcRBC(e_False);
//    HW_EN_WBC(e_True);             /* get the ADC data */
    IT_ADC_SetTicks(0);            /* reset the ADC-TICK */
    //IT_LIST_SetTicks(IT_LIST_GetTicks() - (TIME_TS_ACTION_ON - TIME_TS_ACTION_OFF)); /* 根据上位机算法要求，统计开阀门的实际时间 */
	IT_LIST_SetTicks(nPreTicks);
    nCurTicks = IT_SYS_GetTicks();
    nLstTicks = nCurTicks;
	nTempTicks = nCurTicks;
	nTempTicks1 = nCurTicks;
	// ---------open get data switch--------
	HW_Valve_On(EN_VALVE_LIQUID);  /* 开阀准备检测 */
	//HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_DROP_FPGA_DATA);
    HW_Enable_Data_Channel(eMode);//HW_Start_WBC(); // todo...
	while (nCurTicks <= (nLstTicks + nTestTimeOutTicks)) //24s
    {
		//----------getting data and send ------------------------------
		//HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_SEND_FPGA_DATA);
		Data_Circle_Handle(eMode);
		//------get wbc data every 500ms------
//		if((nCurTicks - nTempTicks1) >= 1000) // 1ms per time
//		{
//			wbc_v = Get_XK_V_Value();
//			printf("%d,", (int)wbc_v);
//#ifdef DEBUG_INFO_UP_LOAD
//			sprintf((char*)sTempInfo,"%d,", (int)wbc_v);
//			Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
//			*pDILen = nDILen;
//#endif
//			nTempTicks1 = nCurTicks;
//		}	
//		
		//------to check the ELECTRODE------
		nTemp = hw_filter_get_electrode(INDEX_ELECTRODE);
        if (ELECTRODE_WASTE == nTemp)  /* 流程正常结束 */
        {
			HW_Disable_Data_Channel(eMode);//HW_End_WBC();
			//Send_Last_FIFO_Data();
			printf("\r\nCount Status: Success, ticks=%05d, adc_ticks=%05d, udp=%d, q=%d, elec=%d, wbc_v=%d, press=%010d\r\n",\
				(int)IT_LIST_GetTicks(), (int)IT_ADC_GetTicks(), (int)Get_Udp_Count(), (int)g_Frame_Count,\
				 nTemp,(int)Get_XK_V_Value(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE));
#ifdef DEBUG_INFO_UP_LOAD
	    	sprintf((char*)sTempInfo, "\r\nCount Status: Success, ticks=%05d, adc_ticks=%05d, udp=%d, q=%d, elec=%d, wbc_v=%d, press=%010d\r\n",\
				(int)IT_LIST_GetTicks(), (int)IT_ADC_GetTicks(), (int)Get_Udp_Count(), (int)g_Frame_Count,\
				nTemp,(int)Get_XK_V_Value(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE));
			Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
			memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
			*pDILen = nDILen;
#endif		
            break;
        }
		//------to check press------
//		nPress = 0;
//		nPress =  Get_Press_Value(GET_PRESS_NUM_THREE);
//		if(nPress <= COUNT_MIN_PRESS)
//		{
//			HW_Disable_Data_Channel(eMode);//HW_End_WBC();
//			Send_Last_FIFO_Data();	
//			printf("\r\nCount Error: press error, ticks=%05d, adc_ticks=%05%, udp=%d, q=%d, elec=%d, wbc_v=%d, press=%010d\r\n",\
//				(int)IT_LIST_GetTicks(), (int)IT_ADC_GetTicks(), (int)Get_Udp_Count(), (int)g_Frame_Count,\
//				(int)hw_filter_get_electrode(INDEX_ELECTRODE),(int)Get_XK_V_Value(), (int)nPress);
//#ifdef DEBUG_INFO_UP_LOAD		
//			sprintf((char*)sTempInfo, "\r\nCount Error: press error, ticks=%05d, adc_ticks=%05d, udp=%d, q=%d, elec=%d, wbc_v=%d, press=%010d\r\n",\
//				(int)IT_LIST_GetTicks(), (int)IT_ADC_GetTicks(), (int)Get_Udp_Count(), (int)g_Frame_Count,\
//				(int)hw_filter_get_electrode(INDEX_ELECTRODE),(int)Get_XK_V_Value(), (int)nPress);
//			Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
//			memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
//			*pDILen = nDILen;
//#endif
//			printf("------------------End Count: num=%d, udp=%d, q=%d, f=%d, ticks=%05d---nDILen=%d-------\r\n", (int)(Num -1), (int)Get_Udp_Count(), (int)g_Frame_Count, (int)g_Send_Fail, (int)IT_LIST_GetTicks(), nDILen);
//			
//			collect_return_hdl(COLLECT_RET_FAIL_AIR_COKE); 
//			return e_Feedback_Error;
//		}
		nCurTicks = IT_SYS_GetTicks();	
    }
	//------after count, stop all--------
	HW_Disable_Data_Channel(eMode);//HW_End_WBC();
	//Send_Last_FIFO_Data();
	HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);     // off
    HW_Valve_Off(EN_VALVE_AIR);  // all the air way
    HW_Valve_Off(EN_VALVE_LIQUID);   // WBC
	
	//-------log info after count------
	nTempTicks1  = IT_LIST_GetTicks();
	nTempTicks = IT_ADC_GetTicks();
	//nTemp = hw_filter_get_electrode(INDEX_ELECTRODE);
	printf("Count Status(after count): ticks=%05d, adc_ticks=%05d, udp=%d, q=%d, f=%d, elec=%d, wbc_v=%d, press=%010d\r\n",\
				(int)nTempTicks1, (int)nTempTicks, (int)Get_Udp_Count(), (int)g_Frame_Count, (int)g_Send_Fail,\
				 nTemp,(int)Get_XK_V_Value(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE));
#ifdef DEBUG_INFO_UP_LOAD
	sprintf((char*)sTempInfo, "Count status(after count): ticks=%05d, adc_ticks=%05d, udp=%d, q=%d, f=%d, elec=%d, wbc_v=%d, press=%010d\r\n",\
				(int)nTempTicks1, (int)nTempTicks, (int)Get_Udp_Count(), (int)g_Frame_Count, (int)g_Send_Fail,\
				 nTemp,(int)Get_XK_V_Value(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE));
	Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
	memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
	*pDILen = nDILen;
#endif

    //-----------timeout -------------- nPreTicks
//    if (nCurTicks > (nLstTicks + (TIME_OVER_TS_ADC - TIME_TS_ACTION_ON)))  /* 超时判断 */
	if (nCurTicks > (nLstTicks + nTestTimeOutTicks)) 
	{
        nCurTicks = IT_SYS_GetTicks();
		printf("Count Error: timeout, ticks=%05d, adc_ticks=%05d, udp=%d, q=%d, f=%d, elec=%d, wbc_v=%d, press=%010d,maxt=%d\r\n",\
					(int)nTempTicks1, (int)nTempTicks, (int)Get_Udp_Count(), (int)g_Frame_Count, (int)g_Send_Fail,\
					(int)hw_filter_get_electrode(INDEX_ELECTRODE),(int)Get_XK_V_Value(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)nTestTimeOutTicks);
#ifdef DEBUG_INFO_UP_LOAD
		sprintf((char*)sTempInfo, "Count Error: timeout, ticks=%05d, adc_ticks=%05d, udp=%d, q=%d, f=%d, elec=%d, wbc_v=%d, press=%010d, maxt=%d\r\n",\
								(int)nTempTicks1, (int)nTempTicks, (int)Get_Udp_Count(),(int)g_Frame_Count, (int)g_Send_Fail,\
								hw_filter_get_electrode(INDEX_ELECTRODE),(int)Get_XK_V_Value(), (int)Get_Press_Value(GET_PRESS_NUM_FIVE), (int)nTestTimeOutTicks);
		Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
		memset((char*)sTempInfo, 0, DEBUG_INFO_TEMP_LEN);
		*pDILen = nDILen;
#endif
        
#ifdef DEBUG_INFO_UP_LOAD
	   sprintf((char*)sTempInfo, "------------------End Count: num=%d, q=%d, ticks=%05d---nDILen=%d-------\r\n", (int)(Num -1), (int)g_Frame_Count, (int)IT_LIST_GetTicks(), nDILen);
	   Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
	   *pDILen = nDILen;
#endif		
	   printf("------------------End Count: num=%d, udp=%d, q=%d, f=%d, ticks=%05d---nDILen=%d-------\r\n", (int)(Num -1), (int)Get_Udp_Count(), (int)g_Frame_Count, (int)g_Send_Fail, (int)IT_LIST_GetTicks(), nDILen);
       collect_return_hdl(COLLECT_RET_FAIL_TIMEOVER);  /* 采集异常 */
	   return e_Feedback_Error;
    }
	
	// ------ count success------------
    
	printf("------------------End Count: num=%d, udp=%d, q=%d, f=%d, ticks=%08d---nDILen=%d-------\r\n", (int)(Num -1), (int)Get_Udp_Count(), (int)g_Frame_Count, (int)g_Send_Fail, (int)IT_LIST_GetTicks(), nDILen);
#ifdef DEBUG_INFO_UP_LOAD
//	sprintf((char*)sTempInfo, "------------------End Count: num=%d, udp=%d, q=%d, ticks=%08d---nDILen=%d-------\r\n", (int)(Num -1), (int)Get_Udp_Count(), (int)g_Frame_Count, (int)IT_LIST_GetTicks(), nDILen);
//	Append_Debug_Info((INT8*)pDInfo+nDILen, (INT8*)sTempInfo, (UINT16*)&nDILen);
//	*pDILen = nDILen;
#endif
	//collect_return_hdl(COLLECT_RET_SUCESS);  /* 采集完成 */
    return e_Feedback_Success;
}


//-----------------------------------------------------------------------------------------
// yaolan_20190220
_EXT_ UINT8 MT_X_IN_Self_Check(CALL_STYLE_E eCall)
{
    UINT8 eStatus;
	UINT32 nStartTicks, nEndTicks;

	nStartTicks = IT_SYS_GetTicks();
    eStatus = MT_X_Home(eCall);
	if(eStatus == e_Feedback_Success)
	{
		nEndTicks = IT_SYS_GetTicks();
		if((nEndTicks - nStartTicks) > MOTO_SELF_CHECK_TIMEOUT) // time out
		{
			Moto_Work_Time(e_Moto_In, e_Feedback_Fail);  
		}else{ // sucess
			Moto_Work_Time(e_Moto_In, e_Feedback_Success);  
			return e_Feedback_Success;
		}
	}else{
		Moto_Work_Time(e_Moto_In, e_Feedback_Fail);  
	}
    return e_Feedback_Fail;
}

// out press
_EXT_ UINT8 MT_X_OUT_Self_Check(CALL_STYLE_E eCall)
{
//	UINT8 buf[30] = {0};
	UINT8 eStatus = 0;
	UINT32 nStartTicks, nEndTicks;
	nStartTicks = IT_SYS_GetTicks();

	eStatus = MT_X_MoveToPosRel(eCall);
	if(eStatus == e_Feedback_Success)
	{
		nEndTicks = IT_SYS_GetTicks();
		printf("Out SelfCheck: t=%08d, s=%d\r\n", (int)(nEndTicks - nStartTicks), eStatus);
		if((nEndTicks - nStartTicks) < MOTO_SELF_CHECK_TIMEOUT) // time out
		{
			Moto_Work_Time(e_Moto_Out, e_Feedback_Success);
			
		}else{ // success
			Moto_Work_Time(e_Moto_Out, e_Feedback_Fail);
			return e_Feedback_Success;
		}
	}else{ // other error
		Moto_Work_Time(e_Moto_Out, e_Feedback_Fail);
	}
	return e_Feedback_Fail;
}


INT32 Build_Press_Self_Check(void)
{
    IO_ INT32 nPress = 0;
    IO_ UINT32 nCurTicks = 0, nStartTicks = 0;
    IO_ UINT32 nLstTicks = 0;
	IO_ UINT32 nTempTicks = 0;
	IO_ UINT8 i;
	
//    if (E_AXIS_X_POS_HOME != MT_X_get_posi())
//    {
//		MT_X_IN_Self_Check(e_SelfCheck_Call);
//        return e_Feedback_Error;
//    }
	
	HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);     // off
	HW_Valve_Off(EN_VALVE_AIR);  /* all the air way  CKP CHANGED 20171219*/
	HW_Valve_Off(EN_VALVE_LIQUID);   // WBC
	
    // 1. build the pressure
    // nPress = HW_ADC_SpiGetPress();  /* 获取泵压 */
	nPress = Get_Press_Value(GET_PRESS_NUM_FIVE);
    nCurTicks = IT_SYS_GetTicks();  /* 获取系统时钟 */
	nStartTicks = nCurTicks;
	printf("Build start: tick=%08d, press=%010d, addpress=%010d\r\n", (int)(nCurTicks - nStartTicks), (int)nPress, (int)g_Record_Param.nAddPress);
    if (nPress < BUILD_PRESS_RIGHT)
    {
		HW_PUMP_Pulse(PUMP_PRESS_FREQ, e_Dir_Pos);     // off
		HW_Valve_On(EN_VALVE_AIR);  /* all the air way  CKP CHANGED 20171219*/
		HW_Valve_Off(EN_VALVE_LIQUID);   // WBC

        // detect the press 1 and press 2 all the time
        nCurTicks = IT_SYS_GetTicks();
        nLstTicks = nCurTicks;
		nTempTicks = nCurTicks;
        while (nCurTicks <= (nLstTicks + TIME_OVER_TS_BUILD_PRESS)) // 
        {
            nCurTicks = IT_SYS_GetTicks();
            if (nCurTicks - nTempTicks >= 10)    // 10 ms
            {
				nTempTicks = nCurTicks;
				nPress = Get_Press_Value(GET_PRESS_NUM_FIVE);
				//Msg_Return_Handle_32(e_Msg_Status, CMD_STATUS_BUILD_PRESS, nPress);
				//
                if (nPress >=  (INT32)BUILD_PRESS_RIGHT /*&& nPress <= BUILD_PRESS_MAX*/)
                {
					HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);     // off
					HW_Valve_Off(EN_VALVE_AIR);  /* all the air way  CKP CHANGED 20171219*/
					HW_Valve_Off(EN_VALVE_LIQUID);   // WBC
                    break;
                }
				printf("Building Press tick=%08d, press=%010d, addpress=%010d\r\n", (int)(nCurTicks - nStartTicks), (int)nPress, (int)g_Record_Param.nAddPress);
            }
            //HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_DROP_FPGA_DATA);
			//HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks());
        }
        // turn off
       
		HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);     // off
		HW_Valve_Off(EN_VALVE_AIR);  /* all the air way  CKP CHANGED 20171219*/
		HW_Valve_Off(EN_VALVE_LIQUID);   // WBC
        //-----------
        // to check the preeure
        //nPress = HW_ADC_SpiGetPress();
        if (nCurTicks > (nLstTicks + TIME_OVER_TS_BUILD_PRESS))  // 
        {
            //*****************************
            // Exception 1
            //*****************************
            //
            nCurTicks = IT_SYS_GetTicks();
            printf("Build Press Fail: tick=%08d, press=%010d, addpress=%010d\r\n", (int)(nCurTicks - nStartTicks), (int)nPress, (int)g_Record_Param.nAddPress);
            //
			Msg_Return_Handle_16(e_Msg_Status, CMD_STATUS_BUILD_PRESS, e_Feedback_Fail);	
            //collect_return_hdl(COLLECT_RET_FAIL_PUMP);  /* 采集异常 */
            return nPress;     
        }
    }
    else
    {
        // do nothing
        nCurTicks = IT_SYS_GetTicks();
        //printf("\r\n OK tick - %0.8d, press - %d", (int)nCurTicks, (int)nPress);
    }
	printf("Build press success: tick=%08d, press=%010d, addpress=%010d\r\n", (int)(nCurTicks - nStartTicks), (int)nPress, (int)g_Record_Param.nAddPress);
	Msg_Return_Handle_16(e_Msg_Status, CMD_STATUS_BUILD_PRESS, e_Feedback_Success);
	return nPress;
}

// exec after Build_Press_Self_Check
UINT8 AirLight_Self_Check(CALL_STYLE_E eCall)
{
//	UINT8 i;
	INT32 nPress, nCurPress;
	UINT32 nCurTicks, nLstTicks, nTempTicks;
	// build press
	nPress = Build_Press_Self_Check();
	IT_SYS_DlyMs(1500);
	nPress = Build_Press_Self_Check();
	IT_SYS_DlyMs(1000);
	
	nPress = 0;
	nPress = Get_Press_Value(GET_PRESS_NUM_FIVE);
	if(nPress < (BUILD_PRESS_RIGHT - BUILD_PRESS_DEVIATION))
	{
		Msg_Return_Handle_16(e_Msg_Status, CMD_STATUS_AIRLIGHT_PRESS, e_Feedback_Fail);
		printf("AirLight fail(Build Press Error): Press=%010d, addpress=%010d\r\n", (int)nPress, (int)g_Record_Param.nAddPress);
		return e_Feedback_Fail;
	}
//	nPress = HW_ADC_SpiGetPress();
    // Status_Return_Handle_32(CMD_STATUS_BUILD_PRESS, nPress);
	
	// compute
	nCurTicks = IT_SYS_GetTicks();
	nLstTicks = nCurTicks;
	nTempTicks = nCurTicks;
	printf("AirLight start: ticks=%08d, press=%010d, addpress=%010d\r\n", (int)nCurTicks, (int)nPress, (int)g_Record_Param.nAddPress);
	if(eCall == e_PartTest_Call) // for part test
	{
		HW_Valve_Off(EN_VALVE_AIR); 
		HW_Valve_On(EN_VALVE_LIQUID); 

	}
	while(nCurTicks <= (nLstTicks + TIME_AIRLIGHT_CHECK))
	{
		nCurTicks = IT_SYS_GetTicks();
		if(nCurTicks - nTempTicks >= 10)
		{
			nTempTicks = nCurTicks;
			nCurPress = 0;
			nCurPress = Get_Press_Value(GET_PRESS_NUM_FIVE);
			if((nCurPress > (nPress + BUILD_PRESS_DEVIATION)) || (nCurPress < (nPress - BUILD_PRESS_DEVIATION)))
			{
				printf("AirLight error: ticks=%08d, nCurPress=%010d, press=%010d, addpress=%010d\r\n", (int)nCurTicks, (int)nCurPress, (int)nPress, (int)g_Record_Param.nAddPress);
				break;
			}
			if(eCall == e_PartTest_Call) // for part test
			{
				if(e_Feedback_Fail == HW_LWIP_Working_Recv_Handle(IT_LIST_GetTicks(), IT_ADC_GetTicks())) break;
			}
		}
	}
	if(eCall == e_PartTest_Call) // for part test
	{
		HW_Valve_Off(EN_VALVE_AIR); 
		HW_Valve_Off(EN_VALVE_LIQUID); 
	}
	/////////////
	//Msg_Return_Handle_32(e_Msg_Status, CMD_STATUS_AIRLIGHT_PRESS, nCurPress);
	//Msg_Return_Handle_32(e_Msg_Status, CMD_STATUS_AIRLIGHT_PRESS, nPress);
	//Msg_Return_Handle_32(e_Msg_Status, CMD_STATUS_AIRLIGHT_PRESS, g_Record_Param.nAddPress);
	// result 
	if(nCurTicks < (nLstTicks + TIME_AIRLIGHT_CHECK))
	{
		// error
//		if(eCall == e_NormalCheck_Call){
			Msg_Return_Handle_16(e_Msg_Status, CMD_STATUS_AIRLIGHT_PRESS, e_Feedback_Fail);
//		}
		printf("AirLight fail: ticks=%08d, nCurpress=%010d, Press=%010d, diff=%010d, addpress=%010d\r\n", (int)nCurTicks, (int)nCurPress, (int)nPress, (int)(nPress - nCurPress), (int)g_Record_Param.nAddPress);
		return e_Feedback_Fail;
	} else {
//		if(eCall == e_NormalCheck_Call){
			Msg_Return_Handle_16(e_Msg_Status, CMD_STATUS_AIRLIGHT_PRESS, e_Feedback_Success);
//		}
		printf("AirLight Success: ticks=%08d, nCurpress=%010d, Press=%010d, diff=%010d, addpress=%010d\r\n", (int)nCurTicks, (int)nCurPress, (int)nPress, (int)(nPress - nCurPress), (int)g_Record_Param.nAddPress);
	}
	
	return e_Feedback_Success;
}

/*
////////////////// for debug 
void Return_Press_Value(void)
{
	UINT32 nLstTicks, nCurTicks;
	INT32 nCurPress;
	UINT16 i;
	nLstTicks = IT_SYS_GetTicks();
	for(i = 0; i < 10; i++)
	{
		nCurTicks = IT_SYS_GetTicks();
		nCurPress = HW_ADC_SpiGetPress();
		//Debug_Send_32(nCurPress);
		printf("Return_Press_Value ticks=%08d, press=%010d, addpress=%010d\r\n", (int)(nCurTicks - nLstTicks), (int)nCurPress, (int)g_Record_Param.nAddPress);
		IT_SYS_DlyMs(10);
	}
	Msg_Return_Handle_32(e_Msg_Status, CMD_STATUS_PRESSURE, nCurPress);
	printf("Return_Press_Value ticks=%09d, press=%010d, addpress=%010d\r\n", (int)(nCurTicks - nLstTicks), (int)nCurPress, (int)g_Record_Param.nAddPress);
}
*/

_EXT_ UINT8 Negative_Pressure_Self_Check(void)
{
	INT32 nPress;
	UINT32 StartTicks, EndTicks;
	
	HW_PUMP_Pulse(PUMP_PRESS_FREQ, e_Dir_Pos);  // on, 25000 ticks per ms
	HW_Valve_On(EN_VALVE_AIR);    // all the air way
	HW_Valve_Off(EN_VALVE_LIQUID);    // WBC
	
	StartTicks = IT_SYS_GetTicks();
	EndTicks = StartTicks;
	while((EndTicks - StartTicks) <= TIME_OVER_TS_BUILD_PRESS)
	{
		EndTicks = IT_SYS_GetTicks();
		nPress = Get_Press_Value(GET_PRESS_NUM_FIVE); 
		if(nPress >= PRESS_BUILD)
		{
			HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);    // off
			HW_Valve_Off(EN_VALVE_AIR); // all the air way
			HW_Valve_Off(EN_VALVE_LIQUID);  // WBC
			break;
		}
	}
	if((EndTicks - StartTicks) > TIME_OVER_TS_BUILD_PRESS)
	{
		HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);    // off
		HW_Valve_Off(EN_VALVE_AIR); // all the air way
		HW_Valve_Off(EN_VALVE_LIQUID);  // WBC
		return e_Feedback_Fail;
		
	}
	return e_Feedback_Success;
}


_EXT_ UINT32 Get_WBC_V_Value(void)
{
	UINT16 nWord;
	UINT32 nValue;
	#if USE_STM32F407_ONLY
		nWord = Get_XK_ADC();	
		nValue = nWord*3300/4096;
	#else
		nWord = HW_ADC_SpiGetADC(INDEX_ADC_48V);
		nValue = nWord*10000/4096;
	#endif
	
//	printf("WBC_V: adc=%d, wbc_v=%d\r\n", nWord, (int)nValue);
	return nValue;
}


_EXT_ UINT32 WBC_48V_Self_Check(void)
{
	UINT32 nVCheck, nVIn;
	// adc
	nVCheck = Get_XK_V_Value(); //Get_WBC_V_Value();// Get_XK_V_Value();
	nVIn = nVCheck*21;
	
//	Msg_Return_Handle_32(e_Msg_Status, CMD_STATUS_WBC_48V, nWord);
//	Msg_Return_Handle_32(e_Msg_Status, CMD_STATUS_WBC_48V, nVCheck);
//	Msg_Return_Handle_32(e_Msg_Status, CMD_STATUS_WBC_48V, nVIn);
	
    printf("WBC_48V Slef Check: wbc_v=%d, 48v=%d\r\n",\
					 (int)nVCheck, (int)nVIn);
	if(nVIn >= 55000 && nVIn <= 60000)
	{
		Msg_Return_Handle_16(e_Msg_Status, CMD_STATUS_WBC_48V, e_Feedback_Success);
	}else{ // error
		Msg_Return_Handle_16(e_Msg_Status, CMD_STATUS_WBC_48V, e_Feedback_Fail);
	}
	return nVIn;
}


_EXT_ UINT8 Valve1_Self_Check(void)
{
	UINT8 i;
    for(i = 0; i < 3; i++)
	{
		HW_Valve_On(EN_VALVE_AIR);
		IT_SYS_DlyMs(1000);
		HW_Valve_Off(EN_VALVE_AIR);
		IT_SYS_DlyMs(1000);
	}
	return e_Feedback_Success;
}

_EXT_ UINT8 Valve2_Self_Check(void)
{
	UINT8 i;
	for(i = 0; i < 3; i++)
	{
		HW_Valve_On(EN_VALVE_LIQUID);
		IT_SYS_DlyMs(1000);
		HW_Valve_Off(EN_VALVE_LIQUID);
		IT_SYS_DlyMs(1000);
	}
	return e_Feedback_Success;
}

_EXT_ UINT8 Pump_Self_Check(void)
{
	UINT32 StartTicks = 0, EndTicks = 0;
	
	StartTicks = IT_SYS_GetTicks();
	EndTicks = StartTicks;
	HW_PUMP_Pulse(PUMP_PRESS_FREQ, e_Dir_Pos);
	
	//HW_Valve_On(EN_VALVE_AIR); 
	
	while((EndTicks - StartTicks) <= PUMP_SELF_CHECK_TIME)
	{
		EndTicks = IT_SYS_GetTicks();
		IT_SYS_DlyMs(1);
	}
	HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);     // off
	//HW_Valve_Off(EN_VALVE_AIR); 
	
	return e_Feedback_Success;
}

/*
UINT8 Set_CRP_Param(UINT16 nTime, UINT16 nHZ)
{
	UINT8 nRet;
	g_Record_Param.nTime 		= nTime;
	g_Record_Param.nHZ  	 	= nHZ;
	g_Record_Param.nTotal_Num 	= g_Record_Param.nTime*g_Record_Param.nHZ;
	printf("CRP Param Set: nTime=%d,nHz=%d, nTotal=%d\r\n", g_Record_Param.nTime,\
												g_Record_Param.nHZ, g_Record_Param.nTotal_Num);
	nRet = Flash_Write_Param(&g_Record_Param, RECORD_PARAM_LEN);
	if(nRet == e_Feedback_Success){
		printf("save CRP param success\r\n");
		Msg_Return_Handle_8(e_Msg_Status, CMD_STATUS_CRP_PARAM_SET, e_Feedback_Success);
	}else{
		printf("save CRP param failure\r\n");
		Msg_Return_Handle_8(e_Msg_Status, CMD_STATUS_CRP_PARAM_SET, e_Feedback_Fail);
	}
	return nRet;
}
*/

UINT8 Set_Press_Add(UINT16 nAdd)
{
	UINT8 nRet;
	g_Record_Param.nAddPress = nAdd*PRESS_PRECISION_FACTOR;
	g_Record_Param.nFlag = FLASH_INIT_FLAG;
	printf("Set add=%d, addpress=%010d\r\n", nAdd, (int)g_Record_Param.nAddPress);
	nRet = Flash_Write_Param(&g_Record_Param, RECORD_PARAM_LEN);
	if(nRet == e_Feedback_Success){
		printf("save press add param success\r\n");
		Msg_Return_Handle_8(e_Msg_Status, CMD_STATUS_PRESS_ADD, e_Feedback_Success);
	}else{
		printf("save press add param failure\r\n");
		Msg_Return_Handle_8(e_Msg_Status, CMD_STATUS_PRESS_ADD, e_Feedback_Fail);
	}
	return nRet;
}


UINT8 Set_Register_Param(UINT8 nIndex, UINT8 nVal)
{
	UINT8 nRet;
	HW_ADJ_SetResistor(nIndex, nVal);
	// todo
	switch(nIndex)
	{
		case EN_REGISTER_WBC:
		{
			g_Record_Param.nRegister_WBC =  nVal;
			printf("Set WBC register Value=%d\r\n", g_Record_Param.nRegister_WBC);
			
		}
		break;
		case EN_REGISTER_RBC:
		{
			g_Record_Param.nRegister_RBC =  nVal;
			printf("Set RBC register Value=%d\r\n", g_Record_Param.nRegister_RBC);		
		}
		break;
		case EN_REGISTER_PLT:
		{
			g_Record_Param.nRegister_PLT =  nVal;
			printf("Set PLT register Value=%d\r\n", g_Record_Param.nRegister_PLT);		
		}
		break;
		case EN_REGISTER_HGB:
		{
			g_Record_Param.nRegister_HGB =  nVal;
			printf("Set HGB register Value=%d\r\n", g_Record_Param.nRegister_HGB);		
		}
		break;
		case EN_REGISTER_CRP:
		{
			g_Record_Param.nRegister_CRP =  nVal;
			printf("Set CRP register Value=%d\r\n", g_Record_Param.nRegister_CRP);		
		}
		break;
		case EN_REGISTER_RBC_PLT:
		{
			g_Record_Param.nRegister_RBC_PLT =  nVal;
			printf("Set RBC PLT register Value=%d\r\n", g_Record_Param.nRegister_RBC_PLT);		
		}
		break;
		default:break;
	
	}
	HW_ADJ_SetResistor(nIndex, nVal);
	printf("set register(index=%d, val=%d)\r\n", nIndex, nVal);
	nRet = Flash_Write_Param(&g_Record_Param, RECORD_PARAM_LEN);
	if(nRet == e_Feedback_Success){
		printf("save register(index=%d, val=%d) param success\r\n", nIndex, nVal);
		Msg_Return_Handle_8(e_Msg_Status, CMD_STATUS_WBC_SET, e_Feedback_Success);
	}else{
		printf("save register(index=%d, val=%d) param failure\r\n", nIndex, nVal);
		Msg_Return_Handle_8(e_Msg_Status, CMD_STATUS_WBC_SET, e_Feedback_Fail);
	}
	return nRet;
}

//void Cmd_Wave_Exec(UINT8 nFlag)
//{
//	if (0 == nFlag)
//	{
//		HW_Valve_Off(nFlag);
//	}
//	else
//	{
//		HW_Valve_On(nFlag);
//	}
//}

void Reset_ADC_InitDataType(void)
{
	memset((void*)&ADC1_Status, 0, sizeof(ADC_Status_InitTypeDef));
	memset((void*)&ADC2_Status, 0, sizeof(ADC_Status_InitTypeDef));		
}

void Eable_ADC(EN_TypeADC eType)
{
	if(eType == EN_ADC1){
		Reset_ADC_InitDataType();
		ADC1_Init(); //APP_ADC_Init(EN_ADC1);
		ADC_SoftwareStartConv(ADC1);
	}else if(eType == EN_ADC2){
		Reset_ADC_InitDataType();
		//DMA_Cmd(DMA2_Stream3, DISABLE);
		ADC2_Init(); //APP_ADC_Init(EN_ADC2);
		ADC_SoftwareStartConv(ADC2);
	}else if(eType == EN_ADC3){
		
	}
}

// yaolan_
UINT8 HW_Enable_Data_Channel(eTestMode eMode)
{
#if 1 //#if USE_STM32F407_ONLY
	
	switch(eMode)
	{
		case EN_WBC_TEST:
		{
			printf("---WBC MODE---\r\n");
			Eable_ADC(EN_ADC1);
		}
		break;
		case EN_RBC_TEST:
		{
			printf("---RBC MODE---\r\n");
			Eable_ADC(EN_ADC2);
//			Disable_ADC(EN_ADC2);
//			Eable_ADC(EN_ADC2);
			
		}
		break;
		case EN_PLT_TEST:
		{
			printf("---PLT MODE---\r\n");
			Eable_ADC(EN_ADC2);
		}
		break;
		case EN_RBC_PLT_TEST:
		{
			printf("---RBC PLT MODE---\r\n");
			Eable_ADC(EN_ADC2);
		}
		break;
		default:break;
	}
#else 
	switch(eMode)
	{
		case EN_WBC_TEST:
		{
			HW_Start_WBC();			
		}
		break;
		case EN_RBC_TEST:
		{
			HW_Start_WBC();	
		}
		break;
		case EN_PLT_TEST:
		{
			HW_Start_WBC();	
		}
		break;
		case EN_RBC_PLT_TEST:
		{
			HW_Start_WBC();	
		}
		break;
		default:break;
	}

#endif
	return 0;
}

void Disable_ADC(EN_TypeADC eType)
{
	if(eType == EN_ADC1){
		//ADC_Cmd(ADC1, DISABLE);
		ADC_DMACmd(ADC1, DISABLE);
		DMA_Cmd(DMA2_Stream0, DISABLE);
		
	}else if(eType == EN_ADC2){
		ADC_Cmd(ADC2, DISABLE);
		ADC_DMACmd(ADC2, DISABLE);
		DMA_Cmd(DMA2_Stream3, DISABLE);
		
	}else if(eType == EN_ADC3){
		//
	}
}

// yaolan_
UINT8 HW_Disable_Data_Channel(eTestMode eMode)
{

#if 1  // #if USE_STM32F407_ONLY
	switch(eMode)
	{
		case EN_WBC_TEST:
		{
			Disable_ADC(EN_ADC1);
		}
		break;
		case EN_RBC_TEST:
		{
			Disable_ADC(EN_ADC2);
		}
		break;
		case EN_PLT_TEST:
		{
			Disable_ADC(EN_ADC2);
		}
		break;
		case EN_RBC_PLT_TEST:
		{
			Disable_ADC(EN_ADC2);
		}
		break;
		default:break;
	}
#else
	switch(eMode)
	{
		case EN_WBC_TEST:
		{
			HW_End_WBC();
		}
		break;
		case EN_RBC_TEST:
		{
			HW_End_WBC();
		}
		break;
		case EN_PLT_TEST:
		{
			HW_End_WBC();
		}
		break;
		case EN_RBC_PLT_TEST:
		{
			HW_End_WBC();
		}
		break;
		default:break;
	}
#endif
	return 0;
}

UINT8 HW_Clear_Data_Channel(eTestMode eMode)
{
	switch(eMode)
	{
		case EN_WBC_TEST:
		{
		
		}
		break;
		case EN_RBC_TEST:
		{
		
		}
		break;
		case EN_PLT_TEST:
		{
		
		}
		break;
		case EN_RBC_PLT_TEST:
		{
		
		}
		break;
		default:break;
	}
	
	return 0;
}


#endif

