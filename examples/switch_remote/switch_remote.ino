/**
 * @example switch_remote.ino
 * 
 * @par Show how to use API of class Switch
 *
 * The sequence:
 *      -# create an object. 
 *      -# connect to internet for later communication. 
 *      -# initialize the device(atctually, connect to sever). 
 *      -# now, it can be update or query the parameter. 
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/14
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

/* Include all device classes supported by IoTgo Library */
#include <IoTgo_device.h>
#include <SoftwareSerial.h>
#include <Arduino.h>

#ifdef NET_USE_GSM
#endif

#ifdef NET_USE_W5X00
#include <SPI.h>
#endif

/* 
 * An identifier of device which has been created and belongs to
 * a user registed to IoTgo platform. The first two letters indicates
 * the categories: 
 *  00 : Customized device which can has as many as you want
 *       parameters and the corresponding values.
 *  01 : Switch which has at least one parameter: 
 *       "switch" with value "on" or "off". 
 *  02 : Light which has at least one parameter:
 *       "light" with value "on" or "off". 
 *  03 : THSensor which has at least two parameters: 
 *       "temperature" with value "xx.xx" in Celsius, 
 *       "humidity" with value "xx.xx" in persentage. 
 */
#define SWITCH_ID            "0100000006"

/*
 * If you have a finished product, a check code or apikey-like code or 
 * factory apikey will be found around it. We are naming the code 
 * "SWITCH_CHECK_CODE". Here is empty. 
 */
#define SWITCH_CHECK_CODE    ""

/* An unique identifier of user registed on IoTgo platform */
#define SWITCH_APIKEY        "d8742379-9aca-45d9-8ff4-f4caf68156fa"


#ifdef NET_USE_ESP8266

/* 
 * The SSID (more generally, WiFi's name) for accessing to internet.
 * Maybe you need to replace "ITEAD" with yours. 
 */
#define WIFI_SSID           "ITEAD"

/* 
 * The SSID's password
 * Is it the same to yours? if not, change it.
 */
#define WIFI_PASS           "12345678"
ESP8266 eth;

#endif

#ifdef NET_USE_GSM
Gsm eth;

#endif

#ifdef NET_USE_W5X00
//#define IOT_SERVER          "172.16.7.6"     //Request by the IP address
#define reset_pin           (47)

EthW5X00 eth;
uint8_t mac[] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

#endif


/*
 * IP address or domain name of IoTgo platform servers. 
 * Maybe you need to change it.
 */
#define IOT_SERVER          "iotgo.iteadstudio.com"
#define IOT_DOMAIN_NAME     "iotgo.iteadstudio.com"

#define SWITCH_PIN           (13)

Switch sw(&eth, SWITCH_PIN);

void setup()
{
    const char *apikey;
    
    Serial.begin(9600);
    
#ifdef NET_USE_ESP8266
    if (!eth.connectWiFi(WIFI_SSID, WIFI_PASS))
    {
        Serial.println("connectWiFI error and halt...");
        while(1);
    }
    
#endif

#ifdef NET_USE_GSM
    if(!eth.intialGSM())
    {
        Serial.println(" Network error and halt....... ");
        while(1);
    }

#endif

#ifdef NET_USE_W5X00
    pinMode(reset_pin, OUTPUT);
    digitalWrite(reset_pin, LOW);
    delay(1000);
    digitalWrite(reset_pin, HIGH);
    delay(1000);
    
    if(!eth.configW5X00(mac))
    {
        Serial.println(" Config W5X00 and initial, So halt......");
        while(1);
    }
    
#endif

    sw.setHost(IOT_SERVER, IOT_DOMAIN_NAME);

    Serial.println("Connecting device to server...");
    apikey = sw.init(SWITCH_ID, SWITCH_APIKEY);
    //apikey = sw.init(SWITCH_ID, SWITCH_CHECK_CODE, DEVICE_PRODUCT);
    Serial.print("apikey = ");
    Serial.println(apikey);    
    
    Serial.println("setup done.");
}

void loop()
{
    static int32_t counter = 0;
    Serial.print("\ncounter = ");
    Serial.println(++counter);
    int32_t ret;

    ret = sw.sync();
    if(!ret)
    {
        Serial.println("sync okay");
    }
    else
    {
        Serial.println("sync() err!");
    }
    
    delay(1000);
}
