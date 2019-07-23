#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_leuart.h"
#include "em_dma.h"
#include "em_ebi.h"

extern "C" {
	extern DMA_DESCRIPTOR_TypeDef dmaControlBlock[];
}

/** LEUART Rx/Tx Port/Pin Location */
#define LEUART_LOCATION     0
#define LEUART_TXPORT       gpioPortD        /* LEUART transmission port */
#define LEUART_TXPIN        4                /* LEUART transmission pin  */
#define LEUART_RXPORT       gpioPortD        /* LEUART reception port    */
#define LEUART_RXPIN        5                /* LEUART reception pin     */

/** DMA Configurations            */
#define RX_CHANNEL          0          /* DMA channel is 0 */
#define TX_CHANNEL          1          /* DMA channel is 0 */
#define NUM_TRANSFER        32       /* Number of transfers per DMA cycle */

/** DMA callback structure */
static DMA_CB_TypeDef rx_callback;
static DMA_CB_TypeDef tx_callback;

/**************************************************************************//**
 * @brief  DMA callback function
 *
 * @details This function is invoked once a DMA transfer cycle is completed.
 *          It then refreshes the completed DMA descriptor.
 *****************************************************************************/
#define MAX_LEN 24
char source[ MAX_LEN ] = "\nFull command received\n";
static void rxCallback(unsigned int channel, bool primary, void *user)
{
	DMA_ActivateBasic(TX_CHANNEL,
			true,
			false,
			(void*) (&(LEUART0->TXDATA)),
			(void*) source,
			MAX_LEN - 2);
}

static void txCallback(unsigned int channel, bool primary, void *user)
{

}

/**************************************************************************//**
 * @brief  Setup DMA
 *
 * @details
 *   This function initializes DMA controller.
 *   It configures the DMA channel to be used for LEUART0 transmit and receive.
 *   The primary descriptor for channel0 is configured for N_MINUS_1 byte
 *   transfer. For continous data reception and transmission using LEUART in
 *   basic mode DMA Callback function is configured to reactivate
 *   basic transfer cycle once it is completed.
 *
 *****************************************************************************/
void setupDma(void)
{
	/* DMA configuration structs */
	DMA_Init_TypeDef       dmaInit;
	DMA_CfgChannel_TypeDef rxChannelCfg;
	DMA_CfgChannel_TypeDef txChannelCfg;

	DMA_CfgDescr_TypeDef   descrCfgRx;
	DMA_CfgDescr_TypeDef   descrCfgTx;

	/* Initializing the DMA */
	dmaInit.hprot        = 0;
	dmaInit.controlBlock = dmaControlBlock;
	DMA_Init(&dmaInit);
	rx_callback.cbFunc = rxCallback;
	rx_callback.userPtr = NULL;
	rxChannelCfg.highPri   = false; /* Not useful with peripherals */
	rxChannelCfg.enableInt = true;  /* Enabling interrupt to refresh DMA cycle*/
	rxChannelCfg.select = DMAREQ_LEUART0_RXDATAV;
	rxChannelCfg.cb     = &rx_callback;
	DMA_CfgChannel(RX_CHANNEL, &rxChannelCfg);
	descrCfgRx.dstInc = dmaDataIncNone;
	descrCfgRx.srcInc = dmaDataIncNone;
	descrCfgRx.size   = dmaDataSize1;
	descrCfgRx.arbRate = dmaArbitrate1;
	descrCfgRx.hprot   = 0;
	DMA_CfgDescr(RX_CHANNEL, true, &descrCfgRx);


	tx_callback.cbFunc = txCallback;
	tx_callback.userPtr = NULL;
	// DMA TX channel configuration
	txChannelCfg.enableInt = true;
	txChannelCfg.highPri = false;
	txChannelCfg.select = DMAREQ_LEUART0_TXBL;
	txChannelCfg.cb = &tx_callback;
	txChannelCfg.cb->primary = true;
	txChannelCfg.cb->userPtr = NULL;
	DMA_CfgChannel(TX_CHANNEL, &txChannelCfg);
	// DMA TX configuration descriptor
	descrCfgTx.arbRate = dmaArbitrate1;
	descrCfgTx.dstInc = dmaDataIncNone;
	descrCfgTx.hprot = 0;
	descrCfgTx.size = dmaDataSize1;
	descrCfgTx.srcInc = dmaDataInc1;
	DMA_CfgDescr(TX_CHANNEL, true, &descrCfgTx);
}

/**************************************************************************//**
 * @brief  Setting up LEUART
 *****************************************************************************/
void setupLeuart(void) {
	/* Enable peripheral clocks */
	CMU_ClockEnable(cmuClock_HFPER, true);
	/* Configure GPIO pins */
	CMU_ClockEnable(cmuClock_GPIO, true);
	/* To avoid false start, configure output as high */
	GPIO_PinModeSet(LEUART_TXPORT, LEUART_TXPIN, gpioModePushPull, 1);
	GPIO_PinModeSet(LEUART_RXPORT, LEUART_RXPIN, gpioModeInput, 0);

	LEUART_Init_TypeDef init = LEUART_INIT_DEFAULT;

	/* Enable CORE LE clock in order to access LE modules */
	CMU_ClockEnable(cmuClock_CORELE, true);

	/* Select LFXO for LEUARTs (and wait for it to stabilize) */
	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
	CMU_ClockEnable(cmuClock_LEUART0, true);

	/* Do not prescale clock */
	CMU_ClockDivSet(cmuClock_LEUART0, cmuClkDiv_1);

	/* Configure LEUART */
	init.enable = leuartDisable;

	LEUART_Init(LEUART0, &init);

	LEUART0->FREEZE = 1;

	/* Enable pins at default location */
	LEUART0->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN | LEUART_LOCATION;
	LEUART0->CTRL = LEUART_CTRL_SFUBRX | LEUART_CTRL_TXDMAWU | LEUART_CTRL_RXDMAWU | LEUART_CTRL_AUTOTRI;
	LEUART0->CMD |= LEUART_CMD_RXBLOCKEN;
	LEUART0->SIGFRAME = (uint8_t)'\r';
	LEUART0->STARTFRAME = (uint8_t)'$';

	LEUART0->FREEZE = 0;
	while (LEUART0->SYNCBUSY) { }

	/* Set RXDMAWU to wake up the DMA controller in EM2 */
	LEUART_RxDmaInEM2Enable(LEUART0, true);

	/* Clear previous RX interrupts */
	LEUART_IntClear(LEUART0, LEUART_IF_RXDATAV);
	NVIC_ClearPendingIRQ(LEUART0_IRQn);

	/* Finally enable it */
	LEUART_Enable(LEUART0, leuartEnable);
}

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
void leuartInit() {
	/* Chip errata */
	CHIP_Init();

	/* Initialize LEUART */
	setupLeuart();

	/* Setup DMA */
	setupDma();
}

void readNBytes(bool primary, char *buffer, int bytes, void (*funcPtr)(unsigned int channel, bool primary, void *user), void *instance)
{

	DMA_CfgChannel_TypeDef rxChannelCfg;
	rx_callback.cbFunc = funcPtr;
	rx_callback.userPtr = instance;
	rxChannelCfg.highPri   = false; /* Not useful with peripherals */
	rxChannelCfg.enableInt = true;  /* Enabling interrupt to refresh DMA cycle*/
	rxChannelCfg.select = DMAREQ_LEUART0_RXDATAV;
	rxChannelCfg.cb     = &rx_callback;
	DMA_CfgChannel(RX_CHANNEL, &rxChannelCfg);

	DMA_CfgDescr_TypeDef   descrCfgRx;
	descrCfgRx.dstInc = dmaDataInc1;
	descrCfgRx.srcInc = dmaDataIncNone;
	descrCfgRx.size   = dmaDataSize1;
	descrCfgRx.arbRate = dmaArbitrate1;
	descrCfgRx.hprot   = 0;
	DMA_CfgDescr(RX_CHANNEL, true, &descrCfgRx);

	DMA_ActivateBasic(RX_CHANNEL,
	                  primary,
	                  false,
	                  (void *)buffer,
	                  (void *)&LEUART0->RXDATA,
	                  (bytes-1));
}

void sendNBytes(bool primary, char *buffer, int bytes, void (*funcPtr)(unsigned int channel, bool primary, void *user), void *instance = NULL)
{
	DMA_CfgChannel_TypeDef txChannelCfg;
	tx_callback.cbFunc = funcPtr;
	tx_callback.userPtr = instance;
	txChannelCfg.highPri   = false; /* Not useful with peripherals */
	txChannelCfg.enableInt = true;  /* Enabling interrupt to refresh DMA cycle*/
	txChannelCfg.select = DMAREQ_LEUART0_TXBL;
	txChannelCfg.cb     = &tx_callback;
	DMA_CfgChannel(TX_CHANNEL, &txChannelCfg);

	DMA_ActivateBasic(TX_CHANNEL,
	                  primary,
	                  false,
	                  (void *)&LEUART0->TXDATA,
	                  (void *)buffer,
	                  (bytes-1));
}
