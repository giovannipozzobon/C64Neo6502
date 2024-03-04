/**
 * Written by Bernd Krekeler, Herne, Germany
 * 
 * CIA#1 ($DC00) PA0-PA7: JOY0-3, BNT-4 /COL0-COL7
 *               PB0-PB7: ROW0-ROW7
 * 
*/

#include "stdinclude.hxx"

CIA6526::CIA6526(Logging *pLogging, RpPetra *pGlue)
{
    m_pLog=pLogging;
    m_pGlue=pGlue;
}

CIA6526::~CIA6526() {}


void CIA6526::Reset() 
{
  m_i64Clks=0;
}
void CIA6526::Clk() 
{
  m_i64Clks++;
  m_registerSet[0x01]=0xff; // No key pressed.

  // size_t const BufferSize = 512;
  //char buffer[BufferSize];

  if (m_registerSetWrite[0x0e] & 0x01) // Start timer set?
  {
    // Interrupt Timer A allowed, decrease counter
    int16_t timer=m_registerSet[0x05]*256+m_registerSet[0x04];
    if (timer>0 && (m_registerSet[0x0e] & 0x01)) // 0x0e bit 0 indicates start countdown
    {
      timer--;
      m_registerSet[0x04]=timer % 256;
      m_registerSet[0x05]=timer / 256;
      if (timer==0)
      {
        if (m_registerSetWrite[0x0d] & 0x01) // IRQ timer A allowed?
        {
          m_registerSet[0x0d]|=0x81; // IRQ, timer A
          //m_pGlue->m_pLog->LogInfo({"CIA: IRQ signalled.\n\r"});
          m_pGlue->SignalIRQ(true);

          m_registerSet[0x04]=m_registerSetWrite[0x04];
          m_registerSet[0x05]=m_registerSetWrite[0x05];
          
          if (m_registerSetWrite[0x0e] & 0x08) // Single shot timer?
          {
              m_registerSetWrite[0x0e]&=0xfe; // Stop timer
          }
        }
      }
    }
  }
}

uint8_t CIA6526::ReadRegister(uint8_t reg) 
{
  uint8_t ret = m_registerSet[reg];
  if (reg==0x0d)
  {
    if (ret & 0x80)
    {
      m_pGlue->SignalIRQ(false);
    }
    m_registerSet[reg]=0x00;
  }
  return ret;
};


void CIA6526::WriteRegister(uint8_t reg, uint8_t value)
{
  if (reg==0x0d)
  {
    if (value & 0x80)
    {
      // Set bits 0..4
      m_registerSetWrite [reg]|=value & 0x1f;  
    }
    else
    {
      // Clear bits 0..4
      m_registerSetWrite [reg]|=~(value & 0x1f);  
    }
    
  }
  else {
    m_registerSet[reg]=value;
    m_registerSetWrite[reg]=value;
  }
}

