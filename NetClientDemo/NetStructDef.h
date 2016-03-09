#ifndef __NETSTRUCTDEF_H__
#define __NETSTRUCTDEF_H__

#define MAX_HOST_LEN					128
#define MAX_SERVER_NAME_LEN			32
#define MAX_USER_NAME_LEN			32
#define MAX_PASSWORD_LEN			32

#define MAX_FILENAME_LEN			255

enum PROTOCOL
{
	PROTOCOL_RTSP_UDP   = 0,
	PROTOCOL_RTSP_TCP   = 1,
	PROTOCOL_UDP		=    2,
};

typedef struct _CHANNEL_CLIENTINFO
{	
	char host[MAX_HOST_LEN];
	DWORD port;
	unsigned long channel;              //通道号 
	char username[MAX_USER_NAME_LEN];   //用户名
	char password[MAX_PASSWORD_LEN];    //密码

	
	//char svrName[MAX_SERVER_NAME_LEN];			//通道目标名称
	HWND  hVideoWnd;			//视频窗口句柄
	//HWND  m_hChMsgWnd;        //消息窗口句柄，只有设置了消息回调函数才不设置此参数
	//UINT  m_nChMsgID;                //通道消息ID
	BYTE  protocol;				//传输协议类型 见 PROTOCOL
	BYTE  notPlay;				//当设置为非0时, 则不启动解码显示模块,作为取视频的操作
	BYTE  tryRGBfirst;				 //视频显示时,首先尝试使用RGB表面,默认使用YUV表面
	BYTE  model;					//设备型号 
	BYTE  tryOverlay;				//是否使用overlay,暂时无效
	BYTE res[3];
	COLORREF nColorKey;                //color key
	
	//void  *lpreserved;
	                                     
	void *lpContext;				//回调自定义参数

}CHANNEL_CLIENTINFO;


typedef struct _STREAM_INFO
{
	unsigned short	videoCoder;		//视频编码方式
	unsigned short	audioCoder;		//音频编码方式
	unsigned short	frameRate;		//帧率
	unsigned short	bitRate;			//比特率
	unsigned short	nWidth;			// 视频宽
	unsigned short	nHeight;			// 视频高
	unsigned short	frameType;		// H264帧类型 见下面定义
	unsigned short     isKeyFrame;		//是否为关键帧
	unsigned short	type;			//数据类型，0 视频， 1 音频  2码流头信息
	unsigned short	res;
	long			nStamp;			// 时标信息，单位毫秒
}STREAM_INFO;

//typedef struct _DATA_INFO
//{
//	long			nStamp;			// 时标信息，单位毫秒	 
//	unsigned short	videoCoder;		//视频编码方式
//	unsigned short	audioCoder;		//音频编码方式
//	unsigned short	frameRate;		// 视频的图像帧率
//	unsigned short	frameType;		// H264帧类型 见下面定义
//
//	unsigned short	type;		//数据类型，0 视频， 1 音频  2码流头信息
//	unsigned short	nWidth;		// 画面宽，单位为像素，如果是音频数据则为0
//	unsigned short	nHeight;		// 画面高，单位为像素，如果是音频数据则为0
//	unsigned short	reserved1;
//
//	_DATA_INFO()
//	{
//		ZeroMemory(this,  sizeof(_DATA_INFO));
//	}
//
//}DATA_INFO;




//FRAME_INFO.avType
enum Stream_Type
{
	STREAM_VIDEO = 0,
	STREAM_AUDIO,
	STREAM_HEADER,
};


//FRAME_INFO.nalType
#define NAL_TYPE_SLICE      1
#define NAL_TYPE_IDR        5
#define NAL_TYPE_SEI        6
#define NAL_TYPE_SPS        7
#define NAL_TYPE_PPS        8
//#define NAL_TYPE_SEQ_END    9
//#define NAL_TYPE_STREAM_END 10

//FRAME_INFO.videoCoder
#ifndef VIDEO_CODER_H264
#define VIDEO_CODER_H264				96
#define VIDEO_CODER_MJPEG				26
#define VIDEO_CODER_MPEG4				99
#endif

//FRAME_INFO.audioCoder
#define AUDIO_CODER_G711_A				0x01  /* 64kbps G.711 A, see RFC3551.txt  4.5.14 PCMA */
#define AUDIO_CODER_G711_U				0x02  /* 64kbps G.711 U, see RFC3551.txt  4.5.14 PCMU */
#define AUDIO_CODER_G726_16KBPS			0x04  /* 16kbps G.726, see RFC3551.txt  4.5.4 G726-16 */
#define AUDIO_CODER__G726_24KBPS			0x05  /* 24kbps G.726, see RFC3551.txt  4.5.4 G726-24 */
#define AUDIO_CODER_G726_32KBPS			0x06  /* 32kbps G.726, see RFC3551.txt  4.5.4 G726-32 */
#define AUDIO_CODER_G726_40KBPS			0x07  /* 40kbps G.726, see RFC3551.txt  4.5.4 G726-40 */


/******移动侦测*****/
#define MAX_MOTION_REGION		16
typedef struct
{
	DWORD dwMotionMask;	
	SHORT	cbSensitive[MAX_MOTION_REGION];
	RECT	rcRegion[MAX_MOTION_REGION];
	DWORD	dwMotionTick[MAX_MOTION_REGION];
}MOTION_DETECT_INFO;
/***************/

typedef struct
{
	unsigned short	videoWidth;
	unsigned short	videoHeight;
	unsigned short	frameRate;
	BYTE			videoCoder;
	BYTE			audioCoder;
		
	unsigned long	bitRate;

	unsigned long	reserved1;
	unsigned long	reserved2;
	unsigned long	reserved3;
	unsigned long	reserved4;

}CLIENT_PARAM;

enum CaptureType
{
	CAPTURE_JPEG = 0,//默认
	CAPTURE_BMP
};

//回调函数申明
//设备消息回调函数
typedef void(CALLBACK *MessageCallBack)(HANDLE handle, long msgType,  void*lpStruct, void *lpContext);

//视频数据回调函数
typedef void ( CALLBACK* AVDataCallBack )( HANDLE handle, char* pBuf, long nSize, STREAM_INFO* pBufInfo, void *lpContext  );


/*************************** 消息 ********************************/
//状态消息
enum MSG_TYPE
{
	//连接状态 void*lpStruct = NULL
	MSG_LINK_OK = 0,			//成功
	MSG_LINK_CONNECTING,		//正在连接(中间状态)
	MSG_LINK_RECONNECT,		//重连
	MSG_LINK_FAILED,			//最终失败,放弃重连,需要断开连接释放资源
	MSG_LINK_UNAUTHORIZED,	//用户或密码错误

	//转发使用
	MSG_TRANS_SVR_GETID_FAILED,		//获取转发设备时失败
	MSG_TRANS_SVR_NOTFIND_DEST ,	//转发服务器连接成功,但是找不到转发的设备

	//录像状态void*lpStruct = char*filename
	MSG_RECORD_BEGIN = 0x100,			//第一帧数据写入时回调产生此消息
	MSG_RECORD_WRITE_ERROR,			//录像异常错误,写文件失败
	MSG_RECORD_FORMAT_CHANGED,	//音视频格式,分辨率,帧率发生改变,内部已经停止录像,如果仍需要录像则需要重新调用开始录像, 一般此消息在视频重连后发生
	MSG_RECORD_PACKET_FINISH	,		//录象打包完成,内部已经调用停止录像,外部直接再调用开始录像即可

	//另外一份录像文件,可用于专门录制报警录像void*lpStruct = char*filename
	MSG_RECORD_BEGIN_ANOTHER,				//同上
	MSG_RECORD_WRITE_ERROR_ANOTHER,		//同上
	MSG_RECORD_FORMAT_CHANGED_ANOTHER,	//同上
	MSG_RECORD_PACKET_FINISH_ANOTHER,		//同上


	//报警
	MSG_MOTION_DETECT = 0x200,		//视频移动报警 void*lpStruct = MOTION_DETECT_INFO*pMDInfo
	MSG_VIDEO_LOST,					//视频丢失报警
	MSG_ALARM_IN,						//探头报警 void *lpstruct = int* index

	//抓拍使用	void *lpstruct = (char*)filename
	MSG_SHOT_FINISHED = 0X300,		//抓拍完成, 
	MSG_SHOT_FAILED,					//抓拍文件, 文件打开失败,或者系统内存不够

	//回放使用
	MSG_REPLAY_FINISHED = 0X400,		//文件回放结束

	MSG_LIVEVIEW_FAILED = 0X410,

	MSG_SOUND_FAILED = 0X420,
};




/*wParam*/




#endif




