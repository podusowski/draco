#include "draco.h"
#include "drax.h"
#include "libc.h"
#include "floppyd.h"
#include "../storaged/storaged.h"

uint8 MaskReg[8]   = { 0x0A, 0x0A, 0x0A, 0x0A, 0xD4, 0xD4, 0xD4, 0xD4 };
uint8 ModeReg[8]   = { 0x0B, 0x0B, 0x0B, 0x0B, 0xD6, 0xD6, 0xD6, 0xD6 };
uint8 ClearReg[8]  = { 0x0C, 0x0C, 0x0C, 0x0C, 0xD8, 0xD8, 0xD8, 0xD8 };

uint8 PagePort[8]  = { 0x87, 0x83, 0x81, 0x82, 0x8F, 0x8B, 0x89, 0x8A };
uint8 AddrPort[8]  = { 0x00, 0x02, 0x04, 0x06, 0xC0, 0xC4, 0xC8, 0xCC };
uint8 CountPort[8] = { 0x01, 0x03, 0x05, 0x07, 0xC2, 0xC6, 0xCA, 0xCE };

void dma_xfer(uint8 channel, uint8 page, uint16 length, int read)
{
	outb(MaskReg[channel], 0x04 | channel); // setup DMA channel
	outb(ClearReg[channel], 0x00);          // reset channel (if sometn is transfered)

	if (read)
		outb(ModeReg[channel], 0x46);
	else
		outb(ModeReg[channel], 0x4A);

	outb(AddrPort[channel], 0);             // send offset (low, next high byte)
	outb(AddrPort[channel], 0);
	outb(PagePort[channel], page);             // send the physical page that the data lies on.
	outb(CountPort[channel], length & 0x00FF); // data length (low, high byte)
	outb(CountPort[channel], length >> 8);
	outb(MaskReg[channel], channel);           // enable DMA channel
}

char fdc_drive = 0;
volatile char fdc_sr0;
volatile char fdc_track;
volatile char fdc_status[7] = { 0 };
volatile char fdc_statsz = 0;

void fdc_block2hts(uint16 block, uint16 *head, uint16 *track, uint16 *sector)
{
	*head = (block % (18 * 2)) / (18);
	*track = block / (18 * 2);
	*sector = block % 18 + 1;
}

uint8 fdc_wait(uint8 send_sensei)
{
	uint32 i = 0;

	while (!systemcall(SYSCALL_GET_IRQ_STATUS, 6) && (i < 40000))
		i++;

	if (systemcall(SYSCALL_GET_IRQ_STATUS, 6))
	{
		systemcall(SYSCALL_CLEAR_IRQ_STATUS, 6);
	}

	fdc_statsz = 0;
	while ((fdc_statsz < 7) && (inb(FDC_MSR) & (1<<4)))
		fdc_status[fdc_statsz++] = fdc_getbyte();

	if (send_sensei)
	{
		fdc_sendbyte(FDC_CMD_SENSEI);
		fdc_sr0 = fdc_getbyte();
		fdc_track = fdc_getbyte();
	}

	systemcall(SYSCALL_CLEAR_IRQ_STATUS, 6);

	if (i >= 40000)
	{
		printf("floppyd: operation timeout\n", 7);
		return 0;
	}
	return 1;
}

/* wg intela */
void fdc_sendbyte(uint8 byte)
{
	volatile int msr;
	int tmo;

	for (tmo = 0; tmo < 128; tmo++)
	{
		msr = inb(FDC_MSR);
		if ((msr & 0xc0) == 0x80)
		{
			outb(FDC_DATA,byte);
			return;
		}
		inb(0x80);   /* delay */
	}
}

uint8 fdc_getbyte()
{
	volatile int msr;
	int tmo;

	for (tmo = 0; tmo < 128; tmo++)
	{
		msr = inb(FDC_MSR);
		if ((msr & 0xd0) == 0xd0)
			return inb(FDC_DATA);
		inb(0x80);   /* delay */
	}

	return -1;   /* read timeout */
}

void fdc_start_motor()
{
	outb(FDC_DOR, 0x1C);
	int i = 0; for(; i < 10000; i++);
}

void fdc_stop_motor()  { outb(FDC_DOR, 0x0C); }

void fdc_seek(uint16 track)
{
	if (fdc_track == track) return;

	/* send actual command bytes */
	fdc_sendbyte(FDC_CMD_SEEK);
	fdc_sendbyte(0);
	fdc_sendbyte(track);

	/* wait until seek finished */
	fdc_wait(1);

	/* now let head settle for 15ms */
	int i = 0; for(; i < 10000; i++);
}

void fdc_recalibrate()
{
   fdc_sendbyte(FDC_CMD_RECAL);
   fdc_sendbyte(0);
   fdc_wait(1);
}

void fdc_reset()
{
	/* stop the motor and disable IRQ/DMA */
	outb(FDC_DOR,0);

	/* program data rate (500K/s) */
	outb(FDC_DRS,0);

	/* re-enable interrupts */
	outb(FDC_DOR,0x0C);

	/* resetting triggered an interrupt - handle it */
	fdc_wait(1);

	/* specify drive timings (got these off the BIOS) */
	fdc_sendbyte(FDC_CMD_SPECIFY);
	fdc_sendbyte(0xDF);  /* SRT = 3ms, HUT = 240ms */
	fdc_sendbyte(0x02);  /* HLT = 16ms, ND = 0 */

	/* clear "disk change" status */
	fdc_start_motor();
	fdc_seek(1);
	fdc_recalibrate();
	fdc_stop_motor();
}

void fdc_readwrite(uint16 block, int read)
{
	systemcall(SYSCALL_CLEAR_IRQ_STATUS, 6);

	uint16 head, track, sector;
	uint32 i;

	fdc_block2hts(block, &head, &track, &sector);
	fdc_start_motor();

	for (i = 1; i <= 3; i ++)
	{
		fdc_seek(track);
		outb(FDC_CCR,0);

		dma_xfer(2, KERNEL_DMA_BUFFER >> 16, 511, read);

		fdc_sendbyte(read? FDC_CMD_READ : FDC_CMD_WRITE);
		fdc_sendbyte(0);            // drive 0, head 0
		fdc_sendbyte(track);            // cylinder
		fdc_sendbyte(head);            // head
		fdc_sendbyte(sector);       // sector
		fdc_sendbyte(2);            // sector size 2 - 512
		fdc_sendbyte(18);           // sectors per cylinder
		fdc_sendbyte(DG144_GAP3RW); // narazie tylko 1.44
		fdc_sendbyte(0xFF);         // length? (podobno nieuzywane)

		fdc_wait(0); //nie wymaga SENSEI

		if ( (fdc_status[0] & 0xC0) == 0 )
			break;

		if ( (fdc_status[0] & 0xC0) == 0xC0 )
			printf("floppyd: abnormal termination by polling (%d retries left)\n", 3-i);
	}

	fdc_stop_motor();
}


/*
   1 - 360kb 5.25in
   2 - 1.2mb 5.25in
   3 - 720kb 3.5in
   4 - 1.44mb 3.5in
   5 - 2.88mb 3.5in
   0 - No drive
*/
void fdc_init()
{
	/* obslugujemy jednego flopa (narazie) */
	uint8 cmos;

	/* info o stacji wezmiemy sobie z cmos'a */
	outb(0x70, 0x10);
	cmos = inb(0x71);
	fdc_drive = cmos >> 4;

	static request_t request;
	static message_t msg;
	static uint32 result;
	uint32 storaged_pid = 0;

	/* przyda tez sie wersja FDC */
	/* uint8 i = 0;
	fdc_sendbyte(FDC_CMD_VERSION);
	i = fdc_getbyte(); */

	/* narazie bawimy sie tylko flopami 1.44 */
	if (fdc_drive == 4)
	{
		printf("floppyd: detected 1.44 floppy drive\n");
		enableirq(6);
		fdc_reset();

		/* zarejestruj urzadzenie */
		request.type = STORAGED_REGISTER;
		request.a = "fd0";
		request.b = 0;
		request.result = &result;

		msg.lparam = &request;

		while (!storaged_pid) storaged_pid = findprocess("storaged");
		sendmessage(storaged_pid, &msg);
		//sleep(SLEEP_SLAVE);
	}
	else
		printf("floppyd: no devices i can handle, sorry\n");
}

void main()
{
	static message_t msg;
	unsigned long sharedframe;

	fdc_init();

	setmyname("floppyd");

	while(1)
	{
		sleep(SLEEP_MESSAGE);
		if (getmessage(&msg))
		{
			switch (msg.type)
			{
				case FLOPPYD_READ:
				{
					fdc_readwrite(msg.lparam, 1);
					sharedframe = createsharedframe(msg.from);
					//printf("buf == %x\n", msg.hparam);
					memcpy(sharedframe + msg.hparam, KERNEL_DMA_BUFFER, 512);

					/* powiadamiamy, ze skonczylismy */
					msg.type = FLOPPYD_DONE;
					sendmessage(msg.from, &msg);

					freesharedframe();

					break;
				}
				case FLOPPYD_WRITE:
				{
					sharedframe = createsharedframe(msg.from);
					//printf("buf == %x\n", msg.hparam);
					memcpy(KERNEL_DMA_BUFFER, sharedframe + msg.hparam, 512);
					

					fdc_readwrite(msg.lparam, 0);

					freesharedframe();

					/* powiadamiamy, ze skonczylismy */
					msg.type = FLOPPYD_DONE;
					sendmessage(msg.from, &msg);

					break;
				}
			}
		}
	}
}

