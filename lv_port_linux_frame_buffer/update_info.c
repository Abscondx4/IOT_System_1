#include "ui.h"

double temperture;
int humidity;
int aqi;

#define OWNADDR "192.168.8.111"
#define OWNPORT 9999

#define DESTI_PORT 9999
#define DESTI_IP "192.168.8.16"

#define HOME_PORT 9997

#define TIME_PORT 9998

#define COMMAND_PORT 11000

#define WEATHER_PORT 11001

int light;
int light_c = 33;

wMsg w;
/**
 * @brief 更新时间的回调函数
 *
 * 此函数用作定时器的回调，用来更新显示在界面中的当前时间
 * 和日期。它不需要任何参数，也不返回任何值。
 *
 * @return void* 返回NULL，表示无返回值
 */
void * update_time_call()
{
    // 获取当前时间
    time_t now;
    time(&now);

    // 将时间转换为本地时间格式
    struct tm * tm = localtime(&now);

    // 定义存储日期和时间的字符数组
    char date[20];
    char time[20];

    // 格式化当前日期为 "年/月/日" 的形式
    sprintf(date, "%d / %d / %d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);

    // 格式化当前时间为 "时:分" 的形式
    sprintf(time, "%d:%d", tm->tm_hour, tm->tm_min);

    // 定义常量字符指针，指向日期和时间
    const char * date_n = date;
    const char * time_n = time;

    // 更新界面中标签的文本为当前日期和时间
    lv_label_set_text_fmt(ui_Label14, date_n);
    lv_label_set_text_fmt(ui_Label13, time_n);

    // 函数结束，返回NULL
    return NULL;
}

void * update_time_pth()
{
    while(1) {
        lv_async_call(update_time_call, NULL);
        sleep(60);
    }
    return NULL;
}

/**
 * @brief 更新环境信息并显示
 *
 * 该函数生成随机的空气质量指数(AQI)、温度和湿度数据，根据AQI值确定空气质量状况，
 * 并将这些信息更新到用户界面的标签中。
 *
 * @note 本函数无输入参数，返回值为NULL，表示调用该函数无需期待返回的值。
 */
void * update_info_call()
{
    // 生成0到500之间的随机AQI值
    aqi = rand() % 501;
    // 生成10.0到99.9之间的随机温度值
    double temperature_temp = (double)(rand() % 900 + 100) / 10;
    // 生成0到99之间的随机湿度值
    humidity = rand() % 100;
    // 如果温度低于40，则直接赋值给temperture
    if(temperature_temp < 40) {
        temperture = temperature_temp;
    }

    // 存储AQI信息的字符数组
    char aqi_str[20];
    // 将AQI数值转换为字符串格式
    sprintf(aqi_str, "aqi:%d", aqi);

    // 存储温度和湿度信息的字符数组
    char temhum_str[120];
    // 将温度和湿度转换为字符串格式
    sprintf(temhum_str, "房屋温度: %.1f / 湿度: %d %%", temperture, humidity);

    // 存储空气质量信息的字符数组
    char airquan_str[40];
    // 根据AQI值判断空气质量并转换为相应的字符串格式
    if(aqi < 50) {
        sprintf(airquan_str, "空气质量: 优");
    } else if(aqi >= 50 && aqi < 100) {
        sprintf(airquan_str, "空气质量: 良");
    } else if(aqi >= 100 && aqi < 150) {
        sprintf(airquan_str, "空气质量: 轻度污染");
    } else if(aqi >= 150 && aqi < 200) {
        sprintf(airquan_str, "空气质量: 中度污染");
    } else if(aqi >= 200 && aqi < 300) {
        sprintf(airquan_str, "空气质量: 重度污染");
    } else if(aqi >= 300) {
        sprintf(airquan_str, "空气质量: 严重污染");
    }

    // 将信息字符串更新到用户界面的标签中
    lv_label_set_text_fmt(ui_Label17, temhum_str);
    lv_label_set_text_fmt(ui_Label18, airquan_str);
    lv_label_set_text_fmt(ui_Label19, aqi_str);

    // 函数执行完毕，返回NULL
    return NULL;
}

void * update_info_pth()
{
    while(1) {
        lv_async_call(update_info_call, NULL);
        sleep(15);
    }
    return NULL;
}

/**
 * 接收UDP数据包
 *
 * 本函数创建一个UDP套接字并绑定到指定的本地地址和端口，然后进入循环等待接收数据包
 * 接收到的数据包将被存储在resu数组中，该数组最大可存储100个字节
 *
 * 注意：本函数不返回任何值，因为UDP是无连接的，理论上可以无限期地等待数据包
 *       为了保持代码逻辑的简单性，这里没有处理正常退出的情况，实际使用时应考虑添加适当的退出条件
 */
void * udp_tram_recv()
{
    char resu[1024];
    int sockedfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockedfd == -1) {
        perror("socket error");
        exit(-1);
    }

    struct sockaddr_in own_addr;
    own_addr.sin_addr.s_addr = inet_addr(OWNADDR);
    own_addr.sin_port        = htons(COMMAND_PORT);
    own_addr.sin_family      = AF_INET;

    bind(sockedfd, (struct sockaddr *)&own_addr, sizeof(own_addr));

    struct sockaddr_in target_addr;
    int len = sizeof(target_addr);

    while(1) {
        char buf[1024] = {0};

        int ret = recvfrom(sockedfd, buf, sizeof(buf), 0, (struct sockaddr *)&target_addr, &len);
        if(ret <= 0) {
            break;
        }
        printf("用户：%s:\n%s\n", inet_ntoa(target_addr.sin_addr), buf);
        strcpy(resu, buf);
        printf("resu: %s\n", resu);
        light = strtol(resu, NULL, 10);
    }

    close(sockedfd);
    return;
}

/**
 * 通过UDP协议接收时间信息并更新系统时间
 *
 * 该函数创建一个UDP套接字服务器，监听特定地址和端口，接收时间信息，解析并执行时间更新命令
 * 使用recvfrom函数接收UDP数据，解析接收到的时间信息，并通过system调用执行日期命令更新系统时间
 *
 * 注意：该函数假定接收到的数据格式正确，并且系统支持date命令的时间设置格式
 */
void * udp_recv_time()
{
    // 定义用于存储时间信息的数据结构
    timeC t;
    wMsg w;

    // 创建UDP套接字
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketfd == -1) {
        // 处理套接字创建失败的情况
        perror("socket error");
        return NULL;
    }

    // int socketfd1 = socket(AF_INET, SOCK_DGRAM, 0);
    // if(socketfd1 == -1) {
    //     // 处理套接字创建失败的情况
    //     perror("socket error");
    //     return NULL;
    // }
    // 初始化服务器地址结构
    struct sockaddr_in ownAddr;
    ownAddr.sin_addr.s_addr = inet_addr(OWNADDR);
    ownAddr.sin_family      = AF_INET;
    ownAddr.sin_port        = htons(TIME_PORT);

    // struct sockaddr_in weaAddr;
    // weaAddr.sin_addr.s_addr = inet_addr(OWNADDR);
    // weaAddr.sin_family      = AF_INET;
    // weaAddr.sin_port        = htons(WEATHER_PORT);

    // 将套接字绑定到指定的地址和端口
    bind(socketfd, (struct sockaddr *)&ownAddr, sizeof(ownAddr));
    // bind(socketfd1, (struct sockaddr *)&ownAddr, sizeof(ownAddr));
    // 持续监听并处理来自客户端的时间信息
    while(1) {
        // 接收UDP数据
        int ret = recvfrom(socketfd, &t, sizeof(t), 0, NULL, NULL);
        if(ret == -1) {
            // 处理接收数据失败的情况
            perror("recvfrom error");
            return NULL;
        }
        printf("recv\n");
        // int ret1 = recvfrom(socketfd1, &w, sizeof(w), 0, NULL, NULL);
        // if(ret1 == -1) {
        //     // 处理接收数据失败的情况
        //     perror("recvfrom error");
        //     return NULL;
        // }
        // printf("wea\n");
        // lv_label_set_text_fmt(ui_Label15, w.weather);
        // lv_label_set_text_fmt(ui_Label16, w.temper);
        // 构造日期命令字符串，用于更新系统时间
        char time_command[100];
        sprintf(time_command, "date -s \"%s %s\"", t.date, t.time);

        // 执行系统命令以更新时间
        system(time_command);

        // 输出时间更新提示信息
        printf("time update\n");
    }
}

/**
 * udp_sent函数负责通过UDP协议发送环境数据。
 * 该函数不接受参数。
 * 注意：该函数不会返回，直到程序被外部中断。
 */
void * udp_sent()
{
    hMsg h; // 定义一个hMsg结构体变量h，用于存储环境数据。

    // 创建UDP套接字
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketfd == -1) {
        perror("socket error"); // 如果创建套接字失败，输出错误信息。
        return;                 // 直接退出函数。
    }

    // 初始化目标地址结构
    struct sockaddr_in destiAddr;
    destiAddr.sin_addr.s_addr = inet_addr(DESTI_IP); // 设置目标IP地址。
    destiAddr.sin_family      = AF_INET;             // 设置地址族为Internet。
    destiAddr.sin_port        = htons(HOME_PORT);    // 设置目标端口。

    // 主循环，持续发送数据
    while(1) {
        // 将环境数据转换为字符串，填充到hMsg结构体中
        sprintf(h.homeAqi, "%d", aqi);           // 空气质量指数
        sprintf(h.homeTemp, "%.1f", temperture); // 温度
        sprintf(h.homeHum, "%d", humidity);      // 湿度

        // 发送数据到目标地址
        sendto(socketfd, &h, sizeof(h), 0, (struct sockaddr *)&destiAddr, sizeof(destiAddr));
        sleep(2); // 休眠2秒，控制发送频率。
    }

    // 关闭套接字，正常情况下该行代码不会被执行。
    close(socketfd);
    return; // 函数结束，不返回任何值。
}

void * weather_update_call()
{
    

    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    int flags    = fcntl(socketfd, F_GETFL, 0);
    fcntl(socketfd, F_SETFL, flags | O_NONBLOCK);
    if(socketfd == -1) {
        // 处理套接字创建失败的情况
        perror("socket error");
        return NULL;
    }

    // 初始化服务器地址结构
    struct sockaddr_in ownAddr;
    ownAddr.sin_addr.s_addr = inet_addr(OWNADDR);
    ownAddr.sin_family      = AF_INET;
    ownAddr.sin_port        = htons(WEATHER_PORT);

    // // 将套接字绑定到指定的地址和端口
    bind(socketfd, (struct sockaddr *)&ownAddr, sizeof(ownAddr));

    // 持续监听并处理来自客户端的时间信息
    // 接收UDP数据
    while(1) {
        int ret = recvfrom(socketfd, &w, sizeof(w), 0, NULL, NULL);
        printf("wea\n");
        if(ret == -1) {
            // 处理接收数据失败的情况
            // perror("recvfrom error");
            return NULL;
        }
        printf("weather: %s\n", w.weather);
        printf("temper: %s\n", w.temper);

        char tem_t[20];
        strcpy(tem_t, w.temper);
        char wea_t[20];
        strcpy(wea_t, w.weather);
        char *tem_t_p = tem_t;
        char *wea_t_p = wea_t;
        lv_label_set_text_fmt(ui_Label16, wea_t_p);
        lv_label_set_text_fmt(ui_Label15, tem_t_p);
        // 更新天气状态
        sleep(3);
    }
}

void * weather_update_pth()
{
    while(1) {
        lv_async_call(weather_update_call, NULL);
        sleep(60);
    }
    return NULL;
}


/**
 * @brief LED状态改变的函数
 *
 * 本函数通过与LED设备驱动程序通信来改变LED的状态。根据全局变量light的值，
 * 以不同的方式控制LED的亮度。使用字符设备"/dev/led_drv"进行读写操作。
 *
 * 注意：这里假设有一个全局变量light，表示LED的亮度级别，该变量决定了LED的点亮模式。
 */
void * change_led()
{
    // 缓冲区，用于与LED设备交互
    char buf[2];

    // 打开LED设备文件
    int fd = open("/dev/led_drv", O_RDWR);
    if(fd == -1) {
        // 如果打开设备失败，打印错误信息并返回
        perror("open");
        return -1;
    }

    // 无限循环，持续改变LED状态
    while(1) {
        // 根据light的值设置LED状态
        if(light == 0) {
            // 当light为0时，关闭所有LED
            buf[0] = 0;
            buf[1] = 8;
            write(fd, buf, sizeof(buf));
            buf[1] = 9;
            write(fd, buf, sizeof(buf));
            buf[1] = 10;
            write(fd, buf, sizeof(buf));

        } else if(light <= 33) {
            // 当light在1到33之间时，以特定方式点亮LED
            buf[0] = 0;
            buf[1] = 9;
            write(fd, buf, sizeof(buf));
            buf[1] = 10;
            write(fd, buf, sizeof(buf));
            buf[0] = 1;
            buf[1] = 8;
            write(fd, buf, sizeof(buf));
        } else if(light <= 66 && light > 33) {
            // 当light在34到66之间时，以另一种特定方式点亮LED
            buf[0] = 0;
            buf[1] = 10;
            write(fd, buf, sizeof(buf));
            buf[0] = 1;
            buf[1] = 9;
            write(fd, buf, sizeof(buf));
            buf[1] = 8;
            write(fd, buf, sizeof(buf));
        } else if(light > 66) {
            // 当light大于66时，完全打开所有LED
            buf[0] = 1;
            buf[1] = 8;
            write(fd, buf, sizeof(buf));
            buf[1] = 9;
            write(fd, buf, sizeof(buf));
            buf[1] = 10;
            write(fd, buf, sizeof(buf));
        }
        // 暂停500毫秒，以观察LED状态的变化
        usleep(500000);
    }

    // 关闭LED设备文件
    close(fd);
    // 函数不会正常返回，因为while(1)是无限循环
    return;
}

/**
 * change_led_flash函数用于控制LED的闪烁方式，根据空气质量指数(AQI)的不同，改变LED的闪烁频率和方式。
 * 该函数通过与LED驱动程序(/dev/led_drv)进行通信来实现对LED的控制。
 */
void * change_led_flash()
{
    // 定义一个缓冲区，用于向LED驱动发送控制命令
    char buf[2];

    // 打开LED驱动设备文件，O_RDWR表示以读写模式打开
    int fd = open("/dev/led_drv", O_RDWR);
    // 如果打开设备失败，打印错误信息并返回
    if(fd == -1) {
        perror("open");
        return -1;
    }

    // 无限循环，持续监控AQI并调整LED闪烁方式
    while(1) {
        // 根据AQI选择不同的LED闪烁方式
        if(aqi <= 50) {
            // AQI在0-50时，LED每秒闪烁一次
            buf[0] = 1;                  // 设置LED为开启状态
            buf[1] = 7;                  // 未知功能，可能保留给未来使用
            write(fd, buf, sizeof(buf)); // 向LED驱动发送开启命令
            usleep(500000);              // 等待500毫秒
            buf[0] = 0;                  // 设置LED为关闭状态
            write(fd, buf, sizeof(buf)); // 向LED驱动发送关闭命令
            usleep(500000);              // 再次等待500毫秒
        } else if(aqi <= 100 && aqi > 50) {
            // AQI在51-100时，LED每半秒闪烁一次，共闪烁4次
            for(int i = 0; i < 4; i++) {
                buf[0] = 1;                  // 开启LED
                write(fd, buf, sizeof(buf)); // 发送开启命令
                usleep(250000);              // 等待250毫秒
                buf[0] = 0;                  // 关闭LED
                write(fd, buf, sizeof(buf)); // 发送关闭命令
                usleep(250000);              // 再次等待250毫秒
            }
        } else if(aqi > 100 && aqi <= 150) {
            // AQI在101-150时，LED每200毫秒闪烁一次，共闪烁5次
            for(int i = 0; i < 5; i++) {
                buf[0] = 1;                  // 开启LED
                write(fd, buf, sizeof(buf)); // 发送开启命令
                usleep(200000);              // 等待200毫秒
                buf[0] = 0;                  // 关闭LED
                write(fd, buf, sizeof(buf)); // 发送关闭命令
                usleep(200000);              // 再次等待200毫秒
            }
        } else if(aqi > 150 && aqi <= 200) {
            // AQI在151-200时，LED每100毫秒闪烁一次，共闪烁10次
            for(int i = 0; i < 10; i++) {
                buf[0] = 1;                  // 开启LED
                write(fd, buf, sizeof(buf)); // 发送开启命令
                usleep(100000);              // 等待100毫秒
                buf[0] = 0;                  // 关闭LED
                write(fd, buf, sizeof(buf)); // 发送关闭命令
                usleep(100000);              // 再次等待100毫秒
            }
        } else if(aqi > 200 && aqi <= 300) {
            // AQI在201-300时，LED每50毫秒闪烁一次，共闪烁20次
            for(int i = 0; i < 20; i++) {
                buf[0] = 1;                  // 开启LED
                write(fd, buf, sizeof(buf)); // 发送开启命令
                usleep(50000);               // 等待50毫秒
                buf[0] = 0;                  // 关闭LED
                write(fd, buf, sizeof(buf)); // 发送关闭命令
                usleep(50000);               // 再次等待50毫秒
            }
        } else if(aqi > 300) {
            // AQI大于300时，LED每25毫秒闪烁一次，共闪烁40次
            for(int i = 0; i < 40; i++) {
                buf[0] = 1;                  // 开启LED
                write(fd, buf, sizeof(buf)); // 发送开启命令
                usleep(25000);               // 等待25毫秒
                buf[0] = 0;                  // 关闭LED
                write(fd, buf, sizeof(buf)); // 发送关闭命令
                usleep(25000);               // 再次等待25毫秒
            }
        }
    }
    // 关闭LED驱动设备文件
    close(fd);
    // 函数结束，返回空指针
    return;
}