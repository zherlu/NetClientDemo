#ifndef __NETCLIENT_H__
#define __NETCLIENT_H__

#include "NetStructDef.h"

//#define __BUILD_LIB

#if defined __BUILD_DLL
	#define NET_DLLAPI		extern "C" __declspec(dllexport)
#elif defined __BUILD_LIB
	#define NET_DLLAPI
#else
	#define NET_DLLAPI		extern "C" __declspec(dllimport)
#endif


/***************************��̨����******************************/
#define PTZ_STOP			0	/*	��ֹ̨ͣ*/

#define TILT_UP				1	/* ��̨��Speed���ٶ���ת */
#define TILT_DOWN			2	/* ��̨��Speed���ٶ���ת */
#define PAN_LEFT			3	/* ��̨��Speed���ٶ���ת */
#define PAN_RIGHT			4	/* ��̨��Speed���ٶ���ת */


#define PT_LEFT_UP			5	/* ��̨��Speed���ٶ����� */
#define PT_LEFT_DOWN		6	/* ��̨��Speed���ٶ����� */
#define PT_RIGHT_UP			7	/* ��̨��Speed���ٶ����� */
#define PT_RIGHT_DOWN		8	/* ��̨��Speed���ٶ����� */

#define PTZ_ZOOM_IN		9 	/* �������ٶ�Speed���(���ʱ��) */
#define PTZ_ZOOM_OUT		10	/* �������ٶ�Speed��С(���ʱ�С) */
#define FOCUS_NEAR			11	/* �������ٶ�Speedǰ�� */
#define FOCUS_FAR			12	/* �������ٶ�Speed��� */
#define IRIS_OPEN			13	/* ��Ȧ���ٶ�Speed���� */
#define IRIS_CLOSE			14	/* ��Ȧ���ٶ�Speed��С */


#define GOTO_PRESET	15	/* ת��Ԥ�õ� */
#define CLE_PRESET		16	/* ���Ԥ�õ� */
#define SET_PRESET		17	/* ����Ԥ�õ� */


#define PAN_AUTO		18	/* ��̨��Speed���ٶ�����Ѳ�� */
#define PAN_AUTO_STOP	19	/* Ѳ��ֹͣ */

//extern "C"
//{
/******************************************************************/


//typedef void(CALLBACK *DataCallBack) (long handle, long dwDataType, BYTE *pBuffer,DWORD dwBufSize,void* dwUser)

//��Ƶ��ʾ�ص�����
typedef void* ( CALLBACK* DrawCallback )( LONG hHandle, HDC hDC, void *lpContext );

//��ʼ������
NET_DLLAPI BOOL __stdcall NET_Startup();

//�ͷſ⺯��
NET_DLLAPI void __stdcall  NET_Cleanup();

//ulMessage hWnd Ϊ��Ϣ��ʽ, msgfun Ϊ�ص���ʽ
NET_DLLAPI void __stdcall SetMessageCallBack(MessageCallBack msgFun=NULL);



//WaitTime�����ȴ�ʱ��,��λ��, TryNum�������� -1��־һֱ����, TryInterval�������ʱ��,��λ��
NET_DLLAPI void __stdcall NET_SetWaitTime(int WaitTime=5, int TryNum=-1, int TryInterval=10);


//ʵʱԤ��
NET_DLLAPI HANDLE __stdcall NET_ClientStart(CHANNEL_CLIENTINFO *pChaninfo, AVDataCallBack dataFun = NULL);
NET_DLLAPI BOOL __stdcall NET_ClientStop(HANDLE hHandle);
NET_DLLAPI BOOL __stdcall NET_ClientSetWnd(HANDLE hHandle, HWND hWnd);
NET_DLLAPI BOOL __stdcall NET_ClientRefreshWnd(HANDLE hHandle);

NET_DLLAPI LONG __stdcall NET_GetClientLinkState(HANDLE hHandle);
NET_DLLAPI BOOL __stdcall NET_GetBitStreamInfo(HANDLE hHandle, ULONG *pBitRate, ULONG *pFrameRate);
NET_DLLAPI BOOL __stdcall NET_ClientSetVideoParam(HANDLE hHandle, BYTE cbBrightness, BYTE cbContrast, BYTE cbSaturation, ULONG bDefault = 0);
NET_DLLAPI BOOL __stdcall NET_ClientGetVideoParam(HANDLE hHandle, BYTE *pcbBrightness, BYTE *pcbContrast, BYTE *pcbSaturation);


//��������
NET_DLLAPI BOOL __stdcall NET_ClientPlayAudio(HANDLE hHandle);
NET_DLLAPI BOOL __stdcall NET_ClientStopAudio(HANDLE hHandle);

//ץͼ
NET_DLLAPI BOOL __stdcall NET_ClientCapturePicture(HANDLE hHandle, const char* filename, CaptureType type = CAPTURE_JPEG);

//¼��
NET_DLLAPI BOOL __stdcall NET_ClientStartRecord(HANDLE hHandle, const char* filename, DWORD dwDurationSeconds=0);
NET_DLLAPI BOOL __stdcall NET_ClientStopRecord(HANDLE hHandle);
//����һ��¼��
NET_DLLAPI BOOL __stdcall NET_ClientStartRecord_Another(HANDLE hHandle, const char* filename, DWORD dwDurationSeconds=0);
NET_DLLAPI BOOL __stdcall NET_ClientStopRecord_Another(HANDLE hHandle);

//��̨����
NET_DLLAPI BOOL __stdcall NET_ClientPTZCtrl(HANDLE hHandle, int type, int param);

//�ƶ����
NET_DLLAPI BOOL __stdcall NET_ClientGetMDInfo(HANDLE hHandle, MOTION_DETECT_INFO* pMDInfo/*out*/);
NET_DLLAPI BOOL __stdcall NET_ClientSaveMDInfo(HANDLE hHandle, MOTION_DETECT_INFO* pMDInfo/*in*/);

NET_DLLAPI BOOL __stdcall NET_ClientStartMD(HANDLE hHandle);
NET_DLLAPI BOOL __stdcall NET_ClientStopMD(HANDLE hHandle);


NET_DLLAPI BOOL __stdcall NET_ClientStopAlarm(HANDLE hHandle, ULONG ulDeviceNum);

//ulShow = 0x01��ʾ�ƶ������������, ulShow = 0x02��ʾ��������(Ĭ��),����ֵ����ȡ��������һ����
NET_DLLAPI BOOL __stdcall NET_ClientShowMDRegion(HANDLE hHandle, ULONG ulShow);

//˫������
NET_DLLAPI BOOL __stdcall NET_ClientStartTalk(HANDLE hHandle);
NET_DLLAPI void __stdcall   NET_ClientStopTalk(HANDLE hHandle);

//�ļ��طź���
NET_DLLAPI HANDLE __stdcall  NET_OpenFile(const char* filename, HWND hWnd, BOOL bPause=FALSE);
NET_DLLAPI BOOL __stdcall  NET_CloseFile(HANDLE hHandle);
NET_DLLAPI BOOL __stdcall  NET_ReplayPause(HANDLE hHandle);
NET_DLLAPI BOOL __stdcall  NET_ReplayContinue(HANDLE hHandle);
NET_DLLAPI BOOL __stdcall  NET_ReplayStepByStep(HANDLE hHandle);
NET_DLLAPI BOOL __stdcall  NET_SpeedFast(HANDLE hHandle);
NET_DLLAPI BOOL __stdcall  NET_SpeedNormal(HANDLE hHandle);
NET_DLLAPI BOOL __stdcall  NET_SpeedSlow(HANDLE hHandle);
NET_DLLAPI ULONG __stdcall NET_ReplayTotalTime(HANDLE hHandle);
NET_DLLAPI ULONG __stdcall NET_ReplayCurTime(HANDLE hHandle);
NET_DLLAPI BOOL  __stdcall NET_ReplaySeek(HANDLE hHandle, ULONG ulSeconds);

// ��ȡ��Ƶ������ʵʱ�����ļ����Ŷ����Ե��ô˲���
NET_DLLAPI BOOL __stdcall  NET_GetClientParam(HANDLE hHandle, CLIENT_PARAM* pClientParam);


//��Ƶ��ʾDC�ص�����
NET_DLLAPI void __stdcall  SetDrawCallback(HANDLE handle, DrawCallback dawFun);


//���ô�ֱˢ��ͬ�� param 0 Ϊ��ͬ�� 1Ϊȫ��ͬ��
NET_DLLAPI void __stdcall  NET_SetVertBlank(HANDLE hHandle, UINT param );

//��Ƶ�������л����֡��
NET_DLLAPI void __stdcall  NET_SetRestoreFrame(HANDLE hHandle, UINT frame );

//ת��������ר��ָ��,��ѯĿ���豸��ת���������е�ͨ����(ID),����ID��
NET_DLLAPI long __stdcall  NET_QueryServerChannel(const char*serverhost, long serverport,
													const char* srchost, 
												   long rtspPort, 
												   long channel, 
												   const char* user,
												   const char* psw);

//ʹ��ת��������Ƶ,��ʵ���Ǳ�NET_ClientStart ����һ��GSNET_QueryServerChannel�ĵ���
NET_DLLAPI HANDLE __stdcall NET_ClientStart_Trans(CHANNEL_CLIENTINFO *pChaninfo, 
													const char*srchost,
													long srcrtspPort, 
													long srcchannel, 
													const char* srcuser,
													const char* srcpsw,
													long useTrans);

//ʹ����ɫ��ǿģʽ
NET_DLLAPI void __stdcall  NET_VideoEnhanceMode(HANDLE hHandle, BOOL bUse );





#endif


