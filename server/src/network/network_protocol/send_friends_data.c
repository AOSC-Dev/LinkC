#include "def.h"
#include "linkc_network.h"
#include "linkc_network_protocol.h"
#include <string.h>

int send_friends_data(struct user_data user,void *data)
{
	char buffer[MAXBUF];
	int friend_count,length,byte,tmp,result;
	struct friend_data *My_friend;
	if(((LinkC_User_Request *)data)->Flag == 0)
	{
		((LinkC_Sys_Status *)data)->Action = USER_FRIEND_DATA;
		friend_count = get_friends_data (user.UID,&My_friend);
		if (friend_count == 0)		// 如果好友个数为 0
		{
			((LinkC_Sys_Status *)data)->Status = LINKC_FAILURE;
			length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);
			result = TCP_Send (user.sockfd,buffer,length,0);	// 发送 没有好友
			My_friend = NULL;
		}
		else if (friend_count < 0)	// 如果执行失败
		{
			((LinkC_Sys_Status *)data)->Status = LINKC_FAILURE;
			length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);
			result = TCP_Send (user.sockfd,buffer,length,0);		// 发送 错误
			My_friend = NULL;
			if (byte < 0)
				return -1;
		}
		else
		{
			((LinkC_Sys_Status *)data)->Status = SUCCESS;
			length = pack_message(SYS_ACTION_STATUS,data,LSS_L,buffer);
			result = TCP_Send (user.sockfd,buffer,length,0);	// 发送 执行成功
#if DEBUG
			printf ("UID = %d\nHave %d friend(s)\n",user.UID,friend_count);
			printf ("------Friends------\n");
			for (tmp=0;tmp<friend_count;tmp++)	printf ("\tUID\t= %d\tNAME\t= %s\n",My_friend[tmp].UID,My_friend[tmp].name);
			printf ("------End----------\n");
#endif
			non_std_m_message_send(My_friend,user.sockfd,friend_count,sizeof(friend_data),SYS_FRIEND_DATA,0);
			free(My_friend);
			My_friend = NULL;
		}
	}
	 else
	{
		tmp = ((LinkC_User_Request *)data)->UID;
		printf("%d\n",tmp);
		tmp = get_friend_data (user.UID,tmp,&My_friend);
		printf("%d\n",tmp);
		memcpy(buffer,My_friend,sizeof(friend_data));
		length = pack_message(SYS_FRIEND_DATA,buffer,sizeof(friend_data),data);
		TCP_Send (user.sockfd,data,length,0);	// 发送好友信息
		free (My_friend);
	}
    return 0;
}