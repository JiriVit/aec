/******************************************************************************
  Target Script for SAM devices.

  Copyright (c) 2012 Rowley Associates Limited.

  This file may be distributed under the terms of the License Agreement
  provided with this software.

  THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 ******************************************************************************/

function _Reset()
{
  TargetInterface.pokeWord(0xE000EDFC, 0x00000001); // DEMCR - enable reset vector catch
  TargetInterface.pokeWord(0xE000ED0C, 0x05FA0001); // AIRCR - reset system
  TargetInterface.waitForDebugState(100);
}

function SAM3N_Reset()
{
  var impl = TargetInterface.implementation ? TargetInterface.implementation() : "";
  if (impl == "j-link")
    TargetInterface.resetAndStop(100);
  else
    {
      _Reset();
      TargetInterface.pokeWord(0x400E1400, 0xA5000005); // RSTC_CR - reset processor and peripherals
    }
}

function SAM3S_Reset()
{
  var impl = TargetInterface.implementation ? TargetInterface.implementation() : "";
  if (impl == "j-link")
    TargetInterface.resetAndStop(100);
  else
    {
      _Reset();
      TargetInterface.pokeWord(0x400E1400, 0xA5000005); // RSTC_CR - reset processor and peripherals
    }
}

function SAM3SD8_Reset()
{
  var impl = TargetInterface.implementation ? TargetInterface.implementation() : "";
  if (impl == "j-link")
    TargetInterface.resetAndStop(100);
  else
    {
      _Reset();
      TargetInterface.pokeWord(0x400E1400, 0xA5000005); // RSTC_CR - reset processor and peripherals
    }
}

function SAM3U_Reset()
{
  var impl = TargetInterface.implementation ? TargetInterface.implementation() : "";
  if (impl == "j-link")
    TargetInterface.resetAndStop(100);
  else
    {
      _Reset();
      TargetInterface.pokeWord(0x400E1200, 0xA5000004); // RSTC_CR - reset peripherals
    }
}

function SAM3XA_Reset()
{
  var impl = TargetInterface.implementation ? TargetInterface.implementation() : "";
  if (impl == "j-link")
    TargetInterface.resetAndStop(100);
  else
    {
      _Reset();
      TargetInterface.pokeWord(0x400E1A00, 0xA5000005); // RSTC_CR - reset processor and peripherals
    }
}

function SAM4E_Reset()
{
  var impl = TargetInterface.implementation ? TargetInterface.implementation() : "";
  if (impl == "j-link")
    TargetInterface.resetAndStop(100);
  else
    {
      _Reset();
      TargetInterface.pokeWord(0x400E1800, 0xA5000005); // RSTC_CR - reset processor and peripherals
    }
}

function SAM4N_Reset()
{
  var impl = TargetInterface.implementation ? TargetInterface.implementation() : "";
  if (impl == "j-link")
    TargetInterface.resetAndStop(100);
  else
    {
      _Reset();
      TargetInterface.pokeWord(0x400E1400, 0xA5000005); // RSTC_CR - reset processor and peripherals
    }
}

function SAM4S_Reset()
{
  var impl = TargetInterface.implementation ? TargetInterface.implementation() : "";
  if (impl == "j-link")
    TargetInterface.resetAndStop(100);
  else
    {
      _Reset();
      TargetInterface.pokeWord(0x400E1400, 0xA5000005); // RSTC_CR - reset processor and peripherals
    }
}

function SAM4SP_Reset()
{
  var impl = TargetInterface.implementation ? TargetInterface.implementation() : "";
  if (impl == "j-link")
    TargetInterface.resetAndStop(100);
  else
    {
      _Reset();
      TargetInterface.pokeWord(0x400E1400, 0xA5000005); // RSTC_CR - reset processor and peripherals
    }
}

function SAM4L_Reset()
{
  var impl = TargetInterface.implementation ? TargetInterface.implementation() : "";
  if (impl == "j-link")
    TargetInterface.resetAndStop(100);
  else
    TargetInterface.stopAndReset(100);
}

function SAMD20_Reset()
{
  var impl = TargetInterface.implementation ? TargetInterface.implementation() : "";
  if (impl == "j-link")
    TargetInterface.resetAndStop(100);
  else
    TargetInterface.stopAndReset(100);
}

function SAMD21_Reset()
{
  var impl = TargetInterface.implementation ? TargetInterface.implementation() : "";
  if (impl == "j-link")
    TargetInterface.resetAndStop(100);
  else
    TargetInterface.stopAndReset(100);
}

function SAMG51_Reset()
{
  var impl = TargetInterface.implementation ? TargetInterface.implementation() : "";
  if (impl == "j-link")
    TargetInterface.resetAndStop(100);
  else
    {
      _Reset();
      TargetInterface.pokeWord(0x400E1400, 0xA5000005); // RSTC_CR - reset processor and peripherals
    }
}

function SAMG53_Reset()
{
  var impl = TargetInterface.implementation ? TargetInterface.implementation() : "";
  if (impl == "j-link")
    TargetInterface.resetAndStop(100);
  else
    {
      _Reset();
      TargetInterface.pokeWord(0x400E1400, 0xA5000005); // RSTC_CR - reset processor and peripherals
    }
}

function SAMR21_Reset()
{
  var impl = TargetInterface.implementation ? TargetInterface.implementation() : "";
  if (impl == "j-link")
    TargetInterface.resetAndStop(100);
  else
    TargetInterface.stopAndReset(100);
}

function _GetPartNameM0()
{
  var did = TargetInterface.peekWord(0x41002018);
  var family = (did >> 23) & 0x1F;
  var series = (did >> 16) & 0x3F;
  var devsel = did & 0xFF;
  if (family == 0)
    {
      if (series == 0)
        {
          switch (devsel)
            {
              case 0x00:
                return "SAMD20J18";
              case 0x01:
                return "SAMD20J17";
              case 0x02:
                return "SAMD20J16";
              case 0x03:
                return "SAMD20J15";
              case 0x04:
                return "SAMD20J14";

              case 0x05:
                return "SAMD20G18";
              case 0x06:
                return "SAMD20G17";
              case 0x07:
                return "SAMD20G16";
              case 0x08:
                return "SAMD20G15";
              case 0x09:
                return "SAMD20G14";

              case 0x0A:
                return "SAMD20E18";
              case 0x0B:
                return "SAMD20E17";
              case 0x0C:
                return "SAMD20E16";
              case 0x0D:
                return "SAMD20E15";
              case 0x0E:
                return "SAMD20E14";
            }
        }
      else if (series == 1)
        {
          switch (devsel)
            {
              case 0x00:
                return "SAMD21J18A";
              case 0x01:
                return "SAMD21J17A";
              case 0x02:
                return "SAMD21J16A";
              case 0x03:
                return "SAMD21J15A";
              case 0x04:
                return "SAMD21J14A";

              case 0x05:
                return "SAMD21G18A/SAMR21G18A";
              case 0x06:
                return "SAMD21G17A/SAMR21G17A";
              case 0x07:
                return "SAMD21G16A/SAMR21G16A";
              case 0x08:
                return "SAMD21G15A";
              case 0x09:
                return "SAMD21G14A";

              case 0x0A:
                return "SAMD21E18A/SAMR21E18A";
              case 0x0B:
                return "SAMD21E17A/SAMR21E17A";
              case 0x0C:
                return "SAMD21E16A/SAMR21E16A";
              case 0x0D:
                return "SAMD21E15A";
              case 0x0E:
                return "SAMD21E14A";

              case 0x19:
                return "SAMR21G18A";
              case 0x1A:
                return "SAMR21G17A";
              case 0x1B:
                return "SAMR21G16A";
              case 0x1C:
                return "SAMR21E18A";
              case 0x1D:
                return "SAMR21E17A";
              case 0x1E:
                return "SAMR21E16A";
            }
        }
    }
    
  return "";
}

function _GetPartNameM3(REG_CHIPID_CIDR)
{
  var chipid = TargetInterface.peekWord(REG_CHIPID_CIDR);
  var nvpsiz = (chipid >> 8) & 0xF;
  var arch = (chipid >> 20) & 0xFF;

  switch (arch)
    {
      case 0x80:
        return "SAM3U4C/SAM3U2C/SAM3U1C";
      case 0x81:
        return "SAM3U4E/SAM3U2E/SAM3U1E";

      case 0x83:
        return "SAM3A8C/SAM3A4C";
      case 0x84:
        return "SAM3X8C/SAM3X4C";
      case 0x85:
        return "SAM3X8E/SAM3X4E";
      case 0x86:
        return "SAM3X8H";

      case 0x88:
        return "SAM3S4A/SAM3S2A/SAM3S1A";
      case 0x89:
        return "SAM3S4B/SAM3S2B/SAM3S1B/SAM3S8B/SAM3S16B";
      case 0x8A:
        return "SAM3S4C/SAM3S2C/SAM3S1C/SAM3S8C/SAM3S16C";

      case 0x93:
        return "SAM3N4A/SAM3N2A/SAM3N1A/SAM3N0A/SAM3N00A";
      case 0x94:
        return "SAM3N4B/SAM3N2B/SAM3N1B/SAM3N0B/SAM3N00B";
      case 0x95:
        return "SAM3N4C/SAM3N2C/SAM3N1C/SAM3N0C";

      case 0x99:
        return "SAM3SD8B";
      case 0x9A:
        return "SAM3SD8C";
    }

  return ""
}

function _GetPartNameM4(REG_CHIPID_CIDR)
{
  var chipid = TargetInterface.peekWord(0x400E0740);
  var nvpsiz = (chipid >> 8) & 0xF;
  var arch = (chipid >> 20) & 0xFF;

  switch (arch)
    {
      case 0x3C:
        return "SAM4E16E/SAM4E16C/SAM4E8E/SAM4E8C";

      case 0x43:
        return "SAMG51G18/SAMG51N18";

      case 0x47:
        return "SAMG53G19/SAMG53N19";

      case 0x88:
        return "SAM4S16A/SAM4S2A/SAM4S4A/SAM4S8A";
      case 0x89:
        return "SAM4S16B/SAM4S2B/SAM4S4B/SAM4S8B";
      case 0x8A:
        return "SAM4S16C/SAM4S2C/SAM4S4C/SAM4S8C";

      case 0x93:
        return "SAM4N8A";
      case 0x94:
        return "SAM4N8B/SAM4N16B";
      case 0x95:
        return "SAM4N8C/SAM4N16C";

      case 0x97:
        return "SAM4SP32A";

      case 0x99:
        return "SAM4SD16B/SAM4SD32B";
      case 0x9A:
        return "SAM4SD16C/SAM4SD32C";

      case 0xB0:
        switch (nvpsiz)
          {
            case 0x7:
              return "SAM4LC2A/SAM4LC2B/SAM4LC2C/SAM4LS2A/SAM4LS2B/SAM4LS2C";
            case 0x9:
              return "SAM4LC4A/SAM4LC4B/SAM4LC4C/SAM4LS4A/SAM4LS4B/SAM4LS4C";
            case 0xA:
              return "SAM4LC8A/SAM4LC8B/SAM4LC8C/SAM4LS8A/SAM4LS8B/SAM4LS8C";
          }
    }

  return ""
}

function GetPartName()
{
  switch ((TargetInterface.peekWord(0xE000ED00) >> 4) & 0xF)
    {
      case 0:
        result = _GetPartNameM0();
        break;
      case 3:
        result = _GetPartNameM3(0x400E0740);
        if (result == "")
          result = _GetPartNameM3(0x400E0940);
        break;
      case 4:
        result = _GetPartNameM4();
        break;
    }
  return result;
}

function MatchPartName(name)
{
  var partName;
  if (name.indexOf("SAMD") == 0 || name.indexOf("SAMR") == 0)
    {
      partName = _GetPartNameM0();
    }
  else if (name.indexOf("SAM3") == 0)
    {
      if (name.indexOf("SAM3X") == 0 || name.indexOf("SAM3A") == 0)
        partName = _GetPartNameM3(0x400E0940);
      else
        partName = _GetPartNameM3(0x400E0740);
    }
  else if (name.indexOf("SAM4") == 0 || name.indexOf("SAMG") == 0)
    {
      partName = _GetPartNameM4();
    }

  if (partName == "")
    return false;

  return partName.indexOf(name) != -1;
}
