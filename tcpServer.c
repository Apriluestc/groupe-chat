int main(int argc, char *argv[])
{
    /* ����һ��listenָ�롣�ýṹ�����Լ������ */
    struct listen *_listen;
    /* ��ʼ��socket�������ʼ�����̸���ͨ��UDP��ʼ�� socket�׽���һ�� */
    sockfd = init_socket(); 

    /*
     * ��ʼ�������socket. ����������Ϊ10��Ҳ����˵���ֻ��10���ͻ���
     * ��װ�õ�һ�������������е������� TCPЭ���е� listen ����
     * */
    server_listen(&sockfd, 10);

    while(1) {
        /* 
         * ���һ�����ӡ�������TCP�� accept ���� 
         * ��Ҫע����ǣ����û�����ӣ� server_accept ��������������״̬��ֱ����һ���µĿͻ�������
         * �ͻ���ֻ���ڵ�һ�η������ݹ�����ʱ�򣬲Żᴴ��һ���µ� listen �������� server_accept ����
         * ֮������ͻ��˵��������ݶ������͵� ����µ� listen �����ݶ����С�
         * ���ԡ�ͨ����� listen �����ǿ��Դ���һ�����̣��ɸý���ȥ��������ͻ���֮�������
         * ���listen �е��� TCP Э���е� accept �����½��� sockfd
         * */
        _listen = server_accept();

        /* 
         * ��Ȼ˵ server_accept ��������ߣ�������Ȼ�ᱻ�����źŻ��ѣ�����Ҫ�����ж�
         * �ж����Ƿ�Ϊ NULL ��Ϊ NULL ��˵��û���µ����� 
         * */
        if (_listen == NULL){
            continue;
        }

        printf("new client \r\n");
        /* 
         * ����һ�� listen_phread �̣߳����ң��ɸ��߳�ȥ�����������
         * ������TCP ��fork
         * */
        listen_pthread(_listen, listen_phread);
    }
}
