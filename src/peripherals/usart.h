/*
 * Copyright (C) 2012 Henry Hallam <henry@swift-nav.com>
 * Copyright (C) 2012 Fergus Noble <fergusnoble@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SWIFTNAV_USART_H
#define SWIFTNAV_USART_H

#include <libopencm3/cm3/common.h>

/** \addtogroup peripherals
 * \{ */

/** \addtogroup usart
 * \{ */

#define USART_TX_BUFFER_LEN 4096
#define USART_RX_BUFFER_LEN 4096

#define USART_DEFAULT_BAUD 1000000

/** USART RX DMA state structure. */
typedef struct {
  u8 buff[USART_RX_BUFFER_LEN]; /**< USART RX DMA buffer. DMA xfers from USART to buffer, message processing routine reads out of buffer. */
  u32 rd;       /**< Address of next byte to read out of buffer. */
  /* TODO : is u32 big enough for rd_wraps and wr_wraps? */
  u32 rd_wraps; /**< Number of times rd has wrapped around the buffer. */
  u32 wr_wraps; /**< Number of times wr has wrapped around the buffer. */

  u32 dma;      /**< DMA for particular USART. */
  u32 usart;    /**< USART peripheral this state serves. */
  u8 stream;    /**< DMA stream for this USART. */
  u8 channel;   /**< DMA channel for this USART. */
} usart_rx_dma_state;

/** USART TX DMA state structure. */
typedef struct {
  u8 buff[USART_TX_BUFFER_LEN]; /**< USART TX DMA buffer. DMA xfers from buffer to USART_DR. */
  u32 rd;       /**< Address of next byte to read out of buffer. */
  u32 wr;       /**< Next buffer address to write to. */
  u32 xfer_len; /**< Number of bytes to DMA from buffer to USART_DR. */

  u32 dma;      /**< DMA for particular USART. */
  u32 usart;    /**< USART peripheral this state serves. */
  u8 stream;    /**< DMA stream for this USART. */
  u8 channel;   /**< DMA channel for this USART. */
} usart_tx_dma_state;

/** \} */

/** \} */

extern const u8 dma_irq_lookup[2][8];

extern usart_tx_dma_state ftdi_tx_state;
extern usart_rx_dma_state ftdi_rx_state;
extern usart_tx_dma_state uarta_tx_state;
extern usart_rx_dma_state uarta_rx_state;
extern usart_tx_dma_state uartb_tx_state;
extern usart_rx_dma_state uartb_rx_state;

void usarts_setup(u32 ftdi_baud, u32 uarta_baud, u32 uartb_baud);
void usarts_disable();

void usart_tx_dma_setup(usart_tx_dma_state* s, u32 usart,
                        u32 dma, u8 stream, u8 channel);
void usart_tx_dma_disable(usart_tx_dma_state* s);
u32 usart_tx_n_free(usart_tx_dma_state* s);
void usart_tx_dma_isr(usart_tx_dma_state* s);
u32 usart_write_dma(usart_tx_dma_state* s, u8 data[], u32 len);

void usart_rx_dma_setup(usart_rx_dma_state* s, u32 usart,
                        u32 dma, u8 stream, u8 channel);
void usart_rx_dma_disable(usart_rx_dma_state* s);
void usart_rx_dma_isr(usart_rx_dma_state* s);
u32 usart_n_read_dma(usart_rx_dma_state* s);
u32 usart_read_dma(usart_rx_dma_state* s, u8 data[], u32 len);

#endif /* SWIFTNAV_USART_H */
