/**
 * Az UART kommunikációs feladat megoldása.
 * Az itt található függvények elsődleges feladata a log.c logolási funkcióinak kiszolgálása, valamint
 * a  mainloop.c számára lehetővé tenni, hogy soros porton egyetlen bájtos parancsokat
 * fogadjon.
 *
 * Az inicializálás kereteiben belül egyrészt konfigurálni kell a mikrovezérlő megfelelő lábait
 * (alternate function, push-pull stb.), valamint az UART perifériát (baud rate, stop bitek száma stb.)
 * Ehhez a MARCOM.ioc fájlban található grafikus konfigurátort használjuk, és az általa generált inicializáló
 * függvényt hívjuk meg itt az UART_Init() függvényben.
 *
 * Ezután a UART_SendString() függvényben már meg lehet írni egy string küldését. Ezúttal
 * nem blokkoló, interrupt alapú küldést használunk, hogy küldés közben a rendszer többi
 * funkciója ne akadjon meg.
 *
 * A feladat sikeres elkészítése után oszcilloszkóppal vizsgáljon meg egy UART keretet (PD8 pin), és a jegyzőkönyvben
 * helyezzen el egy screenshotot, melyen látszik a keret eleje, vége, és mutassa be az egyes bitek pontos jelentését
 * (vagyis hogy mit küldött éppen). Ellenőrizze a scope kurzorai segítségével a baud rate-et.
 * Nagyon figyeljen oda, hogy a tüskesor lábait nehogy rövidre zárja! A rövidzárból eredő károkért a hallgató felel!
 * Földpontnak használja péládul az RS-232 csatlakozó csavarját.
 *
 * A HAL koncepciója szerint az interruptokról callback függvényeken keresztül kapunk
 * értesítést, melyeket a beépített interrupt kezelő hív meg. Ezért néz ki az igazi interrupt kezelő
 * (USART3_IRQHandler() az stm32f4xx_it.c-ben) függvény úgy, ahogy: csak egy már a HAL-lal kapott
 * kezelő függvény meghívását látjuk benne.
 *
 * Fogadott adatokról értesítést a HAL_UART_RxCpltCallback() függvény segítségével kapunk, így a kapott adatok
 * feldolgozását itt kell megoldani. Ezen kívül az interruptos fogadást engedélyezni kell a UART_Init()-ben.
 * Jelen esetben a callback függvényben a fogadott bájtot csak elmentjük a lastReceivedUartCommand globális változóba, 
 * melyet a főciklus figyel a mainloop.c-ben, és szükség esetén reagál rá. A feladat másik része éppen ezért a
 * mainloop.c-ben található.
 */
 
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
#include "string.h"
#include "bsp_uart.h"

/* ----------------- Publikus globális változók ----------------- */

/** Last received command. 0 if none. */
uint8_t lastReceivedUartCommand = 0;

/* ----------------- Belső, globális változók ----------------- */

// Egyetlen bájtos adatfogadási buffer
char rxBuffer;
// Ide másoljuk a küldés alatt álló stringet. Az UART_SendString() használja.
#define TXBUFFERSIZE 255
char txBuffer[TXBUFFERSIZE];

/* ----------------- Publikus függvények ----------------- */


/** String küldése, nem blokkolva. */
HAL_StatusTypeDef UART_SendString(const char *str)
{
	// Várjon addig, amig az UART periféria készen nem áll az adatküldésre.
	// Akkor tudunk küldeni, ha a huart3.gState vagy HAL_UART_STATE_READY, vagy 
	// HAL_UART_STATE_BUSY_RX (vagyis csak fogadás van folyamatban).
	// (Ez a várakozás részben blokkolóvá teszi az amúgy nem blokkoló küldést, de
	// az egyszerűség kedvéért most ezt bevállaljuk, mert különben egy várakozó sort kellene
	// készíteni a küldéseknek, ami meghaladja a mérés célkitűzését.)

	while ((huart2.gState != HAL_UART_STATE_BUSY_RX) && (huart2.gState != HAL_UART_STATE_READY))
	{
	}

	// Ha ide eljutottunk, akkor az UART periféria készen áll az adatküldésre.
	// A paraméterként kapott stringet nem blokkolva küldje el a HAL_UART_Transmit_IT()
	// függvény segítségével. Figyeljen rá, hogy ha visszatér ebből a függvényből, az str által
	// mutatott hely már megváltozhat, pedig még nem küldtük el. Így itt másolja át a tartalmát
	// a txBuffer-be (de csak maximum TXBUFFERSIZE bájtot)! (strncpy()).
	// A HAL_UART_Transmit_IT() 2. paraméterének típusa (uint8_t*) kell legyen, így a txBuffer-t
	//	castolni kell.

	//strncpy(txBuffer, str, TXBUFFERSIZE); // --> Bugos, ezt használva néha HardFault-ra fut a program...
	//uint32_t length = strlen(txBuffer); // --> Bugos, ezt használva néha HardFault-ra fut a program...
	uint16_t length = 0;
    while(*(str+length) && length<TXBUFFERSIZE)
    {
    	*(txBuffer+length) = *(str+length);
    	length++;
    }
	return HAL_UART_Transmit_IT(&huart2, (uint8_t*)txBuffer, length);
}
