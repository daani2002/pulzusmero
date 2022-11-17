#include <project.h>
#include <stdio.h>
#include <math.h>

#define REQUEST_PER_BURST        (1u)
#define BYTES_PER_BURST          (3u)
#define UPPER_SRC_ADDRESS        CYDEV_PERIPH_BASE
#define UPPER_DEST_ADDRESS       CYDEV_PERIPH_BASE


volatile int cntr=0;
char TransmitBuffer[32];
volatile int Timer_flag=0;

CY_ISR_PROTO(Filter_Done);


//CY_ISR(PWM_1_handler)
//{
//    sprintf(TransmitBuffer, "%d \r\n\n", cntr);
//    UART_1_PutString(TransmitBuffer); 
//    cntr=0;
//    PWM_1_ReadStatusRegister();
//}

void init(){
    VDAC8_1_Start();
    ADC_SAR_1_Start();
    PGA_1_Start();
    PGA_2_Start();
    TIA_1_Start();
    UART_1_Start();
    ADC_DelSig_1_Start();
    Filter_1_Start();
    isr_1_StartEx(Filter_Done);
    PWM_1_Start();
}


int32 Filter_Out;
int32 Filter_A_Out, Filter_B_Out;
int filterA_done=0;
volatile int Filter_flag=0;

CY_ISR(Filter_Done)
{
    if(filterA_done==0)
    {
	    Filter_A_Out = Filter_1_Read24(Filter_1_CHANNEL_A);
        filterA_done = 1;
        Filter_1_Write24(Filter_1_CHANNEL_B, Filter_A_Out);
        //Chart_1_Plot(Filter_A_Out);
	}
    else
    {
        Filter_flag=1;
        filterA_done = 0;
        Filter_B_Out = Filter_1_Read24(Filter_1_CHANNEL_B);
	    Chart_1_Plot(Filter_B_Out);
//        if(i==235)
//            i=0;
//        buffer[i]=Filter_B_Out;
//        i++;
    }
}


void DMA_Config(void)
{
    /* Declare variable to hold the handle for DMA channel */
    uint8 channelHandle;

    /* Declare DMA Transaction Descriptor for memory transfer into
     * Filter Channel. */
    uint8 tdChanA;

    /* Configure the DMA to Transfer the data in 1 burst with individual trigger
     * for each burst.*/
    channelHandle = DMA_1_DmaInitialize(BYTES_PER_BURST, REQUEST_PER_BURST,
                                        HI16(UPPER_SRC_ADDRESS), HI16(UPPER_DEST_ADDRESS));

    /* This function allocates a TD for use with an initialized DMA channel */
    tdChanA = CyDmaTdAllocate();

	/* Source and Destination address increments are needed as we are using 3 byte transfers
	but Spoke Width is 16 bit */
    CyDmaTdSetConfiguration(tdChanA, 3u, tdChanA, TD_INC_SRC_ADR | TD_INC_DST_ADR);

    /* Set the source address as ADC_DelSig and the destination as
     * Filter Channel A.*/
    CyDmaTdSetAddress(tdChanA, LO16((uint32)ADC_DelSig_1_DEC_SAMP_PTR), LO16((uint32)Filter_1_STAGEA_PTR));

    /* Set tdChanA to be the initial TD associated with channelHandle */
    CyDmaChSetInitialTd(channelHandle, tdChanA);

    /* Enable the DMA channel represented by channelHandle and preserve the TD */
    CyDmaChEnable(channelHandle, 1u);
}



int main(){
    
    CyGlobalIntEnable;
    
    int32 Output;
    
    
    
    init();
    
    //isr_2_StartEx(PWM_1_handler);
    
    
    int l=0;
    
    while(1)
    { 
    
    
    
    
   
        //ADC_SAR_1_GetResult8();
   
        
         if(Filter_B_Out>-200 && l==0)
         {
        cntr++;
            l=1;
            }
        
   
    
   
    //sprintf(TransmitBuffer, "%d \r\n\n", VDAC8_1_value);
    //UART_1_PutString(TransmitBuffer);
    
    DMA_Config();
    
    /* Set the Filter Coherency to High Byte */
	Filter_1_SetCoherency(Filter_1_CHANNEL_A, Filter_1_KEY_HIGH);
    Filter_1_SetCoherency(Filter_1_CHANNEL_B, Filter_1_KEY_HIGH);
    //Chart_1_Plot(Output);
    
    //CyDelay(1000);
    
    
    int lever=0;
    
   
//    if(Filter_flag)
//    {
//        
//        if( lever==0)
//        {
//            cntr++;
//            lever=1;
//        }
//        else if(Filter_B_Out<-110 && lever==1)
//            lever=0;
//        Filter_flag=0;
//    }
    
    
    }

}


//#include <project.h>
//#include <stdio.h>
//#include <math.h>
//
//#define REQUEST_PER_BURST        (1u)
//#define BYTES_PER_BURST          (3u)
//#define UPPER_SRC_ADDRESS        CYDEV_PERIPH_BASE
//#define UPPER_DEST_ADDRESS       CYDEV_PERIPH_BASE
//
//CY_ISR_PROTO(Filter_Done);
////CY_ISR_PROTO(Counter_Done);
//
//int Counter_flag=0;
//
////cy_isr(counter_done)
////{
////    counter_flag=1;
////}
//
//void init(){
//    VDAC8_1_Start();
//    ADC_SAR_1_Start();
//    PGA_1_Start();
//    PGA_2_Start();
//    TIA_1_Start();
//    UART_1_Start();
//    ADC_DelSig_1_Start();
//    Filter_1_Start();
//    isr_1_StartEx(Filter_Done);
//    //Counter_1_Start();
//    //isr_2_StartEx(Counter_Done);
//}
//
//
//int32 Filter_A_Out, Filter_B_Out;
//int filterA_done=0;
//
//int32 buffer[236];
//int i=0;
//int Filter_flag=0;
//
//CY_ISR(Filter_Done)
//{
//    if(filterA_done==0)
//    {
//	    Filter_A_Out = Filter_1_Read24(Filter_1_CHANNEL_A);
//        filterA_done = 1;
//        Filter_1_Write24(Filter_1_CHANNEL_B, Filter_A_Out);
//	}
//    else
//    {
//        Filter_flag=1;
//        filterA_done = 0;
//        Filter_B_Out = Filter_1_Read24(Filter_1_CHANNEL_B);
//	    Chart_1_Plot(Filter_B_Out);
////        if(i==235)
////            i=0;
////        buffer[i]=Filter_B_Out;
////        i++;
//    }
//}
//
//
//void DMA_Config(void)
//{
//    /* Declare variable to hold the handle for DMA channel */
//    uint8 channelHandle;
//
//    /* Declare DMA Transaction Descriptor for memory transfer into
//     * Filter Channel. */
//    uint8 tdChanA;
//
//    /* Configure the DMA to Transfer the data in 1 burst with individual trigger
//     * for each burst.*/
//    channelHandle = DMA_1_DmaInitialize(BYTES_PER_BURST, REQUEST_PER_BURST,
//                                        HI16(UPPER_SRC_ADDRESS), HI16(UPPER_DEST_ADDRESS));
//
//    /* This function allocates a TD for use with an initialized DMA channel */
//    tdChanA = CyDmaTdAllocate();
//
//	/* Source and Destination address increments are needed as we are using 3 byte transfers
//	but Spoke Width is 16 bit */
//    CyDmaTdSetConfiguration(tdChanA, 3u, tdChanA, TD_INC_SRC_ADR | TD_INC_DST_ADR);
//
//    /* Set the source address as ADC_DelSig and the destination as
//     * Filter Channel A.*/
//    CyDmaTdSetAddress(tdChanA, LO16((uint32)ADC_DelSig_1_DEC_SAMP_PTR), LO16((uint32)Filter_1_STAGEA_PTR));
//
//    /* Set tdChanA to be the initial TD associated with channelHandle */
//    CyDmaChSetInitialTd(channelHandle, tdChanA);
//
//    /* Enable the DMA channel represented by channelHandle and preserve the TD */
//    CyDmaChEnable(channelHandle, 1u);
//}
//
//
////int32 average()
////{
////    int32 sum=0;
////    
////    for(int j=0; j<236; j++)
////        sum+=buffer[i];
////        
////    return sum/236;
////}
////
////int32 maximum()
////{
////    int32 max=buffer[0];
////    
////    for(int j=0; j<236; j++)
////        if(buffer[j]>max)
////            max=buffer[j];
////            
////    return max;
////}
////
////int32 minimum()
////{
////    int32 min=buffer[0];
////    
////    for(int j=0; j<236; j++)
////        if(buffer[j]<min)
////            min=buffer[j];
////            
////    return min;
////}
//
//int main(){
//    
//    CyGlobalIntEnable;
//    
//    int32 Output;
//    
//    char TransmitBuffer[32];
//    
//    init();
//    
//    int counter=0;
//    int Lever=1;
//    
////    int32 avg=average();
////    int32 max=maximum();
////    int32 min=minimum();
//    
//    
//    
//    
//    
//    while(1)
//    { 
//        
//        
//        
////    if(Filter_flag)
////    {
////        if(Lever==1)
////        {
////            if(Filter_B_Out > (avg + (max-avg)/4*3))
////            { 
////                counter++;
////                Lever=-1;
////            }
////        }
////        else
////        {
////            if(Filter_B_Out < avg-(avg-min)/4*3)
////            {
////                Lever=1;
////            }
////        }
////    }
////    
////    if(Counter_flag)
////    {
////        Counter_flag=0;
////        sprintf(TransmitBuffer, "%d \r\n", counter);
////        counter=0;
////        //UART_1_PutString(TransmitBuffer);
////    }
//    
//    DMA_Config();
//    Filter_1_SetCoherency(Filter_1_CHANNEL_A, Filter_1_KEY_HIGH);
//    //Filter_1_SetCoherency(Filter_1_CHANNEL_B, Filter_1_KEY_HIGH);
//    
//    
//    }
//
//}