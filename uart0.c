/*---------------------------------UART0_C---------------------------------*/

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <stdint.h>
#include <stdio.h>

static const eUSCI_UART_Config uartConfig = //UART_CONFIG
{
  EUSCI_A_UART_CLOCKSOURCE_SMCLK,          //选用SMCLK（1M）时钟源
  26,                                      // BRDIV = 26 ，clockPrescalar时钟分频系数
  0,                                       // UCxBRF = 8  firstModReg （BRDIV、UCxBRF、 UCxBRS和SMCLK，用于设置串口波特率）
  111,                                       // UCxBRS = 17 secondModReg
  EUSCI_A_UART_NO_PARITY,                  // 校验位None
  EUSCI_A_UART_LSB_FIRST,                  // 低位优先，小端模式
  EUSCI_A_UART_ONE_STOP_BIT,               // 停止位1位
  EUSCI_A_UART_MODE,                       // UART mode
  EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // 设置为过采样，该数值为1
};

void UART0_init(void)
{
  /* P1.2和P1.3选用UART模式并分别作为UCA0_RXD和UCA0_TXD */
  MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
  /* 使用uartConfig配置UART_A0 */
  MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
  /* 使能UART_A0 */
  MAP_UART_enableModule(EUSCI_A0_BASE);

}

void UART0_send_str(char *string){ // send string by UART
  for(uint32_t i=0;string[i];i++){
    MAP_UART_transmitData(EUSCI_A0_BASE,string[i]);
  }
}

void UART0_send_int(int x){
  char str[100];
  sprintf(str,"%d",x);
  for(uint32_t i=0;str[i];i++){
    MAP_UART_transmitData(EUSCI_A0_BASE,str[i]);
  }
}

void UART0_send_float(float x){ // send string by UART
  char str[100];
  int f = (int)x;
  int b = (int)((x-f)*1000);
  sprintf(str,"%d.%d",f,b);
  for(uint32_t i=0;str[i];i++){
    MAP_UART_transmitData(EUSCI_A0_BASE,str[i]);
  }
}
