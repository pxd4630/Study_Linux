#include<iostream>
#include <sys/timerfd.h>
#include <unistd.h>
//主要是处理非活跃连接(析构对象)
//那如果大量连接到来,又该如何处理--->时间轮思想(二维数组) 创建多个二维数组进行管理(划分等级) 天,时,分,秒(到秒级直接处理该非活跃任务)
//效率太低-->?怎么提高效率-->智能指针(处理活跃连接的销毁事件)
int main()
{
    //1.创建一个定时器
    //定时器类型,定时器的行为
    int timerfd=timerfd_create(CLOCK_MONOTONIC,0);
    //创建定时器失败
    if(timerfd<0)
    {
        perror("create timer error!");
        exit(0);
    }
    struct itimerspec itm;

    //it_value: 表示定时器的初始到期时间。
    //如果设置为 0，则定时器被禁用。
    //如果设置了非零值，则定时器会在指定的时间后第一次触发。

    // it_interval: 表示定时器的周期。
    // 如果设置为 0，则定时器只触发一次（单次定时器）。
    // 如果设置为非零值，则定时器会按照指定的周期重复触发。

    itm.it_value.tv_sec=3;//设置第一次的超时时间
    itm.it_value.tv_nsec=0;
    itm.it_interval.tv_sec=3;
    itm.it_interval.tv_nsec=3;
    // int timerfd_settime(int fd, int flags,
    //     const struct itimerspec *new_value,
    //     struct itimerspec *old_value);
    //2.启动定时器
    timerfd_settime(timerfd,0,&itm,NULL);

    //计时器,重置
    while(1)
    {
        uint64_t times;
        int ret=read(timerfd,&times,8);
        if(ret<0)
        {
            perror("read error");
            return-1;
        }
        std::cout<<times<<std::endl;
        printf("超时了,距离上次超时了%lu次\n",times);
        sleep(10);
    }
    close(timerfd);
    return 0;
}