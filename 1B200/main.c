/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Loongson 1B Bare Program, Sample main file
 */

#include <stdio.h>
#include <string.h>

#include "ls1b.h"
#include "mips.h"

#include "src\press\press.h"
#include "src\i2c\i2c.h"
#include "src\link\link.h"
#include "src\uart\uart.h"
#include "ls1x_rtc.h"

//-------------------------------------------------------------------------------------------------
// BSP
//-------------------------------------------------------------------------------------------------

#include "bsp.h"

//RTCʱ��
struct tm tmp, now = {
    .tm_sec = 30,
    .tm_min = 35,
    .tm_hour = 10,
    .tm_mday = 14,
    .tm_mon = 5,
    .tm_year = 2023,
};

//-------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------

int main(void)
{   
    int fire = 0,cnt = 0;
    float temp = 0,hum = 0,press = 0,eleva = 0;
    char loc[50];
    strcpy(loc,"112.342473&16.842207");
    I2C1_init();
    Get_HDC_ID();
    TSL_init();
    SPL06_init();


    // ��ʼ������
    UART4_Config_Init();
    UART5_Config_Init();
    //-----------------------
    // ��ȡ����ʱ��
    //-----------------------
    // ��ʼ��RTC������
    ls1x_rtc_init(NULL, NULL);
    // ����RTCʱ��
    ls1x_rtc_set_datetime(&now);

    /*
     * �����ѭ��
     */

    for (;;)
    {
        // RTC��ȡʱ��
        ls1x_rtc_get_datetime(&tmp);
        sprintf(t1, "[\"%d\",\"%d\",\"%d\"]", tmp.tm_year + 1900,tmp.tm_mon + 1, tmp.tm_mday);
        sprintf(t2, "[\"%d\",\"%d\",\"%d\"]", tmp.tm_hour, tmp.tm_min, tmp.tm_sec);



        //��ȡ��ʪ��
        HDC_Get_Temp_Hum(&temp, &hum);
        //��ȡ��ѹ
        SPL06_Get_Prs(&press, &eleva);

        sprintf(t3,"{\"F1\":\"%f\",\"F2\":\"%f\",\"F3\":\"%f\",\"F4\":\"%s\",\"F5\":\"%d\"}",temp,hum,press,loc,fire);

        // ƴ�����ݰ�
        link(t1, t2, t3);
        // ���ڷ���json��ʽ���ݰ�
        UART4_Test(result);
        UART5_Test(result);
        // result�ڴ��ͷţ��������
        memset(result, 0, sizeof(result));
        delay_ms(5000);
    }

    /*
     * Never goto here!
     */
    return 0;
}

/*
 * @@ End
 */
