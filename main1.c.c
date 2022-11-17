#include <project.h>
#include <stdio.h>
#include <math.h>

#define REQUEST_PER_BURST        (1u)
#define BYTES_PER_BURST          (3u)
#define UPPER_SRC_ADDRESS        CYDEV_PERIPH_BASE
#define UPPER_DEST_ADDRESS       CYDEV_PERIPH_BASE

CY_ISR_PROTO(Filter_Done);


volatile int cntr=0;
char TransmitBuffer[32];

CY_ISR(PWM_1_handler)
{
    sprintf(TransmitBuffer, "%d bpm\r\n\n", cntr*12);
    UART_1_PutString(TransmitBuffer); 
    cntr=0;
    PWM_1_ReadStatusRegister();
}


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
    isr_2_StartEx(PWM_1_handler);
}


int32 Filter_Out;
int32 Filter_A_Out, Filter_B_Out;
int filterA_done=0;

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
        //Filter_flag=1;
        filterA_done = 0;
        Filter_B_Out = Filter_1_Read24(Filter_1_CHANNEL_B);
	    //Chart_1_Plot(Filter_B_Out);
        
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
    
    char TransmitBuffer[32];
    
    init();
    
    
    uint8 VDAC8_1_value = 74;
    
    int level =0;
    while(1)
    { 
    ADC_DelSig_1_Read32();
    
    
    //sprintf(TransmitBuffer, "%ld \r\n", Output);
   // UART_1_PutString(TransmitBuffer);
    
   
    //sprintf(TransmitBuffer, "%d \r\n\n", VDAC8_1_value);
    //UART_1_PutString(TransmitBuffer);
    
    DMA_Config();
    
    /* Set the Filter Coherency to High Byte */
	Filter_1_SetCoherency(Filter_1_CHANNEL_A, Filter_1_KEY_HIGH);
    Filter_1_SetCoherency(Filter_1_CHANNEL_B, Filter_1_KEY_HIGH);
    //Chart_1_Plot(Output);
    
    
    if(Filter_B_Out>-100 && level==0)
    {
        cntr++;
        level=1;
    }
    else if(Filter_B_Out<-115 && level==1)
    level =0;
    
    
    }

}
