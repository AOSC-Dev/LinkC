#include "def.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

errorcode list_init(list_t *list)
{
	if (list == NULL)
		return ERROR_NULL_ARG;
	
	list->head = NULL;
	list->size = 0;

	return SUCCESS;
}

errorcode list_destroy(list_t *list)
{
	if (list == NULL)
		return ERROR_NULL_ARG;
	
	list_node *node;

	while (list->head != NULL)
	{
		node = list->head;
		list->head = node->next;
		free(node);
	}
	return SUCCESS;
}

errorcode list_connection_find(list_t *list, conn_info info,struct sockaddr_in *Dest,flag_t FLAG)
{
	CHECK_NOT_NULL(list,ERROR_NULL_ARG);
	list_node *node;
	node = list->head;

	while(1)
	{
		if (node == NULL)		// If The Next points NULL,Break the loop
			break;

		if(node->info->Src.sin_addr.s_addr==info.Dest.sin_addr.s_addr && node->info->Dest.sin_addr.s_addr==info.Src.sin_addr.s_addr)
		{
			*Dest=node->info->Src;
			return SUCCESS;
		}
		node=node->next;
	}
	return -14;	// Error Not Found
}

errorcode list_item_add(list_t *list,conn_info item)
{
	CHECK_NOT_NULL(list,ERROR_NULL_ARG);

	list_node *node = (list_node *)malloc(sizeof(list_node));
	conn_info *info = (conn_info *)malloc(sizeof(conn_info));

	memcpy(info,(void*)&item,sizeof(conn_info));
	node->info = info;

	node->next=list->head;
	node->perv=NULL;
	list->head->perv=node;
	list->head=node;
	list->size++;
	return SUCCESS;
}

errorcode list_node_remove(list_t *list,conn_info info)
{
	CHECK_NOT_NULL(list,ERROR_NULL_ARG);
	list_node *node,*tmp;
	node = list->head;

	while(1)
	{
		if (node == NULL)
		{
			break;
		}
		if(node->info->Src.sin_addr.s_addr==info.Dest.sin_addr.s_addr && node->info->Dest.sin_addr.s_addr==info.Src.sin_addr.s_addr)
		{
			node->next->perv = node->perv;
			node->perv->next = node->next;
			free(node);
			list->size--;
			return SUCCESS;
		}
		node=node->next;
	}
	return ERROR_NOT_FOUND;
}