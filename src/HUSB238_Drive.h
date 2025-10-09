#ifndef HUSB238_Drive_H
#define HUSB238_Drive_H

#include "gd32f10x.h"

/*参考https://www.hynetek.com/uploadfiles/site/219/news/c3afa59e-1c03-4b92-8525-873c152bae4b.pdf*/
#define HUSB238_ADDR        0x08  // HUSB238 I2C地址

#define PD_STATUS0 0x00  // 假设状态寄存器地址
#define PD_STATUS1 0x01  // 假设状态寄存器地址
#define SRC_PDO_5V 0x02  // 假设5V输出电压选择寄存器值
#define SRC_PDO_9V 0x03  // 假设9V输出电压选择寄存器值
#define SRC_PDO_12V 0x04 // 假设12V输出电压选择寄存器值
#define SRC_PDO_15V 0x05 // 假设15V输出电压选择寄存器值
#define SRC_PDO_18V 0x06 // 假设20V输出电压选择寄存器值
#define SRC_PDO_20V 0x07 // 假设20V输出电压选择寄存器值
#define SRC_PDO 0x08 // 假设0V输出电压选择寄存器值
#define GO_COMMAND 0x09 // 假设命令寄存器地址

/*PD_STATUS0[7:4]   default：0000*/
#define PD_5V 0001b // 5V输出电压选择位
#define PD_9V 0010b // 9V输出电压选择位
#define PD_12V 0011b // 12V输出电压选择位
#define PD_15V 0100b // 15V输出电压选择位
#define PD_18V 0101b // 18V输出电压选择位
#define PD_20V 0110b // 20V输出电压选择位

/*PD_STATUS0[3:0]   default：0000*/
#define PD_0_5A 0000b // 0.5A输出电流选择位
#define PD_0_7A 0001b // 0.7A输出电流选择位
#define PD_1_A 0010b // 1A输出电流选择位
#define PD_1_25A 0011b // 1.25A输出电流选择位
#define PD_1_5A 0100b // 1.5A输出电流选择位
#define PD_1_75A 0101b // 1.75A输出电流选择位
#define PD_2_A 0110b // 2A输出电流选择位
#define PD_2_25A 0111b // 2.25A输出电流选择位
#define PD_2_5A 1000b // 2.5A输出电流选择位
#define PD_2_75A 1001b // 2.75A输出电流选择位
#define PD_3_A 1010b // 3A输出电流选择位
#define PD_3_25A 1011b // 3.25A输出电流选择位
#define PD_3_5A 1100b // 3.5A输出电流选择位
#define PD_4_A 1101b // 4A输出电流选择位
#define PD_4_5A 1110b // 4.5A输出电流选择位
#define PD_5_A 1111b // 5A输出电流选择位

/*PD_STATUS1[7] CC_DIR  default：0*/
#define PD_CC_DIR 1b // 已连接CC线
#define PD_CC_NO_DIR 0b // 未连接CC线
/*PD_STATUS1[6] ATTACH  default：0*/
#define PD_ATTACH  0b // HUSB238 is in unattached mode
#define PD_ATTACH_NO 1b // HUSB238 is in modes other than unattached mode
/*PD_STATUS1[5:3] PD_RESPONSE   default：000*/
#define PD_RESPONSE_NO 000b // 无响应
#define PD_RESPONSE_1 001b // 响应1 Success
#define PD_RESPONSE_2 011b // 响应2 Invalid command or argument
#define PD_RESPONSE_3 100b // 响应3 Command not supported
#define PD_RESPONSE_4 101b // 响应4 Transaction fail. No GoodCRC is received after sending
/*PD_STATUS1[2] 5V_VOLTAGE Voltage information of 5V contract default：0*/
#define PD_5V_VOLTAGE 1b // 5V输出电压
#define PD_5V_VOLTAGE_NO 0b // 其他电压输出 
/*PD_STATUS1[1:0] 5V_CURRENT Current information of 5V contract default：00*/
#define PD_5V_CURRENT_DFC 00b // Default current
#define PD_5V_CURRENT_1_5A 01b // 1.5A输出电流
#define PD_5V_CURRENT_2_4A 10b // 2.4A输出电流
#define PD_5V_CURRENT_3A 11b // 3A输出电流

/*SRC_PDO_5V[7] SRC_5V_DETECT   default：0*/
#define SRC_5V_DETECT 1b // 检测到5V输出
#define SRC_5V_DETECT_NO 0b // 其他电压输出
/*SRC_PDO_12V[7] SRC_12V_DETECT   default：0*/
#define SRC_12V_DETECT 1b // 检测到12V输出
#define SRC_12V_DETECT_NO 0b // 其他电压输出
/*SRC_PDO_15V[7] SRC_15V_DETECT   default：0*/
#define SRC_15V_DETECT 1b // 检测到15V输出
#define SRC_15V_DETECT_NO 0b // 其他电压输出
/*SRC_PDO_18V[7] SRC_18V_DETECT   default：0*/
#define SRC_18V_DETECT 1b // 检测到18V输出
#define SRC_18V_DETECT_NO 0b // 其他电压输出
/*SRC_PDO_20V[7] SRC_20V_DETECT   default：0*/
#define SRC_20V_DETECT 1b // 检测到20V输出
#define SRC_20V_DETECT_NO 0b // 其他电压输出

/*SRC_PDO_5V[3:0] SRC_5V_CURRENT   default：0000*/
/*SRC_PDO_12V[3:0] SRC_12V_CURRENT   default：0000*/
/*SRC_PDO_15V[3:0] SRC_15V_CURRENT   default：0000*/
/*SRC_PDO_18V[3:0] SRC_18V_CURRENT   default：0000*/
/*SRC_PDO_20V[3:0] SRC_20V_CURRENT   default：0000*/

/*SRC_PDO[7:4] PDO_SELECT  TYPE:RW default：0000*/
#define SRC_PDO_0V 0000b // 未选择输出电压选择位
#define SRC_PDO_5V 0001b // 选择5V输出电压选择位
#define SRC_PDO_9V 0010b // 选择9V输出电压选择位
#define SRC_PDO_12V 0011b // 选择12V输出电压选择位
#define SRC_PDO_15V 0100b // 选择15V输出电压选择位
#define SRC_PDO_18V 1001b // 选择18V输出电压选择位
#define SRC_PDO_20V 1010b // 选择20V输出电压选择位

/*GO_COMMAND[4:0] COMMAND_FUNC  TYPE:RW  default：00000*/
#define GO_1 00000b // 状态1 Requests the PDO set by PDO_SELECT
#define GO_2 00100b // 状态2 Send out Get_SRC_Cap command 
#define GO_3 10000b // 状态3 Send out hard reset command

#define HUSB238_I2C         I2C1
#define HUSB238_I2C_SCL_PIN GPIO_PIN_8
#define HUSB238_I2C_SDA_PIN GPIO_PIN_9
#define HUSB238_I2C_PORT    GPIOB

#endif /* HUSB238_Drive_H */

