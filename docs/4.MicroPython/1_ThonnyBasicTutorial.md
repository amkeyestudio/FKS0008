# 4.1 Thonny Basic Tutorial

MicroPython is a streamlined implementation of the Python 3 language, which includes a small part of the Python standard library and is optimized to run in microcontrollers and restricted environments. Here are the main features of MicroPython:

1. **Compatibility** : MicroPython strives to be as compatible as possible with regular Python (referred to as CPython), which means that if you know Python, you have already mastered the basics of MicroPython.
2. **Hardware Access** : In addition to implementing a series of core Python libraries, MicroPython also includes modules such as “machines” for accessing low-level hardware, enabling developers to directly control the hardware resources of microcontrollers.
3. **Interactive Prompt (REPL)** : MicroPython offers an interactive prompt (REPL) that enables users to execute commands directly from a desktop computer on an embedded platform. This is very useful for the rapid real-time testing and debugging of embedded systems.
4. **Multi-threading support** : The MicroPython firmware supports multi-threading functionality, which enables a single microcontroller to handle multiple embedded tasks simultaneously, thereby accelerating the execution of embedded applications.
5. **Open-source Project** : MicroPython is an open-source project, and its source code can be obtained on Github. It follows the MIT license and can be used freely for educational and commercial purposes.
6. **Wide Support** : MicroPython supports a variety of microcontroller boards and RTOS (Real-time Operating System), such as ESP32, ESP8266, STM32, ESP32S3, etc. In addition, it also offers a rich library and modules to meet different development needs.

## 4.1.1 Download Thonny

### 4.1.1.1 For Windows

**Operating System: Windows 10**

Enter [Thonny IDE official](https://thonny.org) to download the Thonny for Windows version. 

![1101](./media/thonny-1-1789957334188-1.png)

![1102](./media/thonny-2-1789957334188-2.png)

### 4.1.1.2 For MAC

Please refer to Windows.

![1103](./media/thonny-3-1789957334189-6.png)

![1104](./media/thonny-4-1789957334189-3.png)

## 4.1.2 Install Thonny

Thonny supports two installation methods:

- Thonny+Python package

  Python IDE and Thonny are packaged together and can be directly installed and used. It is fast and easy so is recommended for beginners. There are also two ways to install this package:

  - Installer

    ![1201](./media/thonny-5-1789957334189-4.png)

  - Portable variant

    ![1202](./media/thonny-6-1789957334189-5.png)

- Thonny only

  Thonny is actually a package of python. When the user already has a python environment, click `pip install thonny` to install Thonny only, which is more suitable for developers.

  ![1203](./media/thonny-7-1789957334189-7.png)

Please download the corresponding version according to your requirements.

### 4.1.2.1 Installer

Download the app according to your operation system. Here we demonstrate under <span style="background:#ff0;color:#000">64bit Windows 10</span> to download `Installer with 64-bit Python 3.10`.

(1) Click ![1204](./media/thonny-8-1789957334189-8.png) and you will see “Select Setup Install Mode”. Choose **Install for me only**.

![1205](./media/thonny-9-1789957334189-9.png)

(2) **Next**.

![1206](./media/thonny-10-1789957334189-10.png)

(3) Tick **I accept the agreement** and Click **Next**.

![1207](./media/thonny-11-1789957334189-11.png)

(4) The default installation path is in Disk C. You may click **Browse...** to choose another disk. And then click **Next**.

![1208](./media/thonny-12-1789957334189-12.png)

(5) By default, a shortcut to the program is created in the Start menu folder. Click **Browse...** to set another path for the shortcut and **Next**.

![1209](./media/thonny-13-1789957334189-13.png)

(6) Tick **Create desktop icon** and **Next**.

![1210](./media/thonny-14-1789957334189-14.png)

(7) Install.

![1211](./media/thonny-15-1789957334189-15.png)

(8) “**Finish**”!

![1212](./media/thonny-16-1789957334189-16.png)

(9) Back to the desktop and start Thonny to choose a language.

![1213](./media/thonny-17-1789957334189-17.png)

(10) Done.

![1214](./media/thonny-18-1789957334189-18.png)

### 4.1.2.2 Portable Variant

Download the app according to your operation system. Here we demonstrate under <span style="background:#ff0;color:#000">64bit Windows 10</span> to download `Installer with 64-bit Python 3.10`.

(1) Click ![1105](./media/thonny-19-1789957334189-19.png) to start Thonny and choose a language.

![1213](./media/thonny-17-1789957334189-17.png)

(2) Done.

![1214](./media/thonny-18-1789957334189-18.png)

## 4.1.3 Burn Firmware (Important)

To run MicroPython code on ESP32S3, it is necessary to burn MicroPython firmware to ESP32S3.

A. 将ESP32S3开发板通过USB线连接到电脑

B.点击`Tools`-->`Options...`

![image-20260921103027794](./media/image-20260921103027794.png)

C. 点击`lnterpreter`选项，然后选择MicroPython版本为`MicroPython（ESP32）`

![image-20260921103359047](./media/image-20260921103359047.png)

D. 选择串口COM10，请注意你的串口不一定是COM10，请根据实际串口选择

![image-20260921103541448](./media/image-20260921103541448.png)

E. 点击`lnstall or update MicroPython(esptool)`进入固件选择页面（请注意这一步一定要有串口）

![image-20260921103724617](./media/image-20260921103724617.png)

F. 按照下方图片选择好各个选线，`version`尽量选择`1.28.0`版本因为教程是基于这个版本制作的，选择完成后点击`lnstall`

![image-20260921103926309](./media/image-20260921103926309.png)

G. 等待下载完成即可

![image-20260921104115510](./media/image-20260921104115510.png)

H. 下载完成后返回编程页面，点击![image-20260921104240910](./media/image-20260921104240910.png)图标即可连接上ESP32S3开发板，连接成功会在左下角出现一个MciroPython device的功能框如下图：

![image-20260921104343156](./media/image-20260921104343156.png)

我们就成功烧录固件了！！

## 4.1.4 Homepage

Click **View** and tick **Files** to open the file manager.

![1401](./media/thonny-26-1789957334190-24.png)

![1402](./media/thonny-27-1789957334190-25.png)

## 4.1.5 Toolbar

![1403](./media/thonny-28-1789957334190-26.png)

|              Icon              |       Function       |
| :----------------------------: | :------------------: |
| ![1404](./media/thonny-29.png) |         New          |
| ![1405](./media/thonny-30.png) |       Open...        |
| ![1406](./media/thonny-31.png) |         Save         |
| ![1407](./media/thonny-32.png) |  Run current script  |
| ![1408](./media/thonny-33.png) | Debug current script |
| ![1409](./media/thonny-34.png) |      Step over       |
| ![1410](./media/thonny-35.png) |      Step into       |
| ![1411](./media/thonny-36.png) |       Step out       |
| ![1412](./media/thonny-37.png) |        Resume        |
| ![1413](./media/thonny-38.png) | Stop/Restart backend |



## 4.1.6 Connect ESP32S3 to Thonny

Connect the ESP32S3development board to the computer, then set the development board to ESP32S3and select the port. 

Click `Tools`-->`Options...`

![thonny-42](./media/thonny-42-1789957334190-27.png)

Select `Interpreter` , then set the development board to `MicroPython(Raspberry Pi ESP32S3)`, and choose the ports automatically recognized, and click `OK`.

![image-20260921105415779](./media/image-20260921105415779.png)

After connection, “ MicroPython device” will appear on the left side of Thonny and there will also be a prompt in the lower right corner.

![image-20260921105433844](./media/image-20260921105433844.png)

## 4.1.7 Load Code from Computer



Click Files –> This computer, open the file `MicroPython_Resources`-->`codes`.

![image-20260921105900734](./media/image-20260921105900734.png)



Enter `codes` and you can see the codes. Click it to open the file in Thonny.

![image-20260921110111092](./media/image-20260921110111092.png)



## 4.1.8 Upload Library to ESP32S3

Errors may be reported during running code without saving the library to ESP32S3.

![image-20260921110343623](./media/image-20260921110343623.png)



**Add library:** 

In Files, open `MicroPython_Resources` –> `lib` to check library:

![image-20260921110410995](./media/image-20260921110410995.png)



Choose `Sengo1.py` to “Upload to/”.

![image-20260921110442246](./media/image-20260921110442246.png)



After that, `ESP32S3_4WD_Car.py` is displayed in “MicroPython device”.

![image-20260921110637324](./media/image-20260921110637324.png)

## 4.1.9 Online Running

Here we take code file “4_1_Buzzer.py” as an example to run online.

Open “4_1_Buzzer.py” in Files and click ![1407](./media/thonny-32-1789957334190-36.png). Note that wrong wiring of Sengo1 AI and ESP32S3will cause errors in output. If you want to stop current run, just click ![1413](./media/thonny-38-1789957334190-37.png).

![thonny-48](./media/thonny-48-1789957334190-38.png)

## 4.1.10 Offline Running

Here we use “4_1_Buzzer.py” again. 

Click ![thonny-50](./media/thonny-50-1789957334190-39.png) followed by “MicroPython device” and select “New file...”.

![image-20260921110913513](./media/image-20260921110913513.png)



Name it to `boot.py`. (Note: It must be `boot.py`. This name cannot be changed or entered incorrectly.)

![image-20260921111102122](./media/image-20260921111102122.png)

The file was created successfully.

![image-20260921111124912](./media/image-20260921111124912.png)

Open the “4_1_Buzzer.py” code file and then press “Ctrl + A” and “Ctrl + C”. Open “main.py” to “Ctrl + V” in it.

![image-20260921111142693](./media/image-20260921111142693.png)

Click Save.

![image-20260921111203367](./media/image-20260921111203367.png)

Saved successfully. Now, when the ESP32S3development board is powered on again, the code in `boot.py` will run automatically. (Note: When saving, you need to stop the running of the online code; otherwise, it may fail to be saved.)

![image-20260921111244913](./media/image-20260921111244913.png)
