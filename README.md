Wireless IAP
===

在STM32F4开发板上实现的基于串口的在线应用编程，与PC端采用ESP8266模块进行无线连接，再在电脑端将二进制的应用代码下载到单片机中，从而实现无线下载程序。

ESP8266目录里包含了ESP8266模块的指令说明文档、程序烧写工具和STM32-F407的测试程序，另三个文件夹是IAP下载程序。

Contents
---
- [Wireless IAP](#wireless-iap)
  - [Contents](#contents)
  - [用AT指令对ESP8266进行配置](#%e7%94%a8at%e6%8c%87%e4%bb%a4%e5%af%b9esp8266%e8%bf%9b%e8%a1%8c%e9%85%8d%e7%bd%ae)
      - [1. 两块直连WiFi模块的透传配置](#1-%e4%b8%a4%e5%9d%97%e7%9b%b4%e8%bf%9ewifi%e6%a8%a1%e5%9d%97%e7%9a%84%e9%80%8f%e4%bc%a0%e9%85%8d%e7%bd%ae)
      - [2. ESP8266作为AP，智能终端作为STA接入AP](#2-esp8266%e4%bd%9c%e4%b8%baap%e6%99%ba%e8%83%bd%e7%bb%88%e7%ab%af%e4%bd%9c%e4%b8%basta%e6%8e%a5%e5%85%a5ap)
      - [注意事项](#%e6%b3%a8%e6%84%8f%e4%ba%8b%e9%a1%b9)
    - [参考指令](#%e5%8f%82%e8%80%83%e6%8c%87%e4%bb%a4)
    
## 用AT指令对ESP8266进行配置
ESP8266用作无线串口的配置

#### 1. 两块直连WiFi模块的透传配置

- 模块A：  
作为AP时产生ESP8266TX热点；   
作为STA时连接到模块B产生的热点ESP8266RX  
端口号：8080  
IP：192.168.4.1   

```
+++            //退出透传  
AT+CWMODE_DEF=3 //AP+STA模式  
AT+CWSAP_DEF="ESP8266TX","2017champion",1,4  //配置SoftAP参数       
AT+CWLAP        //列出附近的AP       
AT+CWJAP_DEF="ESP8266RX","2017champion"     //连接AP,保存到flash     
AT+CIPSTART="UDP","192.168.4.2",8086,8080,0  //建立连接（TCP,UDP,SSL)        
AT+CIPMUX=0                                  //设置为单连接       
AT+SAVETRANSLINK=1,"192.168.4.2",8086,"UDP",8080  
```  

模块B：  
作为AP时产生ESP8266RX热点；
作为STA时连接到模块A产生的热点ESP8266TX   
端口号：8086  
IP：192.168.4.2    
```
+++             //退出透传     
AT+CWMODE_DEF=3 //AP+STA模式      
AT+CWSAP_DEF="ESP8266RX","2017champion",1,4  //配置SoftAP参数       
AT+CWLAP        //列出附近的AP       
AT+CWJAP_DEF="ESP8266TX","2017champion"     //连接AP,保存到flash     
AT+CIPSTART="UDP","192.168.4.1",8080,8086,0  //建立连接（TCP,UDP,SSL)        
AT+CIPMUX=0                                  //设置为单连接       
AT+SAVETRANSLINK=1,"192.168.4.1",8080,"UDP",8086 
```   

#### 2. ESP8266作为AP，智能终端作为STA接入AP
硬件连接：
- PB11-GBC_TX
- PB12-GBC_RX

STM32F407:   
测试时将2_USART程序下载到STM32F407开发板

PC:调试串口助手配置方法
```
+++        
AT+CWMODE_DEF=2     
//以下两行二选一      
AT+CWSAP_DEF="ESP8266EX","2017champion",1,4  //多连接,广播SSID   
AT+CWSAP_DEF="ESP8266EX","2017champion",1,4,1,1 //(AT指令固件要在1.5.4版本以上)  单连接,隐藏SSID  
AT+CIPMUX=0     
AT+SAVETRANSLINK=1,"192.168.4.2",8080,"TCP"     
```

配置完后，可在Android端用网络调试助手测试效果。 
用手机上的网络调试助手即可接收到1-100的循环数据  


#### 注意事项
1. *若出现不正常可先进行恢复出厂设置再试*        
2. *刷新AT固件时将小模块剩余两个引脚用跳帽线相互连接，然后按“AT指令烧写及说明文件中指示操作”*       
3. *连接隐藏SSID时，如出现添加SSID后无作任何响应时，可先改成广播SSID，在移动端测试连接正常后，记住wifi,并设为自动连接，然后再把模块改成隐藏SSID模式即可*        
4. *连接隐藏SSID时，若出现以下情况：能看到wifi信号显示已保存状态，但是却始终连不上。原因可能是已经有其它设备连接成功（只能同时保持1个station连接），此时要么断开其它连接，要么修改密码*
5. *隐藏SSID使用建议：wifi名字最好只有使用者知道，密码最好只有开发者知道*


### 参考指令
基础指令

```
ATE0         //关闭回显     
AT           //测试AT启动       
AT+RST       //重启       
AT+GMR       //查询版本信息       
AT+RESTORE   //恢复出厂设置  
```     

WiFi功能指令:TCP/IP指令        

```
AT+CIPSTATUS //查询网络连接信息     
AT+CIPMODE=1        
AT+CIPSEND   //发送数据     
AT+CIPCLOSE  //关闭传输 
```    