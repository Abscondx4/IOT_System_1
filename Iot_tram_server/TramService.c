#include "TramService.h"

int aqi;
int temperature;
int humidity;
int CommandNum;
int testnum;
int retTest;

pid_t pidUTR;
pid_t pidUST;
pid_t pidUW;

#define OWNADDR "192.168.8.16"
#define OWNPORT 9999

#define DESTI_PORT 9999
#define DESTI_IP "192.168.8.111"

#define TIME_PORT 9998

#define HOME_PORT 9997

#define COMMAND_PORT 11000

#define WEATHER_PORT 11001

typedef struct homeMsg
{
    char homeAqi[10];
    char homeTemp[10];
    char homeHum[10];

} hMsg;

typedef struct time_c
{
    char date[20];
    char time[20];
} timeC;

typedef struct weather
{
    char weather[100];
    char temper[10];
} wMsg;
/**
 * 通过UDP协议发送数据。
 *
 * 该函数初始化一个UDP套接字，绑定本地地址，并向指定服务器地址发送数据。
 *
 * @param result 包含要发送的数据的字符串。
 */
void UdpTramSent(char *result)
{
    // 定义一个缓冲区来存储要发送的数据
    char resu[100];
    // 将要发送的数据复制到缓冲区
    strcpy(resu, result);

    // 创建一个UDP套接字
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd == -1)
    {
        // 处理套接字创建失败的情况
        perror("socket error");
        return;
    }
    
    // 设置目标服务器的地址信息
    struct sockaddr_in destiAddr;
    // 设置服务器IP地址
    destiAddr.sin_addr.s_addr = inet_addr(DESTI_IP);
    // 设置地址族为Internet地址族
    destiAddr.sin_family = AF_INET;
    // 设置服务器端口
    destiAddr.sin_port = htons(COMMAND_PORT);

    // 向服务器发送数据
    int ret = sendto(socketfd, resu, strlen(resu), 0, (struct sockaddr *)&destiAddr, sizeof(destiAddr));

    // 关闭套接字
    close(socketfd);
    return;
}

/**
 * UdpTramRecv函数用于通过UDP协议接收消息。
 * 该函数创建一个UDP套接字，并将其绑定到指定的本地地址和端口上，
 * 以便接收来自网络的消息，并打印消息中的环境质量、温度和湿度信息。
 */
void *UdpTramRecv()
{
    // 创建UDP套接字
    int socketfd1 = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd1 == -1)
    {
        // 如果创建套接字失败，输出错误信息并返回
        perror("socket error");
        return;
    }

    // 冗余代码，此处应避免重复创建套接字
    // int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    // if (socketfd == -1)
    // {
    //     perror("socket error");
    //     return;
    // }

    // 初始化本机地址结构
    struct sockaddr_in ownAddr;
    ownAddr.sin_addr.s_addr = inet_addr(OWNADDR); // 设置本机IP地址
    ownAddr.sin_family = AF_INET;                 // 设置地址族为Internet地址族
    ownAddr.sin_port = htons(HOME_PORT);            // 设置本机端口并进行字节序转换

    // 将套接字绑定到本机地址和端口
    bind(socketfd1, (struct sockaddr *)&ownAddr, sizeof(ownAddr));

    // 无限循环等待接收消息
    while (1)
    {
        hMsg recv;                                                         // 定义用于接收消息的结构体变量
        int ret = recvfrom(socketfd1, &recv, sizeof(recv), 0, NULL, NULL); // 接收消息
        if (ret == -1)
        {
            // 如果接收消息失败，输出错误信息并返回
            perror("recvfrom error");
            return;
        }

        printf("aqi:%s\n", recv.homeAqi);
        printf("hum:%s\n", recv.homeHum);
        printf("temperature:%s\n", recv.homeTemp);
        // 打印接收到的消息中的环境数据
        aqi = strtol(recv.homeAqi, NULL, 10);
        humidity = strtol(recv.homeHum, NULL, 10);
        temperature = strtol(recv.homeTemp, NULL, 10);
        printf("recev!\n");
    }

    // 关闭套接字
    close(socketfd1);
    return;
}

/*
 * UdpSentTime
 * 发送当前时间到目标地址的函数
 */
void *UdpSentTime()
{
    // 创建UDP套接字
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd == -1)
    {
        // 处理套接字创建失败的情况
        perror("socket error");
        return;
    }

    // 设置目标地址和端口
    struct sockaddr_in destiAddr;
    destiAddr.sin_addr.s_addr = inet_addr(DESTI_IP);
    destiAddr.sin_family = AF_INET;
    destiAddr.sin_port = htons(TIME_PORT);

    // 无限循环，每分钟发送一次当前时间
    while (1)
    {
        // 获取当前时间并格式化
        time_t curtime = time(NULL);
        struct tm *tm = localtime(&curtime);
        char date[20];
        char time[20];
        sprintf(date, "%d-%d-%d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
        sprintf(time, "%d:%d", tm->tm_hour, tm->tm_min);
        printf("date:%s\n", date);
        printf("time:%s\n", time);

        // 将时间信息打包到结构体
        timeC t;
        strcpy(t.date, date);
        strcpy(t.time, time);

        // 发送时间信息到目标地址
        int ret = sendto(socketfd, &t, sizeof(t), 0, (struct sockaddr *)&destiAddr, sizeof(destiAddr));
        sleep(60);
        printf("sent time\n");
    }

    // 关闭套接字
    close(socketfd);
    return;
}

/**
 * get_weather函数用于获取天气信息，并通过UDP发送给特定接收方。
 * 该函数通过HTTP请求从远程服务器获取天气数据，解析JSON格式的响应，
 * 然后将天气信息打包并通过UDP socket发送。
 * 
 * 参数: 无
 * 返回值: 无
 */
void *get_weather()
{
    // 定义变量用于存储天气信息
    char weather1[20];
    double temperaTure;

    // 创建TCP socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1)
    {
        perror("socket error");
        return -1;
    }

    // 创建UDP socket
    int socketfd1 = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd1 == -1)
    {
        perror("socket error");
        return -1;
    }

    // 根据域名获取服务器的IP地址
    struct hostent *info = gethostbyname("cn.apihz.cn");
    if (info == NULL)
    {
        perror("gethostbyname error");
        return -1;
    }
    char ip[100] = {0};
    strcpy(ip, inet_ntoa(*(struct in_addr *)((info->h_addr_list)[0])));
    printf("%s\n", ip);

    // 设置服务器地址并连接
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(80);
    serveraddr.sin_addr.s_addr = inet_addr(ip);
    int ret = connect(socketfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (ret == -1)
    {
        perror("perror error");
        return -1;
    }
    printf("connect success\n");

    // 发送HTTP请求
    char *requestHttp = "GET https://cn.apihz.cn/api/tianqi/tqyb.php?id=88888888&key=88888888&sheng=海南&place=海口 HTTP/1.1\r\n"
                        "Content-Type:application/x-www-form-urlencoded\r\n"
                        "Host:cn.apihz.cn\r\n\r\n";

    ret = write(socketfd, requestHttp, strlen(requestHttp));
    if (ret == -1)
    {
        perror("write error");
        return -1;
    }

    // 接收HTTP响应
    char buf[1024 * 3] = {0};
    ret = read(socketfd, buf, sizeof(buf));
    if (ret == -1)
    {
        perror("read error");
        return -1;
    }

    // 从响应中提取JSON数据
    const char *start_pos = strstr(buf, "{");
    if (start_pos == NULL)
    {
        printf("No JSON found in the response.\n");
        return 1;
    }

    const char *end_pos = strstr(start_pos, "}");
    if (end_pos == NULL)
    {
        printf("No closing '}' found in the JSON.\n");
        return 1;
    }

    size_t json_length = end_pos - start_pos + 1;
    char *json_str = (char *)malloc(json_length + 1);
    strncpy(json_str, start_pos, json_length);
    json_str[json_length] = '\0';

    // 解析JSON数据
    cJSON *root = cJSON_Parse(json_str);
    cJSON *temperature_item = cJSON_GetObjectItem(root, "temperature");
    if (temperature_item != NULL && temperature_item->type == cJSON_Number)
    {
        temperaTure = temperature_item->valuedouble;
        printf("Temperature: %.1f\n", temperaTure);
    }
    else
    {
        printf("Temperature field not found or is not a number\n");
    }

    cJSON *weather1_item = cJSON_GetObjectItem(root, "weather1");
    if (weather1_item != NULL && weather1_item->type == cJSON_String)
    {
        strcpy(weather1, weather1_item->valuestring);
        printf("Weather1: %s\n", weather1);
    }
    else
    {
        printf("Weather1 field not found or is not a number\n");
    }

    // 准备UDP发送的地址和数据
    struct sockaddr_in destiAddr;
    destiAddr.sin_addr.s_addr = inet_addr(DESTI_IP);
    destiAddr.sin_family = AF_INET;
    destiAddr.sin_port = htons(WEATHER_PORT);

    wMsg w;
    strcpy(w.weather, weather1);
    sprintf(w.temper, "%.1f 度", temperaTure);
    printf("sent weather: %s\n", w.temper);

    // 持续发送天气信息
    while (1)
    {
        sendto(socketfd1, &w, sizeof(w), 0, (struct sockaddr *)&destiAddr, sizeof(destiAddr));
        sleep(5);
    }

    // 关闭socket
    close(socketfd1);
    close(socketfd);
}