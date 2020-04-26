/**
* \brief Main source file for the I2C-Master project.
*
* In this project we set up a I2C master device with
* to understand the I2C protocol and communicate with a
* a I2C Slave device (LIS3DH Accelerometer).
*
* \author Gabriele Belotti
* \date , 2020
*/

// Include required header files
#include "I2C_Interface.h"
#include "InterruptRoutines.h"
#include "project.h"
#include "stdio.h"
#include "stdlib.h"

/**
*   \brief 7-bit I2C address of the slave device.
*/
#define LIS3DH_DEVICE_ADDRESS 0x18


/**
*   \brief Address of the Status register
*/
#define LIS3DH_STATUS_REG 0x27

#define LIS3DH_STATUS_REG_DATA_AVAILABLE 0x4 //X, Y and Z axis new data available 

/**
*   \brief Address of the Control register 1
*/
#define LIS3DH_CTRL_REG1 0x20

#define LIS3DH_CTRL_REG1_SET 0x57  //set the accelerometer to output 3-Axis data in normal mode at 100Hz 


/**
*   \brief Address of the Control register 4
*/
#define LIS3DH_CTRL_REG4 0x23

#define LIS3DH_CTRL_REG4_BDU_ACTIVE 0x80 //output registers not updated until MSB and LSB reading 


#define LIS3DH_OUT_X_L  0x28

#define LIS3DH_OUT_X_H  0x29

#define LIS3DH_OUT_Y_L  0x2A

#define LIS3DH_OUT_Y_H  0x2B

#define LIS3DH_OUT_Z_L  0x2C

#define LIS3DH_OUT_Z_H  0x2D



int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_Peripheral_Start();
    UART_Debug_Start();
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
    // String to print out messages on the UART
    char message[50];

    // Check which devices are present on the I2C bus
    for (int i = 0 ; i < 128; i++)
    {
        if (I2C_Peripheral_IsDeviceConnected(i))
        {
            // print out the address is hex format
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_Debug_PutString(message); 
        }
        
    }
    
    /******************************************/
    /*            I2C Reading                 */
    /******************************************/
    
 
    ErrorCode error;
    
    /*      I2C Reading Status Register       */
    
    uint8_t status_register; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_STATUS_REG,
                                        &status_register);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "STATUS REGISTER: 0x%02X\r\n", status_register);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read status register\r\n");   
    }
    
    /******************************************/
    /*        Read Control Register 1         */
    /******************************************/
    uint8_t ctrl_reg1; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1: 0x%02X\r\n", ctrl_reg1);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
    /******************************************/
    /*            I2C Writing                 */
    /******************************************/
    
        
    UART_Debug_PutString("\r\nWriting new values..\r\n");
    
    if (ctrl_reg1 != LIS3DH_CTRL_REG1_SET)
    {
        ctrl_reg1 = LIS3DH_CTRL_REG1_SET;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    }
    
    /******************************************/
    /*     Read Control Register 1 again      */
    /******************************************/

    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1 after overwrite operation: 0x%02X\r\n", ctrl_reg1);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
     
    /******************************************/
    /*            set BDU option              */
    /******************************************/

       
    
    uint8_t ctrl_reg4;

    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
    }
    
    
    ctrl_reg4 = LIS3DH_CTRL_REG4_BDU_ACTIVE; // non cambierei altro a parte il BDU
    
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG4,
                                         ctrl_reg4);
    
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4 after being updated: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
    }
    
    
    
    int16_t OutX;
    int16_t OutX_shifted;
    
    int16_t OutY;
    int16_t OutY_shifted;
    
    int16_t OutZ;
    int16_t OutZ_shifted;
    
    uint8_t header = 0xA0;
    uint8_t footer = 0xC0;
    
    
    
    uint8_t Out_Array[8];
    
    Out_Array[0]= header;
    Out_Array[7]= footer;
    
    
    uint8_t AccelerationData[6];
    

    
    timer_flag=0;
    isr_StartEx(custom_ISR_TIMER);
    
    Timer_Start();
    
    for(;;)
    {
        if(timer_flag ==1)
        {
            
            while(!(LIS3DH_STATUS_REG & LIS3DH_STATUS_REG_DATA_AVAILABLE)){};  //credo che sia meglio un if
            
                
                
                
                timer_flag=0;
                error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                        LIS3DH_OUT_X_L,6,
                                                        &AccelerationData[0]);
        
        
                if(error == NO_ERROR)
                {
                    
                    OutX= (int16)((AccelerationData[0] | (AccelerationData[1]<<8))); //>>4 perchè uso 12 bit
                    
                    OutX_shifted= (abs(OutX))>>4;
                    
                    Out_Array[1]= (uint8_t)(OutX_shifted & 0xFF);  //seleziono il LSB
                    Out_Array[2]= (uint8_t)(OutX_shifted >> 8);    //seleziono il MSB
                    
                    OutY= (int16)((AccelerationData[2] | (AccelerationData[3]<<8)));
                    
                    OutY_shifted = (abs(OutY))>>4;
                    
                    Out_Array[3]= (uint8_t)(OutY_shifted & 0xFF);
                    Out_Array[4]= (uint8_t)(OutY_shifted >> 8);
                    
                    OutZ= (int16)((AccelerationData[4] | (AccelerationData[5]<<8)));
                    
                    OutZ_shifted= (abs(OutZ))>>4;
                    
                    Out_Array[5]= (uint8_t)(OutZ_shifted & 0xFF);
                    Out_Array[6]= (uint8_t)(OutZ_shifted >> 8);
                    
                    UART_Debug_PutArray(Out_Array, 8);
                    
               
                }
                
            
        }
    }
}

/* [] END OF FILE */
