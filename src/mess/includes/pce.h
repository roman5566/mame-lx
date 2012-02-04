/*****************************************************************************
 *
 * includes/pce.h
 *
 * NEC PC Engine/TurboGrafx16
 *
 ****************************************************************************/

#ifndef PCE_H_
#define PCE_H_

#include "cdrom.h"
#include "sound/msm5205.h"

#define C6280_TAG			"c6280"

#define	MAIN_CLOCK		21477270
#define PCE_CD_CLOCK	9216000

#define PCE_HEADER_SIZE		512

#define TG_16_JOY_SIG		0x00
#define PCE_JOY_SIG			0x40
#define NO_CD_SIG			0x80
#define CD_SIG				0x00
/* these might be used to indicate something, but they always seem to return 1 */
#define CONST_SIG			0x30

/* the largest possible cartridge image (street fighter 2 - 2.5MB) */
#define PCE_ROM_MAXSIZE		0x280000

typedef struct
{
	UINT8	regs[16];
	UINT8	*bram;
	UINT8	*adpcm_ram;
	int		bram_locked;
	int		adpcm_read_ptr;
	UINT8	adpcm_read_buf;
	int		adpcm_write_ptr;
	UINT8	adpcm_write_buf;
	int		adpcm_length;
	int		adpcm_clock_divider;
	UINT32  msm_start_addr;
	UINT32	msm_end_addr;
	UINT32	msm_half_addr;
	UINT8	msm_nibble;
	UINT8	msm_idle;

	/* SCSI signals */
	int		scsi_BSY;	/* Busy. Bus in use */
	int		scsi_SEL;	/* Select. Initiator has won arbitration and has selected a target */
	int		scsi_CD;	/* Control/Data. Target is sending control (data) information */
	int		scsi_IO;	/* Input/Output. Target is sending (receiving) information */
	int		scsi_MSG;	/* Message. Target is sending or receiving a message */
	int		scsi_REQ;	/* Request. Target is requesting a data transfer */
	int		scsi_ACK;	/* Acknowledge. Initiator acknowledges that it is ready for a data transfer */
	int		scsi_ATN;	/* Attention. Initiator has a message ready for the target */
	int		scsi_RST;	/* Reset. Initiator forces all targets and any other initiators to do a warm reset */
	int		scsi_last_RST;	/* To catch setting of RST signal */
	int		cd_motor_on;
	int		selected;
	UINT8	*command_buffer;
	int		command_buffer_index;
	int		status_sent;
	int		message_after_status;
	int		message_sent;
	UINT8	*data_buffer;
	int		data_buffer_size;
	int		data_buffer_index;
	int		data_transferred;

	/* Arcade Card specific */
	UINT8	*acard_ram;
	UINT8	acard_latch;
	UINT8	acard_ctrl[4];
	UINT32	acard_base_addr[4];
	UINT16	acard_addr_offset[4];
	UINT16	acard_addr_inc[4];
	UINT32	acard_shift;
	UINT8	acard_shift_reg;

	UINT32	current_frame;
	UINT32	end_frame;
	UINT32	last_frame;
	UINT8	cdda_status;
	UINT8	cdda_play_mode;
	UINT8	*subcode_buffer;
	UINT8	end_mark;
	cdrom_file	*cd;
	const cdrom_toc*	toc;
	emu_timer	*data_timer;
	emu_timer	*adpcm_dma_timer;

	emu_timer	*cdda_fadeout_timer;
	emu_timer	*cdda_fadein_timer;
	double	cdda_volume;
	emu_timer	*adpcm_fadeout_timer;
	emu_timer	*adpcm_fadein_timer;
	double	adpcm_volume;
} pce_cd_t;


class pce_state : public driver_device
{
public:
	pce_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag) { }

	unsigned char *m_user_ram;
	UINT8 *m_cd_ram;
	UINT8 m_io_port_options;
	UINT8 m_sys3_card;
	UINT8 m_acard;
	pce_cd_t m_cd;
	UINT8 *m_cartridge_ram;
	int m_joystick_port_select;
	int m_joystick_data_select;
	UINT8 m_joy_6b_packet[5];
};


/*----------- defined in machine/pce.c -----------*/

DEVICE_IMAGE_LOAD(pce_cart);
NVRAM_HANDLER( pce );
WRITE8_HANDLER ( pce_mess_joystick_w );
 READ8_HANDLER ( pce_mess_joystick_r );

extern const msm5205_interface pce_cd_msm5205_interface;
WRITE8_HANDLER( pce_cd_bram_w );
WRITE8_HANDLER( pce_cd_intf_w );
READ8_HANDLER( pce_cd_intf_r );
WRITE8_HANDLER( pce_cd_acard_w );
READ8_HANDLER( pce_cd_acard_r );
WRITE8_HANDLER( pce_cd_acard_wram_w );
READ8_HANDLER( pce_cd_acard_wram_r );

DRIVER_INIT( pce_mess );
DRIVER_INIT( tg16 );
DRIVER_INIT( sgx );
MACHINE_START( pce );
MACHINE_RESET( pce_mess );


#endif /* PCE_H_ */
