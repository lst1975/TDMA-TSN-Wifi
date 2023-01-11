/*
  Copyright (c) 2012, Dust Networks 
  All rights reserved. 
  
  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions are met: 
    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer. 
    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in the 
      documentation and/or other materials provided with the distribution. 
    * Neither the name of Dust Networks nor the 
      names of its contributors may be used to endorse or promote products 
      derived from this software without specific prior written permission. 
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL DUST NETWORKS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef DN_INCL_FCS_H
#define DN_INCL_FCS_H

/*
 * Frame Check Sequence.
 *
 * This is the 16-bit frame check sequence that is defined in STD 51.
 */

#define FCS_INITIAL_FCS16 ((uint16_t) 0xffff)
#define FCS_GOOD_FCS16    ((uint16_t) 0xf0b8)

/** 
 * Calculate FCS16 over the given buffer
 * 
 * @param buf - start of buffer
 * @param len - length of buffer
 * 
 * @return INT16U - calculated FCS16
 */
uint16_t fcs_calcFcs(uint8_t *buf, uint32_t len);

/** 
 * Internal function for updating FCS16 calculation with one byte
 * Used in the HDLC decode operation
 * 
 * @param fcs  - current FCS16 value
 * @param data - byte of data
 * 
 * @return INT16U - calculated FCS16
 */
uint16_t fcs_fcs16(uint16_t fcs, uint8_t data);

#endif
