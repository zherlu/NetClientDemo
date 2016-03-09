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
	unsigned long channel;              //ͨ���� 
	char username[MAX_USER_NAME_LEN];   //�û���
	char password[MAX_PASSWORD_LEN];    //����

	
	//char svrName[MAX_SERVER_NAME_LEN];			//ͨ��Ŀ������
	HWND  hVideoWnd;			//��Ƶ���ھ��
	//HWND  m_hChMsgWnd;        //��Ϣ���ھ����ֻ����������Ϣ�ص������Ų����ô˲���
	//UINT  m_nChMsgID;                //ͨ����ϢID
	BYTE  protocol;				//����Э������ �� PROTOCOL
	BYTE  notPlay;				//������Ϊ��0ʱ, ������������ʾģ��,��Ϊȡ��Ƶ�Ĳ���
	BYTE  tryRGBfirst;				 //��Ƶ��ʾʱ,���ȳ���ʹ��RGB����,Ĭ��ʹ��YUV����
	BYTE  model;					//�豸�ͺ� 
	BYTE  tryOverlay;				//�Ƿ�ʹ��overlay,��ʱ��Ч
	BYTE res[3];
	COLORREF nColorKey;                //color key
	
	//void  *lpreserved;
	                                     
	void *lpContext;				//�ص��Զ������

}CHANNEL_CLIENTINFO;


typedef struct _STREAM_INFO
{
	unsigned short	videoCoder;		//��Ƶ���뷽ʽ
	unsigned short	audioCoder;		//��Ƶ���뷽ʽ
	unsigned short	frameRate;		//֡��
	unsigned short	bitRate;			//������
	unsigned short	nWidth;			// ��Ƶ��
	unsigned short	nHeight;			// ��Ƶ��
	unsigned short	frameType;		// H264֡���� �����涨��
	unsigned short     isKeyFrame;		//�Ƿ�Ϊ�ؼ�֡
	unsigned short	type;			//�������ͣ�0 ��Ƶ�� 1 ��Ƶ  2����ͷ��Ϣ
	unsigned short	res;
	long			nStamp;			// ʱ����Ϣ����λ����
}STREAM_INFO;

//typedef struct _DATA_INFO
//{
//	long			nStamp;			// ʱ����Ϣ����λ����	 
//	unsigned short	videoCoder;		//��Ƶ���뷽ʽ
//	unsigned short	audioCoder;		//��Ƶ���뷽ʽ
//	unsigned short	frameRate;		// ��Ƶ��ͼ��֡��
//	unsigned short	frameType;		// H264֡���� �����涨��
//
//	unsigned short	type;		//�������ͣ�0 ��Ƶ�� 1 ��Ƶ  2����ͷ��Ϣ
//	unsigned short	nWidth;		// �������λΪ���أ��������Ƶ������Ϊ0
//	unsigned short	nHeight;		// ����ߣ���λΪ���أ��������Ƶ������Ϊ0
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


/******�ƶ����*****/
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
	CAPTURE_JPEG = 0,//Ĭ��
	CAPTURE_BMP
};

//�ص���������
//�豸��Ϣ�ص�����
typedef void(CALLBACK *MessageCallBack)(HANDLE handle, long msgType,  void*lpStruct, void *lpContext);

//��Ƶ���ݻص�����
typedef void ( CALLBACK* AVDataCallBack )( HANDLE handle, char* pBuf, long nSize, STREAM_INFO* pBufInfo, void *lpContext  );


/*************************** ��Ϣ ********************************/
//״̬��Ϣ
enum MSG_TYPE
{
	//����״̬ void*lpStruct = NULL
	MSG_LINK_OK = 0,			//�ɹ�
	MSG_LINK_CONNECTING,		//��������(�м�״̬)
	MSG_LINK_RECONNECT,		//����
	MSG_LINK_FAILED,			//����ʧ��,��������,��Ҫ�Ͽ������ͷ���Դ
	MSG_LINK_UNAUTHORIZED,	//�û����������

	//ת��ʹ��
	MSG_TRANS_SVR_GETID_FAILED,		//��ȡת���豸ʱʧ��
	MSG_TRANS_SVR_NOTFIND_DEST ,	//ת�����������ӳɹ�,�����Ҳ���ת�����豸

	//¼��״̬void*lpStruct = char*filename
	MSG_RECORD_BEGIN = 0x100,			//��һ֡����д��ʱ�ص���������Ϣ
	MSG_RECORD_WRITE_ERROR,			//¼���쳣����,д�ļ�ʧ��
	MSG_RECORD_FORMAT_CHANGED,	//����Ƶ��ʽ,�ֱ���,֡�ʷ����ı�,�ڲ��Ѿ�ֹͣ¼��,�������Ҫ¼������Ҫ���µ��ÿ�ʼ¼��, һ�����Ϣ����Ƶ��������
	MSG_RECORD_PACKET_FINISH	,		//¼�������,�ڲ��Ѿ�����ֹͣ¼��,�ⲿֱ���ٵ��ÿ�ʼ¼�񼴿�

	//����һ��¼���ļ�,������ר��¼�Ʊ���¼��void*lpStruct = char*filename
	MSG_RECORD_BEGIN_ANOTHER,				//ͬ��
	MSG_RECORD_WRITE_ERROR_ANOTHER,		//ͬ��
	MSG_RECORD_FORMAT_CHANGED_ANOTHER,	//ͬ��
	MSG_RECORD_PACKET_FINISH_ANOTHER,		//ͬ��


	//����
	MSG_MOTION_DETECT = 0x200,		//��Ƶ�ƶ����� void*lpStruct = MOTION_DETECT_INFO*pMDInfo
	MSG_VIDEO_LOST,					//��Ƶ��ʧ����
	MSG_ALARM_IN,						//̽ͷ���� void *lpstruct = int* index

	//ץ��ʹ��	void *lpstruct = (char*)filename
	MSG_SHOT_FINISHED = 0X300,		//ץ�����, 
	MSG_SHOT_FAILED,					//ץ���ļ�, �ļ���ʧ��,����ϵͳ�ڴ治��

	//�ط�ʹ��
	MSG_REPLAY_FINISHED = 0X400,		//�ļ��طŽ���

	MSG_LIVEVIEW_FAILED = 0X410,

	MSG_SOUND_FAILED = 0X420,
};




/*wParam*/




#endif




