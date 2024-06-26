#include "stdinclude.hxx"

CIA2::CIA2(Logging *pLogging, RpPetra *pGlue) : CIA6526(pLogging, pGlue)
{
}

CIA2::~CIA2()
{
  m_registerSet[0x00]=0x03; // VIC base address to bank 0
}

// CIA-2 is connected to NMI, not IRQ pin
void CIA2::SignalInterrupt(bool signal)
{
  if (signal)
  {
    m_pGlue->SignalNMI(false);
    m_pGlue->SignalNMI(true);
  }
  else
  {
    m_pGlue->SignalNMI(true);
  }
}

