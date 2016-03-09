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


/***************************云台控制******************************/
#define PTZ_STOP			0	/*	云台停止*/

#define TILT_UP				1	/* 云台以Speed的速度上转 */
#define TILT_DOWN			2	/* 云台以Speed的速度下转 */
#define PAN_LEFT			3	/* 云台以Speed的速度左转 */
#define PAN_RIGHT			4	/* 云台以Speed的速度右转 */


#define PT_LEFT_UP			5	/* 云台以Speed的速度左上 */
#define PT_LEFT_DOWN		6	/* 云台以Speed的速度左下 */
#define PT_RIGHT_UP			7	/* 云台以Speed的速度右上 */
#define PT_RIGHT_DOWN		8	/* 云台以Speed的速度右下 */

#define PTZ_ZOOM_IN		9 	/* 焦距以速度Speed变大(倍率变大) */
#define PTZ_ZOOM_OUT		10	/* 焦距以速度Speed变小(倍率变小) */
#define FOCUS_NEAR			11	/* 焦点以速度Speed前调 */
#define FOCUS_FAR			12	/* 焦点以速度Speed后调 */
#define IRIS_OPEN			13	/* 光圈以速度Speed扩大 */
#define IRIS_CLOSE			14	/* 光圈以速度Speed缩小 */


#define GOTO_PRESET	15	/* 转到预置点 */
#define CLE_PRESET		16	/* 清除预置点 */
#define SET_PRESET		17	/* 设置预置点 */


#define PAN_AUTO		18	/* 云台以Speed的速度左右巡航 */
#define PAN_AUTO_STOP	19	/* 巡航停止 */

//extern "C"
//{
/******************************************************************/


//typedef void(CALLBACK *DataCallBack) (long handle, long dwDataType, BYTE *pBuffer,DWORD dwBufSize,void* dwUser)

//视频显示回调函数
typedef void* ( CALLBACK* DrawCallback )( LONG hHandle, HDC hDC, void *lpContext );

//初始化函数
NET_DLLAPI BOOL __stdcall NET_Startup();

//释放库函数
NET_DLLAPI void __stdcall  NET_Cleanup();

//ulMessage hWnd 为消息方式, msgfun 为回调方式
NET_DLLAPI void __stdcall SetMessageCallBack(MessageCallBack msgFun=NULL);



//WaitTime重连等待时间,单位秒, TryNum重连次数 -1标志一直重连, TryInterval重连间隔时间,单位秒
NET_DLLAPI void __stdcall NET_SetWaitTime(int WaitTime=5, int TryNum=-1, int TryInterval=10);


//实时预览
NET_DLLAPI HANDLE __stdcall NET_ClientStart(CHANNEL_CLIENTINFO *pChaninfo, AVDataCallBack dataFun = NULL);
NET_DLLAPI BOOL __stdcall NET_ClientStop(HANDLE hHandle);
NET_DLLAPI BOOL __stdcall NET_ClientSetWnd(HANDLE hHandle, HWND hWnd);
NET_DLLAPI BOOL __stdcall NET_ClientRefreshWnd(HANDLE hHandle);

NET_DLLAPI LONG __stdcall NET_GetClientLinkState(HANDLE hHandle);
NET_DLLAPI BOOL __stdcall NET_GetBitStreamInfo(HANDLE hHandle, ULONG *pBitRate, ULONG *pFrameRate);
NET_DLLAPI BOOL __stdcall NET_ClientSetVideoParam(HANDLE hHandle, BYTE cbBrightness, BYTE cbContrast, BYTE cbSaturation, ULONG bDefault = 0);
NET_DLLAPI BOOL __stdcall NET_ClientGetVideoParam(HANDLE hHandle, BYTE *pcbBrightness, BYTE *pcbContrast, BYTE *pcbSaturation);


//声音控制
NET_DLLAPI BOOL __stdcall NET_ClientPlayAudio(HANDLE hHandle);
NET_DLLAPI BOOL __stdcall NET_ClientStopAudio(HANDLE hHandle);

//抓图
NET_DLLAPI BOOL __stdcall NET_ClientCapturePicture(HANDLE hHandle, const char* filename, CaptureType type = CAPTURE_JPEG);

//录像
NET_DLLAPI BOOL __stdcall NET_ClientStartRecord(HANDLE hHandle, const char* filename, DWORD dwDurationSeconds=0);
NET_DLLAPI BOOL __stdcall NET_ClientStopRecord(HANDLE hHandle);
//另外一份录像
NET_DLLAPI BOOL __stdcall NET_ClientStartRecord_Another(HANDLE hHandle, const char* filename, DWORD dwDurationSeconds=0);
NET_DLLAPI BOOL __stdcall NET_ClientStopRecord_Another(HANDLE hHandle);

//云台控制
NET_DLLAPI BOOL __stdcall NET_ClientPTZCtrl(HANDLE hHandle, int type, int param);

//移动侦测
NET_DLLAPI BOOL __stdcall NET_ClientGetMDInfo(HANDLE hHandle, MOTION_DETECT_INFO* pMDInfo/*out*/);
NET_DLLAPI BOOL __stdcall NET_ClientSaveMDInfo(HANDLE hHandle, MOTION_DETECT_INFO* pMDInfo/*in*/);

NET_DLLAPI BOOL __stdcall NET_ClientStartMD(HANDLE hHandle);
NET_DLLAPI BOOL __stdcall NET_ClientStopMD(HANDLE hHandle);


NET_DLLAPI BOOL __stdcall NET_ClientStopAlarm(HANDLE hHandle, ULONG ulDeviceNum);

//ulShow = 0x01显示移动侦测设置区域, ulShow = 0x02显示报警区域(默认),两个值可以取或运算在一起传入
NET_DLLAPI BOOL __stdcall NET_ClientShowMDRegion(HANDLE hHandle, ULONG ulShow);

//双向语音
NET_DLLAPI BOOL __stdcall NET_ClientStartTalk(HANDLE hHandle);
NET_DLLAPI void __stdcall   NET_ClientStopTalk(HANDLE hHandle);

//文件回放函数
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

// 获取视频参数，实时流和文件播放都可以调用此参数
NET_DLLAPI BOOL __stdcall  NET_GetClientParam(HANDLE hHandle, CLIENT_PARAM* pClientParam);


//视频显示DC回调函数
NET_DLLAPI void __stdcall  SetDrawCallback(HANDLE handle, DrawCallback dawFun);


//设置垂直刷新同步 param 0 为不同步 1为全屏同步
NET_DLLAPI void __stdcall  NET_SetVertBlank(HANDLE hHandle, UINT param );

//视频缓存区中缓存的帧数
NET_DLLAPI void __stdcall  NET_SetRestoreFrame(HANDLE hHandle, UINT frame );

//转发服务器专用指令,查询目标设备在转发服务器中的通道号(ID),返回ID号
NET_DLLAPI long __stdcall  NET_QueryServerChannel(const char*serverhost, long serverport,
													const char* srchost, 
												   long rtspPort, 
												   long channel, 
												   const char* user,
												   const char* psw);

//使用转发开启视频,其实就是比NET_ClientStart 多了一个GSNET_QueryServerChannel的调用
NET_DLLAPI HANDLE __stdcall NET_ClientStart_Trans(CHANNEL_CLIENTINFO *pChaninfo, 
													const char*srchost,
													long srcrtspPort, 
													long srcchannel, 
													const char* srcuser,
													const char* srcpsw,
													long useTrans);

//使用颜色增强模式
NET_DLLAPI void __stdcall  NET_VideoEnhanceMode(HANDLE hHandle, BOOL bUse );





#endif


