/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "InterruptRoutines.h"
#include "project.h"


CY_ISR(custom_ISR_TIMER)
{
    Timer_ReadStatusRegister();
    
    timer_flag=1;   
    
    
    
    
}

/* [] END OF FILE */
