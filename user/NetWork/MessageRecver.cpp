#include "MessageRecver.h"
#include "linkc_network_protocol.h"
#include "linkc_types.h"
#include <QMessageBox>

TCP_MessageRecver::TCP_MessageRecver(class TCP_csocket *sk){
    Server = *sk;
    buffer = malloc(MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
   package = malloc(MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
}

TCP_MessageRecver::~TCP_MessageRecver(){
    free(buffer);
    free(package);
}

void TCP_MessageRecver::run(){
    int header;
    while(1){
        bzero(buffer,MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
        bzero(package,MAX_BUFFER_SIZE + STD_BUFFER_SIZE + 1);
        if(Server.TCP_Recv(buffer,MAX_BUFFER_SIZE,0) == LINKC_FAILURE){
            emit RecvError();
            sleep(10);
            continue;
        }
        header = get_message_header(buffer);
        if(header == USER_MESSAGE){
            unpack_message(buffer,package);
            printf("Action == %d\n",((LUM*)package)->Action);
            emit UserMessage(((LUM*)package)->Action,((LUM*)package)->SrcUID);
            continue;
        }
        fprintf(stderr,"This Message Is Not Supposed!\n");
    }
}

void TCP_MessageRecver::MessageSend(const void *data, int legth, int flag){

}

UDP_MessageRecver::UDP_MessageRecver(UDP_csocket *sk, QThread *parent){
    Server = *sk;
}

void UDP_MessageRecver::run(){

}

void UDP_MessageRecver::MessageSend(const void *data, int legth, int flag){

}